/*
	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <vector>
#include <Windows.h>
#include <DbgHelp.h>

#define MAX_EXCEPTIONS_TO_LOG 4

//Try to let the game handle the exception first...
#define CALL_FIRST 0

static void *h;

const void DUMP_INT_REGISTERS(PCONTEXT context);

const char *seDescription(const DWORD &code);

const void DUMP_REGISTERS(PCONTEXT context);

const void SetPageGuard(uintptr_t startAddr, uintptr_t length, void *callback, bool setExecuteBit);

LONG WINAPI CEInternalExceptionHandler(struct _EXCEPTION_POINTERS *ExceptionInfo);

//https://msdn.microsoft.com/en-us/library/windows/desktop/aa366549(v=vs.85).aspx
