#include "camo_fix.h"
#include "../client_signature.h"
#include "../hooks/tick.h"
#include "../halo_data/table.h"
#include "../messaging/messaging.h"
#include "../interpolation/camera.h"

void camo_fix() noexcept {
    **reinterpret_cast<char **>(get_signature("camo_fix_sig").address() + 2) = 0;
    remove_tick_event(camo_fix);
}

void dart_fix() noexcept {
    write_code_any_value(get_signature("alpha_blend_transparency_sig").address() + 4, static_cast<float>(0.98));
}
