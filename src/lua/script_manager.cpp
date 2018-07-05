#pragma once

#include <addlog.h>
#include "script_manager.h"
#include "../CurrentEngine.h"
#include "../Dinput/dinput.h"

int l_print(lua_State *L) {
	const bool tocmd = lua_toboolean(L, 1);
	const char *str  = lua_tostring(L, 2);

	if (!tocmd) {
		PrintLn<false>(str);
	} else {
		PrintLn<true>(str);
	}

	return 0;
}

int l_registerLuaCallback(lua_State *L) {
	const int  id   = lua_tointeger(L, 1);
	const char *str = lua_tostring(L, 2);
	if (!isValidCbId(id)) {
		PrintLn("\tCan't register hook id: %d for: %s Invalid hook id!", id, str);
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
		PrintLn<false>("\tCan't spawn player because core is not initialized!\n");

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
		PrintLn<false>("\tCan't get player address because core is not initialized!\n");

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

template <typename F>
int LuaSetPlayerFunctionNoArg(lua_State *L, F func) {
	const auto idx = static_cast<ushort>(lua_tointeger(L, 1));

	func(idx);

	return 0;
}

template <typename F>
int LuaSetPlayerFunctionWithArg(lua_State *L, F func) {
	const auto idx = static_cast<ushort>(lua_tointeger(L, 1));
	const auto x   = static_cast<float>(lua_tonumber(L, 2));

	func(idx, x);

	return 0;
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
		PrintLn<false>("\tRegisterLuaCallback exiting b/c of invalid context. :(");
		return;
	}

	if (!loaded) {
		Print(false, "\tCan't register a callback b/c Lua's not registered as being loaded.");
		return;
	}

	PrintLn<false>("Checking if Hook valid inside script_manager.h");

	if (isValidCbId((uint) cb_type)) {
		PrintLn<false>("\tPushing down the cb: %s", cb_name.c_str());

		callbacks[cb_type].push_back(cb_name);

		PrintLn<false>("\tHook %s should register now! # Hooks now registered: %d", cb_name.c_str(), callbacks[cb_type].size());
	} else {
		PrintLn("Rejected callback: %s with id: %d because the id is invalid.", cb_name, cb_type);
	}
}

/**
 * Something something Lua and void function call by functionName
 */
void LuaScriptManager::call_void_lua_func(const std::string &funcName) {
	if (!this->IsLoaded()) {
		PrintLn<false>("Can't call lua func: %s - Lua isn't loaded?!?.", funcName.c_str());
		return;
	}

	// PrintLn<false>("\t***Attempting to call lua hook func name: %s", funcName.c_str());
	lua_getglobal(L, funcName.c_str()); /* function to be called */

	if (!lua_isfunction(L, -1)) {
		PrintLn<false>("\t***Can't call lua function: %s Because it's not a function?", funcName.c_str());
	}

	lua_pcall(L, 0, 0, 0);
}

void LuaScriptManager::DoFirstRun() {
	if (!this->IsLoaded()) {
		PrintLn("Unable to run script- Lua State isn't loaded!");
	}

	auto result = lua_pcall(L, 0, 0, 0);
	if (result != LUA_OK) {
		PrintLn("Error: script failed on first run! (%s)", GetFileName());

		HandleLuaError(result);
	}
}

void LuaScriptManager::HandleLuaError(int result) {
	switch (result) {
		case LUA_ERRRUN:
			PrintLn("\t* Runtime error occurred.");
			break;
		case LUA_ERRSYNTAX:
			PrintLn("\t* Syntax Error!");

			if(lua_isstring(L, 1)) {
				PrintLn("\t#->%s<-#", lua_tostring(L, 1));
			} else {
				PrintLn("\t** unreadable error!");
			}
			break;
		case LUA_ERRMEM:
			PrintLn("\t* Unable to allocate enough memory for Lua! What are you doing?!?");
			break;
		case LUA_ERRERR:
			PrintLn("\t* Err while running the message handler...");
			break;
		case LUA_ERRGCMM:
			PrintLn("\t* Err while running the GC Metamethod. How does this even happen?");
			break;
		case LUA_ERRFILE:
			PrintLn("\t* Err - cannot open/read file.");
		default:
			PrintLn("Congratulations, your script has come across a never before seen error! :V");
	}
}

/**
 * Initialize Lua, look for file filename.
 * @param filename
 */
void LuaScriptManager::InitializeLua(const std::string &filename) {
	PrintLn("Trying To load: \n\t%s", filename.c_str());

	L = luaL_newstate();
	luaL_openlibs(L);
	auto result = luaL_loadfilex(L, filename.c_str(), 0);

	if (result != LUA_OK) {
		PrintLn("Error: script failed to load: (%s)\n\tDwood is still learning how to add features to Lua.", filename.c_str());
		HandleLuaError(result);
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

	registerGlobalLuaFunction("GetMaxLocalPlayers", [](lua_State* L) {
		lua_pushinteger(L, MAX_PLAYER_COUNT_LOCAL);
		return 1;
	});

	registerGlobalLuaFunction("AreWeInMainMenu", [](lua_State *L) {
		lua_pushboolean(L, CurrentEngine.GetCore()->AreWeInMainMenu());
		return 1;
	});

	registerGlobalLuaFunction("GetEngineContext", l_GetEngineContext);

	registerGlobalLuaFunction("CallVoidEngineFunctionByFunctionMapName", l_CallVoidEngineFunctionByFunctionMapName);

	//Some helpers for controlling the player.
	registerGlobalLuaFunction("SetPlayerPrimaryTriggerFlag", [](lua_State *L) {
		return LuaSetPlayerFunctionNoArg(L, [](ushort idx) {
			CurrentEngine.SetPlayerPrimaryTriggerFlag(idx);
		});
	});

	registerGlobalLuaFunction("SetPlayerTriggerPressure", [](lua_State *L) {
		return LuaSetPlayerFunctionWithArg(L, [](ushort idx, float t) {
			CurrentEngine.SetPlayerTriggerPressure(t, idx);
		});
	});

	registerGlobalLuaFunction("SetPlayerXLook", [](lua_State *L) {
		return LuaSetPlayerFunctionWithArg(L, [](ushort idx, float t) {
			CurrentEngine.SetPlayerLookX(t, idx);
		});
	});

	registerGlobalLuaFunction("SetPlayerYLook", [](lua_State *L) {
		return LuaSetPlayerFunctionWithArg(L, [](ushort idx, float t) {
			CurrentEngine.SetPlayerLookY(t, idx);
		});
	});

	registerGlobalLuaFunction("SetPlayerXVelocity", [](lua_State *L) {
		return LuaSetPlayerFunctionWithArg(L, [](ushort idx, float t) {
			CurrentEngine.SetPlayerXVelocity(t, idx);
		});
	});

	registerGlobalLuaFunction("SetPlayerYVelocity", [](lua_State *L) {
		return LuaSetPlayerFunctionWithArg(L, [](ushort idx, float t) {
			CurrentEngine.SetPlayerYVelocity(t, idx);
		});
	});

	registerGlobalLuaFunction("MakePlayerJump", [](lua_State *L) {
		return LuaSetPlayerFunctionNoArg(L, [](ushort idx) {
			CurrentEngine.MakePlayerJump(idx);
		});
	});

	registerGlobalLuaFunction("IsCoreInitialized", [](lua_State *L) {
		lua_pushboolean(L, CurrentEngine.IsCoreInitialized());
		return 1;
	});

	registerGlobalLuaFunction("GetPlayerAddress", l_GetPlayerAddress);

	registerGlobalLuaFunction("IsPlayerSpawned", l_IsPlayerSpawned);

	Input::DInput::RegisterLuaFunctions(this);

	registerGlobalLuaFunction("IsCustomEd", [](lua_State *L) {
		lua_pushboolean(L, CurrentEngine.IsCustomEd());
		return 1;
	});

	registerGlobalLuaFunction("IsSapien", [](lua_State *L) {
		lua_pushboolean(L, CurrentEngine.IsSapien());
		return 1;
	});

	registerGlobalLuaFunction("IsHek", [](lua_State *L) {
		lua_pushboolean(L, CurrentEngine.IsHek());
		return 1;
	});

	this->fileName = filename;
	this->loaded   = true;
}

void LuaScriptManager::call_lua_event_by_type(LuaCallbackId eventType) {
	if (!this) {
		PrintLn<false>("Can't call lua event: %d b/c we're in an invalid context.", eventType);
		return;
	}

	if (!this->IsLoaded()) {
		PrintLn<false>("Can't call lua event: %d b/c we're not even loaded!", eventType);
		return;
	}

	static bool printedInvalid[LuaCallbackId::max_callback_id];

	auto itm = callbacks[eventType];

	if (itm.empty()) {
		if (!printedInvalid[eventType]) {
			//PrintLn<false>("Can't call lua hook for event id: %d b/c there are no functions registered!", eventType);
			printedInvalid[eventType] = true;
		}
		return;
	}

	for (std::string elem : itm) {
		call_void_lua_func(elem);
	}
}
