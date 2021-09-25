#pragma once

#include "macros_generic.h"
#include "../math/real_math.h"
#include "../math/colors.h"
#include <enums/player_enums.h>

namespace Yelo::Scenario {
	// made up name
	struct s_scenario_player_atmospheric_fog {
		bool is_inside;
		unsigned char unk1;
		unsigned short unk2;
		real_vector3d  pad00;
		real_vector3d  pad01;
		real_rgb_color pad02;
		real           pad03;
	};
	STAT_ASSERT(s_scenario_player_atmospheric_fog, 0x2C);

	struct sound_environment {
		unsigned long unk1;
		short priority;
		unsigned short unk2;
		real room_intensity_db;
		real room_intensity_hf_db;
		real room_rolloff_factor;
		real decay_time;
		real decay_hf_ratio;
		real reflections_intensity_db;
		real reflections_delay;
		real reverb_intensity_db;
		real reverb_delay;
		real diffusion;
		real density;
		real hf_reference;
		unsigned long long unk3;
		unsigned long long unk4;
	};
	STAT_ASSERT(sound_environment, 0x48);

#pragma pack(push, 1)
	struct scen_sound {
		bool copy_environment_tag;
		unsigned char padA;
		unsigned short padB; // never seen this set to true
		sound_environment environment;
	};
#pragma pack(pop)
	STAT_ASSERT(scen_sound, 0x4C);

	struct s_scenario_globals {
		short current_structure_bsp_index;
		unsigned short unkA;
		s_scenario_player_atmospheric_fog player_fog[MAX_PLAYER_COUNT_LOCAL];
		scen_sound sound;
	};
	STAT_ASSERT(s_scenario_globals, 0x7C + (0x2C * (MAX_PLAYER_COUNT_LOCAL - 1)));

	//Xbox Beta 0x100
	//static_assert(sizeof(s_scenario_globals) + sizeof(s_scenario_player_atmospheric_fog)* 3 == 0x100);

	// struct s_sky_definition {
	// 	enum { k_group_tag = 'sky ' };
	// 	tag_reference         model;
	// 	tag_reference         animation;
	// 	tag_block     padA[2];
	// 	struct s_ambient_radiosity {
	// 		real_rgb_color color;
	// 		real           power;
	// 	}                     indoor_ambience, outdoor_ambience;
	// 	s_sky_atmospheric_fog outdoor_fog, indoor_fog;
	// 	tag_reference         indoor_fog_screen; // 'fog '
	// 	unsigned long : 32; // this could be angle 'Global Sky Rotation' (a la Halo 2). TODO: try to find references to this field offset
	// 	tag_block padC[3]; // s_sky_light
	// };
	// STAT_ASSERT(s_sky_definition, 0xD0);
};

