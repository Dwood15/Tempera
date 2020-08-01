#pragma once

#include <versions.h>
#include <engine_interface.h>
#include <lua.hpp>
#include <string>
#include <vector>
#include "players_interface.h"
#include "memory_interface.h"
#include "enums/generic_enums.h"

static bool isValidCbId(uint id) {
	return (id < LuaCallbackId::max_callback_id && id >= before_game_tick);
}

int l_print(lua_State *L);

int l_registerLuaCallback(lua_State *L);

/**
 * Returns (to Lua) the engine state this dll was compiled with targets for.
 * Sapien as a compiler target will shortly arrive.
 * @param L Lua state.
 * @return (In C) Num args for lua.
 */
int l_GetEngineContext(lua_State *L);

static ::std::vector<::std::string> callbacks[LuaCallbackId::max_callback_id];
struct s_player_action;

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpadded"
#endif

//This code was (for the most part) copy-pasted from
//The tutorial series here: https://eliasdaler.wordpress.com/2013/10/20/lua_and_cpp_pt2/
class LuaScriptManager {
	lua_State   *L = nullptr;
	::std::string fileName;
	bool        loaded = false;

	void PassInteger(int val);
public:
	LuaScriptManager() = default;

	~LuaScriptManager() {
		if (L) {
			lua_close(L);
		}
	}

	template<int numArg = 0, int numRet = 0>
	void PCall(const char * funcName);

	bool HandleFunctionNameEvent(const char * funcName);

	const char *GetFileName() {
		return this->fileName.c_str();
	}

	bool IsLoaded() {
		return this->loaded;
	}

	void HandleLuaError(int result);

	void DoFirstRun();

	/**
 	* Tells our class to call this function when the associated CB id is triggered.
 	* @param cb_name Name of Lua func to call.
 	* @param cb_type On which event this func is called.
 	*/
	void registerLuaCallback(const char * cb_name, LuaCallbackId cb_type);

	void registerGlobalLuaFunction(const char * funcName, lua_CFunction funcAddr) {
		lua_pushcclosure(L, funcAddr, 0);
		lua_setglobal(L, funcName);
	}

	void InitializeLua(const char *filename = "tempera.init.lua");

	void lua_on_tick(uint32 remaining, uint32 since_map_begin);
	void lua_on_player_update(s_player_action * control, ushort plyrIdx);

	void call_void_lua_func(const ::std::string &funcName);

	void call_lua_event_by_type(LuaCallbackId eventType);
};

#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wpadded"
#endif

static LuaScriptManager mgr;
