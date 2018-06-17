#pragma once

#include <precompile.h>
#include <limits>
#include "../function_rewrite.h"

extern "C" {
#include "../../include/lua_headers/lua.h"
#include "../../include/lua_headers/lauxlib.h"
#include "../../include/lua_headers/lualib.h"
}


static auto getLuaInt(lua_State *L) {
	if (!lua_isinteger(L, 1)) {
		lua_pushliteral(L, "incorrect argument! needed integer!");
		lua_error(L);
	}

	return (uintptr_t) lua_tointeger(L, 1);
}

template <typename T>
static bool inBounds(int val) {
	return val >= (int) std::numeric_limits<T>::min && (int) std::numeric_limits<T>::max >= val;
}

//Little helper functions to assist people with their modding efforts...

//READ from memory

static int l_readByte(lua_State *L) {
	auto loc = (byte *) getLuaInt(L);
	lua_pushinteger(L, lua_tonumber(L, *loc));
	return 1;
}

static int l_readFloat(lua_State *L) {
	auto loc = (float *) getLuaInt(L);
	lua_pushnumber(L, lua_tointeger(L, *loc));
	return 1;
}

static int l_readInt(lua_State *L) {
	auto loc = (int *) getLuaInt(L);
	lua_pushinteger(L, lua_tointeger(L, *loc));
	return 1;
}

static int l_readShort(lua_State *L) {
	auto loc = (short *) getLuaInt(L);
	lua_pushinteger(L, lua_tointeger(L, *loc));
	return 1;
}

//WRITE to memory
static int l_writeByte(lua_State *L) {
	uintptr_t  loc    = getLuaInt(L);
	const uint l_byte = lua_tointeger(L, 2);

	if (inBounds<byte>(l_byte)) {
		spcore::memory::patchValue<byte>(loc, static_cast<byte>(l_byte));
	}

	return 0;
}

static int l_writeFloat(lua_State *L) {
	uintptr_t   loc   = getLuaInt(L);
	const float l_flt = lua_tonumber(L, 2);

	if (loc != (uint) -1) {
		spcore::memory::patchValue<float>(loc, l_flt);
	}

	return 0;
}

static int l_writeInt(lua_State *L) {
	uintptr_t loc   = getLuaInt(L);
	const int l_int = lua_tointeger(L, 2);

	if (loc != (uint) -1) {
		spcore::memory::patchValue<int>(loc, l_int);
	}

	return 0;
}

static int l_writeShort(lua_State *L) {
	uintptr_t loc   = getLuaInt(L);
	const int l_sht = lua_tointeger(L, 2);

	if (loc != (uint)-1 && inBounds<short>(l_sht)) {
		spcore::memory::patchValue<short>(loc, l_sht);
	}

	return 0;
}

