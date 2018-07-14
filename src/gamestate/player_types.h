/**
 *  You should have received a copy of the GNU General Public License
 * along with Tempera. If not, see <http://www.gnu.org/licenses/>.
 **/
#pragma once
#pragma pack(push)
#pragma pack(1)

#include <cstdio>
#include <macros_generic.h>
#include "../ce_base_types.h"
#include "enums/generic_enums.h"
#include "../math/real_math.h"

STAT_ASSRT(sbyte, 1);
using namespace Yelo;
struct s_player_hud_messages {
	byte unk[0x460];
};
STAT_ASSRT(s_player_hud_messages, 0x460);

struct s_hud_messaging_state {
	s_player_hud_messages hmi[MAX_PLAYER_COUNT_LOCAL];
	byte                  unknown[0x28];
};   STAT_ASSRT(s_hud_messaging_state, 0x28 + (0x460 * MAX_PLAYER_COUNT_LOCAL));

//this _fucking_ struct.
//	struct s_hud_message_state_player {
//		char unk[0x82];
//		s_hud_messaging_state scripted_hud_messages[4];
//	}; STAT_ASSRT(s_hud_message_state_player, 0x)

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
static_assert(sizeof(s_player_control_globals_data) < (unsigned int) 0xFF);





#pragma pack(pop)
