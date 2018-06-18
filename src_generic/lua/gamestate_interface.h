#pragma once
#include <precompile.h>
#include <lua.h>
#include <versions.h>
#include "../gamestate/core.h"
#include "../gamestate/forge.h"

int l_InMainMenu(lua_State * L) {
	if(!core) {
		lua_pushboolean(L, false);
		return 1;
	}

	lua_pushboolean(L, *at_main_menu);
	return 1;
}

int l_GetEngineContext(lua_State * L) {
	if constexpr (supported_target()) {
		switch (ENGINE_TARGET) {
			case engines::major::DEDI:
				lua_pushstring(L, "dedi");
				break;
			case engines::major::HEK:
				lua_pushstring(L, "hek");
				break;
			case engines::major::PC:
				lua_pushstring(L, "pc");
				break;
			case engines::major::MAC:
				lua_pushstring(L, "mac");
				break;
			case engines::major::STUBBZ:
				lua_pushstring(L, "stubbz");
				break;
			case engines::major::TRIAL:
				lua_pushstring(L, "trial");
				break;
			case engines::major::CE:
				lua_pushstring(L, "ce");
				break;
			default:
				lua_pushstring(L, "");
		}

		lua_pushinteger(L, GAME_MINOR);
		lua_pushboolean(L, true);
		return 3;
	}

	lua_pushstring(L, "");
	lua_pushnumber(L, -1);
	lua_pushboolean(L, false);
	return 3;
}
