#pragma once

#include "player_types.h"

#define WIN32_LEAN_AND_MEAN

static short last_respawn_count       = 0x0;
static short last_spawn_count         = 0x0;
static short last_render_window_count = 0x0;

//Live-updating number of players game should spawn.
static short *to_respawn_count = (short *) 0x6B4802;

//Number of players to spawn - set on map initialization/unloading.
static short *spawn_count         = (short *) 0x624A9C;
///num windows to render. Fills with black for invalid.    f
static short *render_window_count = (short *) 0x6B4098;
static bool         *at_main_menu        = (bool *) 0x6B4051;

int __stdcall forgeMain();


