#include <math.h>
#include <string.h>
#include "../../math/data_types.h"
#include "../halo_data/table.h"
#include "widget.h"

extern interpolate_vector_fn interpolate_vector_objects;

struct AntennaVertex {
    Vector3D position;
    Vector3D velocity;
    float scale;
    uint32_t counter;
};

struct Antenna {
    uint32_t unknown_0;
    uint32_t unknown_1;
    uint32_t tag_id;
    uint32_t parent_object_id;
    Vector3D position;
    AntennaVertex vertices[0x15];
};

struct FlagPart {
    Vector3D position;
    Vector3D velocity;
};

struct Flag {
    uint32_t some_id;
    uint32_t unknown0;
    uint32_t parent_object_id;
    uint32_t some_id1;
    Vector3D position;
    FlagPart parts[241];
    char padding[8];
};

#define MAX_ANTENNA 0xC
static Antenna antenna_buffer_0[MAX_ANTENNA] = {};
static Antenna antenna_buffer_1[MAX_ANTENNA] = {};

#define MAX_FLAG 0x2
static Flag flag_buffer_0[MAX_FLAG] = {};
static Flag flag_buffer_1[MAX_FLAG] = {};

extern float interpolation_tick_progress;

static bool rollback_flag = false;

void do_antenna_interpolation() noexcept {
    static auto *ant = reinterpret_cast<Antenna *>(get_antenna_table().first);
    for(uint32_t i=0;i<MAX_ANTENNA;i++) {
        if(distance_squared(antenna_buffer_1[i].position, antenna_buffer_0[i].position) > 1) continue;
        for(uint32_t j=0;j<21;j++) {
            interpolate_vector_objects(antenna_buffer_1[i].vertices[j].position, antenna_buffer_0[i].vertices[j].position, ant[i].vertices[j].position, interpolation_tick_progress);
        }
    }
}
void do_flag_interpolation() noexcept {
    static auto *flag = reinterpret_cast<Flag *>(get_flag_table().first);
    rollback_flag = false;
    for(int i=0;i<MAX_FLAG;i++) {
        HaloObject o(flag[i].parent_object_id);
        char *odata = o.object_data();
        if(odata && odata[0xB4] == 2) {
            if(distance_squared(flag_buffer_0[i].position, flag_buffer_1[i].position) > 4) continue;
            interpolate_vector_objects(flag_buffer_1[i].position, flag_buffer_0[i].position, flag[i].position, interpolation_tick_progress);
            rollback_flag = true;
            for(uint32_t j=0;j<sizeof(flag_buffer_1[i].parts) / sizeof(flag_buffer_1[i].parts[0]);j++) {
                interpolate_vector_objects(flag_buffer_1[i].parts[j].position, flag_buffer_0[i].parts[j].position, flag[i].parts[j].position, interpolation_tick_progress);
            }
        }
    }
}
void buffer_widgets() noexcept {
    memcpy(antenna_buffer_1,antenna_buffer_0,sizeof(antenna_buffer_0));
    memcpy(flag_buffer_1,flag_buffer_0,sizeof(flag_buffer_1));
}
void buffer_widgets_l() noexcept {
    memcpy(antenna_buffer_0,get_antenna_table().first,sizeof(antenna_buffer_0));
    memcpy(flag_buffer_0,get_flag_table().first,sizeof(flag_buffer_0));
}

void rollback_widget_interpolation() noexcept {
    static auto *flag = reinterpret_cast<Flag *>(get_flag_table().first);
    for(int i=0;i<MAX_FLAG;i++) {
        flag[i].position = flag_buffer_0[i].position;
        for(uint32_t j=0;j<sizeof(flag_buffer_1[i].parts) / sizeof(flag_buffer_1[i].parts[0]);j++) {
            flag[i].parts[j].position = flag_buffer_0[i].parts[j].position;
        }
    }
}
