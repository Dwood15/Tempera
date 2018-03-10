#include "mouse.h"
#include "../client_signature.h"
#include "../messaging/messaging.h"

static bool block_mouse_acceleration = false;

ChimeraCommandError mouse_sensitivity_command(size_t argc, const char **argv) noexcept {
    static bool active = false;
    static float horiz = 0;
    static float vert = 0;

    auto &mouse_horiz_1_sig = get_signature("mouse_horiz_1_sig");
    auto &mouse_horiz_2_sig = get_signature("mouse_horiz_2_sig");
    auto &mouse_vert_1_sig = get_signature("mouse_vert_1_sig");
    auto &mouse_vert_2_sig = get_signature("mouse_vert_2_sig");

    if(argc == 2) {
        if(!active) {
            write_code_any_value(mouse_horiz_1_sig.address() + 2, &horiz);
            write_code_any_value(mouse_horiz_2_sig.address() + 1, &horiz);
            write_code_any_value(mouse_vert_1_sig.address() + 2, &vert);
            write_code_any_value(mouse_vert_2_sig.address() + 2, &vert);
            active = true;
        }
        horiz = strtof(argv[0], nullptr) - 2.5;
        vert = strtof(argv[1], nullptr) - 2.5;

        if(!block_mouse_acceleration && (horiz < -0.5 || vert < -0.5)) {
            console_out_warning("You are using a low sensitivity value.");
            console_out_warning("Enable chimera_block_mouse_acceleration to prevent issues.");
        }
    }
    else if(argc == 1) {
        mouse_horiz_1_sig.undo();
        mouse_horiz_2_sig.undo();
        mouse_vert_1_sig.undo();
        mouse_vert_2_sig.undo();
        active = false;
    }
    if(active) console_out(std::to_string(horiz + 2.5) + " horizontal; " + std::to_string(vert + 2.5) + " vertical");
    else console_out("off");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}

ChimeraCommandError block_mouse_acceleration_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != block_mouse_acceleration) {
            auto &mouse_accel_1_sig = get_signature("mouse_accel_1_sig");
            auto &mouse_accel_2_sig = get_signature("mouse_accel_2_sig");
            if(new_value) {
                static float zero = 0;
                write_code_any_value(mouse_accel_1_sig.address(), static_cast<unsigned char>(0xEB));
                write_code_any_value(mouse_accel_2_sig.address() + 1, static_cast<unsigned char>(0x05));
                write_code_any_value(mouse_accel_2_sig.address() + 2, &zero);
                write_code_any_value(mouse_accel_2_sig.address() + 6 + 1, static_cast<unsigned char>(0x25));
                write_code_any_value(mouse_accel_2_sig.address() + 6 + 2, &zero);
            }
            else {
                mouse_accel_1_sig.undo();
                mouse_accel_2_sig.undo();
            }
            block_mouse_acceleration = new_value;
        }
    }
    console_out(block_mouse_acceleration ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
