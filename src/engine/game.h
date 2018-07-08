#pragma once

//////////////////////////////////////////////////////////////////////////
// game
#include "../ce_base_types.h"
#include "../CurrentEngine.h"

namespace blam
{
	//////////////////////////////////////////////////////////////////////////
	// cheats.c
	__declspec(naked) void __cdecl cheat_all_weapons()
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_ALL_WEAPONS);

		__asm {
		call	FUNCTION
		retn
		}
	}
	__declspec(naked) void __cdecl cheat_spawn_warthog()
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_SPAWN_WARTHOG);

		__asm {
		call	FUNCTION
		retn
		}
	}
	__declspec(naked) void __cdecl cheat_teleport_to_camera()
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_TELEPORT_TO_CAMERA);

		__asm {
		call	FUNCTION
		retn
		}
	}
	__declspec(naked) void __cdecl cheat_active_camouflage()
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE);

		__asm {
		call	FUNCTION
		retn
		}
	}
	__declspec(naked) void __cdecl cheat_active_camouflage_local_player()
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER);

		__asm {
		call	FUNCTION
		retn
		}
	}
	__declspec(naked) datum_index __cdecl cheat_local_player()
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_PLAYER_INDEX);

		__asm {
		call	FUNCTION
		retn
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// game_allegiance.c
	bool __cdecl game_team_is_enemy(signed long team, signed long team_to_test)
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(GAME_TEAM_IS_ENEMY);

		__asm {
		mov		ecx, team_to_test
		mov		edx, team
		call	FUNCTION
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// game_engine.c
	datum_index __cdecl find_closest_player_index(datum_index player_index)
	{
		return Engine::Players::FindClosestPlayerIndex(player_index);
	}


	__declspec(naked) void __cdecl game_engine_rasterize_message(wstring message, real alpha)
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(GAME_ENGINE_RASTERIZE_MESSAGE);

		
		push	alpha
		push	message
		call	FUNCTION
		add		esp, 4 * 2
		API_FUNC_NAKED_END_NO_STACK_POP()
	}
	//////////////////////////////////////////////////////////////////////////
	// game_engine_multiplayer_sounds.c
	__declspec(naked) void __cdecl game_engine_play_multiplayer_sound(datum_index player_index, shortmultiplayer_sound_index, bool should_replicate)
	{
		static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(GAME_ENGINE_PLAY_MULTIPLAYER_SOUND);

		
		push	esi
		push	edi

		movzx	esi, should_replicate
		push	esi
		movsx	esi, multiplayer_sound_index
		mov		edi, player_index
		call	FUNCTION
		add		esp, 4 * 1

		pop		edi
		pop		esi
		API_FUNC_NAKED_END_NO_STACK_POP()
	}
	//////////////////////////////////////////////////////////////////////////
	// players.c






	//////////////////////////////////////////////////////////////////////////
	// player_control.c
	__declspec(naked) void __cdecl player_control_get_unit_camera_info(const short player_index, Players::s_unit_camera_info& camera_info) {
		static const uintptr_t FUNCTION = CurrentEngine.getFunctionBegin("PLAYER_CONTROL_GET_UNIT_CAMERA_INFO");

		__asm {
		push   eax
		push   ecx
		push   edx
		push   esi

		mov      ax, player_index
		mov      esi, camera_info
		call   FUNCTION

		pop      esi
		pop      edx
		pop      ecx
		pop      eax
		pop      ebp
		}
	}
};
