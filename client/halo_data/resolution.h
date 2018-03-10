#pragma once

#include "../command/command.h"

struct Resolution {
    uint16_t height;
    uint16_t width;
};

/// Obtain a reference to the current resolution of Halo.
Resolution &get_resolution() noexcept;

/// Function for command chimera_block_vsync
ChimeraCommandError block_vsync_command(size_t argc, const char **argv) noexcept;

/// Function for command chimera_set_resolution
ChimeraCommandError set_resolution_command(size_t argc, const char **argv) noexcept;
