#pragma once

extern "C" {
#include "../../include/lua/lua.h"
#include "../../include/lua/lauxlib.h"
#include "../../include/lua/lualib.h"
}

auto getLuaInt(lua_State *L);

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
