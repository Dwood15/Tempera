#pragma once

#include "../command/command.h"

void setup_fast_startup();

/// Function for command chimera_cache
ChimeraCommandError cache_command(size_t argc, const char **argv) noexcept;

/// Function for command chimera_cache_clear
ChimeraCommandError cache_clear_command(size_t argc, const char **argv) noexcept;

/// Function for command chimera_modded_stock_maps
ChimeraCommandError modded_stock_maps_command(size_t argc, const char **argv) noexcept;
