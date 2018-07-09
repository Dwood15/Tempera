#pragma once

#include <cstring>
#include <macros_generic.h>
#include "scenario_enums.h"
#include "definitions.h"
#include "../memory/upgrades/game_state_yelo.hpp"

namespace Yelo::StructureBSP {
	using namespace Yelo::TagGroups;

	struct s_bsp_modifier_globals {
		struct s_bsp_modifier_state {
			sbyte m_lightmap_set;
			sbyte m_sky_set;
		};

		/// <summary>	Gamestate memory for storing the current variant index of each zone. </summary>
		s_bsp_modifier_state                    *m_bsp_modifier_gamestate;
		TagBlock<const s_scenario_bsp_modifier> *m_bsp_modifier_list;
		sbyte m_bsp_modifier_bsp_map[Enums::k_maximum_structure_bsps_per_scenario_upgrade];
		const s_scenario_bsp_modifier *m_bsp_modifier;
		int m_bsp_modifier_index;

		/// <summary>	Builds the mapping between bsp index and it's modifier map. </summary>
		void BuildBSPMap() {
			const auto &modifier_list = *m_bsp_modifier_list;
			for (int i = 0; i < modifier_list.Count; i++) {
				const auto &modifier = modifier_list[i];

				m_bsp_modifier_bsp_map[modifier.bsp_index] = i;
			}
		}

		/// <summary>	Resets the per-bsp modifier values. </summary>
		void ResetForBSP() {
			m_bsp_modifier_index = NONE;
			m_bsp_modifier       = nullptr;
		}

		/// <summary>	Resets the per-map modifier values. </summary>
		void ResetForMap() {
			ResetForBSP();

			m_bsp_modifier_list = nullptr;
			memset(&m_bsp_modifier_bsp_map, NONE, sizeof(m_bsp_modifier_bsp_map));
		}

		/// <summary>	Resets the modifier gamestate. </summary>
		void ResetGamestate() {
			if (m_bsp_modifier_gamestate) {
				memset(m_bsp_modifier_gamestate, 0, Enums::k_maximum_structure_bsps_per_scenario_upgrade * sizeof(s_bsp_modifier_globals::s_bsp_modifier_state));
			}
		}
	};

	/// <summary>	Contains all globals variables for the bsp modifiers. </summary>
	static s_bsp_modifier_globals g_bsp_modifier_globals;

#pragma region Lightmap

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a lightmap set. </summary>
	///
	/// <param name="modifier_index">	Zero-based index of the modifier. </param>
	/// <param name="bsp_index">	 	Zero-based index of the bsp. </param>
	/// <param name="set_index">	 	Zero-based index of the lightmap set. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	//TODO LIGHTMAPS ENABLE
	static bool SetLightmapSet(const sbyte modifier_index, const sbyte bsp_index, const sbyte set_index) {
		if (!GameState::YeloGameStateEnabled()) {
			return false;
		}

		auto &lightmap_sets = (*g_bsp_modifier_globals.m_bsp_modifier_list)[modifier_index].lightmap_sets;
		if ((set_index < 0) || (set_index >= lightmap_sets.Count)) {
			return false;
		}

		// If the edited bsp is the current bsp, set the lightmaps
		auto &lightmap_set = lightmap_sets[set_index];
		if (Scenario::StructureBspIndex() == bsp_index) {
			// Render::Lightmaps::SetLightmaps(lightmap_set.standard_lightmap, lightmap_set.directional_lightmap_1, lightmap_set.directional_lightmap_2, lightmap_set.directional_lightmap_3);
		}
		//
		// // Update the object render state cache for all objects
		// auto            &object_iterator = Objects::c_object_iterator::all();
		// for (const auto &object : object_iterator) {
		// 	real level_of_detail_pixels = blam::object_get_level_of_detail_pixels(object.index);
		//
		// 	auto *object_data = blam::object_get(object.index);
		// 	if (object_data && !object_data->cached_render_state_index.IsNull()) {
		// 		blam::object_render_state_refresh(object_data->cached_render_state_index, object.index, level_of_detail_pixels, 1);
		// 	}
		// }
		//
		// // Store the bsps current lightmap sequence
		// g_bsp_modifier_globals.m_bsp_modifier_gamestate[modifier_index].m_lightmap_set = set_index;

		return true;
	}

	/// <summary>	Sets the stock lightmap from the bsp tag. </summary>
	static void SetStockLightmap() {
		// auto &bsp_tag = *Scenario::StructureBsp();
		//
		// Render::Lightmaps::SetLightmaps(bsp_tag.lightmap_bitmaps.tag_index);
	}

#pragma endregion
#pragma region Sky

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a sky set. </summary>
	///
	/// <param name="modifier_index">	Zero-based index of the modifier. </param>
	/// <param name="bsp_index">	 	Zero-based index of the bsp. </param>
	/// <param name="set_index">	 	Zero-based index of the sky set. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	//TODO: ENABLE FOR SKY
	static bool SetSkySet(const sbyte modifier_index, const sbyte bsp_index, const sbyte set_index) {
		auto &sky_sets = (*g_bsp_modifier_globals.m_bsp_modifier_list)[modifier_index].sky_sets;
		if ((set_index < 0) || (set_index >= sky_sets.Count)) {
			return false;
		}

		// If the edited bsp is the current bsp, set the skies
		if (Scenario::StructureBspIndex() == bsp_index) {
			// Render::Sky::Reset();
			//
			// auto &sky_set = sky_sets[set_index];
			//
			// for (auto sky : sky_set.skies) {
			// 	Render::Sky::SetSkyIndex((byte) sky.sky_index, sky.sky.tag_index);
			// }
		}

		// Store the bsps current sky set
		g_bsp_modifier_globals.m_bsp_modifier_gamestate[modifier_index].m_sky_set = set_index;

		return true;
	}

#pragma endregion
#pragma region Lightmap Scripting

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a bsp's lightmap set by name. </summary>
	///
	/// <param name="bsp_index">	Zero-based index of the bsp. </param>
	/// <param name="set_name"> 	Name of the lightmap set. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	static bool SetLightmapSetByName(const sbyte bsp_index, const char * set_name) {
		if (!GameState::YeloGameStateEnabled()) {
			return false;
		}

		if ((bsp_index < 0) || (bsp_index >= Enums::k_maximum_structure_bsps_per_scenario_upgrade)) {
			return false;
		}

		const sbyte modifier_index = g_bsp_modifier_globals.m_bsp_modifier_bsp_map[bsp_index];
		if (modifier_index == NONE) {
			return false;
		}

		// Find the sky set by name
		auto &lightmap_sets = (*g_bsp_modifier_globals.m_bsp_modifier_list)[modifier_index].lightmap_sets;

		for (int set_index = 0; set_index < lightmap_sets.Count; ++set_index) {
			if (strcmp(lightmap_sets[set_index].name, set_name) == 0) {
				return SetLightmapSet(modifier_index, bsp_index, set_index);
			}
		}

		return false;
	}

	static void *__stdcall scripting_structure_bsp_set_lightmap_set_evaluate(void **arguments) {
		struct s_arguments {
			short   bsp_index;
					  unsigned short : 16;
			const char * set_name;
		} *args = CAST_PTR(s_arguments * , arguments);
		TypeHolder result;
		result.pointer = nullptr;

		result.boolean = SetLightmapSetByName((sbyte) args->bsp_index, args->set_name);

		return result.pointer;
	}

#pragma endregion

#pragma region Sky Scripting

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a bsp's sky set by name. </summary>
	///
	/// <param name="bsp_index">	Zero-based index of the bsp. </param>
	/// <param name="set_name"> 	Name of the sky set. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	static bool SetSkySetByName(const sbyte bsp_index, const char * set_name) {
		if (!GameState::YeloGameStateEnabled()) {
			return false;
		}

		if ((bsp_index < 0) || (bsp_index >= Enums::k_maximum_structure_bsps_per_scenario_upgrade)) {
			return false;
		}

		const sbyte modifier_index = g_bsp_modifier_globals.m_bsp_modifier_bsp_map[bsp_index];
		if (modifier_index == NONE) {
			return false;
		}

		// Find the sky set by name
		auto &sky_sets = (*g_bsp_modifier_globals.m_bsp_modifier_list)[modifier_index].sky_sets;

		for (int set_index = 0; set_index < sky_sets.Count; ++set_index) {
			if (strcmp(sky_sets[set_index].name, set_name) == 0) {
				return SetSkySet(modifier_index, bsp_index, set_index);
			}
		}

		return false;
	}

	static void *__stdcall scripting_structure_bsp_set_sky_set_evaluate(void **arguments) {
		struct s_arguments {
			short   bsp_index;
					  unsigned short : 16;
			const char * set_name;
		} *args = CAST_PTR(s_arguments * , arguments);
		TypeHolder result;
		result.pointer = nullptr;

		result.boolean = SetSkySetByName((sbyte) args->bsp_index, args->set_name);

		return result.pointer;
	}

#pragma endregion

#pragma region BSP Modifier State

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets current bsp modifier. </summary>
	///
	/// <param name="bsp_index">	Zero-based index of the bsp. </param>
	void SetCurrentModifier(const short bsp_index) {
		if ((bsp_index != NONE) && (g_bsp_modifier_globals.m_bsp_modifier_bsp_map[bsp_index] != NONE)) {
			const int modifier_index = g_bsp_modifier_globals.m_bsp_modifier_bsp_map[bsp_index];

			g_bsp_modifier_globals.m_bsp_modifier_index = modifier_index;
			g_bsp_modifier_globals.m_bsp_modifier       = &(*g_bsp_modifier_globals.m_bsp_modifier_list)[modifier_index];
		} else {
			g_bsp_modifier_globals.ResetForBSP();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the current game state. </summary>
	///
	/// <param name="bsp_index">	Zero-based index of the bsp. </param>
	void SetToCurrentGameState(const short bsp_index) {
		if (GameState::YeloGameStateEnabled()
			 && (bsp_index != NONE)
			 && (g_bsp_modifier_globals.m_bsp_modifier_bsp_map[bsp_index] != NONE)) {
			const sbyte modifier_index = g_bsp_modifier_globals.m_bsp_modifier_bsp_map[bsp_index];

			const auto &gamestate = g_bsp_modifier_globals.m_bsp_modifier_gamestate[modifier_index];

			if (!SetLightmapSet(modifier_index, (sbyte) bsp_index, gamestate.m_lightmap_set)) {
				SetStockLightmap();
			}
			if (!SetSkySet(modifier_index, (sbyte) bsp_index, gamestate.m_sky_set)) {
				// Render::Sky::Reset();
			}
		} else {
			// Render::Sky::Reset();
			SetStockLightmap();
		}
	}

#pragma endregion

#pragma region Scripting

	/// <summary>	Initializes the bsp modifier script functions. </summary>
	static void InitializeScripting() {
		// Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_structure_bsp_set_lightmap_set, scripting_structure_bsp_set_lightmap_set_evaluate);
		// Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_structure_bsp_set_sky_set, scripting_structure_bsp_set_sky_set_evaluate);
	}

#pragma endregion

	/// <summary>	Initializes the bsp modifier script functions. </summary>
	void Initialize() {
		InitializeScripting();
	}

	/// <summary>	Unused. </summary>
	void Dispose() {}

	/// <summary>	Initializes for new game state. </summary>
	void InitializeForNewGameState() {
		if (!GameState::YeloGameStateEnabled()) {
			return;
		}

		g_bsp_modifier_globals.m_bsp_modifier_gamestate = GameState::GameStateMalloc<s_bsp_modifier_globals::s_bsp_modifier_state>(true, Enums::k_maximum_structure_bsps_per_scenario_upgrade);

		YELO_ASSERT_DISPLAY(g_bsp_modifier_globals.m_bsp_modifier_gamestate, "Failed to allocate bsp modifier gamestate memory");

		g_bsp_modifier_globals.ResetGamestate();
	}

	/// <summary>	Initializes for a new map. </summary>
	void InitializeForNewMap() {
		g_bsp_modifier_globals.ResetForMap();
		g_bsp_modifier_globals.ResetGamestate();

		auto &scenario = *Scenario::Scenario();
		g_bsp_modifier_globals.m_bsp_modifier_list = &scenario.bsp_modifiers;

		g_bsp_modifier_globals.BuildBSPMap();

		// InitializeForNewMap is called after the first InitializeForBSPLoad so re-call here to set up for the first BSP
		// InitializeForNewBSP();
	}

	/// <summary>	Resets the bsp modifier system. </summary>
	void DisposeFromOldMap() {
		g_bsp_modifier_globals.ResetForMap();
	}

	/// <summary>	Initializes for new bsp. </summary>
	void InitializeForNewBSP() {
		const short bsp_index = Scenario::StructureBspIndex();

		SetCurrentModifier(bsp_index);
		SetToCurrentGameState(bsp_index);
	}

	/// <summary>	Resets the bsp modifier variables. </summary>
	void DisposeFromOldBSP() {
		g_bsp_modifier_globals.ResetForBSP();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles the game state life cycle described by life_state. </summary>
	///
	/// <param name="life_state">	State of the gamestate. </param>
	void HandleGameStateLifeCycle(short life_state) {
		switch (life_state) {
			case Enums::_game_state_life_cycle_before_save:
				break;
			case Enums::_game_state_life_cycle_before_load:
				// reset the gamestate to default values
				g_bsp_modifier_globals.ResetGamestate();
				break;
			case Enums::_game_state_life_cycle_after_load:
				SetToCurrentGameState(Scenario::StructureBspIndex());
				break;
		}
	}
};
