/*
	Project: haloforge
	File: blam.h
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

#ifndef BLAH
#define BLAH

#include <windows.h>
#define BITFIELD DWORD

// This is the entire blam file structure. It is loaded into memory ( static )
// when you join / start a game. I am mainly using it for a teamplay check
// but there are other uses. You can edit a gametype during game and such.

// Local .blam storage ( if you are host )
#define SERVER_BLAM_ADDRESS 0x0087A620;
// .blam storage ( if you are NOT host )
#define LOCAL_BLAM_ADDRESS 0x0087A6C0;

struct blam
{
	wchar_t GameTypeName[24];	// Unicode
	long GameType;				// 1 CTF, 2 Slayer, 3 Oddball, 4 KOTH, 5 Race
	long TeamPlay;				// 0 Off, 1 On

	BITFIELD BallIndicator		:	1;
	BITFIELD FriendsOnRadar		:	1;
	BITFIELD StartEquipment		:	1;	// 0 Generic, 1 Custom
	BITFIELD Invisible			:	1;
	BITFIELD Shields				:	1;  
	BITFIELD InfiniteGrenades	:	1;
	BITFIELD FriendIndicators	:	1;
	BITFIELD PlayersOnRadar		:	1;

	long Indicator;            // 0 Motion tracker, 1 Navpoints, 2 None
	long OddManOut;            // 0 No, 1 Yes
	long RespawnTimeGrowth;	// 0 Off, 30 units per second eg: 150(0x96) = 30*5 secs
	long RespawnTime;
	long SuicidePenalty;
	long NumOfLives;			// 0 Unlimited, 1 1 life, 3 3 lives, 5 5 lives
	float fUnknown;               // 1.0f
	long ScoreLimit;           // Captures for CTF, laps for RACE, kills for Slayer, minutes for King, etc
	long WeaponSet;            // 0 Normal, 1 Pistols, 2 Rifles, 3 Plasma, 4 Sniper, 5 No sniping, 6 Rockets, 
									// 7 Shotguns, 8 Shortrange, 9 Human, 10 Convenant, 11 Classic, 12 Heavy

	/* Red Team Vehicle Settings */
	BITFIELD RedCustom			:	4;
	BITFIELD RedWarthog			:	3;
	BITFIELD RedGhost			:	3;
	BITFIELD RedScorpion			:	3;
	BITFIELD RedRocketWarthog	:	3;
	BITFIELD RedBanshee			:	3;
	BITFIELD RedTurret			:	3;
	BITFIELD RedZero				:	2;
	BITFIELD RedUnused			:	8;

	/* Blue Team Vehicle Settings */
	BITFIELD BlueCustom			:	4;
	BITFIELD BlueWarthog			:	3;
	BITFIELD BlueGhost			:	3;
	BITFIELD BlueScorpion		:	3;
	BITFIELD BlueRocketWarthog	:	3;
	BITFIELD BlueBanshee			:	3;
	BITFIELD BlueTurret			:	3;
	BITFIELD BlueZero			:	2;
	BITFIELD BlueUnused			:	8;

	long VehicleRespawnTime;

	long FriendlyFire;			// 0 Off, 1 On
	long TKPenalty;
	long AutoTeamBalance;		// 0 Off, 1 On
	long GameTimeLimit;
	long TypeFlags;			// Moving hill 0 Off; 1 On (KOTH)
									// Racetype 0 Normal; 1 Any order; 2 Rally (Race)
									// Random start 0 No; 1 Yes (Oddball)
	char TeamScoring;			// Team scoring 0 Minimum; 1 Maximum; 2 Sum (Race)
									// Ballspeed 0 Slow; 1 Normal; 2 Fast (Oddball)
	char AssaultTimeLimit;		// 0 Two flags
	WORD Unused1;
	long TraitWithBall;		// 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent 
	long TraitWithoutBall;		// 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent
	long BallType;             // 0 Normal, 1 Reverse Tag, 2 Juggernaut 
	long BallCountSpawn;
	BYTE One;                    // Always 1 ( 0 if custom )
	char GameTypeNum;            // # of the game in the game list ( 0000 - for a custom game type )
	WORD Unused2;
	DWORD Checksum;
};

///////////////////////////////
// blam functions

class CBlam
{
public:
	blam* server_blam;
	blam* local_blam;

	CBlam();
	~CBlam();

	void SetGameTypeName(blam* _blam, wchar_t *wName);	// Unicode, 24 characters max
	void SetGameType(blam* _blam, long dwType);		// 1 CTF, 2 Slayer, 3 Oddball, 4 KOTH, 5 Race
	void SetTeamPlay(blam* _blam, long dwNum);

	wchar_t *GetGameTypeName(blam* _blam);
	long GetGameType(blam* _blam);
	long GetTeamPlay(blam* _blam);

	// 0 Off, 1 On
	void SetBallIndicator(blam* _blam, bool bNum);
	void SetFriendsOnRadar(blam* _blam, bool bNum);
	void SetStartEquipment(blam* _blam, bool bNum);
	void SetInvisible(blam* _blam, bool bNum);
	void SetShields(blam* _blam, bool bNum);
	void SetInfiniteGrenades(blam* _blam, bool bNum);
	void SetFriendIndicators(blam* _blam, bool bNum);
	void SetPlayersOnRadar(blam* _blam, bool bNum);

	bool GetBallIndicator(blam* _blam);
	bool GetFriendsOnRadar(blam* _blam);
	bool GetStartEquipment(blam* _blam);
	bool GetInvisible(blam* _blam);
	bool GetShields(blam* _blam);
	bool GetInfiniteGrenades(blam* _blam);
	bool GetFriendIndicators(blam* _blam);
	bool GetPlayersOnRadar(blam* _blam);
};

#endif /* BLAH */