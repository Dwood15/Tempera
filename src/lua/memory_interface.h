#pragma once

extern "C" {
#include "../../include/lua/lua.h"
#include "../../include/lua/lauxlib.h"
#include "../../include/lua/lualib.h"
}

static auto getLuaInt(lua_State *L, int idx = 1) {
	if (!lua_isinteger(L, idx)) {
		lua_pushliteral(L, "incorrect argument! needed integer!");
		lua_error(L);
	}

	return (uintptr_t) lua_tointeger(L, idx);
}

template <typename T>
bool inBounds(T val);

//Little helper functions to assist people with their modding efforts...

//READ from memory
int l_readByte(lua_State *L);

int l_readFloat(lua_State *L);

int l_readInt(lua_State *L);

int l_readShort(lua_State *L);

//WRITE to memory
int l_writeByte(lua_State *L);

int l_writeFloat(lua_State *L);

int l_writeInt(lua_State *L);

int l_writeShort(lua_State *L);
