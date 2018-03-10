#include "gametype_indicator.h"

#include "../client_signature.h"
#include "../messaging/messaging.h"

ChimeraCommandError block_gametype_indicator_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            auto *team_icon_background_name = *reinterpret_cast<unsigned char **>(get_signature("team_icon_background_name_sig").address() + 1);
            if(new_value) {
                write_code_any_value(team_icon_background_name, 'x');
            }
            else {
                write_code_any_value(team_icon_background_name, 'u');
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
