#include "firing_particle.h"
#include "../messaging/messaging.h"
#include "../client_signature.h"

ChimeraCommandError disable_buffering_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            char &setting = **reinterpret_cast<char **>(get_signature("disable_buffering_sig").address() + 1);
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
