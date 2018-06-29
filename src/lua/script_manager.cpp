#pragma once

#include "script_manager.h"
#include "../../CurrentEngine.h"

int l_print(lua_State *L) {
	const bool tocmd = lua_toboolean(L, 1);
	const char *str  = lua_tostring(L, 2);
	Print(tocmd, str);
	return 0;
}

int l_registerLuaCallback(lua_State *L) {
	const int  id   = lua_tointeger(L, 1);
	const char *str = lua_tostring(L, 2);
	if (!isValidCbId(id)) {
		Print(true, "\tCan't register hook id: %d for: %s Invalid hook id!\n", id, str);
	}

	CurrentEngine.registerLuaCallback(str, (LuaCallbackId) id);

	return 0;
}


/**
 * Tells lua if the player is spawned or not.
 * @param L
 * @return
 */
int l_IsPlayerSpawned(lua_State *L) {
	if (!CurrentEngine.IsCoreInitialized()) {
		lua_pushboolean(L, false);
		return 1;
	}

	auto Core = CurrentEngine.GetCore();

	const auto idx = lua_tointeger(L, 1);

	if (idx >= 0 && idx <= 15) {
		lua_pushboolean(L, Core->IsPlayerSpawned(idx));
		return 1;
	}

	lua_pushboolean(L, false);
	return 1;
}

/**
 * Gives the calling lua function the address of the player by index..
 * @param L
 * @return
 */
int l_GetPlayerAddress(lua_State *L) {
	if (!CurrentEngine.IsCoreInitialized()) {
		lua_pushinteger(L, -1);
		return 1;
	}

	auto Core = CurrentEngine.GetCore();

	const auto idx = lua_tointeger(L, 1);

	if (idx >= 0 && idx <= 15) {
		if (!Core->IsPlayerSpawned(idx)) {
			lua_pushinteger(L, -1);
			return 1;
		}

		lua_pushinteger(L, reinterpret_cast<int>(Core->GetPlayer(idx)));
		return 1;
	}

	lua_pushinteger(L, -1);
	return 1;
}

int l_MakePlayerGoForward(lua_State *L) {
	CurrentEngine.MakePlayerGoForward();
	return 0;
}

int l_IsCoreInitialized(lua_State *L) {
	lua_pushboolean(L, CurrentEngine.IsCoreInitialized());
	return 1;
}

int l_IsCustomEd(lua_State *L) {
	lua_pushboolean(L, CurrentEngine.IsCustomEd());
	return 1;
}

int l_IsSapien(lua_State *L) {
	lua_pushboolean(L, CurrentEngine.IsSapien());
	return 1;
}

int l_IsHek(lua_State *L) {
	lua_pushboolean(L, CurrentEngine.IsHek());
	return 1;
}

/**
 * Tells the lua script whether or not the player is in the main menu.
 * @param L Lua state.
 * @return (In C) # args for lua.
 */
int l_InMainMenu(lua_State *L) {
	lua_pushboolean(L, CurrentEngine.GetCore()->AreWeInMainMenu());
	return 1;
}

/**
 * Checks the function map for a function name, then Calls it if it's found.
 * @param L
 * @return (In Lua) Whether or not the function was found and called.
 *
 */
int l_CallVoidEngineFunctionByFunctionMapName(lua_State *L) {
	auto str = lua_tostring(L, 1);
	auto got = CurrentEngine.getFunctionBegin(str);

	if (got != static_cast<uint>(-1)) {
		calls::DoCall<Convention::m_cdecl>(got);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushboolean(L, false);
	return 1;
}

/**
 * Returns (to Lua) the engine state this dll was compiled with targets for.
 * Sapien as a compiler target will shortly arrive.
 * @param L Lua state.
 * @return (In C) Num args for lua.
 */
int l_GetEngineContext(lua_State *L) {

	const char *MAJSTR = CurrentEngine.GetCurrentMajorVerString();

	lua_pushstring(L, MAJSTR);

	if (strcmp(MAJSTR, "") == 0) {
		lua_pushnumber(L, -1);
		return 2;
	}

	lua_pushinteger(L, CurrentEngine.GetMinorVersion());
	return 2;
}

/**
 * Something something lua and callbacks.
 * @param cb_name
 * @param cb_type
 */
void LuaScriptManager::registerLuaCallback(const std::string &cb_name, LuaCallbackId cb_type) {
	if (!this) {
		Print(false, "\tRegisterLuaCallback exiting b/c of invalid context. :(\n");
		return;
	}

	if (!loaded) {
		Print(false, "\tCan't register a callback b/c Lua's not registered as being loaded.");
		return;
	}

	Print(false, "Checking if Hook valid inside script_manager.h\n");

	if (isValidCbId((uint) cb_type)) {
		Print(false, "\tPushing down the cb: %s\n", cb_name.c_str());

		callbacks[cb_type].push_back(cb_name);

		Print(false, "\tHook %s should register now! # Hooks now registered: %d\n", cb_name.c_str(), callbacks[cb_type].size());
	} else {
		Print(true, "Rejected callback: %s with id: %d because the id is invalid.\n", cb_name, cb_type);
	}
}

/**
 * Something something Lua and void function call by functionName
 */
void LuaScriptManager::call_void_lua_func(const std::string &funcName) {
	if (!this->IsLoaded()) {
		Print(false, "Can't call lua func: %s - Lua isn't loaded?!?.\n", funcName.c_str());
		return;
	}

	// Print(false, "\t***Attempting to call lua hook func name: %s\n", funcName.c_str());
	lua_getglobal(L, funcName.c_str()); /* function to be called */

	if (!lua_isfunction(L, -1)) {
		Print(false, "\t***Can't call lua function: %s Because it's not a function?\n", funcName.c_str());
	}

	lua_pcall(L, 0, 0, 0);
}

/**
 * Initialize Lua, look for file filename.
 * @param filename
 */
void LuaScriptManager::InitializeLua(const std::string &filename) {
	Print(true, "Trying To load: \n\t%s\n", filename.c_str());

	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, filename.c_str())) {
		Print(true, "Error: script failed to load: (%s)\n\tDoes it exist?\n", filename.c_str());
		this->loaded = false;
		L = 0;
		return;
	}

	//Takes 1 Param - location to read value from
	registerGlobalLuaFunction("ReadByte", l_readByte);
	registerGlobalLuaFunction("Read32f", l_readFloat);
	registerGlobalLuaFunction("Read32i", l_readInt);
	registerGlobalLuaFunction("Read16i", l_readShort);

	//Takes 2 Params - location to write value, value to write to location.
	registerGlobalLuaFunction("WriteByte", l_writeByte);
	registerGlobalLuaFunction("Write32f", l_writeFloat);
	registerGlobalLuaFunction("Write32i", l_writeInt);
	registerGlobalLuaFunction("Write16i", l_writeShort);

	//2 Params - boolean - tells whether or not to write to console. always writes to dbg log.
	//2nd param - const char * string to print to the log.
	registerGlobalLuaFunction("DebugPrint", l_print);

	//2 Params
	registerGlobalLuaFunction("RegisterCallBack", l_registerLuaCallback);
	registerGlobalLuaFunction("GetMaxLocalPlayers", l_GetMaxLocalPlayers);

	registerGlobalLuaFunction("AreWeInMainMenu", l_InMainMenu);
	registerGlobalLuaFunction("GetEngineContext", l_GetEngineContext);

	registerGlobalLuaFunction("CallVoidEngineFunctionByFunctionMapName", l_CallVoidEngineFunctionByFunctionMapName);

	registerGlobalLuaFunction("MakePlayerGoForward", l_MakePlayerGoForward);

	registerGlobalLuaFunction("IsCoreInitialized", l_IsCoreInitialized);

	registerGlobalLuaFunction("GetPlayerAddress", l_GetPlayerAddress);
	registerGlobalLuaFunction("IsPlayerSpawned", l_IsPlayerSpawned);

	registerGlobalLuaFunction("IsCustomEd", l_IsCustomEd);
	registerGlobalLuaFunction("IsSapien", l_IsSapien);
	registerGlobalLuaFunction("IsHek", l_IsHek);

	this->fileName = filename;
	this->loaded   = true;
}

void LuaScriptManager::call_lua_event_by_type(LuaCallbackId eventType) {
	if (!this) {
		Print(false, "Can't call lua event: %d b/c we're in an invalid context.\n", eventType);
		return;
	}

	if (!this->IsLoaded()) {
		Print(false, "Can't call lua event: %d b/c we're not even loaded!\n", eventType);
		return;
	}

	static bool printedInvalid[LuaCallbackId::max_callback_id];

	auto itm = callbacks[eventType];

	if (itm.empty()) {
		if (!printedInvalid[eventType]) {
			Print(false, "Can't call lua hook for event id: %d b/c there are no functions registered!\n", eventType);
			printedInvalid[eventType] = true;
		}
		return;
	}

	for (std::string elem : itm) {
		call_void_lua_func(elem);
	}
}
