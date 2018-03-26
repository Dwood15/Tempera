/*
 * Insert GPLv3 license
 */
#pragma once
#pragma pack(push)
#pragma pack(1)

#include "../shitty_macros.h"
#include "../ce_base_types.h"
#include "../shitty_enums.h"

STAT_ASSRT(sbyte, 1);

struct s_custom_blip {
	sbyte x;
	sbyte y;
};

struct s_blip : s_custom_blip {
	//end custom blip location
	blip_type type; // set to _blip_type_none when not used
	sbyte     size;    // a la object's size (tiny, large, etc)
}; STAT_ASSRT(s_blip, 0x4);

struct s_team_data {
	s_blip object_blips[MAX_CUSTOM_BLIPS];            // objects belonging to just this team

	struct {
		s_custom_blip goal_blips[MAX_CUSTOM_BLIPS]; //0x10
		sbyte         goal_indices[MAX_CUSTOM_BLIPS];       //0x10 indices to the game engine's global goals
	}      game_engine;

	real_vector2d world_position;
	int32         unk_int; // game time related
	real          unk_float; // related to calculations with objects and facing angle of the player
	sbyte         active_object_blips_count;
	unsigned char  : 8;
	unsigned short : 16;
}; STAT_ASSRT(s_team_data, 0x84);

struct s_local_player {
	s_team_data nearby_team_objects[k_number_of_game_teams]; //0x0
	datum_index nearby_object_indexes[MAX_CUSTOM_BLIPS];
}; STAT_ASSRT(s_local_player, 0x568);

struct s_motion_sensor {
	s_local_player local_players[MAX_PLAYER_COUNT_LOCAL];

	struct {
		__int32 unk_type; // game time related
		__int16 current_object_blip_index;
		sbyte   padA;
		sbyte   padB;
		//Open Sauce has this as 2 bytes of pad, but that. There's 2 bytes on the front of the pointer. :)
	}              update_data;
}; STAT_ASSRT(s_motion_sensor, 0x8 + ( 0x568 * MAX_PLAYER_COUNT_LOCAL ));


struct s_network_game_player {
	wchar_t name[12];         // 0x0
	short   primary_color_index;   // 0x18
	short   icon_index;         // 0x1A
	sbyte   machine_index;      // 0x1C
	sbyte   controller_index;      // 0x1D
// These values will be the same on the client as they are on the server, so
// we can use them safely for player operations in multiplayer code
	sbyte   team_index;         // 0x1E
	sbyte   player_list_index;   // 0x1F
}; STAT_ASSRT(s_network_game_player, 0x20);

struct s_game_engine_state_message {
	long        message_index;
	datum_index message_player_index; // player to use in the message?

	// values used for rendering a target player's name
	datum_index target_player;
	long        target_time; // timer used to fade in the target player name
};

/*
struct s_player_datum {
	short salt;

	int16                  local_player_index;                     // 0x2
	wchar_t                display_name[12];                        //player_name_len + 1 // 0x4
	int32                  unused_unkA;                           // 0x1C
	union {                                    // 0x20
		long       team_index;
		argb_color color;
	};
	s_player_action_result nearest_object_action_result;// 0x24
	long                   respawn_ticks_remain;                     // 0x2C
	/*---------------
	int32                  unknown_typeA;                        // 0x30 respawn time growth related
	datum_index            slave_unit_index;                  // 0x34
	datum_index            last_slave_unit_index;               // 0x38
	int16                  cluster_index;                        // 0x3C
	bool                   weapon_swap_result;                     // 0x3E
	unsigned char : 8;//padding yo
	datum_index                 auto_aim_target;                  // 0x40 biped_index
	long                        auto_aim_update_ticks;               // 0x44
	/*struct_stopped_at
	s_network_game_player       network_player;   // 0x48
	long                        powerup_times[2]; //max # of player powerups   // 0x68
	real                        speed;                                 // 0x6C
	int32                       source_teleporter_netgame_index;            // 0x70 index to a netgame flag in the scenario, or NONE
	s_game_engine_state_message engine_state_messsage;   // 0x74
	long                        last_death_time;                  // 0x84 game tick of the last time this player was killed
	datum_index                 target_player_index;               // 0x88 player index of the slayer target for this player
	bool                        odd_man_out;                           // 0x8C
	unsigned char  : 8;
	unsigned short : 16;
	int32 unused_unkb;                           // 0x90
	short unknown_typeC;                           // 0x94
	short kills_this_lifetime;                     // 0x96 how many kills we've had in this lifetime (since the last spawn)
	short current_spree_count;                     // 0x98 how many kills we've had in our 'spree time' (4 second intervals max)
	short last_kill_time;                        // 0x9A set from game_time_globals::local_time
	union {                                    // 0x9C
		byte player_score_buffer[0x10];
		struct {
			int16 kills[4];                        // 0x9C
			int16 assists[4];                     // 0xA4
		}    player_score;
	};
	short friendly_fire_kills;                     // 0xAC
	short deaths;                              // 0xAE
	short suicides;                              // 0xB0
	int16 pad16A;
	UNUSED_32(int32);                           // 0xB4
	UNUSED_32(int32);                           // 0xB8
	UNUSED_32(int32);                           // 0xBC
	int16 team_kills;                           // 0xC0
	PAD16;
	u_player_multiplayer_stats multiplayer_stats;      // 0xC4
	int32                      telefrag_counter;                        // 0xCC # of ticks spent blocking teleporter
	game_ticks_t               quit_game_tick;                  // 0xD0 game tick the player quick at
	bool                       telefrag_enabled;                        // 0xD4 if we're blocking a teleporter, this will be true
	bool                       quit_out_of_game;                        // 0xD5
	PAD16;
	UNUSED_32(int32);                           // 0xD8
	int32 ping;                                 // 0xDC
	int32 team_kill_number;                        // 0xE0
	int32 team_kill_timer;                        // 0xE4

	union {                                    // 0xE8
		byte _unused_networking_buffer[0x200 - 0xE8];

		struct {
			Networking::s_player_client_update update_data;
			PAD32;                              // 0x1FC
		}    client;
		struct {
			Networking::s_player_server_update update_data;
			// !!! Only use this when the local machine is the host (ie, GameState::IsServer() == true) !!!
			s_player_yelo_server_data          yelo_data;      // 0x1C8
		}    server;
	};
};  STAT_ASSRT(s_player_datum, 0x200); // 0x160 in Stubbs
*/

#pragma pack(pop)