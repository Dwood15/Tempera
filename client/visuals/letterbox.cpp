#include "letterbox.h"

#include "../client_signature.h"
#include "../hooks/tick.h"
#include "../messaging/messaging.h"

static float **letterbox;

void block_letterbox() noexcept {
    **letterbox = -0.5;
}

void set_block_letterbox(bool do_it) noexcept {
    static int count = 0;
    if(do_it) count++;
    else count--;

    letterbox = *reinterpret_cast<float ***>(get_signature("letterbox_sig").address() + 2);

    if(count == 1) {
        add_tick_event(block_letterbox);
    }
    else if(count == 0) {
        remove_tick_event(block_letterbox);
    }
    else if(count == -1) {
        std::terminate();
    }
}

ChimeraCommandError block_letterbox_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            if(new_value) {
                set_block_letterbox(true);
            }
            else {
                set_block_letterbox(false);
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
