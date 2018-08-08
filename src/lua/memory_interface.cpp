#include <limits>
#include <engine_interface.h>
#include "memory_interface.h"
#include <memory_map.h>

template <typename T>
 bool inBounds(T val) {
	return val >= (int)::std::numeric_limits<T>::min && (int)::std::numeric_limits<T>::max >= val;
}

//Little helper functions to assist people with their modding efforts...

//READ from memory
int l_readByte(lua_State *L) {
	//8-bit byte
	auto loc = (byte *) getLuaInt(L);
	lua_pushinteger(L,  *loc);
	return 1;
}

 int l_readFloat(lua_State *L) {
	//32-bit floating point
	auto loc = (float *) getLuaInt(L);
	lua_pushnumber(L, (*loc));
	return 1;
}

 int l_readInt(lua_State *L) {
	//32-bit signed integer
	auto loc = (int *) getLuaInt(L);
	lua_pushinteger(L, *loc);
	return 1;
}

 int l_readShort(lua_State *L) {
	//16-bit signed integer
	auto loc = (short *) getLuaInt(L);
	lua_pushinteger(L, *loc);
	return 1;
}

//WRITE to memory
 int l_writeByte(lua_State *L) {
	uintptr_t  loc    = getLuaInt(L);
	const uint l_byte = lua_tointeger(L, 2);

	if (inBounds<byte>(l_byte)) {
		//8-bit byte
		calls::patchValue<byte>(loc, static_cast<byte>(l_byte));
	}

	return 0;
}

 int l_writeFloat(lua_State *L) {
	uintptr_t   loc   = getLuaInt(L);
	const float l_flt = lua_tonumber(L, 2);

	if (loc != (uint) -1) {
		//32-bit floating point
		calls::patchValue<float>(loc, l_flt);
	}

	return 0;
}

 int l_writeInt(lua_State *L) {
	uintptr_t loc   = getLuaInt(L);
	const int l_int = lua_tointeger(L, 2);

	if (loc != (uint) -1) {
		//32-bit integer
		calls::patchValue<int>(loc, l_int);
	}

	return 0;
}

 int l_writeShort(lua_State *L) {
	uintptr_t loc   = getLuaInt(L);
	const int l_sht = lua_tointeger(L, 2);

	if (loc != (uint)-1 && inBounds<short>(l_sht)) {
		//16-bit signed integer
		calls::patchValue<short>(loc, l_sht);
	}

	return 0;
}

