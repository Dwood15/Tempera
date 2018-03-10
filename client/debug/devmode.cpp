#include "devmode.h"

#include "../messaging/messaging.h"
#include "../client_signature.h"

ChimeraCommandError devmode_command(size_t argc, const char **argv) noexcept {
    static auto enabled = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != enabled) {
            auto &devmode_sig = get_signature("devmode_sig");
            if(new_value) {
                const unsigned char new_v[] = { 0xB1, 0x01, 0x90, 0x90, 0x90, 0x90 };
                write_code_c(devmode_sig.address(), new_v);
            }
            else {
                devmode_sig.undo();
            }
            enabled = new_value;
        }
    }
    console_out(enabled ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
