#pragma once
#include <cstddef>
#include "../memory/datum_index.h"



struct machine // Struct is from haloceded; i'm not sure if it's the same in other versions.
{
	std::byte    Unknown0[32];
	std::byte    Unknown1[8];
	float   Yaw;               // player's rotation - in radians, from 0 to 2*pi, (AKA heading)
	float   Pitch;            // Player's pitch - in radians, from -pi/2 to +pi/2, down to up.
	float   Roll;               // roll - unless walk-on-walls is enabled, this will always be 0.
	std::byte    Unknown2[44];
	wchar_t LastPlayersName[12];   // Odd.. this isnt the name of the player who's on, but i thinkn it's the Previous player's name.
	long    Unknown3;
	long    Unknown4;            // Might be player index, or team.
	char    IP[32];
	char    CDhash[32];         // a solid block array, so it's not necessarily a cstring i think, but there's still usually just 0's afterwards anyways.
	std::byte    Unknown5[44];         // zeros..
}; // Size: 0xFE

