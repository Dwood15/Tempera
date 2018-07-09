/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

// [terminology]	- [tag group]
// yelo scenario	- project_yellow
// yelo globals		- project_yellow_globals
// blam scenario	- scenario

#include "yelo_scenario_definite.h"
#include "../cache/cache_files.hpp"
#include "../hs/hs_yelo.hpp"
#include "yelo_global_definitions.h"

namespace Yelo::Scenario {
	using namespace TagGroups;
	static Yelo::TagGroups::project_yellow_globals null_yelo_globals;
	static project_yellow         null_yelo;
	static project_yellow         null_yelo_invalid(true);
	static project_yellow_globals *g_py_globals     = &null_yelo_globals;
	static project_yellow         *g_project_yellow = &null_yelo;

	static void FindYeloDefinitions();
	static bool VerifyYeloScriptDefinitions();

	void ProjectYellowInitialize() {}

	void ProjectYellowDispose() {}

	// should be called in scenario_tags_load
	static void ProjectYellowInitializeForNewMap() {
		FindYeloDefinitions();

		// Just incase someone else comes out with a tool which
		// sets the bit when they protect the cache, we don't want
		// to undo their courtesy
		if (!g_project_yellow->IsNull() && !g_project_yellow->IsCacheProtected()) {
			bool is_protected = TagGroups::Instances()[0].group_tag == Enums::k_protected_group_tag;

			SET_FLAG(g_project_yellow->flags, Flags::_project_yellow_cache_is_protected_bit, is_protected);
		}

		if (!VerifyYeloScriptDefinitions()) {
			PrepareToDropError(
				"This map's yelo script definitions appear to differ from this build of OS. "
				"We're probably about to crash...");
		}
	}

	static void InitializeForNewYeloDefinitions(datum_index yelo_index, datum_index yelo_globals_index) {
		if (!yelo_index.IsNull()) {
			g_project_yellow = TagGetForModify<project_yellow>(yelo_index);

			if (g_project_yellow->version != project_yellow::k_version)
				g_project_yellow = &null_yelo_invalid;
		} else
			g_project_yellow = &null_yelo;

		if (!yelo_globals_index.IsNull()) {
			g_py_globals = TagGetForModify<project_yellow_globals>(yelo_globals_index);

			if (g_py_globals->version != project_yellow_globals::k_version)
				g_py_globals = &null_yelo_globals;
		} else
			g_py_globals = &null_yelo_globals;
	}

	static void FindYeloDefinitions() {
		s_tag_iterator iter;

		// find the yelo scenario tag
		Yelo::blam::tag_iterator_new(iter, project_yellow::k_group_tag);
		datum_index yelo_index = blam::tag_iterator_next(iter); // there should only be one yelo tag so we only need to call this once

		// find the yelo globals tag
		blam::tag_iterator_new(iter, project_yellow_globals::k_group_tag);
		datum_index yelo_globals_index = blam::tag_iterator_next(iter);

		InitializeForNewYeloDefinitions(yelo_index, yelo_globals_index);
	}

	static bool VerifyYeloScriptDefinitions() {
		bool                                   mismatch = false;

		// Verify that the map's project_yellow's script definitions are included in this
		// build of Yelo.
		const Yelo::TagBlock<const s_scripting_definitions> &user_script_block = g_project_yellow->user_scripting;
		if (user_script_block.Count == 1 &&
			 !Scripting::DefinitionsMatch(user_script_block[0])) {
			mismatch = true;
		}

		// Verify that the map's project_yellow_globals's script definitions are included in this
		// build of Yelo.
		const Yelo::TagBlock<const s_scripting_definitions> &global_script_block = g_py_globals->yelo_scripting;
		if (global_script_block.Count == 1 &&
			 !Scripting::DefinitionsMatch(global_script_block[0])) {
			mismatch = true;
		}

		return !mismatch;
	}

	// should be called in scenario_tags_unload
	void ProjectYellowDisposeFromOldMap() {
		InitializeForNewYeloDefinitions(datum_index::null, datum_index::null());
	}

	const TagGroups::project_yellow_globals *GetYeloGlobals() {
		assert(g_py_globals);

		return g_py_globals;
	}

	const TagGroups::project_yellow *GetYelo() {
		assert(g_project_yellow);

		return g_project_yellow;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Process a blam scenario for the current operating mode (editing or cache building). </summary>
	///
	/// <param name="scenario_name">  	Name of the scenario tag. </param>
	/// <param name="for_build_cache">	True if we're building a cache file, false if we're editing. </param>
	///
	/// <returns>
	/// 	Returns the blam scenario's [project_yellow] index or datum_index::null() if there isn't one or it fails to load.
	/// </returns>
	datum_index YeloPrepareDefinitions(const char * scenario_name, const bool for_build_cache);
};
