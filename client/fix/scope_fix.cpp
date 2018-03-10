#include "scope_fix.h"
#include "../client_signature.h"
#include "../halo_data/resolution.h"
#include "../hooks/tick.h"
#include "../messaging/messaging.h"

void apply_scope_fix() {
    static float *width = reinterpret_cast<float *>(get_signature("widescreen_scope_sig").address() + 4);
    static float *height = reinterpret_cast<float *>(get_signature("widescreen_scope_sig").address() + 4 + 8);
    auto &resolution = get_resolution();
    float ar = static_cast<float>(resolution.width) / resolution.height;
    auto expected = ar * *height;
    if(expected != *width) {
        DWORD prota;
        DWORD protb;
        VirtualProtect(width, sizeof(*width), PAGE_READWRITE, &prota);
        *width = expected;
        VirtualProtect(width, sizeof(*width), prota, &protb);
    }
}

void undo_scope_fix() {
    get_signature("widescreen_scope_sig").undo();
}

bool widescreen_scope_mask_active = false;

ChimeraCommandError widescreen_scope_fix_command(size_t argc, const char **argv) noexcept {
    extern int widescreen_fix_active;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != widescreen_scope_mask_active) {
            if(new_value) {
                if(widescreen_fix_active) {
                    console_out_error("chimera_widescreen_fix is enabled. Turn this off, first.");
                    return CHIMERA_COMMAND_ERROR_FAILURE;
                }
                add_tick_event(apply_scope_fix);
            }
            else {
                remove_tick_event(apply_scope_fix);
                undo_scope_fix();
            }
            widescreen_scope_mask_active = new_value;
        }
    }
    console_out(widescreen_scope_mask_active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
