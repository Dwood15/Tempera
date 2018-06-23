#pragma once
#include <lua.h>
#include "../gamestate/core.h"
#include "../gamestate/forge.h"
#include <precompile.h>
#include <versions.h>

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
 * @return (In C) Num args for lua.
 */
static int l_GetEngineContext(lua_State * L) {

		const char * MAJSTR = CurrentEngine.GetCurrentMajorVerString();

		//TODO More refactor.
		if (strcmp(MAJSTR, "") == 0)
			goto unrecognized;

		lua_pushstring(L, MAJSTR);


		lua_pushinteger(L, CurrentEngine.GetMinorVersion());
		lua_pushboolean(L, true);
		return 3;

unrecognized:
	lua_pushstring(L, "");
	lua_pushnumber(L, -1);
	lua_pushboolean(L, false);
	return 3;
}
