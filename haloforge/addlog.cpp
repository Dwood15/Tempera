/*
	Project: haloforge
	File: addlog.cpp
	Copyright � 2009 SilentK, Abyll
	Copyright � 2018 Dwood15

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
*/
#include <ctime>
#include "headers/addlog.h"

std::ofstream ofile;
char          dlldir[320];

char *GetDirectoryFile(char *filename) {
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

static volatile bool debug_write_locked = false;

const void __cdecl DBGPrnt(const char *fmt, va_list va_alist) {
	if (!ofile || !fmt) {
		return;
	}

	if (debug_write_locked) {
		Sleep(90);
		//Too impatient for this.
		if (debug_write_locked) {
			return;
		}
	}

	debug_write_locked = true;

	char logbuf[256] = {0};
	_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	ofile << logbuf << std::endl;

	debug_write_locked = false;
}

const void __cdecl DEBUG(const char *fmt, ...) {
	va_list va_alist;

		va_start (va_alist, fmt);
	DBGPrnt(fmt, va_alist);
		va_end (va_alist);

}

const void __cdecl Print(bool toConsole, const char *fmt, ...) {
	if (!fmt) {
		return;
	}

	va_list va_alist;

		va_start(va_alist, fmt);
	if (toConsole) {
		vprintf(fmt, va_alist);
	}

	DBGPrnt(fmt, va_alist);

		va_end(va_alist);
}

void PrintHelp() {
	const char *f1     = "F1    -> (ui only!)Set # players to spawn in next map to 2.\n";
	const char *f2     = "F2    -> Dumps Player Globals data to log. Prints to window when not in ui.map\n";
	const char *shift  = "SHIFT -> Select\\Deselect highlighted object.\n";
	const char *f5     = "F5    -> Log object data that player is holding to file.\n";
	const char *akUP   = "AK_UP -> Moves held object away incrementally.\n";
	const char *akDOWN = "AK_DN -> Moves held object toward, incrementally.\n\t ->Arrow keys are for use with RMBTN\n";
	const char *lmBTN  = "LMBTN -> Hold Object - Hard to describe.\n";
	const char *rmBTN  = "RMBTN -> Hold Object - Same as LMBTN.\n\t ->Use these in map if you have selected an object.\n";
	const char *f11    = "F11   -> This Help Text.\n";

	printf("\n\n/******************************************************\\\n");
	printf("  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n", shift, f1, f2, f5, akUP, akDOWN, lmBTN, rmBTN, f11);
	printf("\\*****************************************************/\n");
}

void InitAddLog(HMODULE hModule) {
	GetModuleFileNameA(hModule, dlldir, 512);

	for (int i = (int) strlen(dlldir); i > 0; i--) {
		if (dlldir[i] == '\\') {
			dlldir[i + 1] = 0;
			break;
		}
	}

	ofile.open(GetDirectoryFile((char *) LOGNAME), std::ios::app);
	std::time_t time = std::time(nullptr);
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	DEBUG("Tempera Injected, live_projekt initialized");
	DEBUG(std::asctime(std::localtime(&time)));
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
}

void ExitAddLog() {
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	DEBUG("Tempera Disposed, live_projekt exited");
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
	ofile.close();
}
