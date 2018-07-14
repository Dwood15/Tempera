#pragma once

#include "../cseries/base.h"
#include "../memory/datum_index.h"
#include "actors/actor_perception.hpp"
#include "../scenario/structures.h"
#include "../math/real_math.h"
#include "../memory/array.h"

namespace Yelo {
	namespace Enums {
		enum actor_type : short;
		enum game_team : short;
	};

	namespace AI {
		struct s_prop_datum {
			unsigned short header;                     //	0x0
			unsigned short : 16;                           //	0x2
			datum_index       owner_actor_index;         //	0x4
			datum_index       next_prop_index;         //	0x8
			datum_index       orphan_prop_index;         //	0xC
			Enums::actor_type actor_type;         //	0x10
			Enums::game_team  team;               //	0x12
			unsigned char : 8; // unknown field					//	0x14
			unsigned char : 8; unsigned short : 16;                           //	0x15
			datum_index unit_index;               //	0x18
			datum_index actor_index;            //	0x1C
			real        danger_radius;                  //	0x20
			short       state;                     //	0x24
			unsigned short : 16; // unknown field						0x26
			unsigned long : 32; // unknown field						0x28
			unsigned long : 32; // unknown field						0x2C
			Enums::actor_perception_type perception; //	0x30
			unsigned short : 16; // unknown field						0x32
			unsigned short : 16; // unknown field						0x34
			unsigned short : 16; // unknown field						0x36
			unsigned short : 16; // unknown field						0x38
			unsigned short : 16; // unknown field						0x3A
			unsigned short : 16; // unknown field						0x3C
			unsigned short : 16;                           //	0x3E
			PAD_TYPE(real_point3d); // unknown field	0x40
			unsigned short : 16; // unknown field						0x4C
			unsigned char : 8; // unknown field						0x4E
			unsigned char : 8;                           //	0x4F
			unsigned long : 32; // unknown field						0x50
			unsigned long : 32; // unknown field						0x54
			unsigned long : 32; // unknown field						0x58
			unsigned long : 32; // unknown field						0x5C
			bool enemy;                        //	0x60
			bool ally;                        //	0x61
			unsigned char : 8; // unknown field						0x62
			unsigned char : 8; // unknown field						0x63
			unsigned char : 8; // unknown field						0x64
			unsigned char : 8;                           //	0x65
			unsigned short : 16; // unknown field						0x66
			unsigned short : 16; // unknown field						0x68
			unsigned short : 16; // unknown field						0x6A
			unsigned short : 16; // unknown field						0x6C
			unsigned short : 16;                           //	0x6E
			unsigned long : 32; // unknown field						0x70
			unsigned char : 8; // unknown field						0x74
			unsigned char : 8;                           //	0x75
			unsigned short : 16; // unknown field						0x76
			unsigned short : 16; // unknown field						0x78
			unsigned short : 16;                           //	0x7A
			unsigned long : 32; // unknown field						0x7C
			PAD_TYPE(real_point3d); // unknown field	0x80
			unsigned long : 32; // unknown field						0x8C
			PAD_TYPE(real_point3d); // unknown field	0x90
			unsigned short : 16; // unknown field						0x9C
			unsigned short : 16;                           //	0x9E
			unsigned long : 32; // unknown field						0xA0
			bool is_unopposable;               //	0xA4
			unsigned char : 8;                           //	0xA5
			short               unopposable_casualties_inflicted; //	0xA6
			unsigned short : 16; // unknown field						0xA8
			unsigned short : 16; // unknown field						0xAA
			unsigned short : 16; // unknown field						0xAC
			unsigned short : 16; // unknown field						0xAE
			unsigned short : 16; // unknown field						0xB0
			unsigned short : 16;                           //	0xB2
			unsigned long : 32; // unknown field						0xB4
			unsigned char : 8; // unknown field						0xB8
			unsigned char : 8; // unknown field						0xB9
			unsigned char : 8; // unknown field						0xBA
			unsigned char : 8; // unknown field						0xBB
			PAD_TYPE(real_point3d); // unknown field	0xBC
			PAD_TYPE(real_point3d); // unknown field	0xC8
			PAD_TYPE(real_vector3d); // unknown field	0xD4
			PAD_TYPE(real_vector3d); // unknown field	0xE0
			unsigned long : 32; // unknown field						0xEC
			PAD_TYPE(real_point3d); // unknown field	0xF0
			Scenario::s_scenario_location location;         //	0xFC
			PAD_TYPE(real_point3d); // unknown field	0x104
			unsigned long : 32; // unknown field						0x110
			unsigned long : 32; // unknown field						0x114
			unsigned char : 8; // unknown field						0x118
			unsigned char : 8; unsigned short : 16;                           //	0x119
			unsigned long : 32; // unknown field						0x11C
			unsigned char : 8; // unknown field						0x120
			unsigned char : 8; // unknown field						0x121
			unsigned char : 8; // unknown field						0x122
			unsigned char : 8; // unknown field						0x123
			unsigned char : 8; // unknown field						0x124
			unsigned char : 8; // unknown field						0x125
			unsigned char : 8; // unknown field						0x126
			bool dead;                        //	0x127
			unsigned char : 8; // unknown field						0x128
			unsigned char : 8; // unknown field						0x129
			unsigned char : 8; // unknown field						0x12A
			bool actor_is_noncombat;            //	0x12B
			unsigned char : 8; // unknown field						0x12C
			unsigned char : 8; // unknown field						0x12D
			unsigned char : 8; // unknown field						0x12E
			unsigned char : 8; // unknown field						0x12F
			unsigned char : 8; // unknown field						0x130
			unsigned char : 8; // unknown field						0x131
			unsigned char : 8; // unknown field						0x132
			bool ignored;                     //	0x133
			unsigned char : 8; // unknown field						0x134
			unsigned char : 8; // unknown field						0x135
			unsigned char : 8; // unknown field						0x136
			unsigned char : 8; // unknown field						0x137
		};
		static_assert(sizeof(s_prop_datum) == 0x138);

		typedef Yelo::Memory::DataArray<s_prop_datum, 768> prop_data_t;

		prop_data_t &Props();
	};
};
