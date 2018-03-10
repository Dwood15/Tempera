#include "anisotropic_filtering.h"

#include "../client_signature.h"
#include "../messaging/messaging.h"

ChimeraCommandError af_command(size_t argc, const char **argv) noexcept {
    static bool active = false;
    if(argc == 1) {
        auto new_value = bool_value(argv[0]);
        if(new_value != active) {
            auto &setting = **reinterpret_cast<char **>(get_signature("af_is_enabled_sig").address() + 1);
            if(new_value && setting) {
                console_out_warning("Disable buffering is already enabled (likely via config.txt)!");
            }
            setting = new_value;
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
