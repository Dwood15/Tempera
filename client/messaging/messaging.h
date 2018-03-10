#pragma once

#include <string>

#include "../../math/data_types.h"

/// Output a C string to the console while optionally using a color.
void console_out(const char *text, const ColorARGB &color = ColorARGB()) noexcept;

/// Output a C++ string to the console with a C++ string while optionally using a color.
void console_out(const std::string &text, const ColorARGB &color = ColorARGB()) noexcept;

/// Output a C string to the console using yellow text. This is used for warnings.
void console_out_warning(const char *text) noexcept;

/// Output a C++ string to the console using yellow text. This is used for warnings.
void console_out_warning(const std::string &text) noexcept;

/// Output a C string to the console using red text. This is used for errors.
void console_out_error(const char *text) noexcept;

/// Output a C++ string to the console using red text. This is used for errors.
void console_out_error(const std::string &text) noexcept;

/// Output a C string to the HUD.
void hud_message(const char *message) noexcept;

/// Output a wide string to the HUD.
void hud_message(const short *message) noexcept;

/// Output a C++ string to the HUD.
void hud_message(std::string message) noexcept;
