#pragma once

/*	See license\OpenSauce\OpenSauce for specific license information */
#include <precompile.h>

#include "../math/real_math.h"

namespace Yelo {
	namespace Enums {
		enum {
			k_maximum_actor_starting_locations = 32,
		};

		enum {
			_ai_atom_pause,
			_ai_atom_go_to,
			_ai_atom_go_to_and_face,
			_ai_atom_move_in_direction,
			_ai_atom_look,
			_ai_atom_animation_mode,
			_ai_atom_crouch,
			_ai_atom_shoot,
			_ai_atom_grenade,
			_ai_atom_vehicle,
			_ai_atom_running_jump,
			_ai_atom_targeted_jump,
			_ai_atom_script,
			_ai_atom_animate,
			_ai_atom_recording,
			_ai_atom_action,
			_ai_atom_vocalize,
			_ai_atom_targeting,
			_ai_atom_initiative,
			_ai_atom_wait,
			_ai_atom_loop,
			_ai_atom_die,
			_ai_atom_move_immediate,
			_ai_atom_look_random,
			_ai_atom_look_player,
			_ai_atom_look_object,
			_ai_atom_set_radius,
			_ai_atom_teleport,

			k_number_of_ai_atoms,
		};
	};

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
		}; static_assert(sizeof(s_firing_position) == 0x18);

		struct s_encounter_definition {
			tag_string name;
			unsigned long flags;
			short team_index;
			UNKNOWN_TYPE(short); // set to 1 once postprocessed. not seeing any other references past that
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

		static_assert(sizeof(s_encounter_definition) == 0xB0);

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
		}; static_assert(sizeof(s_ai_command) == 0x20);
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

			tag_block:8 * sizeof(tag_block) * 2;
		};

		static_assert(sizeof(s_ai_command_list) == 0x60);
	};
};
