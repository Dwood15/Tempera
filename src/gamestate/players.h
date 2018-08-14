#pragma once

#include <cstddef>
#include "../memory/datum_index.h"
#include "../scenario/definitions.h"
// Struct is from haloceded; i'm not sure if it's the same in other versions.
struct machine
{
	byte    Unknown0[32];
	byte    Unknown1[8];
	float   Yaw;               // player's rotation - in radians, from 0 to 2*pi, (AKA heading)
	float   Pitch;            // Player's pitch - in radians, from -pi/2 to +pi/2, down to up.
	float   Roll;               // roll - unless walk-on-walls is enabled, this will always be 0.
	byte    Unknown2[44];
	wchar_t LastPlayersName[12];   // Odd.. this isnt the name of the player who's on, but i thinkn it's the Previous player's name.
	long    Unknown3;
	long    Unknown4;            // Might be player index, or team.
	char    IP[32];
	char    CDhash[32];         // a solid block array, so it's not necessarily a cstring i think, but there's still usually just 0's afterwards anyways.
	byte    Unknown5[44];         // zeros..
}; // Size: 0xFE

auto get_scenario_location(Yelo::TagGroups::scenario * scen, uint loc_ix);
void players_set_local_player_unit(ushort requested_plyr_idx, Yelo::datum_index player_unit);
