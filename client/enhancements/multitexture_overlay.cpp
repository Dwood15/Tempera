#include "multitexture_overlay.h"
#include "../client_signature.h"
#include "../messaging/messaging.h"

ChimeraCommandError block_mo_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            auto &multitexture_overlay_sig = get_signature("multitexture_overlay_sig");
            if(new_value) {
                const short multitexture_overlay_mod[] = {  -1,   -1, 0x60};
                write_code_s(multitexture_overlay_sig.address(), multitexture_overlay_mod);
            }
            else {
                multitexture_overlay_sig.undo();
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
