#include <addlog.h>
//#include <enums/weapon_enums.h>
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

	::feature_management::engines::registerLuaCallback(str, (LuaCallbackId) id);

	return 0;
}

/**
 * Tells lua if the player is spawned or not.
 * @param L
 * @return
 */
int l_IsPlayerSpawned(lua_State *L) {
	if (!::feature_management::engines::IsCoreInitialized()) {
		PrintLn<false>("\tCan't spawn player because core is not initialized!\n");

		lua_pushboolean(L, false);
		return 1;
	}

	const auto idx = lua_tointeger(L, 1);

	if (idx >= 0 && idx <= 15) {
		lua_pushboolean(L, CurrentEngine->IsPlayerSpawned(idx));
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
	if (!::feature_management::engines::IsCoreInitialized()) {
		PrintLn<false>("\tCan't get player address because core is not initialized!\n");

		lua_pushinteger(L, -1);
		return 1;
	}

	const auto idx = lua_tointeger(L, 1);

	if (idx >= 0 && idx <= 15) {
		if (!CurrentEngine->IsPlayerSpawned(idx)) {
			lua_pushinteger(L, -1);
			return 1;
		}

		lua_pushinteger(L, reinterpret_cast<int>(CurrentEngine->GetPlayer(idx)));
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
	auto got = CurrentEngine->getFunctionBegin(str);

	if (got) {
		calls::DoCall<Convention::m_cdecl>(*got);
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

	const char *MAJSTR = CurrentEngine->GetCurrentMajorVerString();

	lua_pushstring(L, MAJSTR);

	if (strcmp(MAJSTR, "") == 0) {
		lua_pushnumber(L, -1);
		return 2;
	}

	lua_pushinteger(L, CurrentEngine->GetMinorVersion());
	return 2;
}

/**
 * Something something lua and callbacks.
 * @param cb_name
 * @param cb_type
 */
void LuaScriptManager::registerLuaCallback(const char *cb_name, LuaCallbackId cb_type) {
#ifndef __GNUC__
	//
	if (this == nullptr) {
		PrintLn<false>("\tRegisterLuaCallback exiting b/c of invalid context. :(");
		return;
	}
#endif

	if (!loaded) {
		PrintLn<false>("\tCan't register a callback b/c Lua's not registered as being loaded.");
		return;
	}

	PrintLn<false>("Checking if Hook valid inside script_manager.h");

	if (isValidCbId((uint) cb_type)) {
		PrintLn<false>("\tPushing down the cb: %s", cb_name);

		callbacks[cb_type].emplace_back(cb_name);

		PrintLn<false>("\tHook %s should register now! # Hooks now registered: %d", cb_name, callbacks[cb_type].size());
	} else {
		PrintLn("Rejected callback: %s with id: %d because the id is invalid.", cb_name, cb_type);
	}
}

template<int numArg, int numRet>
void LuaScriptManager::PCall(const char * funcName) {
	if (lua_pcall(L, numArg, numRet, 0)) {
		luaL_error(L, "Error in %s: %s", funcName, lua_tostring(L, -1));
	}
}

//Returns false if function not found, or if error.
bool LuaScriptManager::HandleFunctionNameEvent(const char * funcName) {
	//PrintLn<false>("HandleFunctionNameEvent: %s", funcName);

	if (!this->IsLoaded()) {
		PrintLn<false>("Can't call %s event- doesn't exist!", funcName);
		return false;
	}

	lua_getglobal(L, funcName);

	return lua_isfunction(L, -1);
}

static void setTableField(lua_State * L, const char *keyName, lua_Integer n) {
	// https://stackoverflow.com/questions/20147027/creating-a-simple-table-with-lua-tables-c-api/20148091
	lua_pushinteger(L, n);
	lua_setfield(L, -2, keyName);
}

static void setTableNumber(lua_State * L, const char *keyName, float n) {
	// https://stackoverflow.com/questions/20147027/creating-a-simple-table-with-lua-tables-c-api/20148091
	lua_pushnumber(L, n);
	lua_setfield(L, -2, keyName);
}

static void ReadPlayerControl(lua_State * L, s_unit_control_data * control) {
	PrintLn("Attempting to read our player control back out");
	luaL_checktype(L, 1, LUA_TTABLE);

	lua_getfield(L, 1, "control_flags");
	uint ctrlFlags = luaL_checkinteger(L, -1);
	if(ctrlFlags <= (uint)0xFFFF) {
		control->control_flags.control_flags_a = static_cast<ushort>(ctrlFlags);
	}

	//PrintLn("Control Flags successfully read");
	lua_pop(L, 1);

	lua_getfield(L, 1, "throttle.x");
	lua_getfield(L, 1, "throttle.y");
	lua_getfield(L, 1, "throttle.z");

	control->throttle.z = luaL_checknumber(L, -1);
	control->throttle.y = luaL_checknumber(L, -2);
	control->throttle.x = luaL_checknumber(L, -3);

//	PrintLn("Throttle Successfully read");
	lua_pop(L, 3);

	lua_getfield(L, 1, "weapon_index");
	lua_getfield(L, 1, "grenade_index");

	//This will probably make the game crash :X
	int weapIdx = luaL_checkinteger(L, -1);
//	if (weapIdx > MAX_WEAPONS_PER_UNIT-1) {
//		weapIdx = 0;
//	} else if (weapIdx < 0) {
//		weapIdx = MAX_WEAPONS_PER_UNIT-1;
//	}

	control->weapon_index = static_cast<ushort>(weapIdx);

	int gnadeIdx = luaL_checkinteger(L, -2);
	control->grenade_index = static_cast<ushort>(gnadeIdx);

//	PrintLn("Player Weap/Grenade Indices successfully read");
	lua_pop(L, 2);

	lua_getfield(L, 1, "primary_trigger");
	control->primary_trigger = luaL_checknumber(L, -1);
//	PrintLn("Primary Trigger successfully read");
	lua_pop(L, 1);

	//Last, pop the table off the stack
	lua_pop(L, 1);
}

///The basic sanity checks ensure that we can
///1) Call a Lua function from C++
///2) Read out a returned table to C++.
///There is a plethora of well-documented examples of putting a table onto Lua
///but not so many about actually reading more complex sets of data FROM lua into C++
void LuaScriptManager::lua_run_sanityChecks() {
	static volatile bool sanityChecked = false;
	if (sanityChecked)
		return;

	this->HandleFunctionNameEvent("TestReadingBackTheTable");
	//PrintLn("Attempting a Sanity Check");
	this->PCall<0, 1>("TestReadingBackTheTable");
	if (!lua_istable(L, 1)) {
		PrintLn("Literally Nothing we can do, table rip");
		return;
	}

	lua_getfield(L, 1, "foo");
	lua_getfield(L, 1, "bazz");

	auto bazzNum = lua_tointeger(L, -1);
	auto fooStr = luaL_checkstring(L, -2);

	//PrintLn("Lua Table reading Sanity Check, Bazz: [%d] Foo: [%s]", bazzNum, fooStr);
	lua_pop(L, 2);

	//PrintLn("Popped. Checking Baybe and Barr");

	lua_getfield(L, 1, "baybe");
	lua_getfield(L, 1, "barr");

	auto barrNum = luaL_checknumber(L, -1);
	auto baybeNum = luaL_checknumber(L, -2);

	//PrintLn("barrNum: [%f] && baybeNum: [%f]", barrNum, baybeNum);
	lua_pop(L, 2);

	//Pop the lua table off the stack
	lua_pop(L, 1);
	PrintLn("Finished up with the sanity check demo func");
	sanityChecked = true;
}

void PassPlayerControl(lua_State *L, s_unit_control_data *control) {
	//Send to the function our table
	lua_createtable(L, 0, 7);

	setTableField(L, "control_flags", control->control_flags.control_flags_a);

	float x, y, z;
	x = control->throttle.x;
	y = control->throttle.y;
	z = control->throttle.z;

	setTableField(L, "throttle.x", static_cast<int>(x));
	setTableField(L, "throttle.y", static_cast<int>(y));
	setTableField(L, "throttle.z", static_cast<int>(z));

	setTableField(L,"weapon_index", control->weapon_index);
	setTableField(L, "grenade_index", control->grenade_index);

	setTableNumber(L, "primary_trigger", control->primary_trigger);
}

void LuaScriptManager::lua_on_player_update(s_unit_control_data * control, ushort plyrIdx) {
	if (control == nullptr) {
		PrintLn("Player Action Control is null, no work to do");
		return;
	}

	//Sanity-Check our ability to read a table
	this->lua_run_sanityChecks();

	static volatile bool DebugOnce = false;

	if(!this->HandleFunctionNameEvent("PlayerUpdate")) {
		if (!DebugOnce) {
			PrintLn("Cannot call PlayerUpdate b/c it doesn't exist.");
			DebugOnce = true;
		}
		return;
	}

	if(!DebugOnce) {
		PrintLn("lua_on_player_update");
	}

	PassPlayerControl(L, control);
	lua_pushinteger(L, plyrIdx);

	this->PCall<2, 1>("PlayerUpdate");

	PrintLn("out of Lua, Reading player Control back out");

	ReadPlayerControl(L, control);
}

void LuaScriptManager::lua_on_tick(uint32 remaining, uint32 since_map_begin) {
	constexpr const char * funcName = "OnTick";

	if(!this->HandleFunctionNameEvent(funcName)) {
		return;
	}

	lua_pushinteger(L, int(remaining));
	lua_pushinteger(L, int(since_map_begin));

	this->PCall<2, 0>(funcName);
}

/**
 * Something something Lua and void function call by functionName
 */
void LuaScriptManager::call_void_lua_func(const::std::string &funcName) {
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
void LuaScriptManager::InitializeLua(const char *filename) {
	PrintLn("Trying To load: \n\t%s", filename);

	L = luaL_newstate();
	luaL_openlibs(L);
	auto result = luaL_loadfilex(L, filename, nullptr);

	if (result != LUA_OK) {
		PrintLn("Error: script failed to load: (%s)\n\tDwood is still learning how to add features to Lua.", filename);
		HandleLuaError(result);
		this->loaded = false;
		L = nullptr;
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
		lua_pushboolean(L, feature_management::engines::GlobalEngine::AreWeInMainMenu());
		return 1;
	});

	registerGlobalLuaFunction("GetEngineContext", l_GetEngineContext);

	registerGlobalLuaFunction("CallVoidEngineFunctionByFunctionMapName", l_CallVoidEngineFunctionByFunctionMapName);

	registerGlobalLuaFunction("IsCoreInitialized", [](lua_State *L) {
		lua_pushboolean(L, feature_management::engines::IsCoreInitialized());
		return 1;
	});

	registerGlobalLuaFunction("GetPlayerAddress", l_GetPlayerAddress);

	registerGlobalLuaFunction("IsPlayerSpawned", l_IsPlayerSpawned);

	Input::DInput::RegisterLuaFunctions(this);

	registerGlobalLuaFunction("IsCustomEd", [](lua_State *L) {
		lua_pushboolean(L, feature_management::engines::GlobalEngine::IsCustomEd());
		return 1;
	});

	registerGlobalLuaFunction("IsSapien", [](lua_State *L) {
		lua_pushboolean(L, feature_management::engines::GlobalEngine::IsSapien());
		return 1;
	});

	registerGlobalLuaFunction("IsHek", [](lua_State *L) {
		lua_pushboolean(L, feature_management::engines::GlobalEngine::IsHek());
		return 1;
	});

	this->fileName = filename;
	this->loaded   = true;
}

void LuaScriptManager::call_lua_event_by_type(LuaCallbackId eventType) {
#ifndef __GNUC__
	//Once again, don't trust IDE warnings - "this" can totally be nullptr in certain cases
	if (this == nullptr) {
		PrintLn<false>("Can't call lua event: %d b/c we're in an invalid context.", eventType);
		return;
	}
#endif

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

	for (::std::string elem : itm) {
		call_void_lua_func(elem);
	}
}
