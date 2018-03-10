#pragma once

#include "../command/command.h"

/// Return true if a gamepad is present.
bool gamepad_plugged_in() noexcept;

/// Fix Halo's magnetism problems if a gamepad is present.
void fix_magnetism() noexcept;

/// Function for command chimera_aim_assist
ChimeraCommandError aim_assist_command(size_t argc, const char **argv) noexcept;
