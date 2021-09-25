#pragma once

#include "macros_generic.h"
#include "../memory/datum_index.h"
#include "../math/real_math.h"
#include "scenario.h"
#include "definitions.h"

namespace Yelo {
		// Globals tag data
// 		TagGroups::s_game_globals **GlobalGameGlobalsReference();
//
// #define blam_global_game_globals      *(GameState::GlobalGameGlobalsReference())

		// TagGroups::s_game_globals *GlobalGameGlobals();

	// _Not_ a fan!
	namespace Scenario {
		datum_index *GlobalScenarioIndexReference();

		// ::Yelo::TagGroups::scenario **GlobalScenarioReference();

		// short *GlobalStructureBspIndexReference();

		// TagGroups::structure_bsp **GlobalStructureBspReference();
		//
		// TagGroups::collision_bsp **GlobalBspReference();
		//
		// TagGroups::collision_bsp **GlobalCollisionBspReference();

#define blam_global_scenario_index      *(Scenario::GlobalScenarioIndexReference())
#define blam_global_scenario         *(Scenario::GlobalScenarioReference())
// #define blam_global_structure_bsp_index   *(Scenario::GlobalStructureBspIndexReference())
// #define blam_global_structure_bsp      *(Scenario::GlobalStructureBspReference())
// #define blam_global_bsp3d            *(Scenario::GlobalBspReference())
// #define blam_global_collision_bsp      *(Scenario::GlobalCollisionBspReference())

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the currently loaded scenario. </summary>
		// Yelo::TagGroups::scenario *Scenario();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the currently loaded SBPS's bsp3d block. </summary>
		// TagGroups::collision_bsp *Bsp3d();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the currently loaded SBPS's collision bsp block. </summary>
		// TagGroups::collision_bsp *CollisionBsp();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the currently loaded SBPS's. </summary>
		// TagGroups::structure_bsp *StructureBsp();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tag index of the currently loaded scenario. </summary>
		datum_index ScenarioIndex();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Scenario structure bsp reference index of the currently loaded SBSP. </summary>
		short StructureBspIndex();
	};

	namespace blam {
		// TagGroups::s_game_globals *scenario_get_game_globals() {
		// 	return GameState::GlobalGameGlobals();
		// }

		// TagGroups::scenario *global_scenario_get() {
		// 	return Scenario::Scenario();
		// }
		//
		// TagGroups::scenario *global_scenario_try_and_get() {
		// 	return Scenario::Scenario();
		// }

		// TagGroups::structure_bsp *global_structure_bsp_get() {
		// 	return Scenario::StructureBsp();
		// }
		//
		// TagGroups::collision_bsp *global_collision_bsp_get() {
		// 	return Scenario::CollisionBsp();
		// }

		// Switches the bsp being used to the bsp at [bsp_index] in the scenario structure bsps block
		// returns true if the switch executed. if [bsp_index] is invalid or equal to the current index, returns false
		bool __cdecl scenario_switch_structure_bsp(short bsp_index);

		// Tests to see if [point] exists inside [trigger_volume_index] (index to a definition in the current scenario)
		bool __cdecl scenario_trigger_volume_test_point(long trigger_volume_index, const real_point3d &point);

		// Tests to see if [object_index] is inside [trigger_volume_index] (index to a definition in the current scenario)
		bool __cdecl scenario_trigger_volume_test_object(long trigger_volume_index, datum_index object_index);

		bool __cdecl scenario_load(const char *  scenario_name);

		void __cdecl scenario_unload();

		datum_index __cdecl scenario_tags_load(const char *  scenario_name);

//		datum_index __cdecl scenario_tags_load_impl(const char *  scenario_name) {
//			datum_index scenario_index = datum_index::null();
//
//			// texture_cache_open();
//			// sound_cache_open();
//			// scenario_index = cache_file_tags_load(scenario_name);
//			//
//			// Scenario::ProjectYellowInitializeForNewMap();
//
//			return scenario_index;
//		}

		void __cdecl scenario_tags_unload();

		//TODO: Cache files!
		// void scenario_structure_bsp_unload(TagGroups::scenario_structure_bsp_reference *reference) {
		// 	cache_file_structure_bsp_unload(reference);
		// 	reference->structure_bsp.tag_index = datum_index::null();
		// }
		//
		// bool scenario_structure_bsp_load(TagGroups::scenario_structure_bsp_reference *reference) {
		// 	return cache_file_structure_bsp_load(reference);
		// }

//		void __cdecl scenario_tags_unload_impl() {
//			// texture_cache_close();
//			// sound_cache_close();
//			//
//			// Scenario::ProjectYellowDisposeFromOldMap();
//			//
//			// cache_file_tags_unload();
//		};

//		void __cdecl scenario_unload_impl() {
//			//assert(!bink_playback_active());
//			//scenario_tags_unload_impl();
//			blam_global_scenario_index      = datum_index::null();
//			// blam_global_structure_bsp_index = NONE;
//			//Scenario::ScenarioGlobals()->current_structure_bsp_index = NONE;
//			// blam_global_scenario      = nullptr;
//			// blam_global_structure_bsp = nullptr;
//			// blam_global_collision_bsp = nullptr;
//			// blam_global_bsp3d         = nullptr;
//			// blam_global_game_globals  = nullptr;
//		}

//		bool __cdecl scenario_load_impl(const char *  scenario_name) {
			// tag_load_error_string_clear();
			//
			// datum_index scenario_index = scenario_tags_load_impl(scenario_name);
			// blam_global_scenario_index = scenario_index;
			//
			// if (scenario_index.IsNull()) {
			// 	tag_load_error_string_print();
			// 	return false;
			// }
			//
			// auto *scenario = TagGetForModify<TagGroups::scenario>(scenario_index);
			// blam_global_scenario = scenario;
			//
			// if (scenario->structure_bsps.Count == 0) {
			// 	YELO_WARN("scenario doesn't have a structure bsp");
			// 	return false;
			// }
			//
			// datum_index game_globals_index = tag_loaded<s_game_globals>(Scenario::K_GAME_GLOBALS_TAG_NAME);
			// // NOTE: engine doesn't actually do this
			// if (game_globals_index.IsNull()) {
			// 	YELO_WARN("failed to load game globals for scenario");
			// 	return false;
			// }
			//
			// auto *game_globals = TagGetForModify<s_game_globals>(game_globals_index);
			// blam_global_game_globals = game_globals;
			//
			// // NOTE: engine doesn't print an error message
			// if (!scenario_switch_structure_bsp(0)) {
			// 	YELO_WARN("failed to switch to the scenario's first bsp");
			// 	return false;
			// }
			//
			// for (scenario_netgame_equipment &equipment : scenario->netgame_equipment)
			// 	equipment.runtime_object_index = datum_index::null();
			//
//			return true;
//		}
	};
};
