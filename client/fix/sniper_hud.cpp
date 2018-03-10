#include "sniper_hud.h"
#include "../../version.h"
#include "../open_sauce.h"
#include "../halo_data/resolution.h"

#include "../messaging/messaging.h"
#include "../hooks/map_load.h"
#include "../halo_data/tag_data.h"
#include "../halo_data/hud.h"
#include "../../math/data_types.h"

static void sniper_fix() noexcept {
    try {
        auto &tag = HaloTag::lookup("wphi", "weapons\\sniper rifle\\sniper rifle");
        WeaponHUDInterface &hud_interface = *reinterpret_cast<WeaponHUDInterface *>(tag.data);
        assert_or_bail(hud_interface.anchor == ANCHOR_TOP_LEFT);

        #define STATIC_ELEMENTS_COUNT 3
        assert_or_bail(hud_interface.static_elements_count == STATIC_ELEMENTS_COUNT);
        assert_or_bail(hud_interface.static_elements[0].multitexture_overlay_count == 0);

        auto &first_item = hud_interface.static_elements[1];
        auto &second_item = hud_interface.static_elements[2];

        assert_or_bail(first_item.multitexture_overlay_count == 1);
        assert_or_bail(second_item.multitexture_overlay_count == 1);

        assert_or_bail(first_item.position.anchor_offset.x == 92);
        assert_or_bail(first_item.position.anchor_offset.y == 85);
        assert_or_bail(second_item.position.anchor_offset.x == 445);
        assert_or_bail(second_item.position.anchor_offset.y == 85);

        assert_or_bail(first_item.multitexture_overlays->blend_function == HUDMultitextureOverlay::FRAMEBUFFER_BLEND_ALPHA_BLEND);
        assert_or_bail(second_item.multitexture_overlays->blend_function == HUDMultitextureOverlay::FRAMEBUFFER_BLEND_ALPHA_BLEND);

        assert_or_bail(first_item.multitexture_overlays->blending_function_0_to_1 == MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_MULTIPLY);
        assert_or_bail(second_item.multitexture_overlays->blending_function_0_to_1 == MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_MULTIPLY);

        assert_or_bail(first_item.multitexture_overlays->blending_function_1_to_2 == MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_ADD);
        assert_or_bail(second_item.multitexture_overlays->blending_function_1_to_2 == MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_ADD);

        first_item.multitexture_overlays->blending_function_0_to_1 = MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_SUBTRACT;
        second_item.multitexture_overlays->blending_function_0_to_1 = MULTITEXTURE_OVERLAY_BLENDING_FUNCTION_SUBTRACT;

        int16_t center_x = 320;
        int16_t right_offset = 0;
        int16_t left_offset = 0;

        if(open_sauce_present()) {
            auto &resolution = get_resolution();
            auto ar = static_cast<double>(resolution.width) / resolution.height;
            center_x = 320.0 / (4.0 / 3.0) * ar;
            if(ar != (4.0 / 3.0)) {
                right_offset = 34;
            }
            if(ar > (8.0 / 2.76217)) {
                left_offset = 32;
            }
        }

        first_item.position.anchor_offset.x = center_x - 188 + left_offset;
        first_item.position.anchor_offset.y = 124;
        first_item.position.height_scale = 0.89;

        second_item.position.anchor_offset.x = center_x + 164 + right_offset;
        second_item.position.anchor_offset.y = 124;
        second_item.position.height_scale = 0.89;

        first_item.colors.default_color = ColorByte(1.0F, 0.925F, 0.785F, 0.95F);
        first_item.colors.flashing_color = first_item.colors.default_color;
        first_item.colors.disabled_color = first_item.colors.default_color;
        second_item.colors.default_color = first_item.colors.default_color;
        second_item.colors.flashing_color = first_item.colors.default_color;
        second_item.colors.disabled_color = first_item.colors.default_color;
    }
    catch(...) {
    }
}

ChimeraCommandError sniper_hud_fix_command(size_t argc, const char **argv) noexcept {
    static bool active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            console_out("Changes will apply on next map load.");
            if(new_value) {
                add_map_load_event(sniper_fix, EVENT_PRIORITY_BEFORE);
            }
            else {
                remove_map_load_event(sniper_fix);
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
