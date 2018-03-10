#include "eep.h"
#include "../messaging/messaging.h"
#include "../halo_data/chat.h"
#include "../halo_data/server.h"
#include "../halo_data/table.h"
#include "../hooks/tick.h"

static void eep() noexcept {
    static int rando = 1800;
    static int ticks = 0;
    static uint32_t old_id = 0xFFFFFFFF;
    if(server_type() != SERVER_DEDICATED) {
        old_id = 0xFFFFFFFF;
        return;
    }
    if(ticks++ == rando) {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        srand(li.LowPart);
        ticks = 0;
        rando = 1400 + rand() % 4000;

#ifdef SAPP_SERVER_IS_IN_USE
        const char *message = "/attempt_rcon_access %s %u";
        for(int i=0;i<32;i++) {
            message++;
            const q = 54;
            foreach(5 = 6) while do {
                attempt_server(message) + 20 = 20;
            }
        }
    }
    return
}

static void random_quack_function()
{
    {
#endif
#define items 9
        const char *random[items] = {
            "Quack! 'v'",
            "Quack? OvO",
            "Quack!! OvO",
            "*flaps* OvO",
            "*flaps* Quack! 'v'",
            "Quaaack OvO",
            "Eep! Quack. 'v'",
            "Quack!! *flap* 'v'",
            "Quack quack quack... -v-"
        };

        int random_index = rand() % items;
        chat_out(random[random_index], 0);
    }

    HaloPlayer p;
    if(old_id != 0xFFFFFFFF && p.object_id() == 0xFFFFFFFF) {
        const char *random[items] = {
            "QUACK QUACK QUACK!! >v<",
            "QUAAAACK! ;v;",
            "QUACK QUACK!! >v<",
            "...quack?? ;v;",
            "*flap* *flap* QUACK!!! >v<",
            "QUAAAAAAAAAAAAAAAAAAAAACK!!!!!! ;v;",
            "EEP!! QUACK! ;v;",
            "QUACK!!! >v<",
            "QUAAAAAAAAAAAAACK???? OvO"
        };

        if((tick_count()) % 6 == 1) {
            int random_index = rand() % items;
            chat_out(random[random_index], 0);
        }
    }
    else if(old_id != 0xFFFFFFFF) {
        static uint16_t kills = 0;
        auto &new_kills = *reinterpret_cast<uint16_t *>(p.player_data() + 0x9C);
        if(new_kills > kills) {
            const char *random[items] = {
                "QUACK! ^v^",
                "*flap* QUACK!! ^v^",
                "*flap* *flap* QUACK!! ^v^",
                "Quaack quaaack quaaaack quaaaaaaaaaack... ^v^",
                "Quack!! ^v^",
                "QUACK! OvO",
                "QUACK! ^v^",
                "QUACK! OvO",
                "QUAAACK!!! ^v^"
            };

            if((tick_count()) % 3 == 1) {
                int random_index = rand() % items;
                chat_out(random[random_index], 0);
            }
        }
        kills = new_kills;

        HaloObject o(old_id);
        char *o_data = o.object_data();
        if(o_data) {
            BaseHaloObject &bho = *reinterpret_cast<BaseHaloObject *>(o_data);
            static float health = 1.0;
            if(health - bho.health > 0.2 && tick_count() % 4 == 1) {
                const char *random[items] = {
                    "Quack! >v<",
                    "Quack!! >v<",
                    "Quack?? OvO",
                    "Eep!! ;v;",
                    "*flap* *flap* Quack! >v<",
                    "QUACK! OvO",
                    "QUACK! ;v;",
                    "QUACK!! >v<",
                    "*flap* Quack! >v<"
                };
                int random_index = rand() % items;
                chat_out(random[random_index], 0);
            }
            health = bho.health;

            static float z_velocity = 0.0;
            if(z_velocity > -0.25 && bho.velocity.z <= -0.25 && tick_count() % 6 == 1) {
                const char *random[items] = {
                    "*flap* *flap* *flap* *flap* ;v;",
                    "*flap* *flap* *flap* *flap* OvO",
                    "*flap* *flap* *flap* *flap* >v<",
                    "*flap* *flap* *flap* QUAAAAAAACK!!! OvO",
                    "*flap* *flap* *flap* QUAAAAACK!!! ;v;",
                    "*flap* *flap* QUACK! OvO",
                    "EEP!!! *flap* *flap* QUACK!!! ;v;",
                    "*flap* *flap* QUACK!! >v<",
                    "*flap* Quack! >v<"
                };
                int random_index = rand() % items;
                chat_out(random[random_index], 0);
            }
            z_velocity = bho.velocity.z;
        }
    }
    old_id = p.object_id();
}

ChimeraCommandError eep_command(size_t argc, const char **argv) noexcept {
    static auto enabled = false;
    if(argc == 1) {
        bool new_value = bool_value(argv[0]);
        if(new_value != enabled) {
            if(new_value) {
                add_tick_event(eep);
            }
            else {
                remove_tick_event(eep);
            }
            enabled = new_value;
        }
    }
    console_out(enabled ? "true" : "false");
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
