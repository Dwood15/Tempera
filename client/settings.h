#pragma once

#include "command/command.h"

// Return (and optionally set) whether or not save data can be modified.
bool settings_read_only(int new_value=-1) noexcept;

// Return (and optionally set) whether or not save data can write to disk.
bool settings_do_not_save(int new_value=-1) noexcept;

void commit_command(const char *command, size_t argc, const char **argv) noexcept;
void save_all_changes() noexcept;

bool read_init_file(const char *path, const char *name) noexcept;
ChimeraCommandError verbose_init_command(size_t argc, const char **argv) noexcept;
