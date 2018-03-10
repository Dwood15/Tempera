#include "../client_signature.h"
#include "../messaging/messaging.h"
#include "uncap_cinematic.h"

ChimeraCommandError uncap_cinematic_command(size_t argc, const char **argv) noexcept {
    extern bool uncap_cutscenes;
    if(argc == 1) {
        auto &uncap_cinematic_s = get_signature("uncap_cinematic_sig");
        bool new_value = bool_value(argv[0]);
        if(new_value != uncap_cutscenes) {
            if(new_value) {
                const short uncap_cinematic_mod[] = {0xEB, 0x04, 0xB3, 0x01, 0xEB, 0x02, 0x32, 0xDB, 0x8B, 0x2D};
                write_code_s(uncap_cinematic_s.address(),uncap_cinematic_mod);
            }
            else {
                uncap_cinematic_s.undo();
            }
            uncap_cutscenes = new_value;
        }
    }
    console_out(uncap_cutscenes ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
