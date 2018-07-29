#pragma once

#include "macros_generic.h"
#include "../math/real_math.h"
#include "../math/colors.h"
#include "../tags/group/base.h"

namespace Yelo::Scenario {
	// made up name
	struct __attribute__((packed)) s_scenario_player_atmospheric_fog {
		bool is_inside;
		unsigned char : 8;
		unsigned short : 16;
		real_vector3d  pad00;
		real_vector3d  pad01;
		real_rgb_color pad02;
		real           pad03;
	};

	struct __attribute__((packed)) sound_environment {
		unsigned long : 32;
		short priority;
		unsigned short : 16;
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
		unsigned __int64:64;
		unsigned __int64:64;
	};

	struct __attribute__((packed)) s_scenario_globals {
		short current_structure_bsp_index;
		unsigned short : 16;
		s_scenario_player_atmospheric_fog player_fog[/* Enums::k_maximum_number_of_local_players*/ 1];
		struct __attribute__((packed)) {
			bool copy_environment_tag;
			unsigned char  : 8;
			unsigned short : 16; // never seen this set to true
			sound_environment environment;
		}                                 sound;
	};

	STAT_ASSERT(sound_environment, 0x48);
	STAT_ASSERT(s_scenario_player_atmospheric_fog, 0x2C )
	STAT_ASSERT(s_scenario_globals, 0x7C)

	// struct __attribute__((packed)) s_sky_definition {
	// 	enum { k_group_tag = 'sky ' };
	// 	tag_reference         model;
	// 	tag_reference         animation;
	// 	tag_block<void *>     padA[2];
	// 	struct s_ambient_radiosity {
	// 		real_rgb_color color;
	// 		real           power;
	// 	}                     indoor_ambience, outdoor_ambience;
	// 	s_sky_atmospheric_fog outdoor_fog, indoor_fog;
	// 	tag_reference         indoor_fog_screen; // 'fog '
	// 	unsigned long : 32; // this could be angle 'Global Sky Rotation' (a la Halo 2). TODO: try to find references to this field offset
	// 	tag_block<void *> padC[3]; // s_sky_light
	// };
	// STAT_ASSERT(s_sky_definition, 0xD0);
};

