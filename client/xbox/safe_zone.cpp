#include "safe_zone.h"
#include <string.h>
#include "../hud_mod/offset_hud_elements.h"
#include "../messaging/messaging.h"
#include "../hooks/map_load.h"
#include "../hooks/tick.h"

static void mod_team_background() noexcept {
    for(size_t i=0;i<*reinterpret_cast<uint32_t *>(0x4044000C);i++) {
        auto &tag = HaloTag::from_id(i);
        if(tag.tag_class == 0x6269746D) {
            const char *satanic = "ui\\shell\\bitmaps\\team_background";
            const auto satanic_size = strlen(satanic);
            if(memcmp(tag.path,satanic,satanic_size) == 0) {
                tag.path++;
            }
            else if(memcmp(tag.path-1,satanic,satanic_size) == 0) {
                tag.path--;
            }
        }
    }
}

ChimeraCommandError safe_zones_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            static OffsetterIndex index;
            if(new_value) {
                add_map_load_event(mod_team_background);
                index = create_offsetter(33, 25, true);
            }
            else {
                remove_map_load_event(mod_team_background);
                destroy_offsetter(index);
            }
            mod_team_background();
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
