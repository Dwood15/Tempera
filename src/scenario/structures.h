#pragma once

#include "../cseries/base.h"
#include "../math/real_math.h"
#include "../math/colors.h"
#include "../tags/group/base.h"

namespace Yelo::Scenario {
		struct s_scenario_player_atmospheric_fog // made up name
		{
			bool is_inside;
			unsigned char : 8; unsigned short : 16;
			real_vector3d pad00;
			real_vector3d pad01;
			real_rgb_color pad02;
			real pad03;
		}; static_assert(sizeof(s_scenario_player_atmospheric_fog) == 0x2C);

		struct sound_environment {
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
			PAD128;
		};
		static_assert( sizeof(sound_environment) == 0x48 );

	struct s_scenario_globals {
		short current_structure_bsp_index;
		unsigned short : 16;
		s_scenario_player_atmospheric_fog player_fog[/* Enums::k_maximum_number_of_local_players*/ 1];
		struct {
			bool copy_environment_tag;
			unsigned char : 8; unsigned short : 16; // never seen this set to true
			sound_environment environment;
		}                                 sound;
	}; static_assert(sizeof(s_scenario_globals) == 0x7C);

	// The engine uses this for things (objects, ai, etc) which get dis/connected to the scenario's bsp
	struct s_scenario_location {
		long leaf_index;
		short cluster_index;
		short pad0;
	}; static_assert(sizeof(s_scenario_location) == 0x8);

	struct s_sky_atmospheric_fog {
		real_rgb_color color;
		PAD64;
		real_fraction maximum_density;
		real          start_distance;
		real          opaque_distance;
	};
	static_assert(sizeof(s_sky_atmospheric_fog) == 0x20);

	struct s_sky_definition {
		enum { k_group_tag = 'sky ' };

		tag_reference         model;
		tag_reference         animation;
		tag_block:8 * 0xC * 2;
		struct s_ambient_radiosity {
			real_rgb_color color;
			real           power;
		}                     indoor_ambience, outdoor_ambience;
		s_sky_atmospheric_fog outdoor_fog, indoor_fog;
		tag_reference         indoor_fog_screen; // 'fog '
		unsigned long : 32; // this could be angle 'Global Sky Rotation' (a la Halo 2). TODO: try to find references to this field offset
		tag_block:8 * 0xC * (1 + 1 + 1); // s_sky_light
	};
	static_assert(sizeof(s_sky_definition) == 0xD0);

};

