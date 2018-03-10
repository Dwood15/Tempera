#include "firing_particle.h"
#include <string.h>
#include "../halo_data/table.h"
#include "../halo_data/tag_data.h"
#include "../messaging/messaging.h"
#include "../hooks/tick.h"

static char *objects;
static int32_t ticks_old;

struct FiringParticleMod {
    uint32_t *address;
    uint32_t old_value;
};
static std::vector<FiringParticleMod> mods;

static void block_firing_particles() {
    auto ticks = tick_count();
    if(ticks < ticks_old) {
        memset(objects, 0, 65535);
        mods.clear();
    }
    ticks_old = ticks;
    HaloPlayer player;
    HaloObject object(player.object_id());
    auto *object_data = object.object_data();
    if(object_data) {
        auto &weapon = *reinterpret_cast<uint32_t *>(object_data + 0x2F8 + *reinterpret_cast<uint16_t *>(object_data + 0x2F2) * 4);
        auto object = HaloObject(weapon);
        auto *object_data = object.object_data();
        if(object_data && *reinterpret_cast<uint16_t *>(object_data + 0xB4) == 2) {
            auto &weapon_tag_id = *reinterpret_cast<HaloTagID *>(object_data);
            if(!objects[weapon_tag_id.index]) {
                objects[weapon_tag_id.index] = true;
                auto *&weapon_tag_data = HaloTag::from_id(weapon_tag_id).data;
                auto &triggers_count = *reinterpret_cast<uint32_t *>(weapon_tag_data + 0x4FC);
                auto *&triggers_data = *reinterpret_cast<char **>(weapon_tag_data + 0x4FC + 4);
                for(auto t=0;t<triggers_count;t++) {
                    auto *trigger = triggers_data + t * 276;
                    auto &firing_effects_count = *reinterpret_cast<uint32_t *>(trigger + 0x108);
                    auto *&firing_effects_data = *reinterpret_cast<char **>(trigger + 0x108 + 4);
                    for(auto f=0;f<firing_effects_count;f++) {
                        auto *firing_effect = firing_effects_data + f * 132;
                        for(auto i=0;i<6;i++) {
                            auto &firing_effect_tag = *reinterpret_cast<HaloTagID *>(firing_effect + 0x24 + 0x10 * i + 0xC);
                            if(firing_effect_tag.is_valid() && !objects[firing_effect_tag.index] && *reinterpret_cast<uint32_t *>(firing_effect + 0x24 + 0x10 * i) == 0x65666665) {
                                objects[firing_effect_tag.index] = true;
                                auto *&effect_tag_data = HaloTag::from_id(firing_effect_tag).data;
                                auto &events_count = *reinterpret_cast<uint32_t *>(effect_tag_data + 0x34);
                                auto *&events_data = *reinterpret_cast<char **>(effect_tag_data + 0x34 + 4);
                                for(auto e=0;e<events_count;e++) {
                                    auto &data = *reinterpret_cast<uint32_t *>(events_data + e * 68 + 0x38);
                                    mods.emplace_back(FiringParticleMod {&data, data});
                                    data = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

ChimeraCommandError block_firing_particles_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            if(new_value) {
                objects = new char[65535]();
                ticks_old = tick_count() + 2;
                add_tick_event(block_firing_particles);
                block_firing_particles();
            }
            else {
                delete[] objects;
                remove_tick_event(block_firing_particles);
                for(size_t i=0;i<mods.size();i++) {
                    *mods[i].address = mods[i].old_value;
                }
                mods.clear();
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
