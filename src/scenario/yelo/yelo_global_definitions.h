#pragma once

#include <precompile.h>
#include <enginelayout/Objects.inl>
#include "../../tags/group/markup.h"
#include "../../memory/upgrades/blam_memory_upgrades.hpp"
#include "../../tags/group/tag_groups_base_yelo.hpp"
#include "yelo_scenario_definite.h"

namespace Yelo::Flags {
	enum project_yellow_globals_flags {
		_project_yellow_globals_hide_health_when_zoomed_bit,
		_project_yellow_globals_hide_shield_when_zoomed_bit,
		_project_yellow_globals_hide_motion_sensor_when_zoomed_bit,
		_project_yellow_globals_force_game_to_use_stun_jumping_penalty_bit,
		// TODO:
		_project_yellow_globals_allow_grenade_chain_reactions_in_mp_bit,

		k_number_of_project_yellow_globals_flags,
	};
};

namespace Yelo::TagGroups {
	/* !-- UI --! */
	struct s_project_yellow_scripted_ui_widget {
		tag_string name;
		tag_reference definition;

		unsigned long : 32; // future flags
		tag_block:8 * sizeof(tag_block) * 2;
	};
	/* !-- UI --! */


	/* !-- Netgame --! */
	struct s_network_game_player_unit {
		tag_string name;
		tag_reference definition;
		long:8 * sizeof(long) * 8; // 32
	};
	/* !-- Netgame --! */


	/* !-- Scripting --! */
	struct s_script_construct_definition {
		tag_string name[2]; // So we can have at most 62 characters for the name
		short index;

	};

	struct s_script_function_definition : public s_script_construct_definition {
		Enums::hs_type return_type;
		Yelo::TagBlock<const short> parameters; // 32
	};
	struct s_script_global_definition : public s_script_construct_definition {
		Enums::hs_type type;
	};
	struct s_scripting_definitions {
		Yelo::TagBlock<const s_script_function_definition> new_functions;
		Yelo::TagBlock<const s_script_global_definition> new_globals;
	};
	/* !-- Scripting --! */


	// yelo for globals
	struct project_yellow_globals {
		enum {
			k_group_tag = 'gelo',
			k_version   = 2,
		};

		// internal name of the cache's Yelo Globals when the user doesn't supply a definition
		static const char * k_default_name = "there they are all standing in a row";

		const short version;
		unsigned short flags;
		uint base_address;

		tag_string mod_name;

		tag_reference explicit_references;
		long:8 * sizeof(long) * 8;

		tag_block:8 * sizeof(tag_block) * 1;

		/* !-- UI --! */
		struct {
			tag_reference:8 * sizeof(tag_reference) * 3; // 48
			unsigned long : 32;
			Yelo::TagBlock<const s_project_yellow_scripted_ui_widget> scripted_widgets; // 128
		}           ui;
		/* !-- UI --! */


		/* !-- Netgame --! */
		struct {
			Yelo::TagBlock<const s_network_game_player_unit> player_units; // 32

			long:8 * sizeof(long) * 5; // 20
		}           networking;
		/* !-- Netgame --! */


		/* !-- Scripting --! */
		Yelo::TagBlock<const s_scripting_definitions> yelo_scripting; // 1
		/* !-- Scripting --! */


		long:8 * sizeof(long) * 20; // 80

		project_yellow_globals() : version(project_yellow::k_version) {
			flags = FLAG(Flags::_project_yellow_null_definition_bit) | FLAG(Flags::_project_yellow_invalid_version_bit);
			explicit_references.tag_index = datum_index::null();
		}

	private:
		void CullInvalidNetworkPlayerUnits() {
			for (long x = networking.player_units.Count - 1; x >= 0; x--) {
				const TagGroups::s_network_game_player_unit &player_unit = networking.player_units[x];
				bool remove_element = true;

				if (player_unit.name[0] == '\0')
					Yelo::blam::error(Yelo::Enums::_error_message_priority_warning, "CheApe: Culling unnamed network_game_player_unit element #%n\n", x);
				else if (player_unit.definition.tag_index.IsNull())
					Yelo::blam::error(Yelo::Enums::_error_message_priority_warning, "CheApe: Culling invalid network_game_player_unit element #%n (%s)\n", x, player_unit.name);
				else
					remove_element = false;

				if (remove_element)
					blam::tag_block_delete_element(networking.player_units, x);
			}
		}

	public:
		void Cull() {
			CullInvalidNetworkPlayerUnits();
		}
	};
};

