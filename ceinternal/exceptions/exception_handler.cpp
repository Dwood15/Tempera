/*
 * Insert GPLv3 license
 */

#pragma once

#include "exception_handler.h"
#include "../ceinternal.h"

#if defined(_MSC_VER)
static volatile DWORD ExceptionCount = 0;

void DUMP_INT_REGISTERS(PCONTEXT context) {
	DEBUG("Edi 0x%X", context->Edi);
	DEBUG("Esi 0x%X", context->Esi);
	DEBUG("Ebx 0x%X", context->Ebx);
	DEBUG("Edx 0x%X", context->Edx);
	DEBUG("Ecx 0x%X", context->Ecx);
	DEBUG("Eax 0x%X", context->Eax);
}

const void DUMP_REGISTERS(PCONTEXT context) {
	switch(context->ContextFlags) {
		case CONTEXT_INTEGER: DUMP_INT_REGISTERS(context);
		case CONTEXT_EXTENDED_REGISTERS: DUMP_INT_REGISTERS(context);
		default: DUMP_INT_REGISTERS(context);
	}
}

const char*seDescription(const DWORD &code) {
	switch(code) {
		case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_FLT_DENORMAL_OPERAND: return "EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT: return "EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION: return "EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW: return "EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK: return "EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW: return "EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
		case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
		case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
		default: return "UNKNOWN EXCEPTION";
	}
}

LONG WINAPI CEInternalExceptionHandler(struct _EXCEPTION_POINTERS*ExceptionInfo) {
	ExceptionCount++;
	//Yes, this is shitty, I KNOW.
	Sleep(ExceptionCount * 2);
	#define eirecord ExceptionInfo->ExceptionRecord

	auto eCode = eirecord->ExceptionCode;
	DEBUG("Error Code: 0x%X :: Type: %s @ 0x%X\n", eCode, seDescription(eCode), eirecord->ExceptionAddress);
	auto info = eirecord->ExceptionRecord;
	if( info != 0x0 ) {
		DEBUG("Got a nested Exception! Err Code: 0x%X", info->ExceptionCode);
	}

	DUMP_REGISTERS(ExceptionInfo->ContextRecord);

	if( ExceptionCount < MAX_EXCEPTIONS_TO_LOG ) {
		if( ExceptionCount > MAX_EXCEPTIONS_TO_LOG * 2 ) {
			printf("Above the exception count max, should be failing right about... now.");
			return FAIL_FAST_GENERATE_EXCEPTION_ADDRESS;
		}
	}

	return EXCEPTION_CONTINUE_EXECUTION;
}

#endif