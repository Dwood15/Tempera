#include "fov_fix.h"
#include "../client_signature.h"
#include "../hooks/frame.h"
#include "../messaging/messaging.h"
#include "../halo_data/resolution.h"

static bool fov_fix_active = false;

static void fix_disparity() noexcept {
    static auto *fov_disparity_1_addr = get_signature("fov_disparity_1_sig").address();
    static auto *fov_disparity_2_addr = get_signature("fov_disparity_2_sig").address();
    **reinterpret_cast<uint16_t **>(fov_disparity_1_addr + 3) = 0;
    **reinterpret_cast<uint16_t **>(fov_disparity_1_addr + 0x10 + 3) = get_resolution().height;
    **reinterpret_cast<uint16_t **>(fov_disparity_2_addr + 3) = 0;
    **reinterpret_cast<uint16_t **>(fov_disparity_2_addr + 0xB + 3) = get_resolution().width;
}

void fov_fix_ss() noexcept {
    extern bool simple_score_screen_active;
    if(!fov_fix_active || simple_score_screen_active) return;
    auto *ss_elements_addr = get_signature("ss_elements_sig").address();
    write_code_any_value(ss_elements_addr + 0, static_cast<unsigned char>(0xB9));
    write_code_any_value(ss_elements_addr + 1, 0x80008);
    write_code_any_value(ss_elements_addr + 5, static_cast<unsigned char>(0x90));
}

ChimeraCommandError fov_fix_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(fov_fix_active != new_value) {
            auto &fov_fix_sig = get_signature("fov_fix_sig");
            auto &fov_disparity_3_sig = get_signature("fov_disparity_3_sig");
            fov_fix_active = new_value;
            if(new_value) {
                const unsigned char nop[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
                write_code_c(fov_fix_sig.address(), nop);
                add_frame_event(fix_disparity);

                const unsigned char a[] = { 0x66, 0x83, 0xEA, 0x08, 0x90, 0x90, 0x90 };
                write_code_c(fov_disparity_3_sig.address(), a);

                const unsigned char b[] = { 0x83, 0xE9, 0x08, 0x90, 0x90, 0x90 };
                write_code_c(fov_disparity_3_sig.address() + 0xD, b);

                fov_fix_ss();
            }
            else {
                fov_fix_sig.undo();
                fov_disparity_3_sig.undo();
                auto *fov_disparity_1_addr = get_signature("fov_disparity_1_sig").address();
                auto *fov_disparity_2_addr = get_signature("fov_disparity_2_sig").address();
                **reinterpret_cast<uint16_t **>(fov_disparity_1_addr + 3) = 8;
                **reinterpret_cast<uint16_t **>(fov_disparity_1_addr + 0x10 + 3) = get_resolution().height - 8;
                **reinterpret_cast<uint16_t **>(fov_disparity_2_addr + 3) = 8;
                **reinterpret_cast<uint16_t **>(fov_disparity_2_addr + 0xB + 3) = get_resolution().width - 8;
                extern bool simple_score_screen_active;
                if(!simple_score_screen_active) {
                    get_signature("ss_elements_sig").undo();
                }
                remove_frame_event(fix_disparity);
            }
        }
    }
    console_out(fov_fix_active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
