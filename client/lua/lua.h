#pragma once

#include "../command/command.h"

void setup_lua();
void destroy_lua();

/// Function for command chimera_reload_lua
ChimeraCommandError reload_lua_command(size_t argc, const char **argv) noexcept;
