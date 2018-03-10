#include "zoom_blur.h"

#include "../client_signature.h"
#include "../messaging/messaging.h"
#include "../hac2.h"

ChimeraCommandError block_zoom_blur_command(size_t argc, const char **argv) noexcept {
    static bool active = false;
    if(argc == 1) {
        ChimeraSignature &zoom_blur_1_s = get_signature("zoom_blur_1_sig");
        ChimeraSignature &zoom_blur_2_s = get_signature("zoom_blur_2_sig");
        ChimeraSignature &zoom_blur_3_s = get_signature("zoom_blur_3_sig");
        ChimeraSignature &zoom_blur_4_s = get_signature("zoom_blur_4_sig");

        const short zoom_blur_1_mod[] = {  -1,   -1, 0x38,   -1,   -1, 0x38};
        const short zoom_blur_2_mod[] = {  -1,   -1, 0x38};
        const short zoom_blur_3_mod[] = {0x31, 0xC0, 0x90};
        const short zoom_blur_4_mod[] = {-1,   0x01};

        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            if(new_value) {
                write_code_s(zoom_blur_1_s.address(), zoom_blur_1_mod);
                write_code_s(zoom_blur_2_s.address(), zoom_blur_2_mod);
                if(!hac2_present()) write_code_s(zoom_blur_3_s.address(), zoom_blur_3_mod);
                write_code_s(zoom_blur_4_s.address(), zoom_blur_4_mod);
            }
            else {
                zoom_blur_1_s.undo();
                zoom_blur_2_s.undo();
                if(!hac2_present()) zoom_blur_3_s.undo();
                zoom_blur_4_s.undo();
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
