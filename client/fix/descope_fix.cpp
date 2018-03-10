#include "descope_fix.h"

#include "../client_signature.h"
#include "../hooks/tick.h"
#include "../halo_data/table.h"

typedef void (*ds)(uint32_t object_id);
static ds do_descope = nullptr;

static bool descope_fix_enabled = false;

static void set_halo_descoping(bool do_it) {
    if(!do_it == descope_fix_enabled) return;
    descope_fix_enabled = !do_it;
    auto &descope_fn_sig = get_signature("descope_fix_sig");
    if(do_it) {
        descope_fn_sig.undo();
    }
    else {
        const short mod[] = {0x90,0x90,0x90,0x90,0x90};
        write_code_s(descope_fn_sig.address(), mod);
    }
}

static void fix_descoping() noexcept {
    static float health = 0;
    static float shield = 0;
    auto id = HaloPlayer().object_id();
    HaloObject object(id);
    auto *data = object.object_data();
    if(data) {
        const auto &object_health = *reinterpret_cast<const float *>(data + 0xE0);
        const auto &object_shield = *reinterpret_cast<const float *>(data + 0xE4);

        set_halo_descoping(object_shield > shield && object_shield > 1.0);

        if(object_health < health || (object_shield < shield && (shield - object_shield) > 0.01)) {
            if(descope_fix_enabled) do_descope(id);
        }

        health = object_health;
        shield = object_shield;
    }
}

void enable_descope_fix() noexcept {
    auto &descope_fn_sig = get_signature("descope_fix_sig");
    do_descope = reinterpret_cast<ds>(*reinterpret_cast<char **>(descope_fn_sig.address() + 1) + reinterpret_cast<int>(descope_fn_sig.address() + 5));
    set_halo_descoping(false);
    add_tick_event(fix_descoping);
}
