/**
 *  You should have received a copy of the GNU General Public License
 * along with Tempera. If not, see <http://www.gnu.org/licenses/>.
 **/
#pragma once
#pragma pack(push)
#pragma pack(1)

#include <cstdio>
#include "macros_generic.h"
#include "../ce_base_types.h"
#include "../headers/enums_generic.h"
#include "../headers/colors.h"
#include "../gamestate/headers/core.h"

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
}; STAT_ASSRT(s_motion_sensor, 0x8 + (0x568 * MAX_PLAYER_COUNT_LOCAL));


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

typedef int retIntGivenVoid(void);

static retIntGivenVoid *find_unused_local_player_index = (retIntGivenVoid *) 0x4762f0;

//s_players_globals_data
struct s_players_globals_data {
	long          unused_after_initialize_unk;                                                                // 0x0, initialized to NONE but that's all I can tell
	// the player_index for each local player
	datum_index   local_player_players[MAX_PLAYER_COUNT_LOCAL];        // 0x4
	// the object_index of each local player's dead unit (their old body)
	datum_index   local_player_dead_units[MAX_PLAYER_COUNT_LOCAL];    // 0x8
	__int16       local_player_count;                                                        // 0xC
	__int16       double_speed_ticks_remaining;                                                // 0xE
	bool          are_all_dead;                                                                // 0x10
	bool          input_disabled;                                                            // 0x11
	__int16       _bsp_switch_trigger_idx;                                                // 0x12
	__int16       respawn_failure;                                                            // 0x14
	bool          was_teleported;                                                            // 0x16, or respawned
	char          _unk_pad_char;
	//There's 16 players in a game.
	unsigned long combined_pvs[MAX_PLAYERS_IN_GAME];                        // 0x18 combined pvs of all players in the game
	//TODO: verify if combined_pvs_local is actually the same as regular combined_pvs - Maybe they're separate and synched via network?
	unsigned long combined_pvs_local[MAX_PLAYERS_IN_GAME];                    // 0x58 combined pvs of all local players

	static void DumpData(bool toConsole = false, Core *core = nullptr) {
		// const char *first_unused      = "First unused player Idx: %d";
		// const char *unused_after_init = "unused_after_initialize_unk %d\\n";
		// const char *local_plyr_cnt    = "local_player_count: %d\n";
		// const char *dbl_spd_tx        = "double speed tix remaining: %d\n";
		// const char *all_ded           = "Are all dead?: %d\n";
		// const char *inp_dis           = "Input disabled: %d\n";
		// const char *bsp_trig          = "BSP switch trigger: %d\n";
		//
		// DEBUG("Beginning to dump Data");
		// auto plyrCt          = this->local_player_count;
		// Sleep(30);
		// DEBUG("Dumping Data");
		//
		// int  unused_plyr_idx = find_unused_local_player_index();
		// Sleep(30);
		//
		// DEBUG(first_unused, unused_plyr_idx);
		// DEBUG(unused_after_init, this->unused_after_initialize_unk);
		// DEBUG(local_plyr_cnt, plyrCt);
		// DEBUG(dbl_spd_tx, this->double_speed_ticks_remaining);
		// DEBUG(all_ded, this->are_all_dead);
		// DEBUG(inp_dis, this->input_disabled);
		// DEBUG(bsp_trig, this->_bsp_switch_trigger_idx);
		//
		// if (toConsole) {
		// 	if(core != nullptr) {
		// 		core->ConsoleText(hGreen, first_unused, unused_plyr_idx);
		// 	}
		// 	printf(unused_after_init, this->unused_after_initialize_unk);
		// 	printf(local_plyr_cnt, plyrCt);
		// 	printf(dbl_spd_tx, this->double_speed_ticks_remaining);
		// 	printf(all_ded, this->are_all_dead);
		// 	printf(inp_dis, this->input_disabled);
		// 	printf(bsp_trig, this->_bsp_switch_trigger_idx);
		// }
	}
};

STAT_ASSRT(s_players_globals_data, 0x10 + (0x4 * 2 * MAX_PLAYER_COUNT_LOCAL) + (0x4 * 2 * MAX_PLAYERS_IN_GAME));

static s_players_globals_data *players_global_data = *(s_players_globals_data **) 0x815918;

struct s_player_control {
	datum_index         unit_index;                                                                      // 0x0
	unsigned long       control_flags;                                                                   // 0x4
	__int16             __pad_unk0;                                                                      // 0x8
	__int16             __pad_unk1;                                                                      // 0xA
	real_euler_angles2d desired_angles;                                                                  // 0xC
	real_vector2d       throttle;                                                                        // 0x14
	float               primary_trigger;                                                                 // 0x1C
	__int16             weapon_index;                                                                    // 0x20
	__int16             grenade_index;                                                                   // 0x22
	__int16             zoom_level;                                                                      // 0x24
	signed char         weapon_swap_ticks;                                                               // 0x26
	unsigned char       __cpad_unk;                                                                      // 0x27
	datum_index         target_object_index;                                                             // 0x28
	float               autoaim_level;                                                                   // 0x2C
	unsigned long       _unk_fld0_32;                                                                    // 0x30
	unsigned long       _unk_fld1_32;                                                                    // 0x34
	unsigned long       _unk_fld2_32;                                                                    // 0x38
	unsigned long       _unk_fld3_32;                                                                    // 0x3C
};

static_assert(sizeof(s_player_control) == 0x40);

struct s_player_control_globals_data {
	unsigned long action_flags[2]; // see "action_test" script functions
	unsigned long __pad_unk; // TODO: document the fields in the first 12 bytes of this struct
	unsigned long flags; // FLAG(0) = camera control

	//TODO: Find out what functions access what is immediately after this array when max local count = 1, then modify those.
	//TODO: Find out what functions access this array, and modify them to loop over this array, based on number of local players. :)
	s_player_control local_players[MAX_PLAYER_COUNT_LOCAL]; //0x10
};
STAT_ASSRT(s_player_control_globals_data, (0x10 + sizeof(s_player_control) * MAX_PLAYER_COUNT_LOCAL));



// struct s_player_datum {
// 	short salt;
//
// 	int16       local_player_index;                     // 0x2
// 	wchar_t     display_name[12];                        //player_name_len + 1 // 0x4
// 	int32       unused_unkA;                           // 0x1C
// 	union {                                    // 0x20
// 		long       team_index;
// 		argb_color color;
// 	};
// 	//s_player_action_result nearest_object_action_result;// 0x24
// 	long        respawn_ticks_remain;                     // 0x2C
// 	//---------------
// 	int32       unknown_typeA;                        // 0x30 respawn time growth related
// 	datum_index slave_unit_index;                  // 0x34
// 	datum_index last_slave_unit_index;               // 0x38
// 	int16       cluster_index;                        // 0x3C
// 	bool        weapon_swap_result;                     // 0x3E
// 	unsigned char : 8;//padding yo
// 	datum_index                 auto_aim_target;                  // 0x40 biped_index
// 	long                        auto_aim_update_ticks;               // 0x44
// 	//struct_stopped_at
// 	s_network_game_player       network_player;   // 0x48
// 	long                        powerup_times[2]; //max # of player powerups   // 0x68
// 	real                        speed;                                 // 0x6C
// 	int32                       source_teleporter_netgame_index;            // 0x70 index to a netgame flag in the scenario, or NONE
// 	s_game_engine_state_message engine_state_messsage;   // 0x74
// 	long                        last_death_time;                  // 0x84 game tick of the last time this player was killed
// 	datum_index                 target_player_index;               // 0x88 player index of the slayer target for this player
// 	bool                        odd_man_out;                           // 0x8C
// 	unsigned char  : 8;
// 	unsigned short : 16;
// 	int32 unused_unkb;                           // 0x90
// 	short unknown_typeC;                           // 0x94
// 	short kills_this_lifetime;                     // 0x96 how many kills we've had in this lifetime (since the last spawn)
// 	short current_spree_count;                     // 0x98 how many kills we've had in our 'spree time' (4 second intervals max)
// 	short last_kill_time;                        // 0x9A set from game_time_globals::local_time
// 	union {                                    // 0x9C
// 		byte player_score_buffer[0x10];
// 		struct {
// 			int16 kills[4];                        // 0x9C
// 			int16 assists[4];                     // 0xA4
// 		}    player_score;
// 	};
// 	short friendly_fire_kills;                     // 0xAC
// 	short deaths;                              // 0xAE
// 	short suicides;                              // 0xB0
// 	int16 pad16A;
// 	UNUSED_32(int32);                           // 0xB4
// 	UNUSED_32(int32);                           // 0xB8
// 	UNUSED_32(int32);                           // 0xBC
// 	int16 team_kills;                           // 0xC0
// 	PAD16;
// 	//u_player_multiplayer_stats multiplayer_stats;      // 0xC4
// 	int32                      telefrag_counter;                        // 0xCC # of ticks spent blocking teleporter
// 	game_ticks_t               quit_game_tick;                  // 0xD0 game tick the player quick at
// 	bool                       telefrag_enabled;                        // 0xD4 if we're blocking a teleporter, this will be true
// 	bool                       quit_out_of_game;                        // 0xD5
// 	PAD16;
// 	UNUSED_32(int32);                           // 0xD8
// 	int32 ping;                                 // 0xDC
// 	int32 team_kill_number;                        // 0xE0
// 	int32 team_kill_timer;                        // 0xE4
//
// 	union {                                    // 0xE8
// 		byte _unused_networking_buffer[0x200 - 0xE8];
//
// 		struct {
// 			Networking::s_player_client_update update_data;
// 			PAD32;                              // 0x1FC
// 		}    client;
// 		struct {
// 			Networking::s_player_server_update update_data;
// 			// !!! Only use this when the local machine is the host (ie, GameState::IsServer() == true) !!!
// 			s_player_yelo_server_data          yelo_data;      // 0x1C8
// 		}    server;
// 	};
// };  STAT_ASSRT(s_player_datum, 0x200); // 0x160 in Stubbs


#pragma pack(pop)
