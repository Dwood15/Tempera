#include "show_spawn.h"

#include <math.h>
#include <string.h>

#include "../halo_data/tag_data.h"
#include "../halo_data/server.h"
#include "../messaging/messaging.h"
#include "../hooks/tick.h"
#include "../halo_data/spawn_object.h"
#include "../halo_data/table.h"

static int32_t tickles;

enum TagScenarioSpawnGametype : uint16_t {
    SPAWN_GAMETYPE_NONE = 0,
    SPAWN_GAMETYPE_CTF,
    SPAWN_GAMETYPE_SLAYER,
    SPAWN_GAMETYPE_ODDBALL,
    SPAWN_GAMETYPE_KING,
    SPAWN_GAMETYPE_RACE,
    SPAWN_GAMETYPE_TERMINATOR,
    SPAWN_GAMETYPE_STUB,
    SPAWN_GAMETYPE_IGNORED1,
    SPAWN_GAMETYPE_IGNORED2,
    SPAWN_GAMETYPE_IGNORED3,
    SPAWN_GAMETYPE_IGNORED4,
    SPAWN_GAMETYPE_ALL_GAMES,
    SPAWN_GAMETYPE_ALL_GAMES_EXCEPT_CTF,
    SPAWN_GAMETYPE_ALL_GAMES_EXCEPT_RACE_AND_CTF
};

struct TagScenarioPlayerStartingLocation {
    Vector3D position;
    /// (radians)
    float rotation;
    uint16_t team_index;
    uint16_t bsp_index;
    TagScenarioSpawnGametype type0;
    TagScenarioSpawnGametype type1;
    TagScenarioSpawnGametype type2;
    TagScenarioSpawnGametype type3;
    char unknown[0x18];

    bool can_spawn_in_gametype(const Gametype &gametype) noexcept {
        for(int i=0;i<4;i++) {
            bool x;
            switch(*(&this->type0 + i)) {
                case SPAWN_GAMETYPE_NONE:
                    x = false;
                    break;
                case SPAWN_GAMETYPE_CTF:
                    x = gametype == GAMETYPE_CTF;
                    break;
                case SPAWN_GAMETYPE_SLAYER:
                    x = gametype == GAMETYPE_SLAYER;
                    break;
                case SPAWN_GAMETYPE_ODDBALL:
                    x = gametype == GAMETYPE_ODDBALL;
                    break;
                case SPAWN_GAMETYPE_KING:
                    x = gametype == GAMETYPE_KING;
                    break;
                case SPAWN_GAMETYPE_RACE:
                    x = gametype == GAMETYPE_RACE;
                    break;
                case SPAWN_GAMETYPE_ALL_GAMES:
                    x = true;
                    break;
                case SPAWN_GAMETYPE_ALL_GAMES_EXCEPT_CTF:
                    x = gametype != GAMETYPE_CTF;
                    break;
                case SPAWN_GAMETYPE_ALL_GAMES_EXCEPT_RACE_AND_CTF:
                    x = (gametype != GAMETYPE_CTF) && (gametype != GAMETYPE_RACE);
                    break;
                default:
                    x = false;
                    console_out("default");
                    break;
            }
            if(x) return true;
        }
        return false;
    }
};

struct ObjectEntry {
    uint32_t object_id;
    uint32_t spawn_id;
};

#define max_spawns 256
static ObjectEntry objects[256];
static int total_spawns;
static TagScenarioPlayerStartingLocation *player_spawn_data;

static void show_spawns() noexcept {
    auto stype = server_type();
    if(stype == SERVER_NONE) return;
    static bool biped;
    auto ticks = tick_count();
    if(ticks < tickles) {
        total_spawns = 0;
    }
    if(ticks < tickles || tickles == -1) {
        auto *&tag_data = HaloTag::from_id(*reinterpret_cast<HaloTagID *>(0x40440004)).data;
        auto &player_spawn_count = *reinterpret_cast<uint32_t *>(tag_data + 0x354);
        player_spawn_data = *reinterpret_cast<TagScenarioPlayerStartingLocation **>(tag_data + 0x354 + 4);
        if(player_spawn_count > 256) {
            tickles = ticks;
            return;
        }
        auto marker = 0xFFFFFFFF;

        auto *&tags = *reinterpret_cast<HaloTag **>(0x40440000);
        for(size_t i=0;i<*reinterpret_cast<uint32_t *>(0x4044000C);i++) {
            auto &tag = tags[i];
            if(tag.tag_class == 0x7363656E && strcmp(tag.path,"scenery\\overflow_spawn_marker\\overflow_spawn_marker") == 0) {
                marker = tag.id;
                biped = false;
                break;
            }
        }

        if(marker == 0xFFFFFFFF) {
            biped = true;
            for(size_t i=0;i<*reinterpret_cast<uint32_t *>(0x4044000C);i++) {
                auto &tag = tags[i];
                if(tag.tag_class == 0x6D617467 && strcmp(tag.path,"globals\\globals") == 0) {
                    auto &player_info_count = *reinterpret_cast<uint32_t *>(tag.data + 0x170);
                    auto *&player_info = *reinterpret_cast<char **>(tag.data + 0x174);
                    if(player_info_count) {
                        marker = *reinterpret_cast<HaloTagID *>(player_info + 0xC);
                        auto &mpinfo_count = *reinterpret_cast<uint32_t *>(tag.data + 0x164);
                        auto *&mpinfo = *reinterpret_cast<char **>(tag.data + 0x168);
                        if(mpinfo_count) {
                            if(marker == *reinterpret_cast<uint32_t *>(mpinfo + 0x10 + 0xC)) {
                                tickles = ticks;
                                return;
                            }
                        }
                        else {
                            tickles = ticks;
                            return;
                        }
                    }
                    break;
                }
            }

            auto test_marker = HaloTagID(marker);
            if(test_marker.is_valid()) {
                auto *&unit_tag_data = HaloTag::from_id(test_marker).data;
                if(*reinterpret_cast<uint16_t *>(unit_tag_data) == 0) {
                    *reinterpret_cast<uint32_t *>(unit_tag_data + 0x70 + 0xC) = 0xFFFFFFFF;
                    auto &flags = *reinterpret_cast<uint16_t *>(unit_tag_data + 0x2);
                    if(!(flags & 4)) {
                        flags += 4;
                    }
                    if(!(flags & 1)) {
                        flags += 1;
                    }
                    auto &unit_flags = *reinterpret_cast<uint32_t *>(unit_tag_data + 0x17C);
                    if(!(unit_flags & 65536)) {
                        unit_flags += 65536;
                    }
                    auto &biped_flags = *reinterpret_cast<uint32_t *>(unit_tag_data + 0x2F4);
                    if(!(biped_flags & 32)) {
                        biped_flags += 32;
                    }
                    *reinterpret_cast<float *>(unit_tag_data + 0x20) = 0;
                    *reinterpret_cast<float *>(unit_tag_data + 0x2D8) = 0;
                }
                else {
                    tickles = ticks;
                    return;
                }
            }
            else {
                tickles = ticks;
                return;
            }
        }


        for(auto i=0;i<player_spawn_count;i++) {
            auto &spawn = player_spawn_data[i];
            if(spawn.can_spawn_in_gametype(gametype())) {
                auto id = spawn_object(marker, spawn.position.x, spawn.position.y, spawn.position.z);
                auto &stored_spawn = objects[total_spawns++];
                stored_spawn.object_id = id;
                stored_spawn.spawn_id = i;
                HaloObject object(id);
                auto *data = object.object_data();
                if(data) {
                    auto &rotation = reinterpret_cast<BaseHaloObject *>(data)->orientation[0];
                    rotation.x = cos(spawn.rotation);
                    rotation.y = sin(spawn.rotation);
                    rotation.z = 0;
                }
            }
        }
    }

    auto *pdata = HaloPlayer().player_data();
    if(pdata) {
        auto &team = *reinterpret_cast<uint16_t *>(pdata + 0x20);
        Vector3D allies[16];
        Vector3D enemies[16];
        float weights[max_spawns];
        bool blocked[max_spawns] = {};
        size_t allies_count = 0;
        size_t enemies_count = 0;

        for(size_t p=0;p<16;p++) {
            HaloPlayer player(p);
            HaloObject player_object(player.object_id());
            auto *oplayer_data = player.player_data();
            auto *oplayer_object_data = player_object.object_data();
            if(oplayer_object_data) {
                auto &oteam = *reinterpret_cast<uint16_t *>(oplayer_data + 0x20);
                auto &location = reinterpret_cast<BaseHaloObject *>(oplayer_object_data)->position_script;
                if(oteam == team) {
                    allies[allies_count++] = location;
                }
                else {
                    enemies[enemies_count++] = location;
                }
            }
        }

        int friend_influenced = 0;
        auto ctf = gametype() == GAMETYPE_CTF;

        struct NoSpawnSphere {
            Vector3D position;
            float radius;
        };

        std::vector<NoSpawnSphere> nope;

        for(size_t i=0;i<2048;i++) {
            bool skip = false;
            for(size_t s=0;s<total_spawns;s++) {
                if((objects[s].object_id & 0xFFFF) == i) {
                    skip = true;
                    break;
                }
            }
            if(skip) continue;
            HaloObject o(i);
            char *odata = o.object_data();
            if(odata) {
                BaseHaloObject &bho = *reinterpret_cast<BaseHaloObject *>(odata);
                char *tag_data = HaloTag::from_id(bho.tag_id).data;
                if(*tag_data == 1 || *tag_data == 0) {
                    NoSpawnSphere nss;
                    nss.position = bho.position_script;
                    nss.radius = *reinterpret_cast<float *>(tag_data + 0x04);
                    nope.push_back(nss);
                }
            }
        }

        for(size_t i=0;i<total_spawns;i++) {
            auto &stored_spawn = objects[i];
            auto &spawn_data = player_spawn_data[stored_spawn.spawn_id];
            if(ctf && spawn_data.team_index != team) {
                blocked[i] = true;
                continue;
            }
            for(size_t n=0;n<nope.size();n++) {
                auto &sphere = nope[n];
                if(distance_squared(sphere.position, spawn_data.position) < sphere.radius*sphere.radius) {
                    blocked[i] = true;
                    break;
                }
            }
            if(blocked[i]) continue;
            HaloObject obj(stored_spawn.object_id);
            auto *odata = obj.object_data();
            if(odata) {
                auto &oobject = *reinterpret_cast<BaseHaloObject *>(odata);
                if(biped) {
                    oobject.health = 10000000000.0;
                    oobject.shield = 10000000000.0;
                    oobject.position = spawn_data.position;
                    oobject.velocity = {};
                }

                float enemy_weight = 1.0;
                float friend_weight = 1.0;
                const auto friend_weight_max = 4.0;

                for(size_t a=0;a<allies_count;a++) {
                    auto &ally = allies[a];
                    auto odistance = distance(ally, spawn_data.position);
                    if(odistance < 1.0) {
                        blocked[i] = true;
                    }
                    else if(odistance < 6.0) {
                        float weight = 1 - (odistance - 1) / 5;
                        friend_weight += weight * 3;
                        friend_influenced++;
                    }
                }
                if(friend_weight > friend_weight_max) friend_weight = friend_weight_max;

                if(stype == SERVER_LOCAL) {
                    for(size_t e=0;e<enemies_count;e++) {
                        auto &enemy = enemies[e];
                        auto odistance = distance(enemy, spawn_data.position);
                        if(odistance < 1.0) {
                            blocked[i] = true;
                        }
                        else if(odistance < 2.0) {
                            enemy_weight *= 0;
                        }
                        else if(odistance < 5.0) {
                            float weight = (odistance - 2) / 3;
                            enemy_weight *= weight;
                        }
                    }
                }

                auto weight = enemy_weight * friend_weight;
                if(blocked[i]) weight = 0.0;
                weights[i] = weight;
            }
        }

        float spawn_weight = 1.0 + 3.0 * allies_count;
        if(spawn_weight > 5.0) spawn_weight = 5.0;

        float chances[max_spawns];
        for(size_t i=0;i<total_spawns;i++) {
            if(weights[i] > 1.0) {
                chances[i] = weights[i] / spawn_weight;
                if(chances[i] > 1.0) chances[i] = 1.0;
            }
            else if (weights[i] == 1.0 ){
                chances[i] = 0.0;
            }
            else {
                chances[i] = -(1.0 - weights[i]);
            }
        }

        for(size_t i=0;i<total_spawns;i++) {
            auto &stored_spawn = objects[i];
            HaloObject obj(stored_spawn.object_id);
            auto *odata = obj.object_data();
            if(odata) {
                //auto spawn_chance = weights[i] / total_weight;
                auto &spawn_chance = chances[i];
                auto &oobject = *reinterpret_cast<BaseHaloObject *>(odata);
                auto &invis_if_biped = *reinterpret_cast<float *>(odata + 0x37C);

                ColorRGB color;
                color.blue = 0.0;
                if(blocked[i]) {
                    color.red = 0;
                    color.green = 0;
                    oobject.scale = 0.25;
                }
                else if(spawn_chance > 0) {
                    color.red = (1 - spawn_chance);
                    color.green = (spawn_chance) * 0.5 + 0.5;
                    if(friend_influenced == 1) {
                        color.blue = color.green * 0.8;
                    }
                    oobject.scale = 0.4 + 0.35 * spawn_chance;
                }
                else {
                    color.red = 1.0 + spawn_chance;
                    color.green = 0.0;
                    oobject.scale = 0.4 + spawn_chance * 0.4;
                }

                if(biped) {
                    if(blocked[i]) {
                        invis_if_biped += 0.35;
                        if(invis_if_biped > 1.1) invis_if_biped = 1.1;
                    }
                    else {
                        invis_if_biped -= 0.075;
                        if(invis_if_biped < 0.0) invis_if_biped = 0.0;
                    }
                }

                for(size_t i=0;i<4;i++) {
                    oobject.color1[i] = color;
                    oobject.color2[i] = color;
                }
            }
        }
    }
    tickles = ticks;
}

/// Function for command chimera_show_spawns
ChimeraCommandError show_spawns_command(size_t argc, const char **argv) noexcept {
    static auto active = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != active) {
            if(new_value) {
                tickles = -1;
                total_spawns = 0;
                add_tick_event(show_spawns);
            }
            else {
                for(size_t i=0;i<total_spawns;i++) {
                    delete_object(objects[i].object_id);
                }
                remove_tick_event(show_spawns);
            }
            active = new_value;
        }
    }
    console_out(active ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
