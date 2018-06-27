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
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>

void PrintHelp();
const void __cdecl DEBUG(const char* fmt, ...);
const void __cdecl Print(bool toConsole, const char *fmt, ...);
char *GetDirectoryFile(char* filename);
void InitAddLog(HMODULE hModule);
void ExitAddLog();
