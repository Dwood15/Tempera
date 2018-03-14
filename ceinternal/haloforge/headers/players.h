/*
	Project: haloforge
	File: players.h
	Copyright © 2009 SilentK, Abyll

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef PLAYERS_H
#define PLAYERS_H

#include <windows.h>
#include "halo_types.h"
#include "math.h"

struct player;
struct local_player;

struct player
{
	short	playerid;
	short	host;
	wchar_t PlayerName0[12]; // Unicode / Max - 11 Chars + EOS (12 total)
	long	Unknown0;		 // Always -1 / 0xFFFFFFFF
	long	Team;			 // 0 = Red / 1 = Blue
	long	SwapID;			 // is an ObjectID
	short	SwapType;		 // 8 = Vehicle / 6 = Weapon
	short	SwapSeat;		 // Warthog - Driver = 0 / Passenger = 1 / Gunner = 2 / Weapon = -1
	long	RespawnTimer;	 // ?????? Counts down when dead, Alive = 0
	long	Unknown1;		 // Always 0
	ident	object;			 // matches object table
	long	Unknown3;		 // Some sort of ID
	long	Sector;			 // This is very, very interesting. BG is split into 25 location ID's. 1 -19
	long	Unknown4;		 // Always -1 / 0xFFFFFFFF
	long	BulletCount;	 // Something to do with bullets increases - weird.
	wchar_t PlayerName1[12]; // Unicode / Max - 11 Chars + EOS (12 total)
	long	Unknown5;
	char	MachineIndex;
	BYTE	Unknown6;
	char	Team2;
	char	PlayerIndex;
	long	Unknown7;
	float	SpeedModifier;
	BYTE	Unknown8[42];
	short	Kills;			 // Number of kills
	BYTE	Unknown9[6];
	short	Assists;
	BYTE Unknown10[8];
	short Teamkills;
	short Deaths;
	short Suicides;			// suicides, i think. Suicides make you die, you killed someone on your team, but also count to suicide count too.
	BYTE Unknown11[40];
	short	Ping;
	BYTE	Unknown12[158];
	BYTE	Unknown13[12];	 // 0xFF's
	BYTE	Unknown14[120];
};

struct local_player
{
	ident iplayer;
	BYTE Unknown00[160];
	ident object;
	BYTE Unknown01[8];
	vect3 rot;
};

struct machine // Struct is from haloceded; i'm not sure if it's the same in other versions.
{
	BYTE Unknown0[32];
	BYTE Unknown1[8];
	float Yaw;					// player's rotation - in radians, from 0 to 2*pi, (AKA heading)
	float Pitch;				// Player's pitch - in radians, from -pi/2 to +pi/2, down to up. 
	float Roll;					// roll - unless walk-on-walls is enabled, this will always be 0.
	BYTE Unknown2[44];
	wchar_t LastPlayersName[12];	// Odd.. this isnt the name of the player who's on, but i thinkn it's the Previous player's name.
	long Unknown3;
	long Unknown4;				// Might be player index, or team.
	char IP[32];
	char CDhash[32];			// a solid block array, so it's not necessarily a cstring i think, but there's still usually just 0's afterwards anyways.
	BYTE Unknown5[44];			// zeros..
}; // Size: 0xFE


#endif /* PLAYERS_H */