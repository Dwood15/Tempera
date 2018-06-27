/*
	Project: tempera
	File: forge.cpp
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "exception_handler.h"
#include "../../CurrentEngine.h"
#include <memory_map.h>
#include <versions.h>
#include <addlog.h>

//Not sure we need to guarantee we're in MSVC any more...
#if defined(_MSC_VER)
static volatile DWORD ExceptionCount = 0;

const void DUMP_INT_REGISTERS(PCONTEXT context) {
	DEBUG("\tEdi 0x%08X  Esi 0x%08X  Ebx 0x%08X  Edx 0x%08X  Ecx 0x%08X  Eax 0x%08X", context->Edi, context->Esi, context->Ebx, context->Edx, context->Ecx, context->Eax);
}

const void DUMP_STK_REGISTERS(PCONTEXT context) {
	DEBUG("\tEbp 0x%08X Eip 0x%08X Esp 0x%08X", context->Ebp, context->Eip, context->Esp);
}

const void DUMP_REGISTERS(PCONTEXT context) {
	DEBUG("~~~ General (INTEGER) Registers ~~~");
	DUMP_INT_REGISTERS(context);
	DEBUG("~~~ Stack Registers ~~~");
	DUMP_STK_REGISTERS(context);
	DEBUG("\n");
}

const char *seDescription(const DWORD &code) {
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
const void SetPageGuard(uintptr_t startAddr, uintptr_t length, void *callback, bool setExecuteBit = true) {
	DEBUG("PageGuard should be getting set now. Not implemented yet tho.");
}




LONG WINAPI CEInternalExceptionHandler(struct _EXCEPTION_POINTERS *ExceptionInfo) {
	ExceptionCount++;
	//Yes, this is bad practice.
	Sleep(ExceptionCount * 4);

	auto eirecord = ExceptionInfo->ExceptionRecord;
	auto eCode = eirecord->ExceptionCode;

	DEBUG("Error Code: 0x%X - %s @ 0x%X (%s)", eCode, seDescription(eCode), eirecord->ExceptionAddress, CurrentEngine.getMemoryRegionDescriptor((uintptr_t)eirecord->ExceptionAddress));

	auto info = eirecord->ExceptionRecord;
	if (info != 0x0) {
		DEBUG("Got a nested Exception! Err Code: 0x%X\n", info->ExceptionCode);
	}

	if (ExceptionCount < MAX_EXCEPTIONS_TO_LOG) {
		DUMP_REGISTERS(ExceptionInfo->ContextRecord);
		printf("Exception handled! ");
	} else if (ExceptionCount > MAX_EXCEPTIONS_TO_LOG) {
		printf("Above the exception count max, will fail now :)");
		return FAIL_FAST_GENERATE_EXCEPTION_ADDRESS;
	}

	return EXCEPTION_CONTINUE_EXECUTION;
}

#endif
