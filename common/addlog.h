/*
	Project: haloforge
	File: addlog.h
	Copyright � 2009 SilentK, Abyll

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.

	No addresses needed
*/
#pragma once
#include <Windows.h>

void DBGPrnt(const char *fmt, va_list va_alist);
void DEBUG(const char* fmt, ...);

// void Print(bool toConsole, const char *fmt, ...);
#ifdef __GNUC__
template<bool toConsole = true>
extern void Print(const char *fmt, ...);

template<bool toConsole = true>
extern void PrintLn(const char *fmt, ...);
#else
#include <iostream>
#include <fstream>
template<bool toConsole = true>
static void Print(const char *fmt, ...)  {
	if (!fmt) {
		return;
	}

	va_list va_alist;
		va_start(va_alist, fmt);

	if constexpr (toConsole) {
		vprintf(fmt, va_alist);
	}

	DBGPrnt(fmt, va_alist);
		va_end(va_alist);
}

template<bool toConsole = true>
static void PrintLn(const char *fmt, ...) {
	if (!fmt) {
		return;
	}

	va_list va_alist;

		va_start(va_alist, fmt);
	if constexpr (toConsole) {
		vprintf(fmt, va_alist);
		//I'm incredibly lazy. Hopefully the compiler can optimize it out.
		vprintf("\n", va_alist);
	}

	DBGPrnt(fmt, va_alist);

		va_end(va_alist);
}
#endif

void InitAddLog(HMODULE hModule, const char * filename);
void ExitAddLog();
