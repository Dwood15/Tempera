#include "offset_hud_elements.h"
#include "../halo_data/table.h"
#include "../hooks/map_load.h"
#include "../hooks/tick.h"
#include "../messaging/messaging.h"

#include <vector>
#include <string.h>
#include <math.h>

struct OriginalHudValue {
    HUDAnchor anchor;
    AnchorOffset *address;
    AnchorOffset original_value;
    AnchorOffset total_delta;
};

static std::vector<OriginalHudValue> original;

struct OffsetHudModMod {
    HUDAnchor anchor;
    AnchorOffset *address;
    AnchorOffset delta;
};

struct OffsetHudMod {
    std::vector<OffsetHudModMod> mods;
    char objects[65535] = {};
    OffsetterIndex index;
    AnchorOffset delta;
    bool text;

    OffsetHudMod(short x, short y, bool text) {
        static OffsetterIndex next_index = OFFSETTER_INDEX_NULL;
        this->delta = {x,y};
        this->index = ++next_index;
        this->text = text;
    }
};

static std::vector<OffsetHudMod> all_mods;

static void change_xy(AnchorOffset &addr, HUDAnchor anchor, OffsetHudMod &mod) {
    OriginalHudValue *v = nullptr;

    for(size_t o=0;o<original.size();o++) {
        if(original[o].address == &addr) {
            v = &original[o];
            break;
        }
    }

    if(!v) {
        OriginalHudValue new_val;
        new_val.anchor = anchor;
        new_val.address = &addr;
        new_val.original_value = addr;
        new_val.total_delta = {};
        original.push_back(new_val);
        v = &original[original.size() - 1];
    }

    AnchorOffset delta = mod.delta;

    #define X_OFFSET 150
    #define Y_OFFSET 140

    int x_offset_from_center = v->original_value.x;
    int y_offset_from_center = v->original_value.y;

    if(anchor != ANCHOR_CENTER) {
        x_offset_from_center -= 320;
        y_offset_from_center -= 240;
    }

    if(x_offset_from_center > 0) delta.x *= -1;
    if(y_offset_from_center > 0) delta.y *= -1;

    if(anchor == ANCHOR_TOP_LEFT || anchor == ANCHOR_TOP_RIGHT) {
        y_offset_from_center *= -1;
    }
    if(anchor == ANCHOR_TOP_RIGHT) {
        x_offset_from_center *= -1;
    }
    if(anchor == ANCHOR_BOTTOM_RIGHT) {
        x_offset_from_center *= -1;
    }

    if(x_offset_from_center >= -X_OFFSET && x_offset_from_center <= X_OFFSET) delta.x = 0;
    if(y_offset_from_center >= -Y_OFFSET && y_offset_from_center <= Y_OFFSET) delta.y = 0;

    if((x_offset_from_center == -188 || x_offset_from_center == 164) && y_offset_from_center == 116) {
        delta.x = 0;
        delta.y = 0;
    }

    if(distance_squared(0,0,x_offset_from_center,y_offset_from_center) < 195*195) {
        delta.x = 0;
        delta.y = 0;
    }

    v->total_delta.x += delta.x;
    v->total_delta.y += delta.y;

    OffsetHudModMod mod_mod;
    mod_mod.anchor = anchor;
    mod_mod.delta = delta;
    mod_mod.address = &addr;
    addr.x += delta.x;
    addr.y += delta.y;
    mod.mods.push_back(mod_mod);
}

static void change_xy(HUDElementPosition &addr, HUDAnchor anchor, OffsetHudMod &mod) {
    return change_xy(addr.anchor_offset, anchor, mod);
}

static void do_things_to_wphi_tag(HaloTagID tag_id, OffsetHudMod &mod) noexcept {
    if(tag_id.is_valid() && !mod.objects[tag_id.index]) {
        mod.objects[tag_id.index] = 1;
        auto &tdata = *reinterpret_cast<WeaponHUDInterface *>(HaloTag::from_id(tag_id).data);
        do_things_to_wphi_tag(tdata.child.tag_id, mod);
        for(int i=0;i<tdata.static_elements_count;i++) {
            change_xy(tdata.static_elements[i].position, tdata.anchor, mod);
        }
        for(int i=0;i<tdata.meter_elements_count;i++) {
            change_xy(tdata.meter_elements[i].position, tdata.anchor, mod);
        }
        for(int i=0;i<tdata.number_elements_count;i++) {
            change_xy(tdata.number_elements[i].position, tdata.anchor, mod);
        }
        for(int o=0;o<tdata.overlay_elements_count;o++) {
            auto &overlay = tdata.overlay_elements[o];
            for(int i=0;i<overlay.overlays_count;i++) {
                change_xy(overlay.overlays[i].position, tdata.anchor, mod);
            }
        }
    }
}

static void do_things_to_unhi_tag(HaloTagID tag_id, OffsetHudMod &mod) noexcept {
    if(tag_id.is_valid() && !mod.objects[tag_id.index]) {
        mod.objects[tag_id.index] = 1;
        auto &unhi_tag = HaloTag::from_id(tag_id);
        auto &tdata = *reinterpret_cast<UnitHUDInterface *>(unhi_tag.data);
        change_xy(tdata.unit_hud_background.position, tdata.anchor, mod);
        change_xy(tdata.shield_panel_background.position, tdata.anchor, mod);
        change_xy(tdata.shield_panel_meter.position, tdata.anchor, mod);
        change_xy(tdata.health_panel_background.position, tdata.anchor, mod);
        change_xy(tdata.health_panel_meter.position, tdata.anchor, mod);
        change_xy(tdata.motion_sensor_background.position, ANCHOR_BOTTOM_LEFT, mod);
        change_xy(tdata.motion_sensor_foreground.position, ANCHOR_BOTTOM_LEFT, mod);
        change_xy(tdata.motion_sensor_center, ANCHOR_BOTTOM_LEFT, mod);
        for(uint32_t i=0;i<tdata.auxiliary_hud_meters_count;i++) {
            change_xy(tdata.auxiliary_hud_meters[i].background.position, tdata.anchor, mod);
            change_xy(tdata.auxiliary_hud_meters[i].meter.position, tdata.anchor, mod);
        }
        for(uint32_t i=0;i<tdata.auxiliary_overlays.overlays_count;i++) {
            change_xy(tdata.auxiliary_overlays.overlays[i].position, tdata.auxiliary_overlays.anchor, mod);
        }
    }
}

static void do_things_to_weap_tag(HaloTagID tag_id, OffsetHudMod &mod) noexcept {
    if(!mod.objects[tag_id.index] && tag_id.is_valid() && *reinterpret_cast<uint16_t *>(HaloTag::from_id(tag_id).data) == 2) {
        mod.objects[tag_id.index] = 1;
        do_things_to_wphi_tag(*reinterpret_cast<HaloTagID *>(HaloTag::from_id(tag_id).data + 0x480 + 0xC), mod);
    }
}

static void do_things_to_unit_tag(HaloTagID tag_id, OffsetHudMod &mod) noexcept {
    if(tag_id.is_valid() && !mod.objects[tag_id.index] && *reinterpret_cast<uint16_t *>(HaloTag::from_id(tag_id).data) <= 1) {
        mod.objects[tag_id.index] = 1;
        auto &tag = HaloTag::from_id(tag_id);
        auto *&tag_data = tag.data;
        auto *&data_ptr = *reinterpret_cast<char **>(tag_data + 0x2A8 + 4);
        for(uint32_t h=0;h<*reinterpret_cast<uint32_t *>(tag_data + 0x2A8);h++) {
            auto &tag_id = *reinterpret_cast<HaloTagID *>(data_ptr + h * 48 + 0xC);
            do_things_to_unhi_tag(tag_id, mod);
        }

        for(uint32_t w=0;w<*reinterpret_cast<uint32_t *>(tag_data + 0x2D8);w++) {
            auto *weapon = *reinterpret_cast<char **>(tag_data + 0x2D8 + 4) + w * 36;
            do_things_to_weap_tag(*reinterpret_cast<HaloTagID *>(weapon + 0xC), mod);
        }

        for(uint32_t s=0;s<*reinterpret_cast<uint32_t *>(tag_data + 0x2E4);s++) {
            auto *seat = *reinterpret_cast<char **>(tag_data + 0x2E4 + 4) + s * 284;
            auto *&hud_ptr = *reinterpret_cast<char **>(seat + 0xDC + 4);
            for(uint32_t h=0;h<*reinterpret_cast<uint32_t *>(seat + 0xDC);h++) {
                do_things_to_unhi_tag(*reinterpret_cast<HaloTagID *>(hud_ptr + h * 48 + 0xC), mod);
            }
        }
    }
}

static void do_things_to_grhi_tag(HaloTagID tag_id, OffsetHudMod &mod) noexcept {
    if(tag_id.is_valid() && !mod.objects[tag_id.index]) {
        mod.objects[tag_id.index] = 1;
        auto &tag = HaloTag::from_id(tag_id);
        GrenadeHUDInterface &tdata = *reinterpret_cast<GrenadeHUDInterface *>(tag.data);
        change_xy(tdata.grenade_hud_background.position, tdata.anchor, mod);
        change_xy(tdata.total_grenades_background.position, tdata.anchor, mod);
        change_xy(tdata.total_grenades_numbers.position, tdata.anchor, mod);
        for(uint32_t o=0;o<tdata.overlays_count;o++) {
            change_xy(tdata.overlays[o].position, tdata.anchor, mod);
        }
    }
}

static void do_things_to_matg_tag(HaloTagID tag_id, OffsetHudMod &mod) noexcept {
    if(tag_id.is_valid() && !mod.objects[tag_id.index]) {
        auto *&tdata = HaloTag::from_id(tag_id).data;
        auto *&grenades = *reinterpret_cast<char **>(tdata + 0x128 + 4);
        for(size_t g=0;g<*reinterpret_cast<uint32_t *>(tdata + 0x128);g++) {
            do_things_to_grhi_tag(*reinterpret_cast<HaloTagID *>(grenades + g * 68 + 0x14 + 0xC), mod);
        }
    }
}

static void do_things_to_hudg_tag(HaloTagID tag_id, OffsetHudMod &mod) noexcept {
    if(tag_id.is_valid() && !mod.objects[tag_id.index]) {
        mod.objects[tag_id.index] = 1;
        auto *&tdata = HaloTag::from_id(tag_id).data;
        change_xy(*reinterpret_cast<AnchorOffset *>(tdata + 0x24), *reinterpret_cast<HUDAnchor *>(tdata), mod);
    }
}

static bool should_rerun_mod_load() {
    if(original.size() == 0) return true;
    for(size_t i=0;i<original.size();i++) {
        auto &o = original[i];
        if((o.address->x != (o.original_value.x + o.total_delta.x)) || (o.address->y != (o.original_value.y + o.total_delta.y))) {
            return true;
        }
    }
    return false;
}

static void map_load_mod(OffsetHudMod &mod) noexcept {
    memset(mod.objects, 0, sizeof(mod.objects));
    mod.mods.clear();
    for(size_t i=0;i<*reinterpret_cast<uint32_t *>(0x4044000C);i++) {
        auto &tag = HaloTag::from_id(i);
        if(tag.tag_class == 0x6D617467) {
            do_things_to_matg_tag(i, mod);
        }
        else if(mod.text && tag.tag_class == 0x68756467) {
            do_things_to_hudg_tag(i, mod);
        }
    }
}

static void on_map_load() noexcept {
    if(!should_rerun_mod_load()) return;
    original.clear();
    for(size_t i=0;i<all_mods.size();i++) {
        map_load_mod(all_mods[i]);
    }
}

static void on_tick() noexcept {
    HaloPlayer player;
    HaloObject object(player.object_id());
    auto *object_data = object.object_data();
    if(object_data) {
        auto &odata = *reinterpret_cast<BaseHaloObject *>(object_data);
        if(odata.object_type > 1) return;

        for(size_t i=0;i<all_mods.size();i++) {
            auto &mod = all_mods[i];
            do_things_to_unit_tag(odata.tag_id, mod);

            HaloObject vehicle_object(odata.parent_object_id);
            auto *vobject_data = vehicle_object.object_data();
            if(vobject_data) {
                auto &vdata = *reinterpret_cast<BaseHaloObject *>(vobject_data);
                if(vdata.object_type <= 1) {
                    do_things_to_unit_tag(vdata.tag_id, mod);
                }
            }

            auto weapon_object = HaloObject(odata.weapon_object_id);
            auto *weapon_data = weapon_object.object_data();
            if(weapon_data && *reinterpret_cast<uint16_t *>(weapon_data + 0xB4) == 2) {
                do_things_to_weap_tag(*reinterpret_cast<HaloTagID *>(weapon_data), mod);
            }
        }
    }
}

static bool hooks_made = false;

static void make_hooks() noexcept {
    hooks_made = true;
    add_map_load_event(on_map_load);
    add_tick_event(on_tick);
}

OffsetterIndex create_offsetter(short x, short y, bool text) noexcept {
    if(!hooks_made) make_hooks();
    all_mods.emplace_back(OffsetHudMod(x,y,text));
    map_load_mod(all_mods.back());
    return all_mods.back().index;
}

void destroy_offsetter(OffsetterIndex index) {
    if(index == OFFSETTER_INDEX_NULL) return;
    on_map_load();
    for(size_t i=0;i<all_mods.size();i++) {
        if(all_mods[i].index == index) {
            auto &mod = all_mods[i];
            for(size_t m=0;m<mod.mods.size();m++) {
                auto &mod_mod = mod.mods[m];
                mod_mod.address->x -= mod_mod.delta.x;
                mod_mod.address->y -= mod_mod.delta.y;
                for(size_t o=0;o<original.size();o++) {
                    if(mod_mod.address == original[o].address) {
                        original[o].total_delta.x -= mod_mod.delta.x;
                        original[o].total_delta.y -= mod_mod.delta.y;
                    }
                }
            }
            all_mods.erase(all_mods.begin() + i);
            return;
        }
    }
    throw std::exception();
}
