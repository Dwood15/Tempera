#pragma once
#ifndef TEMPERA_PLAYER_TYPES_H
#define TEMPERA_PLAYER_TYPES_H

#include "../shitty_macros.h"
#include "../ce_base_types.h"
#include "../shitty_enums.h"

struct s_custom_blip {
	sbyte x;
	sbyte y;
}; STAT_ASSRT(s_custom_blip, 0x2);

struct s_blip {
	s_custom_blip custom_blip; //0x0
	//end custom blip location
	blip_type type; // set to _blip_type_none when not used
	sbyte size;    // a la object's size (tiny, large, etc)
}; STAT_ASSRT(s_blip, 0x4); //(sizeof(s_blip) == 0x4, STATIC_ASSERT_FAIL);

struct s_game_engine_team_info {
	s_custom_blip goal_blips[MAX_CUSTOM_BLIPS]; //0x10
	sbyte goal_indices[MAX_CUSTOM_BLIPS];       //0x10 indices to the game engine's global goals
};
//INTELLISENSE_HACK(s_team_data);
//INTELLISENSE_HACK(s_team_data);

struct s_team_data {
	s_blip object_blips[MAX_CUSTOM_BLIPS];            // objects belonging to just this team

	s_game_engine_team_info game_engine;

	real_vector2d world_position;
	int32 unk_int; // game time related
	real unk_float; // related to calculations with objects and facing angle of the player
	sbyte active_object_blips_count;
	PAD24(p);
}; //static_assert(sizeof(s_team_data) == 0x84);

struct s_local_player {
	s_team_data nearby_team_objects[k_number_of_game_teams]; //0x0
	datum_index nearby_object_indexes[MAX_CUSTOM_BLIPS];
}; //STAT_ASSRT(s_local_player, 0x568);

struct s_motion_sensor {
	short unk_padding;
	s_local_player local_players[MAX_PLAYER_COUNT_LOCAL];

	struct {
		__int32 unk_type; // game time related
		__int16 current_object_blip_index;
		//Open Sauce has this as 2 bytes of pad, but that is incorrect. There's 2 bytes on the front of the pointer. :)
	} update_data;
}; //STAT_ASSRT(s_motion_sensor, 0x8 + (0x568 * MAX_PLAYER_COUNT_LOCAL));

#endif //TEMPERA_PLAYER_TYPES_H
