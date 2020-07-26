#pragma once

extern "C" {
#include "../../include/lua/lua.h"
#include "../../include/lua/lauxlib.h"
#include "../../include/lua/lualib.h"
}

#include <limits>
#include <engine_interface.h>
#include <memory_map.h>

static auto getLuaInt(lua_State *L, int idx = 1) {
	if (!lua_isinteger(L, idx)) {
		lua_pushstring(L, "incorrect argument! needed integer!");
		lua_error(L);
	}

	return (uintptr_t) lua_tointegerx(L, (idx), NULL);
}

template <typename T>
bool inBounds(T val) {
	return val >= (int) ::std::numeric_limits<T>::min && (int) ::std::numeric_limits<T>::max >= val;
}

template <typename T>
int LuaIntReader(lua_State *L) {
	auto loc = (T *) getLuaInt(L);
	lua_pushinteger(L, *loc);
	return 1;
}

template <typename T>
int LuaIntWriter(lua_State *L) {
	uintptr_t loc     = getLuaInt(L);
	const T   toWrite = static_cast<T>(lua_tointeger(L, 2));

	if (inBounds(toWrite)) {
		calls::patchValue(loc, toWrite);
	}

	return 0;
}

//Little helper functions to assist people with lua-based modding.
//READ from memory
static auto l_readByte = [](lua_State *L) {
	return LuaIntReader<byte>(L);
};

static auto l_readInt = [](lua_State *L) {
	return LuaIntReader<int>(L);
};

static auto l_readShort = [](lua_State *L) {
	return LuaIntReader<short>(L);
};

static auto l_readFloat = [](lua_State *L) {
	lua_pushnumber(L, *(float *) getLuaInt(L));
	return 1;
};

//WRITE to memory
static auto l_writeByte = [](lua_State *L) {
	return LuaIntWriter<byte>(L);
};

static auto l_writeFloat = [](lua_State *L) {
	uintptr_t loc = getLuaInt(L);
	if (loc > 0 && loc != (uint) -1) {
		//32-bit floating point
		calls::patchValue<float>(loc, lua_tonumberx(L, 2, 0));
	}

	return 0;
};

static auto l_writeInt = [](lua_State *L) {
	return LuaIntWriter<int>(L);
};

static auto l_writeShort = [](lua_State *L) {
	return LuaIntWriter<byte>(L);
};
