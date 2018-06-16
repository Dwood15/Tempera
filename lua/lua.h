#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include "../haloforge/headers/addlog.h"

extern "C" {
#include "../include/lua.h"
#include "../include/lauxlib.h"
#include "../include/lualib.h"
}

enum LuaCallbackId {
	invalid = -1,
	on_load = 0,
	on_map_load,
	before_scenario_tags_load,
	after_scenario_tags_load,
	before_game_tick,
	after_game_tick,
	max_callback_id
};


static int l_print(lua_State* L) {
	const bool tocmd = lua_toboolean(L, 1);
	const char* str = lua_tostring(L, 2);
	Print(tocmd, str);
	return 0;
}

//This code was (for the most part) copy-pasted from
//The tutorial series here: https://eliasdaler.wordpress.com/2013/10/20/lua_and_cpp_pt2/
class LuaScriptManager {
private:
	lua_State                                  *L;
	int                                 level;
	std::unordered_map<LuaCallbackId, std::vector<std::string>> callbacks;
public:
	LuaScriptManager(const std::string &filename) {
		Print(true, "Trying To load: %s\n", filename.c_str());

		L = luaL_newstate();
		luaL_openlibs(L);
		if (luaL_loadfile(L, filename.c_str())) {
			Print(true, "Error: script not loaded (%s)\n", filename.c_str());
			L = 0;
			return;
		}

		lua_pushcfunction(L, l_print);
		lua_setglobal(L, "DebugPrint");

		if( lua_pcall(L, 0, 0, 0)) {
			Print(true, "Error: script failed to run! (%s)\n", filename.c_str());
		}

	}

	~LuaScriptManager() {
		if (L) {
			lua_close(L);
		}
	}

	void printError(const std::string &variableName, const std::string &reason) {
		Print(true, "Error: can't get [%s]. Reason: %s\n", variableName, reason);
	}

	// Generic get
	template <typename T>
	T lua_get(const std::string &variableName) {
		return 0;
	}

	// Generic default get
	template <typename T>
	T lua_getdefault() {
		return 0;
	}

	// Generic default get
	template <typename T>
	T lua_getdefault(const std::string &variableName) {
		return 0;
	}

	template <>
	inline std::string lua_getdefault() {
		return "null";
	}

	template <>
	inline bool lua_get(const std::string &variableName) {
		return (bool) lua_toboolean(L, -1);
	}

	template <>
	inline float lua_get(const std::string &variableName) {
		if (!lua_isnumber(L, -1)) {
			printError(variableName, "Not a number");
		}
		return (float) lua_tonumber(L, -1);
	}

	template <>
	inline int lua_get(const std::string &variableName) {
		if (!lua_isnumber(L, -1)) {
			printError(variableName, "Not a number");
		}
		return (int) lua_tonumber(L, -1);
	}

	template <>
	inline std::string lua_get(const std::string &variableName) {
		std::string s = "null";
		if (lua_isstring(L, -1)) {
			s = std::string(lua_tostring(L, -1));
		} else {
			printError(variableName, "Not a string");
		}
		return s;
	}

	template <typename T>
	T get(const std::string &variableName) {
		if (!L) {
			printError(variableName, "Script is not loaded");
			return lua_getdefault<T>();
		}

		// variable succesfully on top of stack
		T result;
		if (lua_gettostack(variableName)) {
			result = lua_get<T>(variableName);
		} else {
			result = lua_getdefault<T>();
		}

		lua_pop(L, level + 1); // pop all existing elements from stack
		return result;
	}

	bool lua_gettostack(const std::string &variableName) {
		level                 = 0;
		std::string       var = "";
		for (unsigned int i   = 0; i < variableName.size(); i++) {
			if (variableName.at(i) == '.') {
				if (level == 0) {
					lua_getglobal(L, var.c_str());
				} else {
					lua_getfield(L, -1, var.c_str());
				}

				if (lua_isnil(L, -1)) {
					printError(variableName, var + " is not defined");
					return false;
				} else {
					var = "";
					level++;
				}
			} else {
				var += variableName.at(i);
			}
		}
		if (level == 0) {
			lua_getglobal(L, var.c_str());
		} else {
			lua_getfield(L, -1, var.c_str());
		}
		if (lua_isnil(L, -1)) {
			printError(variableName, var + " is not defined");
			return false;
		}

		return true;
	}

	void clean() {
		int n = lua_gettop(L);
		lua_pop(L, n);
	}

	std::vector<int> getIntVector(const std::string &name) {
		std::vector<int> v;
		lua_getglobal(L, name.c_str());
		if (lua_isnil(L, -1)) {
			return std::vector<int>();
		}
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			v.push_back((int) lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
		clean();
		return v;
	}

	bool isValidCbId(LuaCallbackId id) {
		return (id < LuaCallbackId::max_callback_id && id >= id);
	}

	void call_void_lua_func(const std::string & funcName) {
		lua_getglobal(L, funcName.c_str());
		lua_pcall(L, 0, 0, 0);
	}

	/**
	 * Tells our class to call this function when the associated CB id is triggered.
	 * @param cb_name Name of Lua func to call.
	 * @param cb_type On which event this func is called.
	 */
	void registerLuaCallback(const std::string &cb_name, LuaCallbackId cb_type) {
		if(isValidCbId(cb_type)) {
			this->callbacks[cb_type].push_back(cb_name);
		}
	}

	void call_lua_event_by_type(LuaCallbackId eventType) {
		std::vector<std::string> cb_list = this->callbacks[eventType];

		for(std::string elem : cb_list) {
			call_void_lua_func(elem);
		}
	}
};

static LuaScriptManager * LuaState;
