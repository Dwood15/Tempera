#include "vertical_field_of_view.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "../halo_data/table.h"
#include "../halo_data/tag_data.h"
#include "../halo_data/resolution.h"
#include "../hooks/camera.h"
#include "../interpolation/camera.h"
#include "../messaging/messaging.h"

static float vfov = 0.0;

const float standard_vfov = 2.0 * atan(tan(70.0 * M_PI / 180.0 / 2.0) * 3.0 / 4.0) / M_PI * 180.0;

static void on_precamera() {
    static float previous_fov = 0.0;
    auto &data = camera_data();
    if(data.fov == previous_fov) return;
    const float default_fov = 70.00 * M_PI / 180.0;
    auto &resolution = get_resolution();

    char *odata = HaloObject(HaloPlayer().object_id()).object_data();
    if(odata) {
        HaloTag tag = HaloTag::from_id(*reinterpret_cast<HaloTagID *>(odata));
        *reinterpret_cast<float *>(tag.data + 0x1A0) = default_fov;
    }

    float calculated_fov = 2.0 * atan(tan(vfov * M_PI / 180.0 / 2.0) * resolution.width / resolution.height) * (data.fov / default_fov);
    data.fov = calculated_fov;
    previous_fov = data.fov;
}

ChimeraCommandError vfov_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        double new_value = strtod(argv[0], nullptr);
        if(new_value == 0) {
            remove_precamera_event(on_precamera);
        }
        else if(new_value == 1) {
            new_value = standard_vfov;
        }
        else if(new_value < 2 || new_value > 179) {
            console_out_error("FOV must be between 2 and 179");
            return CHIMERA_COMMAND_ERROR_FAILURE;
        }
        if(vfov == 0 && new_value != 0) {
            add_precamera_event(on_precamera, EVENT_PRIORITY_AFTER);
        }
        vfov = new_value;
    }
    if(vfov == 0)
        console_out("off");
    else
        console_out(std::to_string(vfov) + " degrees");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
