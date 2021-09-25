#pragma once

//////////////////////////////////////////////////////////////////////////
// game
#include "../RuntimeManager.h"
#include "../cseries/MacrosCpp.h"
#include "../gamestate/objects/units/unit_camera.h"


//TODO C++ Definition file and use a FUCKINGINLINEMSVCYOULITTLESHIT macro that forces these functions to be inlined.
namespace blam {

	bool callFunc(::std::optional<uintptr_t> addr) {
		if (addr) {
			calls::DoCall(*addr);
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// cheats.c
	__forceinline void  cheat_all_weapons() {
		callFunc(CurrentRuntime->getFunctionBegin("cheat_all_weapons"));
	}

	void cheat_spawn_warthog() {
		callFunc(CurrentRuntime->getFunctionBegin("cheat_spawn_warthog"));
	}

	void cheat_teleport_to_camera() {
		callFunc(CurrentRuntime->getFunctionBegin("cheat_teleport_to_camera"));
	}

	void cheat_active_camouflage() {
		callFunc(CurrentRuntime->getFunctionBegin("cheat_active_camouflage"));
	}

	void cheat_active_camouflage_local_player() {
		callFunc(CurrentRuntime->getFunctionBegin("cheat_active_camouflage_local_player"));
	}

	datum_index cheat_local_player() {
		static const auto FUNCTION = CurrentRuntime->getFunctionBegin("cheat_player_index");

		if (!FUNCTION) {
			return datum_index::null();
		}

		return calls::DoCall<Convention::m_cdecl, datum_index>(*FUNCTION);
	}

	//////////////////////////////////////////////////////////////////////////
	// game_allegiance.c
	bool game_team_is_enemy(signed long team, signed long team_to_test) {
		return calls::DoCall<Convention::m_fastcall, bool, short, short>(*CurrentRuntime->getFunctionBegin("game_team_is_enemy"), team, team_to_test);
	}

	//////////////////////////////////////////////////////////////////////////
	// game_engine.c
	datum_index find_closest_player_index(datum_index player_index) {
		static const auto FUNCTION = CurrentRuntime->getFunctionBegin("find_closest_player_index");

		if (!FUNCTION) {
			return datum_index::null();
		}

		return calls::DoCall<Convention::m_cdecl, datum_index, datum_index>(*FUNCTION, player_index);
	}

	void game_engine_rasterize_message(wstring message, real alpha) {
		static const auto FUNCTION = CurrentRuntime->getFunctionBegin("game_engine_rasterize_message");
		calls::DoCall<Convention::m_cdecl, void, wstring, real>(*FUNCTION, message, alpha);
	}

	//////////////////////////////////////////////////////////////////////////
	// game_engine_multiplayer_sounds.c
	void game_engine_play_multiplayer_sound(datum_index player_index, short multiplayer_sound_index, bool should_replicate) {
		static const auto FUNCTION = CurrentRuntime->getFunctionBegin("game_engine_play_multiplayer_sound");

		if (!FUNCTION) {
			return;
		}

		//TODO: Verify consistency with Sapien
		__asm movsx   esi, multiplayer_sound_index
		__asm mov   edi, player_index
		calls::DoCall<Convention::m_cdecl, void, bool>(*FUNCTION, should_replicate);
	}
	//////////////////////////////////////////////////////////////////////////
	// players.c


	//////////////////////////////////////////////////////////////////////////
	// player_control.c
	void player_control_get_unit_camera_info(const short player_index, Yelo::TagGroups::s_unit_camera_info &camera_info) {
		static const auto FUNCTION = CurrentRuntime->getFunctionBegin("player_control_get_unit_camera_info");

		if (!FUNCTION) {
			return;
		}

		__asm mov      ax, player_index
		__asm mov      esi, camera_info
		calls::DoCall<>(*FUNCTION);
	}
};
