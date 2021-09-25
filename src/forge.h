#pragma once

namespace forge {
	[[noreturn]] int MainLoop();
}

struct _blam_ {
	wchar_t GameTypeName[24];   // Unicode
	long    GameType;            // 1 CTF, 2 Slayer, 3 Oddball, 4 KOTH, 5 Race
	long    TeamPlay;            // 0 Off, 1 On

	unsigned long BallIndicator      :   1;
	unsigned long FriendsOnRadar      :   1;
	unsigned long StartEquipment      :   1;   // 0 Generic, 1 Custom
	unsigned long Invisible            :   1;
	unsigned long Shields            :   1;
	unsigned long InfiniteGrenades   :   1;
	unsigned long FriendIndicators   :   1;
	unsigned long PlayersOnRadar      :   1;

	long  Indicator;            // 0 Motion tracker, 1 Navpoints, 2 None
	long  OddManOut;            // 0 No, 1 Yes
	long  RespawnTimeGrowth;   // 0 Off, 30 units per second eg: 150(0x96) = 30*5 secs
	long  RespawnTime;
	long  SuicidePenalty;
	long  NumOfLives;         // 0 Unlimited, 1 1 life, 3 3 lives, 5 5 lives
	float fUnknown;               // 1.0f
	long  ScoreLimit;           // Captures for CTF, laps for RACE, kills for Slayer, minutes for King, etc
	long  WeaponSet;            // 0 Normal, 1 Pistols, 2 Rifles, 3 Plasma, 4 Sniper, 5 No sniping, 6 Rockets,
	// 7 Shotguns, 8 Shortrange, 9 Human, 10 Convenant, 11 Classic, 12 Heavy

	/* Red Team Vehicle Settings */
	unsigned long RedCustom         :   4;
	unsigned long RedWarthog         :   3;
	unsigned long RedGhost         :   3;
	unsigned long RedScorpion         :   3;
	unsigned long RedRocketWarthog   :   3;
	unsigned long RedBanshee         :   3;
	unsigned long RedTurret         :   3;
	unsigned long RedZero            :   2;
	unsigned long RedUnused         :   8;

	/* Blue Team Vehicle Settings */
	unsigned long BlueCustom         :   4;
	unsigned long BlueWarthog         :   3;
	unsigned long BlueGhost         :   3;
	unsigned long BlueScorpion      :   3;
	unsigned long BlueRocketWarthog   :   3;
	unsigned long BlueBanshee         :   3;
	unsigned long BlueTurret         :   3;
	unsigned long BlueZero         :   2;
	unsigned long BlueUnused         :   8;

	long VehicleRespawnTime;

	long           FriendlyFire;         // 0 Off, 1 On
	long           TKPenalty;
	long           AutoTeamBalance;      // 0 Off, 1 On
	long           GameTimeLimit;
	long           TypeFlags;         // Moving hill 0 Off; 1 On (KOTH)
	// Racetype 0 Normal; 1 Any order; 2 Rally (Race)
	// Random start 0 No; 1 Yes (Oddball)
	char           TeamScoring;         // Team scoring 0 Minimum; 1 Maximum; 2 Sum (Race)
	// Ballspeed 0 Slow; 1 Normal; 2 Fast (Oddball)
	char           AssaultTimeLimit;      // 0 Two flags
	unsigned short Unused1;
	long           TraitWithBall;      // 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent
	long           TraitWithoutBall;      // 0 None, 1 Invisible, 2 Extra damage, 3 Damage Resistent
	long           BallType;             // 0 Normal, 1 Reverse Tag, 2 Juggernaut
	long           BallCountSpawn;
	unsigned char  One;                    // Always 1 ( 0 if custom )
	char           GameTypeNum;            // # of the game in the game list ( 0000 - for a custom game type )
	unsigned short Unused2;
	unsigned long  Checksum;
};

class CBlam {
public:
	_blam_ *server_blam;
	_blam_ *local_blam;

	CBlam();

	~CBlam() {}

	void SetGameTypeName(_blam_ *_blam, wchar_t *wName);   // Unicode, 24 characters max
	void SetGameType(_blam_ *_blam, long dwType);      // 1 CTF, 2 Slayer, 3 Oddball, 4 KOTH, 5 Race
	void SetTeamPlay(_blam_ *_blam, long dwNum);

	wchar_t *GetGameTypeName(_blam_ *_blam);

	long GetGameType(_blam_ *_blam);

	long GetTeamPlay(_blam_ *_blam);

	// 0 Off, 1 On
	void SetBallIndicator(_blam_ *_blam, bool bNum);

	void SetFriendsOnRadar(_blam_ *_blam, bool bNum);

	void SetStartEquipment(_blam_ *_blam, bool bNum);

	void SetInvisible(_blam_ *_blam, bool bNum);

	void SetShields(_blam_ *_blam, bool bNum);

	void SetInfiniteGrenades(_blam_ *_blam, bool bNum);

	void SetFriendIndicators(_blam_ *_blam, bool bNum);

	void SetPlayersOnRadar(_blam_ *_blam, bool bNum);

	bool GetBallIndicator(_blam_ *_blam);

	bool GetFriendsOnRadar(_blam_ *_blam);

	bool GetStartEquipment(_blam_ *_blam);

	bool GetInvisible(_blam_ *_blam);

	bool GetShields(_blam_ *_blam);

	bool GetInfiniteGrenades(_blam_ *_blam);

	bool GetFriendIndicators(_blam_ *_blam);

	bool GetPlayersOnRadar(_blam_ *_blam);
};



