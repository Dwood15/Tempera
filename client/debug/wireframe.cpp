#include "wireframe.h"

#include "../hooks/tick.h"
#include "../messaging/messaging.h"
#include "../client_signature.h"
#include "../halo_data/server.h"

static void set_wireframe(bool on) {
    static auto *wireframe_enabled = *reinterpret_cast<char **>(get_signature("wireframe_sig").address() + 2);
    *wireframe_enabled = on;
}

static void auto_hide_wireframe() {
    if(server_type() == SERVER_DEDICATED) {
        execute_chimera_command("chimera_wireframe false", true);
    }
}

ChimeraCommandError wireframe_command(size_t argc, const char **argv) noexcept {
    static auto enabled = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != enabled) {
            if(new_value) {
                if(server_type() == SERVER_DEDICATED) {
                    console_out_error("This command cannot be used on a dedicated server.");
                    return CHIMERA_COMMAND_ERROR_FAILURE;
                }
                add_tick_event(auto_hide_wireframe);
            }
            else {
                remove_tick_event(auto_hide_wireframe);
            }
            set_wireframe(new_value);
            enabled = new_value;
        }
    }
    console_out(enabled ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
