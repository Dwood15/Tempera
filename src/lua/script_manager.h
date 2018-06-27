#pragma once

#include <versions.h>
#include <engine_interface.h>
#include <lua.hpp>
#include "players_interface.h"
#include "memory_interface.h"
#include "../math/enums_generic.h"

static bool isValidCbId(uint id) {
	return (id < LuaCallbackId::max_callback_id && id >= before_game_tick);
}

int l_print(lua_State *L);

int l_registerLuaCallback(lua_State *L);

int l_MakePlayerGoForward(lua_State *L);

int l_IsCoreInitialized(lua_State *L);
int l_IsCustomEd(lua_State *L);
int l_IsSapien(lua_State *L);
int l_IsHek(lua_State *L);

/**
 * Tells the lua script whether or not the player is in the main menu.
 * @param L Lua state.
 * @return (In C) # args for lua.
 */
int l_InMainMenu(lua_State *L);

/**
 * Returns (to Lua) the engine state this dll was compiled with targets for.
 * Sapien as a compiler target will shortly arrive.
 * @param L Lua state.
 * @return (In C) Num args for lua.
 */
int l_GetEngineContext(lua_State *L);

static std::vector<std::string> callbacks[LuaCallbackId::max_callback_id];

//This code was (for the most part) copy-pasted from
//The tutorial series here: https://eliasdaler.wordpress.com/2013/10/20/lua_and_cpp_pt2/
class LuaScriptManager {
	lua_State   *L;
	std::string fileName;
	bool        loaded = false;
public:

	const char * GetFileName() {
		return this->fileName.c_str();
	}

	bool IsLoaded() {
		return this->loaded;
	}

	void DoFirstRun() {
		if(!this->IsLoaded()) {
			Print(true, "Unable to run script- Lua State isn't loaded!\n");
		}

		if (lua_pcall(L, 0, 0, 0)) {
			Print(true, "Error: script failed to run! (%s)\n", GetFileName());
		}
	}

	/**
 * Tells our class to call this function when the associated CB id is triggered.
 * @param cb_name Name of Lua func to call.
 * @param cb_type On which event this func is called.
 */
	void registerLuaCallback(const std::string &cb_name, LuaCallbackId cb_type);

	void registerGlobalLuaFunction(const std::string &funcName, lua_CFunction funcAddr) {
		lua_pushcclosure(L, funcAddr, 0);
		lua_setglobal(L, funcName.c_str());
	}

	LuaScriptManager() {}

	~LuaScriptManager() {
		if (L) {
			lua_close(L);
		}
	}

	void InitializeLua(const std::string &filename = "tempera.init.lua");

	void call_void_lua_func(const std::string &funcName);

	void call_lua_event_by_type(LuaCallbackId eventType);
};

static LuaScriptManager mgr;
