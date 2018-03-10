#pragma once

#include "../command/command.h"

/// Function for command chimera_interpolate
ChimeraCommandError interpolate_command(size_t argc, const char **argv) noexcept;

/// Function for command chimera_interpolate_predict
ChimeraCommandError interpolate_predict_command(size_t argc, const char **argv) noexcept;
