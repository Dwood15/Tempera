#include "light.h"

#include <string.h>
#include "../../math/data_types.h"
#include "camera.h"
#include "../halo_data/table.h"
#include "../hooks/tick.h"

struct Light {
    // 0x0
    uint32_t unknown0;
    uint32_t some_id;
    uint32_t unknown1;
    uint32_t some_counter;
    // 0x10
    uint32_t unknown2;
    float red;
    float green;
    float blue;
    // 0x20
    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t parent_object_id;
    // 0x30
    Vector3D position;
    Vector3D rotation[2];
    // 0x54
    char i_stopped_caring_at_this_point[0x28];
}; // 0x7C

#define MAX_LIGHTS 0x380

struct MiniLight {
    Vector3D position;
    Vector3D rotation[2];
    uint32_t some_counter;
    bool valid;
};

static MiniLight light_buffer_0[MAX_LIGHTS] = {};
static MiniLight light_buffer_1[MAX_LIGHTS] = {};

void light_before() noexcept {
    extern float interpolation_tick_progress;
    extern size_t chimera_interpolate_setting;
    static int32_t tick_before = 0;
    auto tick_now = tick_count();
    auto &light_table = get_light_table();
    auto *lights = reinterpret_cast<Light *>(light_table.first);

    if(tick_now != tick_before) {
        tick_before = tick_now;
        memcpy(light_buffer_1, light_buffer_0, sizeof(light_buffer_0));
        auto &p = camera_data().position;
        for(size_t i=0;i<MAX_LIGHTS;i++) {
            light_buffer_0[i].some_counter = lights[i].some_counter;
            light_buffer_0[i].valid = light_buffer_0[i].some_counter > light_buffer_1[i].some_counter;
            if(light_buffer_0[i].valid && chimera_interpolate_setting < 9) {
                light_buffer_0[i].valid = distance(p, light_buffer_0[i].position) / zoom_scale() < 20;
            }
            if(light_buffer_0[i].valid) {
                light_buffer_0[i].position = lights[i].position;
                memcpy(light_buffer_0[i].rotation, lights[i].rotation, sizeof(light_buffer_0[i].rotation));
            }
        }
    }

    for(size_t i=0;i<MAX_LIGHTS;i++) {
        if(light_buffer_0[i].valid != true || light_buffer_1[i].valid != true) continue;
        for(int x=0;x<2;x++) interpolate_vector_rotation(light_buffer_1[i].rotation[x], light_buffer_0[i].rotation[x], lights[i].rotation[x], interpolation_tick_progress);
        interpolate_vector(light_buffer_1[i].position, light_buffer_0[i].position, lights[i].position, interpolation_tick_progress);
    }
}
