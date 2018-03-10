#include "budget.h"
#include <ios>
#include <string.h>
#include <math.h>
#include "../command/console.h"
#include "../messaging/messaging.h"
#include "../hooks/frame.h"
#include "../hooks/tick.h"
#include "../client_signature.h"
#include "../halo_data/table.h"

static int active = 0;

#define buffer_size 30
static size_t frames_per_tick[buffer_size];
static double s_per_tick[buffer_size];

static void buffer_ticks() noexcept {
    for(size_t i=buffer_size - 1;i>0;i--) {
        frames_per_tick[i] = frames_per_tick[i-1];
        s_per_tick[i] = s_per_tick[i-1];
    }
}

static void calculate_fps() noexcept {
    frames_per_tick[0]++;
}

static void change_colorargb(double val, double max, ColorARGB &color, bool invert) noexcept {
    color.alpha = 1.0;
    color.blue = 0.0;
    double halfway = max / 2.0;
    if(!invert) {
        if(val <= halfway) {
            color.green = 1.0;
            color.red = val / halfway;
        }
        else {
            color.red = 1.0;
            color.green = 1.0 - (val - halfway) / halfway;
        }
    }
    else {
        if(val <= halfway) {
            color.red = 1.0;
            color.green = val / halfway;
        }
        else {
            color.green = 1.0;
            color.red = 1.0 - (val - halfway) / halfway;
        }
    }
    if(color.green > 1.0) color.green = 1.0;
    if(color.green < 0.0) color.green = 0.0;
    if(color.red > 1.0) color.red = 1.0;
    if(color.red < 0.0) color.red = 0.0;
}

static void change_colorargb_d(double val, double base, ColorARGB &color, double max_distance) noexcept {
    color.alpha = 1.0;
    color.blue = 0.0;
    double d_from_m = fabs(val - base);
    double halfway = max_distance / 2.0;
    if(d_from_m < halfway) {
        color.green = 1.0;
        color.red = d_from_m / halfway;
    }
    else {
        color.red = 1.0;
        color.green = 1.0 - (d_from_m - halfway) / halfway;
    }

    if(color.green > 1.0) color.green = 1.0;
    if(color.green < 0.0) color.green = 0.0;
    if(color.red > 1.0) color.red = 1.0;
    if(color.red < 0.0) color.red = 0.0;

}

static void show_budget() noexcept {
    static LARGE_INTEGER previous_time;
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    s_per_tick[0] = counter_time_elapsed(previous_time, now_time);
    previous_time = now_time;
    static bool clear = false;
    double total_time = 0;
    size_t total_frames = 0;

    for(size_t i=0;i<buffer_size;i++) {
        total_time += s_per_tick[i];
        total_frames += frames_per_tick[i];
    }

    if(!console_is_out()) {
        for(size_t i=0;i<30;i++) {
            console_out("|n");
        }

        double framerate = static_cast<double>(total_frames) / total_time;
        auto etr = effective_tick_rate();

        #define PERCENT_SPACER "   "

        if(active == 1)
            console_out("Metric|t     Value|tMax|t%" PERCENT_SPACER);
        else
            console_out("Metric|t     Value|tMax (Stock Halo)|t%" PERCENT_SPACER);
        ColorARGB f;
        f.alpha = 0.7;
        f.red = 0.7;
        f.blue = 0.7;
        f.green = 0.7;
        char s[256] = {};

        static auto *vis_object_addr = get_signature("visible_object_count_sig").address();
        auto visible_objects = **reinterpret_cast<uint16_t **>(vis_object_addr + 2);
        auto max_visible_objects = active == 1 ? *reinterpret_cast<uint32_t *>(vis_object_addr + 18) : 256;
        change_colorargb(visible_objects, max_visible_objects, f, false);
        sprintf(s,"Rendered objects|t     %u|t%u|t%.02f", visible_objects, max_visible_objects, 100.0 * visible_objects / max_visible_objects);
        console_out(s,f);

        static auto *bsp_polies_addr = get_signature("bsp_poly_count_sig").address();
        auto bsp_polies = **reinterpret_cast<uint16_t **>(bsp_polies_addr + 3);
        auto max_bsp_poly_count = active == 1 ? *reinterpret_cast<uint16_t *>(bsp_polies_addr - 8) : 16384;
        change_colorargb(bsp_polies, max_bsp_poly_count, f, false);
        sprintf(s,"Rendered SBSP polies|t     %u|t%u|t%.02f", bsp_polies, max_bsp_poly_count, 100.0 * bsp_polies / max_bsp_poly_count);
        console_out(s, f);

        auto &object_table = get_object_table();
        static auto object_table_max_count = active == 1 ? object_table.max_count : 1024;
        change_colorargb(object_table.count, object_table_max_count, f, false);
        sprintf(s,"Objects|t     %u|t%u|t%.02f",object_table.count, object_table_max_count, 100.0 * object_table.count / object_table_max_count);
        console_out(s, f);

        auto &effect_table = get_effect_table();
        static auto effect_table_max_count = active == 1 ? effect_table.max_count : 256;
        change_colorargb(effect_table.count, effect_table_max_count, f, false);
        sprintf(s,"Effects|t     %u|t%u|t%.02f",effect_table.count, effect_table_max_count, 100.0 * effect_table.count / effect_table_max_count);
        console_out(s, f);

        auto &light_table = get_light_table();
        static auto light_table_max_count = active == 1 ? light_table.max_count : 896;
        change_colorargb(light_table.count, light_table_max_count, f, false);
        sprintf(s,"Lights|t     %u|t%u|t%.02f",light_table.count, light_table_max_count, 100.0 * light_table.count / light_table_max_count);
        console_out(s, f);

        auto &decal_table = get_decal_table();
        static auto decal_table_max_count = active == 1 ? decal_table.max_count : 2048;
        change_colorargb(decal_table.count, decal_table_max_count, f, false);
        sprintf(s,"Decals|t     %u|t%u|t%.02f",decal_table.count, decal_table_max_count, 100.0 * decal_table.count / decal_table_max_count);
        console_out(s, f);

        auto &particle_table = get_particle_table();
        static auto particle_table_max_count = active == 1 ? particle_table.max_count : 1024;
        change_colorargb(particle_table.count, particle_table_max_count, f, false);
        sprintf(s,"Particles|t     %u|t%u|t%.02f",particle_table.count, particle_table_max_count, 100.0 * particle_table.count / particle_table_max_count);
        console_out(s, f);

        change_colorargb(framerate, etr * 2, f, true);
        sprintf(s,"FPS|t     %.02f|t%.02f+|t%.02f", framerate, etr, framerate/etr*100.0);
        console_out(s,f);

        double current_tps = buffer_size / total_time;
        change_colorargb_d(current_tps, etr, f, 0.12*etr);
        memset(s,0,sizeof(s));
        sprintf(s,"TPS|t     %.02f|t%.02f|t%.02f", current_tps, etr, current_tps/etr*100.0);
        console_out(s,f);
        clear = true;
    }
    else if(clear) {
        for(size_t i=0;i<30;i++) {
            console_out("|n");
        }
        clear = false;
    }

    buffer_ticks();
    frames_per_tick[0] = 0;
}

ChimeraCommandError budget_command(size_t argc, const char **argv) noexcept {
    if(argc == 1) {
        auto new_value = atoi(argv[0]);
        if(new_value > 2 || new_value < 0) new_value = 0;
        if(new_value != active) {
            if(new_value && !active) {
                add_tick_event(show_budget);
                add_frame_event(calculate_fps);
                memset(frames_per_tick,0,sizeof(frames_per_tick));
                memset(s_per_tick,0,sizeof(s_per_tick));
            }
            else if(new_value == 0) {
                remove_frame_event(calculate_fps);
                remove_tick_event(show_budget);
            }
        }
        active = new_value;
    }
    console_out(std::to_string(active));
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}

template <class T>
static std::string int_to_hex(T i) {
    char buff[2 + sizeof(i) + 1];
    sprintf(buff, "0x%X", i);
    return std::string(buff);
}

ChimeraCommandError player_info_command(size_t argc, const char **argv) noexcept {
    HaloPlayer player;
    console_out(std::string("Player index: ") + std::to_string(player.index()));
    auto *data = player.player_data();
    console_out(std::string("Player data address: ") + int_to_hex(data));
    auto object_id = player.object_id();
    auto object = HaloObject(object_id);
    auto *object_data = object.object_data();
    if(object_data) {
        console_out(std::string("Player object ID: ") + int_to_hex(object_id));
        console_out(std::string("Player object data address: ") + int_to_hex(object_data));
    }
    return CHIMERA_COMMAND_ERROR_SUCCESS;
}
