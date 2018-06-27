#pragma once

#include <precompile.h>
#include "objects.h"
#include "../tags/tags.h"

struct player {
	short   playerid;
	short   host;
	wchar_t PlayerName0[12]; // Unicode / Max - 11 Chars + EOS (12 total)
	long    Unknown0;        // Always -1 / 0xFFFFFFFF
	long    Team;            // 0 = Red / 1 = Blue
	long    SwapID;          // is an ObjectID
	short   SwapType;        // 8 = Vehicle / 6 = Weapon
	short   SwapSeat;        // Warthog - Driver = 0 / Passenger = 1 / Gunner = 2 / Weapon = -1
	long    RespawnTimer;    // ?????? Counts down when dead, Alive = 0
	long    RespawnTimeGrowth; // Always 0
	ident   SlaveUnitIndex;          // matches object table
	long    PlayerLastObjectId;        // Some sort of ID
	long    Sector;          // This is very, very interesting. BG is split into 25 location ID's. 1 -19
	long    Unknown4;        // Always -1 / 0xFFFFFFFF
	long    BulletCount;     // Something to do with bullets increases - weird.
	wchar_t PlayerName1[12]; // Unicode / Max - 11 Chars + EOS (12 total)
	long    Color;
	char    MachineIndex;
	BYTE    ControllerIndex;
	char    Team2;
	char    PlayerIndex;   //Should be 0-16F
	long    Unknown7;
	float   SpeedModifier;
	BYTE    Unknown8[42];   //This is multiplayer-related stuff.
	short   Kills;          // Number of kills
	BYTE    Unknown9[6];
	short   Assists;
	BYTE    Unknown10[8];
	short   Teamkills;
	short   Deaths;
	short   Suicides;         // suicides, i think. Suicides make you die, you killed someone on your team, but also count to suicide count too.
	BYTE    Unknown11[40];
	short   Ping;
	long    TeamKills;
	long    TeamKillTimer;
	BYTE    Unknown12[14];
	long    unk;
	long    unk2;
	float   unk3;
	ident   unknown_ident;
	BYTE    Unknown13[120];
	BYTE    Unknown14[12];    // 0xFF's
	BYTE    Unknown15[119];
	BYTE    Unknown16;

	void DumpData(bool toConsole = false) {
		Print(toConsole, "*** Player Data Dump! ***\n");
		Print(toConsole, "Address: 0x%x\n", (int) this);
		Print(toConsole, "Player Id: %x\n", playerid);
		Print(toConsole, "Host: %d\n", host);
		Print(toConsole, "PlayerName 0: %ls\n", PlayerName0);
		Print(toConsole, "Unknown 0: %d\n", Unknown0);
		Print(toConsole, "Team: %d\n", Team);
		Print(toConsole, "SwapId: %d\n", SwapID);
		Print(toConsole, "SwapType: %d\n", SwapType);
		Print(toConsole, "Last Interaction Id: %d\n", PlayerLastObjectId);
		Print(toConsole, "Bullet Count: %d\n", BulletCount);
		Print(toConsole, "Player Name 1: %ls\n", PlayerName1);
		Print(toConsole, "Color: %d\n", Color);
		Print(toConsole, "MachIndex: %x\n", MachineIndex);
		Print(toConsole, "Controller Index: %x\n", ControllerIndex);

		Print(toConsole, "unk 1: %x\n", unk);
		Print(toConsole, "unk 2: %x\n", unk2);
		Print(toConsole, "unk 3: %x\n", unk3);
		Print(toConsole, "unk 16: %x\n", &Unknown16);
		Print(toConsole, "*** End Player Data Dump! ***\n");
	}
};

//The local player structure does not actually affect anything. More lke it just gets updated.
struct local_player {
	ident iplayer;
	BYTE  Unknown00[160];
	ident object;
	BYTE  Unknown01[8];
	vect3 rot;

	void DumpData(bool toConsole = false) {
		Print(toConsole, "Begin Local Player Dump!\n");
		Print(toConsole, "Address: 0x%x\n", (int) this);
		rot.DumpData(toConsole, "Local Player Rot: ");
	}
};

struct machine // Struct is from haloceded; i'm not sure if it's the same in other versions.
{
	BYTE    Unknown0[32];
	BYTE    Unknown1[8];
	float   Yaw;               // player's rotation - in radians, from 0 to 2*pi, (AKA heading)
	float   Pitch;            // Player's pitch - in radians, from -pi/2 to +pi/2, down to up.
	float   Roll;               // roll - unless walk-on-walls is enabled, this will always be 0.
	BYTE    Unknown2[44];
	wchar_t LastPlayersName[12];   // Odd.. this isnt the name of the player who's on, but i thinkn it's the Previous player's name.
	long    Unknown3;
	long    Unknown4;            // Might be player index, or team.
	char    IP[32];
	char    CDhash[32];         // a solid block array, so it's not necessarily a cstring i think, but there's still usually just 0's afterwards anyways.
	BYTE    Unknown5[44];         // zeros..
}; // Size: 0xFE

