#pragma once

#include "../command/command.h"

void set_block_letterbox(bool do_it) noexcept;

/// Function for command chimera_block_letterbox
ChimeraCommandError block_letterbox_command(size_t argc, const char **argv) noexcept;
