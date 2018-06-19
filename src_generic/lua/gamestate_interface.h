#pragma once
#include <precompile.h>
#include <lua.h>
#include <versions.h>
#include "../gamestate/core.h"
#include "../gamestate/forge.h"

/**
 * Tells the lua script whether or not the player is in the main menu.
 * @param L Lua state.
 * @return (In C) # args for lua.
 */
static int l_InMainMenu(lua_State * L) {
	lua_pushboolean(L, *at_main_menu);
	return 1;
}

/**
 * Returns (to Lua) the engine state this dll was compiled with targets for.
 * Sapien as a compiler target will shortly arrive.
 * @param L Lua state.
 * @return (In C) # args for lua.
 */
static int l_GetEngineContext(lua_State * L) {
	if constexpr (engines::platform_has_support<current_engine::ENGINE_TARGET, current_engine::GAME_MINOR>()) {
		switch (current_engine::ENGINE_TARGET) {
			case engines::major::DEDI:
				lua_pushstring(L, "DEDI");
				break;
			case engines::major::HEK:
				lua_pushstring(L, "HEK");
				break;
			case engines::major::PC:
				lua_pushstring(L, "PC");
				break;
			case engines::major::MAC:
				lua_pushstring(L, "MAC");
				break;
			case engines::major::STUBBZ:
				lua_pushstring(L, "STUBBZ");
				break;
			case engines::major::TRIAL:
				lua_pushstring(L, "TRIAL");
				break;
			case engines::major::CE:
				lua_pushstring(L, "CE");
				break;
			default:
				lua_pushstring(L, "");
				goto unsupported;
		}

		lua_pushinteger(L, current_engine::GAME_MINOR);
		lua_pushboolean(L, true);
		return 3;
	}

unsupported:
	lua_pushstring(L, "");
	lua_pushnumber(L, -1);
	lua_pushboolean(L, false);
	return 3;
}
