#pragma once

/*	See license\OpenSauce\OpenSauce for specific license information */
#include "../math/real_math.h"
#include "../tags/group/tagblock.h"
#include <enums/ai_enums.h>

namespace Yelo {

	namespace TagGroups {
		struct s_squad_definition {

		}; //static_assert( sizeof(s_squad_definition)==0x18 );

		struct s_platoon_definition {

		}; //static_assert( sizeof(s_platoon_definition)==0x18 );

		struct s_firing_position {
			real_point3d position;
			short        group_index;
			short        runtime_cluster_index;
			unsigned long : 32;
			long runtime_surface_index; // not valid if the encounter uses 3d firing positions
		}; STAT_ASSERT(s_firing_position, 0x18);

		struct s_encounter_definition {
			tag_string name;
			unsigned long flags;
			short team_index;
			short pad0; // set to 1 once postprocessed. not seeing any other references past that
			short       search_behavior;
			short       manual_structure_reference_index;
			real_bounds respawn_delay;

			byte tag_padding[74];
			short runtime_structure_reference_index;
			tag_block squads;
			tag_block platoons;
			tag_block firing_positions;
			tag_block starting_locations;
		};

		STAT_ASSERT(s_encounter_definition, 0xB0);

		struct s_ai_command {
			short type;
			short atom_modifier;
			real  parameter1;
			real  parameter2;
			short point1_index;
			short point2_index;
			short ai_animation_index;
			short ai_script_index;
			short ai_recording_index;
			short command_index;
			short object_name_index;
			unsigned short : 16;
			unsigned long : 32;
		};
		STAT_ASSERT(s_ai_command, 0x20);

		struct s_ai_command_point {
			real_point3d position;
			long        runtime_surface_index;
			unsigned long : 32;
		}; static_assert(sizeof(s_ai_command_point) == 0x14);

		struct s_ai_command_list {
			tag_string name;
			unsigned long flags;
			unsigned long : 32;
			unsigned long : 32;
			short manual_structure_reference_index;
			short runtime_structure_reference_index;
			tag_block commands;
			tag_block points;

			tag_block padA[2];
		};

		static_assert(sizeof(s_ai_command_list) == 0x60);
	};
};
