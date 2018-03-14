/*
	Project: haloforge
	File: addlog.cpp
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


	No addresses here!
*/
#include <ctime>
#include "headers/addlog.h"

std::ofstream ofile;
char dlldir[320];

char *GetDirectoryFile(char *filename) {
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

void __cdecl DEBUG(const char *fmt, ...) {
	if (ofile) {
		if (!fmt)
			return;

		va_list va_alist;
		char logbuf[256] = {0};

			va_start (va_alist, fmt);
		_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
			va_end (va_alist);

		ofile << logbuf << endl;
	}
}

void InitAddLog(HMODULE hModule) {
	GetModuleFileNameA(hModule, dlldir, 512);

	for (int i = (int) strlen(dlldir); i > 0; i--) {
		if (dlldir[i] == '\\') {
			dlldir[i + 1] = 0;
			break;
		}
	}

	ofile.open(GetDirectoryFile((char *) LOGNAME), ios::app);
	std::time_t time = std::time(nullptr);
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	DEBUG("live_projekt injected");
	DEBUG(std::asctime(std::localtime(&time)));
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*");
}

void ExitAddLog() {
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	DEBUG("live_projekt exited");
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
	ofile.close();
}
