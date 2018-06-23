#pragma once

#include "script_manager.h"

//macros, yip yip.
#define FUNC_GET(funcName)       CurrentEngine.getFunctionBegin(#funcName)
//Calls every registered lua function by that event.
#define CALL_LUA_BY_EVENT(event) LuaState->call_lua_event_by_type<LuaCallbackId::##event>()

/**
 * Called variably based on fps
 * @param current_frame_tick - ticks remaining before rendering the next frame
 */
void game_tick(int current_frame_tick) {
	CALL_LUA_BY_EVENT(before_game_tick);

	auto got = FUNC_GET(game_tick);
	if (got != (uintptr_t) -1) {
		// Print(true, "Game_tick call\n");
		calls::DoCall<Convention::m_cdecl, void, int>(got, current_frame_tick);
		// Print(true, "Post-Game_tick call\n");
	}

	// Print(true, "Lua_post_tick call\n");
	CALL_LUA_BY_EVENT(after_game_tick);
	// Print(true, "Post-Lua-post-tick call\n");
}

/**
 * Called right before game loop starts, memory has already been initialized
 */
void parse_for_connect_invert() {
	// calls::DoCall<cgetFunctionBegin<true>("parse_for_connect_invert"), Convention::m_cdecl>();
	// Print(true, "Post-parse-for-connect\n");
	CALL_LUA_BY_EVENT(post_initialize);
}

void post_dll_load() {
	// Print(true, "Postdll-load\n");
	CALL_LUA_BY_EVENT(post_dll_init);
}
//don't pollute the global macro space.
#undef CALL_LUA_BY_EVENT
#undef FUNC_GET
