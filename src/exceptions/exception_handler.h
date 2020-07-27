/*
	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <vector>
#include <Windows.h>
#include <DbgHelp.h>
#include <locale.h>
#include <locale>

#define MAX_EXCEPTIONS_TO_LOG 2

//Try to let the game handle the exception first...
#define CALL_FIRST 0

static void * ExceptionHandlerHandle;

void DUMP_INT_REGISTERS(PCONTEXT context);

const char *GetExceptionString(const DWORD &code);

void DUMP_REGISTERS(PCONTEXT context);

void SetPageGuard(uintptr_t startAddr, uintptr_t length, void *callback, bool setExecuteBit);

//The descriptor for our header
static struct NtHeaderData {
	bool initialized = false;
	PIMAGE_NT_HEADERS pNtHeader;
	unsigned int imageSize;
	unsigned int imageSizeKB;
	unsigned int baseImageLocation;

} NtHeader;

void InitializeDllImageContext(PVOID imageBase);
char* PrintImageDetails();

LONG WINAPI CEInternalExceptionHandler(struct _EXCEPTION_POINTERS *ExceptionInfo);

//https://msdn.microsoft.com/en-us/library/windows/desktop/aa366549(v=vs.85).aspx
