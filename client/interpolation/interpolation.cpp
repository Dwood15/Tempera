#include <mutex>
#include <math.h>
#include <iostream>
#include "../messaging/messaging.h"
#include "../client_signature.h"
#include "../hooks/camera.h"
#include "../hooks/frame.h"
#include "../hooks/tick.h"
#include "../halo_data/map.h"
#include "../halo_data/tag_data.h"
#include "../halo_data/server.h"
#include "../halo_data/table.h"
#include "light.h"
#include "interpolation.h"
#include "particle.h"
#include "fp.h"
#include "widget.h"
#include "camera.h"

static float *camera_tick_rate = nullptr;
size_t chimera_interpolate_setting = 0;
char chimera_interpolate_predict = 0;

static bool nuked = true;

struct ModelNode {
    float scale = 1.0;
    RotationMatrix rotation;
    Vector3D position;
};

#define MAX_NODES 64

enum InterpolationType {
    INTERPOLATION_NONE = 0,
    INTERPOLATION_POSITION,
    INTERPOLATION_POSITION_ROTATION
};

struct BufferedObject {
    uint32_t tag_id;
    InterpolationType interpolation_type;
    Vector3D position;
    Vector3D position_center;
    ModelNode nodes[MAX_NODES];
};

static BufferedObject objects_buffer_0[2048];
static BufferedObject objects_buffer_1[2048];

const char ilevels[10][9] {
//   B V W E G P S M C
    {},                  // 0
    {1,1,1,0,0,0,0,0,0}, // 1
    {2,2,1,0,0,0,0,1,1}, // 2
    {2,2,1,1,1,0,0,1,1}, // low (first person)
    {2,2,2,2,1,1,0,1,1}, // 4
    {2,2,2,2,1,1,0,2,2}, // 5
    {2,2,2,2,2,1,1,2,2}, // medium (particles)
    {2,2,2,2,2,1,2,2,2}, // 7
    {2,2,2,2,2,2,2,2,2}, // high
    {3,3,3,3,3,3,3,3,3}, // ultra
};

const uint32_t model_node_offset[16] = {
    0x550,
    0x5C0,
    0x340,
    0x294,
    0x244,
    0x2B0,
    0x1F8,
    0x228,
    0x21C
};

float interpolation_tick_progress = 0.0;
extern interpolate_vector_fn interpolate_vector_objects;

static void do_interpolation(uint32_t i) noexcept {
    HaloObject o(i);
    auto *data = o.object_data();
    if(nuked) {
        objects_buffer_0[i].interpolation_type = INTERPOLATION_NONE;
        if(data) objects_buffer_0[i].tag_id = reinterpret_cast<BaseHaloObject *>(data)->tag_id;
        if(objects_buffer_0[i].tag_id != objects_buffer_1[i].tag_id) {
            objects_buffer_1[i].interpolation_type = INTERPOLATION_NONE;
        }
    }
    if(data) {
        auto &object = *reinterpret_cast<BaseHaloObject *>(data);
        const auto &type = object.object_type;
        if(type > 8 || (object.phased_out && type != 5)) {
            return;
        };
        auto ld = ilevels[chimera_interpolate_setting][type];
        if(ld == 0) return;
        uint32_t node_count = 0;
        auto &model_id = *reinterpret_cast<HaloTagID *>(HaloTag::from_id(object.tag_id).data + 0x28 + 0xC);
        if(!model_id.is_null()) node_count = *reinterpret_cast<uint32_t *>(HaloTag::from_id(model_id).data + 0xB8);
        if(type == 0x4 || type == 0x5) node_count = 1;
        if(node_count == 0 || node_count > MAX_NODES) return;

        const auto &offset = model_node_offset[type];
        ModelNode *nodes = reinterpret_cast<ModelNode *>(data + offset);

        objects_buffer_0[i].position = object.position;
        auto &position_center = object.position_script;
        objects_buffer_0[i].position_center = position_center;

        if(nuked) {
            if(type <= 3) {
                if(distance_squared(objects_buffer_0[i].position_center, objects_buffer_1[i].position_center) > 1.5) {
                    objects_buffer_0[i].interpolation_type = INTERPOLATION_NONE;
                    return;
                }
            }

            auto r = INTERPOLATION_NONE;

            if(chimera_interpolate_setting == 9) {
                r = INTERPOLATION_POSITION_ROTATION;
            }
            else {
                float adjusted_scale = distance(objects_buffer_0[i].position_center, camera_data().position)/zoom_scale();
                if(ld == 1) {
                    if(adjusted_scale < 20.0) r = INTERPOLATION_POSITION;
                }
                else if(ld == 2) {
                    if(adjusted_scale < 15.0) r = INTERPOLATION_POSITION_ROTATION;
                    else if(adjusted_scale < 70.0) r = INTERPOLATION_POSITION;
                }
                else if(ld == 3) r = INTERPOLATION_POSITION_ROTATION;
            }

            objects_buffer_0[i].interpolation_type = r;
        }

        if(objects_buffer_0[i].interpolation_type == INTERPOLATION_NONE) return;

        if(objects_buffer_1[i].interpolation_type != INTERPOLATION_NONE)
            interpolate_vector_objects(objects_buffer_1[i].position_center, objects_buffer_0[i].position_center, position_center, interpolation_tick_progress);

        for(uint32_t x=0;x<node_count;x++) {
            objects_buffer_0[i].nodes[x] = nodes[x];
            if(objects_buffer_1[i].interpolation_type != INTERPOLATION_NONE) {
                interpolate_vector_objects(objects_buffer_1[i].nodes[x].position, objects_buffer_0[i].nodes[x].position, nodes[x].position, interpolation_tick_progress);
                nodes[x].scale = objects_buffer_1[i].nodes[x].scale + (objects_buffer_0[i].nodes[x].scale - objects_buffer_1[i].nodes[x].scale) * interpolation_tick_progress;

                if(objects_buffer_0[i].interpolation_type == INTERPOLATION_POSITION_ROTATION) {
                    Quaternion before = objects_buffer_1[i].nodes[x].rotation;
                    Quaternion after = objects_buffer_0[i].nodes[x].rotation;
                    Quaternion out;
                    interpolate_quat(before,after,out,interpolation_tick_progress);
                    nodes[x].rotation = out;
                }
            }
        }
    }
}

extern void buffer_cam() noexcept;

static event_no_args nav_point_address;

static void interpolate_objects() noexcept {
    if(tick_count() == 0) return;

    for(uint32_t t=0;t<2048;t++) {
        do_interpolation(t);
    }

    if(nuked) {
        buffer_widgets_l();
    }

    if(chimera_interpolate_setting >= 6) do_particle_interpolation();
    if(chimera_interpolate_setting >= 1) do_antenna_interpolation();
    if(chimera_interpolate_setting >= 3) do_flag_interpolation();
    if(chimera_interpolate_setting >= 8) light_before();

    if(server_type() != SERVER_NONE && get_map_header().game_type == 1) nav_point_address();

    nuked = false;
}


static void reset() noexcept {
    extern float stored_zoom_scale;
    memcpy(objects_buffer_1,objects_buffer_0,sizeof(objects_buffer_0));
    buffer_widgets();
    stored_zoom_scale = 0;
    nuked = true;
}

static void rollback_interpolation() noexcept {
    if(tick_count() == 0) return;
    if(chimera_interpolate_setting >= 1) rollback_widget_interpolation();
    for(uint32_t i=0;i<2048;i++) {
        HaloObject o(i);
        auto *data = o.object_data();
        if(objects_buffer_0[i].interpolation_type != INTERPOLATION_NONE && objects_buffer_1[i].interpolation_type != INTERPOLATION_NONE && data) {
            const auto &type = *reinterpret_cast<uint8_t *>(data + 0xB4);
            if((*reinterpret_cast<uint32_t *>(data + 0x10) & 1) && type != 5) continue;

            uint32_t node_count = 0;

            auto &model_id = *reinterpret_cast<HaloTagID *>(HaloTag::from_id(*reinterpret_cast<HaloTagID *>(data)).data + 0x28 + 0xC);
            if(!model_id.is_null()) node_count = *reinterpret_cast<uint32_t *>(HaloTag::from_id(model_id).data + 0xB8);

            const auto &offset = model_node_offset[type];
            auto *nodes = reinterpret_cast<ModelNode *>(data + offset);
            if(type == 0x4 || type == 0x5) node_count = 1;

            *reinterpret_cast<Vector3D *>(data + 0xA0) = objects_buffer_0[i].position_center;

            for(uint32_t x=0;x<node_count;x++) {
                nodes[x] = objects_buffer_0[i].nodes[x];
            }
        }
    }
}

ChimeraCommandError interpolate_command(size_t argc, const char **argv) noexcept {
    static bool use_text = true;
    if(argc != 0) {
        auto &camera_coord_s = get_signature("camera_coord_sig");
        auto &camera_tick_rate_s = get_signature("camera_tick_rate_sig");
        auto &camera_change_s = get_signature("camera_change_sig");
        auto &fp_interp_s = get_signature("fp_interp_sig");
        auto &do_reset_particle_s = get_signature("do_reset_particle_sig");
        nav_point_address = reinterpret_cast<event_no_args>(get_signature("nav_point_sig").address());

        static bool initialized = false;

        std::string arg = argv[0];
        for(size_t i=0;i<arg.size();i++) {
            arg[i] = tolower(arg[i]);
        }

        size_t new_setting = atol(argv[0]);

        if(arg == "off" || arg == "0" || arg == "false") {
            new_setting = 0;
            use_text = true;
        }
        else if(arg == "low") {
            new_setting = 3;
            use_text = true;
        }
        else if(arg == "medium" || arg == "med" || arg == "true" || arg == "on") {
            new_setting = 6;
            use_text = true;
        }
        else if(arg == "high") {
            new_setting = 8;
            use_text = true;
        }
        else if(arg == "ultra") {
            new_setting = 9;
            use_text = true;
        }
        else if(new_setting > 9 || new_setting == 0) {
            char z[256] = {};
            sprintf(z,"chimera_interpolate: Invalid setting \"%s\". Use \"chimera interpolate\" for help.",argv[0]);
            console_out_error(z);
            return CHIMERA_COMMAND_ERROR_FAILURE;
        }
        else {
            use_text = false;
        }
        if(initialized && new_setting == 0) {
            camera_coord_s.undo();
            camera_tick_rate_s.undo();
            fp_interp_s.undo();
            do_reset_particle_s.undo();
            camera_change_s.undo();
            remove_pretick_event(unset_camera_change);
            remove_tick_event(reset);
            remove_precamera_event(interpolate_all_cam_before);
            remove_camera_event(interpolate_all_cam_after);
            remove_preframe_event(interpolate_objects);
            remove_frame_event(rollback_interpolation);
            initialized = false;
        }
        else if(!initialized && new_setting != 0) {
            memset(objects_buffer_0,0,sizeof(objects_buffer_0));
            memset(objects_buffer_1,0,sizeof(objects_buffer_1));

            camera_tick_rate = *reinterpret_cast<float **>(camera_tick_rate_s.address() + 2);

            static BasicCodecave do_reset_particle_code;
            write_jmp_call(do_reset_particle_s.address(), reinterpret_cast<void *>(on_particle_physics_before), reinterpret_cast<void *>(on_particle_physics_after), do_reset_particle_code);

            static BasicCodecave fp_code;
            write_jmp_call(fp_interp_s.address(), reinterpret_cast<void *>(fp_before), reinterpret_cast<void *>(fp_after), fp_code);

            add_pretick_event(unset_camera_change);
            add_tick_event(reset);
            add_precamera_event(interpolate_all_cam_before);
            add_camera_event(interpolate_all_cam_after);
            add_preframe_event(interpolate_objects);
            add_frame_event(rollback_interpolation);

            size_t offset = camera_change_s.size();
            static BasicCodecave on_camera_change_code(camera_change_s.signature(), offset);
            on_camera_change_code.data[offset++] = 0x60;
            on_camera_change_code.data[offset + 0] = 0xE8;
            write_code_any_value(on_camera_change_code.data + offset + 1, reinterpret_cast<int>(on_camera_change) - reinterpret_cast<int>(on_camera_change_code.data + offset + 5));
            offset += 5;
            on_camera_change_code.data[offset++] = 0x61;
            on_camera_change_code.data[offset + 0] = 0xE9;
            write_code_any_value(on_camera_change_code.data + offset + 1, reinterpret_cast<int>(camera_change_s.address() + 5) - reinterpret_cast<int>(on_camera_change_code.data + offset + 5));
            unsigned char nop[] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
            write_code_c(camera_change_s.address(), nop);
            write_code_any_value(camera_change_s.address() + 1, reinterpret_cast<int>(on_camera_change_code.data) - reinterpret_cast<int>(camera_change_s.address() + 5));

            initialized = true;
        }
        chimera_interpolate_setting = new_setting;
    }
    if(use_text) {
        const char *what;
        switch(chimera_interpolate_setting) {
        case 0:
            what = "off";
            break;
        case 3:
            what = "low";
            break;
        case 6:
            what = "medium";
            break;
        case 8:
            what = "high";
            break;
        case 9:
            what = "ultra";
            break;
        default:
            what = "custom";
            break;
        }
        console_out(what);
    }
    else {
        console_out(std::to_string(chimera_interpolate_setting));
    }
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}

ChimeraCommandError interpolate_predict_command(size_t argc, const char **argv) noexcept {
    if(argc != 0) {
        auto new_value = atol(argv[0]);
        if(new_value != chimera_interpolate_predict) {
            switch(new_value) {
                case 0:
                    interpolate_vector_objects = interpolate_vector;
                    break;
                case 1:
                case 2:
                    interpolate_vector_objects = interpolate_vector_predict;
                    break;
                default: {
                    console_out_error("Expected a value between 0 and 2.");
                    return CHIMERA_COMMAND_ERROR_FAILURE;
                }
            }
        }
        chimera_interpolate_predict = new_value;
    }
    console_out(std::to_string(chimera_interpolate_predict));
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
