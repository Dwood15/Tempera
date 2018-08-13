#pragma once

#include "enums/objects_enums.h"
#include "macros_generic.h"
#include "../memory/datum_index.h"

struct s_unit_speech {
	Yelo::Enums::unit_speech_priority priority;
	Yelo::Enums::unit_scream_type     scream;
	Yelo::datum_index                       sound_definition_index;
	short                             time_rel_pad0; // time related
	unsigned short                    pad_unknown;
	long                              pad_unverified; // haven't verified what is here yet
	byte                              ai_information[0x20];
};
STAT_ASSERT(s_unit_speech, 0x30);

struct s_unit_speech_data {
	s_unit_speech current;                              // 0x388
	s_unit_speech next;                                 // 0x3B8 not *positive* of this field
	short         pad0;                              // 0x3E8
	short         pad1;                              // 0x3EA
	short         pad2;                              // 0x3EC
	short         pad3;                              // 0x3EE
	long          pad4;                              // 0x3F0 time related
	bool          pad5;                                 // 0x3F4
	bool          pad6;                                 // 0x3F5
	bool          pad7;                                 // 0x3F6
	unsigned char pad8;                                          // 0x3F7
	short         pad9;                              // 0x3F8
	short         padA;                              // 0x3FA
	short         padB;                              // 0x3FC
	short         padC;                              // 0x3FE
	long          padD;                              // 0x400
};

struct s_recent_damage {
	long        game_tick;            // the last game tick damage was dealt
	real        damage;               // total (read: additive) damage the responsible object has done
	Yelo::datum_index responsible_unit;
	Yelo::datum_index responsible_player;   // would be NONE if killed by AI
};
