#pragma once

#include "../command/command.h"

/// Function for command chimera_budget
ChimeraCommandError budget_command(size_t argc, const char **argv) noexcept;

/// Function for command chimera_player_info
ChimeraCommandError player_info_command(size_t argc, const char **argv) noexcept;
