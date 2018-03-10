#include "hud_kill_feed.h"

#include "../client_signature.h"
#include "../messaging/messaging.h"

ChimeraCommandError hud_kill_feed_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            auto &hud_kill_feed_sig = get_signature("hud_kill_feed_sig");
            if(new_value) {
                write_code_any_value(hud_kill_feed_sig.address() + 1, reinterpret_cast<int>(static_cast<void (*)(const short *)>(hud_message)) - reinterpret_cast<int>(hud_kill_feed_sig.address() + 5));
            }
            else {
                hud_kill_feed_sig.undo();
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
