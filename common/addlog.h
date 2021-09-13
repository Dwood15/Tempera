/*
    You should have received a copy of the GNU General Public License
    along with Tempera.  If not, see <http://www.gnu.org/licenses/>.

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

	DBGPrnt(fmt, va_alist);

	if constexpr (toConsole) {
		vprintf(fmt, va_alist);
		//I'm incredibly lazy. Hopefully the compiler can optimize it out.
		vprintf("\n", va_alist);
	}
		va_end(va_alist);
}
#endif

void InitAddLog(HMODULE hModule, const char * filename);
void ExitAddLog();
