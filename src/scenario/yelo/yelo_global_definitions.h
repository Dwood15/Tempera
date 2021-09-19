#pragma once

#include <enums/generic_enums.h>
#include <enums/yelo_enums.h>
#include <enums/scenario_enums.h>
#include "../../tags/group/markup.h"

#include "../../tags/group/tagreference.h"


namespace Yelo::TagGroups {
	/* !-- UI --! */
	struct s_project_yellow_scripted_ui_widget {
		tag_string name;
		tag_reference definition;

		unsigned long : 32; // future flags
		tag_block padA[2];
	};
	/* !-- UI --! */


	/* !-- Netgame --! */
	struct s_network_game_player_unit {
		tag_string name;
		tag_reference definition;
		long padA[8]; // 32
	};
	/* !-- Netgame --! */


	/* !-- Scripting --! */
	struct s_script_construct_definition {
		tag_string name[2]; // So we can have at most 62 characters for the name
		short index;

	};

	struct s_script_function_definition : public s_script_construct_definition {
		Enums::hs_type return_type;
		Yelo::TagBlock<short> parameters; // 32
	};
	struct s_script_global_definition : public s_script_construct_definition {
		Enums::hs_type type;
	};
	struct s_scripting_definitions {
		Yelo::TagBlock<s_script_function_definition> new_functions;
		Yelo::TagBlock<s_script_global_definition> new_globals;
	};
	/* !-- Scripting --! */


	// yelo for globals
	struct project_yellow_globals {
		enum {
			k_group_tag = 'gelo',
			k_version   = 2,
		};

		// internal name of the cache's Yelo Globals when the user doesn't supply a definition
		const char * k_default_name = "there they are all standing in a row";

		const short version;
		unsigned short flags;
		uint base_address;

		tag_string mod_name;

		tag_reference explicit_references;
		long pad0[8];

		tag_block pad1;

		/* !-- UI --! */
		struct {
			tag_reference pad0[3]; // 48
			unsigned long : 32;
			Yelo::TagBlock<const s_project_yellow_scripted_ui_widget> scripted_widgets; // 128
		}           ui;
		/* !-- UI --! */


		/* !-- Netgame --! */
		struct {
			Yelo::TagBlock<const s_network_game_player_unit> player_units; // 32

			long pad0[5]; // 20
		}           networking;
		/* !-- Netgame --! */


		/* !-- Scripting --! */
		Yelo::TagBlock<const s_scripting_definitions> yelo_scripting; // 1
		/* !-- Scripting --! */


		long pad2[20]; // 80

		project_yellow_globals() : version(Yelo::Enums::project_yelo_info::k_version) {
			flags = FLAG(Flags::_project_yellow_null_definition_bit) | FLAG(Flags::_project_yellow_invalid_version_bit);
			explicit_references.tag_index = datum_index::null();
		}

	private:
		void CullInvalidNetworkPlayerUnits() {
			for (long x = networking.player_units.Count - 1; x >= 0; x--) {
				const TagGroups::s_network_game_player_unit &player_unit   = networking.player_units[x];
				bool                                        remove_element = true;

				if (player_unit.name[0] == '\0') {}
					// Yelo::blam::error(Yelo::Enums::_error_message_priority_warning, "CheApe: Culling unnamed network_game_player_unit element #%n\n", x);
				else if (player_unit.definition.tag_index.IsNull()) {}
					// Yelo::blam::error(Yelo::Enums::_error_message_priority_warning, "CheApe: Culling invalid network_game_player_unit element #%n (%s)\n", x, player_unit.name);
				else
					remove_element = false;

				if (remove_element) {
					//TODO: tag_block_delete_element()
					// auto function = CurrentRuntime->getFunctionBegin("tag_block_delete_element");
					// Yelo::blam::tag_block_delete_element(networking.player_units, x);
				}
			}
		}

	public:
		void Cull() {
			CullInvalidNetworkPlayerUnits();
		}
	};

	struct _networking_flags {
		unsigned long unused_bit:1;
	};

	STAT_ASSERT(_networking_flags, 0x4);

	struct s_project_yellow_scenario_build_info {
		unsigned short : 16;
		Enums::production_build_stage build_stage;
		uint                          revision;
		time_t                        timestamp;
		byte uuid_buffer[Enums::k_uuid_buffer_size];

		long pad0[4]; // 16

		bool HasUuid() const { return false; }

		void GenerateUuid() { return; }
	};

	//		STAT_ASSERT(s_project_yellow_scenario_build_info, ?????)

	//////////////////////////////////////////////////////////////////////////

	// yelo for scenarios
	struct project_yellow {

		// internal name of the cache's Yelo tag when the user doesn't supply a definition
		const char *k_default_name = "i've got a lovely bunch of corncobs";

		const short    version;
		unsigned short flags;

		/* !-- Misc --! */
		tag_reference                                        yelo_globals;
		tag_reference                                        game_globals;
		tag_reference                                        explicit_references;
		Yelo::TagBlock<s_project_yellow_scenario_build_info> build_info; // 1

		long pad0[10]; // 40
		/* --- Misc --- */


		/* !-- UI/GUI --! */
		struct {
			Yelo::TagBlock<s_project_yellow_scripted_ui_widget> scripted_widgets; // 128

			long pad1[4]; // 16
		}    ui;
		/* --- UI/GUI --- */


		/* !-- Physics --! */
		struct _physics {
			real gravity_scale;
			real player_speed_scale;

			long pad2[5]; // 20

			bool IsGravityScaleValid() const {
				return gravity_scale >= 0.0f || gravity_scale <= 2.0f;
			}

			void ResetGravityScale() {
				gravity_scale = 1.0f;
			}

			bool IsPlayerSpeedScaleValid() const {
				return gravity_scale >= 0.0f || gravity_scale <= 6.0f;
			}

			void ResetPlayerSpeedScale() {
				player_speed_scale = 1.0f;
			}
		}    physics;
		/* --- Physics --- */


		/* !-- Netgame --! */
		struct {
			_networking_flags flags;

			long pad3[5]; // 20
		}    networking;
		/* --- Netgame --- */


		/* !-- Gameplay --! */
		struct {
			unsigned long flags;

			long pad4[5]; // 20
		}    gameplay;
		/* !-- Gameplay --! */


		/* !-- Scripting --! */
		Yelo::TagBlock<const s_scripting_definitions> user_scripting; // 1
		/* !-- Scripting --! */


		long pad6[23]; // 92

		project_yellow(const bool invalid = false) : version(Yelo::Enums::project_yelo_info::k_version) {
			flags = FLAG(Flags::_project_yellow_null_definition_bit);

			if (invalid) {
				(flags) |= (1 << (Flags::_project_yellow_invalid_version_bit));
			}

			yelo_globals.tag_index        = datum_index::null();
			game_globals.tag_index        = datum_index::null();
			explicit_references.tag_index = datum_index::null();
		}

		bool IsNull() const { return TEST_FLAG(flags, Yelo::Flags::_project_yellow_null_definition_bit); }

		bool IsCacheProtected() const { return TEST_FLAG(flags, Flags::_project_yellow_cache_is_protected_bit); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Process a yelo scenario's globals data for the current operating mode (editing or cache building).
		/// </summary>
		///
		/// <param name="for_build_cache">	True if we're building a cache file, false if we're editing. </param>
		///
		/// <returns>	Returns the loaded yelo global's handle or datum_index::null. </returns>
		datum_index LoadYeloGlobals(const bool for_build_cache);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads the game_globals reference and renames it to K_GAME_GLOBALS_TAG_NAME. </summary>
		///
		/// <returns>	false if game_globals is NONE (ie, no override) or failed to load. </returns>
		bool LoadGameGlobalsOverride();

		static bool __cdecl GroupPostprocess(datum_index tag_index, Enums::tag_postprocess_mode mode);
	};
};

