#include "split_screen_hud.h"

#include "../client_signature.h"
#include "../hac2.h"
#include "../halo_data/hud.h"
#include "../halo_data/tag_data.h"
#include "../hooks/map_load.h"
#include "../messaging/messaging.h"

static void fix_hud() noexcept {
    try {
        HaloTag &tag = HaloTag::lookup("unit_hud_interface", "ui\\hud\\cyborg_mp");
        auto &unhi = *reinterpret_cast<UnitHUDInterface *>(tag.data);
        assert_or_bail(unhi.anchor == ANCHOR_TOP_RIGHT);
        assert_or_bail(unhi.auxiliary_overlays.anchor == ANCHOR_TOP_LEFT);
        assert_or_bail(unhi.unit_hud_background.position.anchor_offset.x == 0);
        assert_or_bail(unhi.unit_hud_background.position.anchor_offset.y == 0);
        assert_or_bail(unhi.shield_panel_background.position.anchor_offset.x == -7);
        assert_or_bail(unhi.shield_panel_background.position.anchor_offset.y == 1);
        assert_or_bail(unhi.shield_panel_meter.position.anchor_offset.x == 0);
        assert_or_bail(unhi.shield_panel_meter.position.anchor_offset.y == 0);
        assert_or_bail(unhi.health_panel_background.position.anchor_offset.x == 28);
        assert_or_bail(unhi.health_panel_background.position.anchor_offset.y == -3);
        assert_or_bail(unhi.health_panel_meter.position.anchor_offset.x == 29);
        assert_or_bail(unhi.health_panel_meter.position.anchor_offset.y == 11);

        unhi.shield_panel_background.position.scaling_flags.use_high_resolution_scale = 0;
        unhi.shield_panel_meter.position.scaling_flags.use_high_resolution_scale = 0;
        unhi.health_panel_background.position.scaling_flags.use_high_resolution_scale = 0;
        unhi.health_panel_meter.position.scaling_flags.use_high_resolution_scale = 0;

        for(size_t i=0;i<unhi.auxiliary_hud_meters_count;i++) {
            unhi.auxiliary_hud_meters[i].background.position.scaling_flags.use_high_resolution_scale = 0;
            unhi.auxiliary_hud_meters[i].meter.position.scaling_flags.use_high_resolution_scale = 0;
        }
    }
    catch(...) {}
}

ChimeraCommandError split_screen_hud_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            auto &ammo_counter_ss_sig = get_signature("ammo_counter_ss_sig");
            auto &hud_text_ss_sig = get_signature("hud_text_ss_sig");
            auto &split_screen_hud_ss_sig = get_signature("split_screen_hud_ss_sig");
            auto &hac2_workaround_ss_sig = get_signature("hac2_workaround_ss_sig");
            if(new_value) {
                const short ammo_counter_mod[] = {-1,   0xB8, 0x02, 0x00};
                const short hud_text_mod[] = {-1,   0xB8, 0x02, 0x00};
                const short split_screen_hud_mod[] = {-1,   -1,   -1,   -1,   0x00};
                write_code_s(ammo_counter_ss_sig.address(), ammo_counter_mod);
                write_code_s(hud_text_ss_sig.address(), hud_text_mod);
                write_code_s(split_screen_hud_ss_sig.address(), split_screen_hud_mod);

                // workaround to keep Halo from crashing if HAC2 is present when split screen is enabled
                if(hac2_present()) {
                    static unsigned char bytecode[] = {
                        // cmp eax, 0x00
                        0x83, 0xF8, 0x00,

                        // jne :continue_fn
                        0x75, 0x01,

                        // ret
                        0xC3,

                        // :continue_fn
                        // sub esp, 0x08
                        0x83, 0xEC, 0x08,

                        // push ebx
                        0x53,

                        // push ebp
                        0x55,

                        // jmp function()+5
                        0xE9, 0xFF, 0xFF, 0xFF, 0xFF
                    };

                    auto *hac2_workaround_ss_address = hac2_workaround_ss_sig.address();
                    *reinterpret_cast<uint32_t *>(bytecode + 11 + 1) = reinterpret_cast<uint32_t>(hac2_workaround_ss_address + 5) - reinterpret_cast<uint32_t>(bytecode + 11 + 5);
                    write_code_any_value(hac2_workaround_ss_address + 0, static_cast<unsigned char>(0xE9));
                    write_code_any_value(hac2_workaround_ss_address + 1, reinterpret_cast<uint32_t>(bytecode) - reinterpret_cast<uint32_t>(hac2_workaround_ss_address + 5));

                    add_map_load_event(fix_hud, EVENT_PRIORITY_BEFORE);
                    console_out("Changes will fully take effect on next map load.");
                }
            }
            else {
                ammo_counter_ss_sig.undo();
                hud_text_ss_sig.undo();
                split_screen_hud_ss_sig.undo();

                if(hac2_present()) {
                    hac2_workaround_ss_sig.undo();
                }

                remove_map_load_event(fix_hud);
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
