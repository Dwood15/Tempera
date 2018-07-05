/*
	Project: haloforge
	File: addlog.cpp
	Copyright � 2009 SilentK, Abyll
	Copyright � 2018 Dwood15

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <fstream>
#include <filesystem>

#include "addlog.h"

static std::ofstream ofile;
static char   dlldir[320];

static volatile bool debug_write_locked = false;

void DBGPrnt(const char *fmt, va_list va_alist) {
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

void DEBUG(const char *fmt, ...) {
	va_list va_alist;

		va_start (va_alist, fmt);
	DBGPrnt(fmt, va_alist);
		va_end (va_alist);

}

/**
 * Initialize the Debug log file.
 */
void InitAddLog(HMODULE hModule, const char * filename) {
	GetModuleFileNameA(hModule, dlldir, 512);

	for (int i = (int) strlen(dlldir); i > 0; i--) {
		if (dlldir[i] == '\\') {
			dlldir[i + 1] = 0;
			break;
		}
	}

	//why in hell is filesystem still experimental???
	auto myPath = std::experimental::filesystem::current_path();

	myPath = myPath /= filename;

	ofile.open(myPath, std::ios::app);

	printf("Initializing log. Path: %ls\n", myPath.c_str());

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
