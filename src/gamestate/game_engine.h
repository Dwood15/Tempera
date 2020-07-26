#pragma once

#include <enums/engine.h>
#include <cassert>
#include <enums/memory_enums.h>
#include <xutility>
#include <engine_interface.h>
#include "../memory/memory_interface_base.hpp"
#include "../game/game_engine.h"
#include "game_types.h"
#include "../cseries/MacrosCpp.h"
// reinterpret_cast<void ***>(0x4B466C);
namespace Yelo::GameEngine {
	s_ctf_globals *CtfGlobals() {
		return reinterpret_cast<Yelo::GameEngine::s_ctf_globals *>(0x64BDB8);
	}

	s_slayer_globals *SlayerGlobals() {
		return reinterpret_cast<Yelo::GameEngine::s_slayer_globals *>(0x64C308);
	}

	s_oddball_globals *OddballGlobals() {
		return reinterpret_cast<Yelo::GameEngine::s_oddball_globals *>(0x64C078);
	}

	s_king_globals *KingGlobals() {
		return reinterpret_cast<Yelo::GameEngine::s_king_globals *>(0x64BDF0);
	}

	s_race_globals *RaceGlobals() {
		return reinterpret_cast<Yelo::GameEngine::s_race_globals *>(0x64C1C0);
	}

	s_game_variant *GlobalVariant() {
		return reinterpret_cast<Yelo::GameEngine::s_game_variant *>(0x68CC48);
	}

	const game_engine_definition *const *Definitions() {
		return reinterpret_cast<Yelo::GameEngine::game_engine_definition **>(0x6238C8);
	}

	const game_engine_definition *Current() {
		return *reinterpret_cast<Yelo::GameEngine::game_engine_definition **>(0x68CD24);

	}

#pragma region New Engines functions
	static game_engine_definition *new_definitions[Enums::_game_engine_custom + 1] = {
		// 			nullptr,
		// 			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // old game engines
		// 			nullptr, nullptr, nullptr, nullptr, nullptr, // space for new game engines
		// 			nullptr, // terminator
	};
	static int32                  new_definitions_count                            = Enums::k_number_of_game_engines;
#define GAME_ENGINE_REAL_COUNT (Enums::k_number_of_game_engines - 2)

	const game_engine_definition *const *NewDefinitions() { return new_definitions; }

	int32 NewDefinitionsCount() { return new_definitions_count; }

	void RegisterNewGameEngine(game_engine_definition *engine) {
		if (new_definitions_count < Enums::_game_engine_custom)
			new_definitions[new_definitions_count++] = engine;
	}

	static void FixHudRenderUnitInterface() {

		static void *jmp_table[Enums::_game_engine_custom - 1];

		// TODO: need to modify pre-jump table code before this works the way we want it to

		// copy the game's jump table into our jmp table
		for (int32 x = 0; x < Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x++)
			//(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE)
			jmp_table[x] = *(void **) (0x424953 + (sizeof(void *) * x));

		// just use the slayer variant of the code, it doesn't matter to us.
		for (uint x = Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x < std::size(jmp_table); x++)
			//(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE)
			jmp_table[x] = *(void **) (0x424953 + (sizeof(void *) * (Enums::_game_engine_slayer - 1)));

		// set the game's jump table address to our's
		//hud_render_unit_interface_jmp_ptr
		**reinterpret_cast<void ***>(0x4B466C) = jmp_table;
	}

	static void FixFunc4F7440() // function for the engine vehicle remapper
	{
		static void *jmp_table[Enums::_game_engine_custom - 1];

		// TODO: reference jump table code for the changes to make this work the way we want it to

		// copy the game's jump table into our jmp table
		for (int32 x = 0; x < Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x++)
			jmp_table[x] = *(void **) (0x4F72CC + (sizeof(void *) * x));
			// jmp_table[x] = *(void **) (GET_FUNC_PTR(Func4F7440JmpTable) + (sizeof(void *) * x));

		// just use the slayer variant of the code, it doesn't matter to us.
		for (uint x = Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x < std::size(jmp_table); x++)
			jmp_table[x] = *(void **) (0x4F72CC + (sizeof(void *) * (Enums::_game_engine_slayer - 1)));

		// set the game's jump table address to our's
		**reinterpret_cast<void ***>(0x4F71E5) = jmp_table;
		// GET_PTR(Func4F7440JmpPtr) = jmp_table;
	}

	static void FixFunc4F7580() // function for the engine vehicle remapper
	{
		// static void *jmp_table[Enums::_game_engine_custom - 1];
		//
		// // TODO: reference jump table code for the changes to make this work the way we want it to
		//
		// // copy the game's jump table into our jmp table
		// for (int32 x = 0; x < Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x++)
		// 	jmp_table[x] = *(void **) (GET_FUNC_PTR(Func4F7580JmpTable) + (sizeof(void *) * x));
		//
		// // just use the slayer variant of the code, it doesn't matter to us.
		// for (int32 x = Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x < std::size(jmp_table); x++)
		// 	jmp_table[x] = *(void **) (GET_FUNC_PTR(Func4F7580JmpTable) + (sizeof(void *) * (Enums::_game_engine_slayer - 1)));
		//
		// // set the game's jump table address to our's
		// GET_PTR(Func4F7580JmpPtr) = jmp_table;
	}

#pragma endregion

	void Dispose() {
	}

	void InitializeForNewMap() {
		if (GameEngine::Current()) {
			// GameState::WriteEvent(GameState::GameGlobals()->options.map_name);
			// GameState::WriteEvent("", false);
		}
	}

	void Update() {
		const real k_update_normal       = 1.6666668e-2f; // 0.016666668
		const real k_update_double_speed = 3.3333335e-2f; // 0.033333335
		//
		// real delta_time = !GameState::GameGlobals()->players_are_double_speed
		// 						? k_update_normal
		// 						: k_update_double_speed;
		//
		// GameState::Update(delta_time);

		// NOTE: Place any update code in the GameState's Update function, or the
		// Update function of the corresponding component (ie, Effect, Players, etc)
		// Don't mess with the GameEngine Update function unless it's related to the game_engine
	}

	naked static void GameEngineUpdate_Hook() {
		API_FUNC_NAKED_START_()
		call   GameEngine::Update
		API_FUNC_NAKED_END_()
	}

	void Initialize() {
		// 		::memory::CreateHookRelativeCall(&GameEngine::InitializeForNewMap, GET_FUNC_VPTR(GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK),
		// 												 Enums::_x86_opcode_ret);
		// 		::memory::WriteRelativeJmp(&GameEngineUpdate_Hook, GET_FUNC_VPTR(GAME_ENGINE_UPDATE_HOOK), true);
		//
		// #pragma region New Engines implementation
		// 		// copy the game engine pointers into our list
		// 		for (int32 x = 0; x < new_definitions_count; x++)
		// 			new_definitions[x] = GET_DPTR2(game_engines)[x];
		//
		// 		// replace the game's game_engines list with ours
		// 		GET_PTR(game_engine_init_mod) = new_definitions;
		//
		// 		FixHudRenderUnitInterface();
		// 		FixFunc4F7440();
		// 		FixFunc4F7580();
		// #pragma endregion
	}
};
