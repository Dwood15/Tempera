#pragma once

constexpr unsigned short MAX_PLAYER_COUNT_LOCAL = 2;
constexpr short          MAX_GAMEPAD_COUNT      = 6;
constexpr short          MAX_PLAYERS_IN_GAME    = 16;
constexpr short          MAX_CUSTOM_BLIPS       = 16;

namespace Yelo::Enums {
	constexpr int k_maximum_number_of_local_players = 4;
	constexpr int k_multiplayer_maximum_players     = 16;

	// for team_data (which has a maximum of 16 datums) and such.
	// This is 1 in Stubbs, I'm positive team_data uses k_multiplayer_maximum_players.
	constexpr int k_multiplayer_maximum_teams = k_multiplayer_maximum_players;
};

//0x006AC4DC; Halo PC 1.07
#define FOV_XBOUND 0.024f
#define FOV_YBOUND 0.012f
#define OBJECT_CLIP 17
