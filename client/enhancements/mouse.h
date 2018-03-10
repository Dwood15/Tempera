#pragma once

#include "../command/command.h"

/// Function for command chimera_mouse_sensitivity
ChimeraCommandError mouse_sensitivity_command(size_t argc, const char **argv) noexcept;

/// Function for command chimera_block_mouse_acceleration
ChimeraCommandError block_mouse_acceleration_command(size_t argc, const char **argv) noexcept;
