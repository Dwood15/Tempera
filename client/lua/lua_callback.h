#pragma once

#include "lua_script.h"

int lua_set_callback(lua_State *state) noexcept;

void setup_callbacks() noexcept;
