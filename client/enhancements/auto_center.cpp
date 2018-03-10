#include "auto_center.h"
#include "../messaging/messaging.h"
#include "../client_signature.h"
#include "../hooks/frame.h"
#include "../hooks/tick.h"

const short mod[] = { 0x8B, 0x51, 0x54, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x90, 0x90, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x90, 0xE9 };

static size_t x = 0;
static void auto_center_frame() noexcept {
    if(x++ == 1) {
        write_code_s(get_signature("auto_center_sig").address(),mod);
        remove_frame_event(auto_center_frame);
    }
}
static void auto_center_tick() noexcept {
    get_signature("auto_center_sig").undo();
    add_frame_event(auto_center_frame);
    x = 0;
}

ChimeraCommandError auto_center_command(size_t argc, const char **argv) noexcept {
    static int auto_center = 0;
    if(argc == 1) {
        auto &auto_center_s = get_signature("auto_center_sig");
        auto new_value = atol(argv[0]);
        if(new_value != auto_center) {
            auto_center_s.undo();
            remove_pretick_event(auto_center_tick);
            remove_frame_event(auto_center_frame);

            switch(new_value) {
            case 0:
                break;
            case 1:
                add_pretick_event(auto_center_tick);
                break;
            case 2:
                write_code_s(auto_center_s.address(),mod);
                break;
            default:
                console_out_error("Setting can only be between 0 and 2.");
                return CHIMERA_COMMAND_ERROR_FAILURE;
            }

            auto_center = new_value;
        }
    }
    console_out(std::to_string(auto_center));
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
