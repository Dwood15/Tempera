#include "gamepad_vertical_scale.h"

#include "../client_signature.h"
#include "../messaging/messaging.h"

ChimeraCommandError gamepad_vertical_scale_command(size_t argc, const char **argv) noexcept {
    static float value = 0;
    if(argc == 1) {
        auto &vertical_1_sig = get_signature("vertical_1_sig");
        auto &vertical_2_sig = get_signature("vertical_2_sig");
        auto new_value = strtof(argv[0], nullptr);
        if(new_value == 1) new_value = 0;
        if((new_value == 0) != (value == 0)) {
            if(new_value != 0) {
                static unsigned char osig[] = {
                    // fld dword ptr [ebx*4+addr]
                    0xD9, 0x04, 0x9D, 0xFF, 0xFF, 0xFF, 0xFF,

                    // fmul dword ptr [addr]
                    0xD8, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF,

                    // jmp addr
                    0xE9, 0xFF, 0xFF, 0xFF, 0xFF
                };
                static BasicCodecave a(osig, sizeof(osig));
                static BasicCodecave b(osig, sizeof(osig));

                write_code_any_value(a.data + 3, *reinterpret_cast<int *>(vertical_1_sig.address() + 3));
                write_code_any_value(b.data + 3, *reinterpret_cast<int *>(vertical_2_sig.address() + 3));

                write_code_any_value(a.data + 7 + 2, &value);
                write_code_any_value(b.data + 7 + 2, &value);

                write_code_any_value(vertical_1_sig.address(), static_cast<unsigned char>(0xE9));
                write_code_any_value(vertical_1_sig.address() + 5, static_cast<unsigned short>(0x9090));
                write_code_any_value(vertical_1_sig.address() + 1, reinterpret_cast<int>(a.data) - reinterpret_cast<int>(vertical_1_sig.address() + 5));
                write_code_any_value(a.data + 7 + 6 + 1, reinterpret_cast<int>(vertical_1_sig.address() + 5) - reinterpret_cast<int>(a.data + 7 + 6 + 5));

                write_code_any_value(vertical_2_sig.address(), static_cast<unsigned char>(0xE9));
                write_code_any_value(vertical_2_sig.address() + 5, static_cast<unsigned short>(0x9090));
                write_code_any_value(vertical_2_sig.address() + 1, reinterpret_cast<int>(b.data) - reinterpret_cast<int>(vertical_2_sig.address() + 5));
                write_code_any_value(b.data + 7 + 6 + 1, reinterpret_cast<int>(vertical_2_sig.address() + 5) - reinterpret_cast<int>(b.data + 7 + 6 + 5));
            }
            else {
                vertical_1_sig.undo();
                vertical_2_sig.undo();
            }
            value = new_value;
        }
    }
    if(value) console_out(std::to_string(value));
    else console_out("off");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
