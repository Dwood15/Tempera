/*
	Project: tempera
	File: forge.cpp
 	Copyright 	 2020 Dwood / Zoru

	This file is part of Tempera.

   You should have received a copy of the GNU General Public License
   along with Tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "exception_handler.h"
#include "../RuntimeManager.h"
#include <memory_map.h>
#include <addlog.h>
#include <atomic>

//Not sure we need to guarantee we're in MSVC any more
#if defined(_MSC_VER)

void DUMP_INT_REGISTERS(PCONTEXT context) {
	DEBUG("\tEdi [0x%08X]  Esi [0x%08X]  Ebx [0x%08X]  Edx [0x%08X]  Ecx [0x%08X]  [Eax 0x%08X]", context->Edi, context->Esi, context->Ebx, context->Edx, context->Ecx, context->Eax);
}

void DUMP_STK_REGISTERS(PCONTEXT context) {
	DEBUG("\tEbp [0x%08X] Eip [0x%08X] Esp [0x%08X]", context->Ebp, context->Eip, context->Esp);
}

void DUMP_REGISTERS(PCONTEXT context) {
	DEBUG("\t~~~ General (INTEGER) Registers ~~~");
	DUMP_INT_REGISTERS(context);
	DEBUG("\t~~~ Stack Registers ~~~");
	DUMP_STK_REGISTERS(context);
	DEBUG("\t~~~~~~~~~~~");
}


//char* PrintImageDetails() {
//	static char imgDta[120];
//	const char* fmt = "\n\tDLL startAddress = [0x%x] |"
//					  " DLL ImageSizeKB: [%d]";
//
//	sprintf(imgDta, fmt, NtHeader.baseImageLocation, NtHeader.imageSizeKB);
//
//	return imgDta;
//}
//
//void InitializeDllImageContext(PVOID imageBase) {
//	NtHeader.baseImageLocation = reinterpret_cast<uint32>(imageBase);
//	NtHeader.pNtHeader = ImageNtHeader(imageBase);
//	NtHeader.imageSize = NtHeader.pNtHeader->OptionalHeader.SizeOfImage;
//	NtHeader.imageSizeKB = NtHeader.imageSize / 1024;
//
//	NtHeader.initialized = true;
//}


const char *GetExceptionString(const DWORD &code) {
	switch (code) {
		case EXCEPTION_ACCESS_VIOLATION:
			return "EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_BREAKPOINT:
			return "EXCEPTION_BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			return "EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			return "EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT:
			return "EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION:
			return "EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW:
			return "EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK:
			return "EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW:
			return "EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			return "EXCEPTION_ILLEGAL_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR:
			return "EXCEPTION_IN_PAGE_ERROR";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			return "EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW:
			return "EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION:
			return "EXCEPTION_INVALID_DISPOSITION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_PRIV_INSTRUCTION:
			return "EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_SINGLE_STEP:
			return "EXCEPTION_SINGLE_STEP";
		case EXCEPTION_STACK_OVERFLOW:
			return "EXCEPTION_STACK_OVERFLOW";
		default:
			return "UNKNOWN EXCEPTION";
	}
}

/**
 * Set a Page Guard to monitor reads/writes/execution bits of various sections of halo's code.
 * For more information, see: https://msdn.microsoft.com/en-us/library/windows/desktop/aa366549(v=vs.85).aspx
 * @param startAddr - The beginning address to monitor
 * @param length 	  - The size of the region (in bytes) to monitor.
 * @param callback	- function to call upon resolution of the pageGuard. struct _EXCEPTION_POINTERS *ExceptionInfo is expected for sole argument.
 * @param setExecuteBit - Whether to use PAGE_EXECUTE_READWRITE, or PAGE_READWRITE.
 */
void SetPageGuard(uintptr_t startAddr, uintptr_t length, void *callback, bool setExecuteBit = true) {
	DEBUG("PageGuard should be getting set now. Not implemented yet tho.");
}

static std::atomic<DWORD> ExceptionCount;

/**
 * This method is intended to capture basic information about exceptions and attempts to
 * give basic information about the exception, including known method and typenames.
 * @param ExceptionInfo
 * @return Whether or not the application should continue running despite the exception
 */
LONG WINAPI CEInternalExceptionHandler(struct _EXCEPTION_POINTERS *ExceptionInfo) {

	DEBUG("Exception being handled.");

	Sleep(10);

	if (ExceptionCount >= MAX_EXCEPTIONS_TO_LOG) {
		DEBUG("\tAbove the exception count max, failing now :)\n");
		RemoveVectoredExceptionHandler(ExceptionHandlerHandle);
		return FAIL_FAST_NO_HARD_ERROR_DLG;
	}

	auto eiRecord = ExceptionInfo->ExceptionRecord;
	auto eCode = eiRecord->ExceptionCode;

	auto exceptionAddr = eiRecord->ExceptionAddress;
	auto regionDescriptor = getMemoryRegionDescriptor((void *)exceptionAddr);
	auto exceptStr = GetExceptionString(eCode);

	DEBUG("\tErr Code: 0x%X - %s @ 0x%X (%s)\n", eCode, exceptStr, exceptionAddr, regionDescriptor);

	auto nestedExc = eiRecord->ExceptionRecord;
	if (nestedExc != nullptr) {
		DEBUG("Got a nested Exception! Err Code: 0x%X\n", nestedExc->ExceptionCode);
	}

	if (ExceptionCount <= MAX_EXCEPTIONS_TO_LOG) {
		DUMP_REGISTERS(ExceptionInfo->ContextRecord);
	}

	ExceptionCount++;

	return EXCEPTION_EXECUTE_HANDLER;
}

#endif
