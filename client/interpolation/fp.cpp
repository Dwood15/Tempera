#include <string.h>
#include "fp.h"
#include "../hooks/tick.h"
#include "../halo_data/table.h"
#include "../../math/data_types.h"

struct FirstPersonNode {
    Quaternion rotation_stuff;
    Vector3D position;
    float unknown_1 = 1.0;
};

FirstPersonNode *fpn = reinterpret_cast<FirstPersonNode *>(0x40000EC0+0x8C);

static FirstPersonNode fpbuffer0[128] = {};
static FirstPersonNode fpbuffer1[128] = {};

void fp_before() noexcept {
    extern float interpolation_tick_progress;
    static int32_t tick_before = 0;
    static uint32_t skip = false;
    auto tick_now = tick_count();

    if(tick_now != tick_before) {
        static uint32_t weapon_id = NULL_ID;
        tick_before = tick_now;
        memcpy(fpbuffer1,fpbuffer0,sizeof(fpbuffer0));
        memcpy(fpbuffer0,fpn,sizeof(fpbuffer0));

        HaloObject o(HaloPlayer().object_id());
        auto *odata = o.object_data();

        uint32_t current_id;
        if(odata) current_id = *(reinterpret_cast<uint32_t *>(odata + 0x2F8) + *reinterpret_cast<uint16_t *>(odata + 0x2F2));
        else current_id = NULL_ID;

        skip = get_camera_type() != CAMERA_FIRST_PERSON || current_id != weapon_id;
        weapon_id = current_id;
    }

    if(skip) return;

    for(int i=0;i<128;i++) {
        interpolate_quat(fpbuffer1[i].rotation_stuff, fpbuffer0[i].rotation_stuff, fpn[i].rotation_stuff, interpolation_tick_progress);
        interpolate_vector(fpbuffer1[i].position, fpbuffer0[i].position, fpn[i].position, interpolation_tick_progress);
        fpn[i].unknown_1 = fpbuffer1[i].unknown_1 + (fpbuffer0[i].unknown_1 - fpbuffer1[i].unknown_1) * interpolation_tick_progress;
    }
}

void fp_after() noexcept {
    memcpy(fpn,fpbuffer0,sizeof(fpbuffer0));
}
