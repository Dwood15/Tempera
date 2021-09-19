/*
    You should have received a copy of the GNU General Public License
    along with Tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <ctime>
#include <filesystem>

#include "addlog.h"

::std::ofstream ofile;
char   dlldir[320];

//Multiple threads could be writing to the file at once, so we back it behind this
//little semaphore. Volatile only affects compiler optimizations for the local function
//It doesn't affect whether the variable is cached.
static std::atomic<bool> debug_write_locked;

#ifdef __GNUC__
template<bool toConsole>
void Print(const char *fmt, ...)  {
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

template<bool toConsole>
void PrintLn(const char *fmt, ...) {
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

int constexpr length(const char* str) {
	return *str ? 1 + length(str + 1) : 0;
}

void bufWarn(int diff) {
	constexpr const char * warnFmt = "Line was too long. We truncated: [%d] bytes";
	//UINT_MAX is 13 characters: add one for null terminator.
	constexpr uint32_t warnBufSize = length(warnFmt) + 14;
	char warnBuf[warnBufSize];
	_vsnprintf(warnBuf, warnBufSize, warnFmt, (va_list)diff);
	ofile << warnBuf << ::std::endl;
}

void DBGPrnt(const char *fmt, va_list va_alist) {
	if (!ofile || !fmt) {
		return;
	}

	if (debug_write_locked) {
		Sleep(10);
		//Too impatient for this.
		if (debug_write_locked) {
			return;
		}
	}

	debug_write_locked = true;

	constexpr int bufSize = 256;
	char logbuf[bufSize];

	int cwrit = _vsnprintf(logbuf, bufSize, fmt, va_alist);

	if (cwrit > bufSize) {
		bufWarn(cwrit - bufSize);
	}

	ofile << logbuf <<::std::endl;

	debug_write_locked = false;
}

/**
 * DEBUG is a helper funciton for interfacing with the Console.
 */
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
	if (!filename ) {
		printf("Filename missing. Cannot add to log\n");
		throw "Tempera FileName missing InitAddLog";
	}

	GetModuleFileNameA(hModule, dlldir, 512);

	for (int i = (int) strlen(dlldir); i > 0; i--) {
		if (dlldir[i] == '\\') {
			dlldir[i + 1] = 0;
			break;
		}
	}

	auto myPath =::std::filesystem::current_path() /= filename;

	ofile.open(myPath.filename().string(),::std::ios_base::app);

	printf("Initializing log. Path: %ls\n", myPath.c_str());

::std::time_t time =::std::time(nullptr);
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	DEBUG("Tempera Injected, live_projekt initialized");
	DEBUG(::std::asctime(::std::localtime(&time)));
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
}

void ExitAddLog() {
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	DEBUG("Tempera Disposed, live_projekt exited");
	DEBUG("*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
	ofile.close();
}
