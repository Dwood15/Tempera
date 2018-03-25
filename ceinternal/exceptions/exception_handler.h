/*
 * Insert GPLv3 license
 */
#pragma once

#include "../ceinternal.h"
#include "../haloforge/headers/addlog.h"
#include <vector>
#include <Windows.h>
#include <dbghelp.h>

#define MAX_EXCEPTIONS_TO_LOG 5
//Call the exception handler first.
//Try to let the game handle the exception first...
#define CALL_FIRST 0

static void*h;

void DUMP_INT_REGISTERS(PCONTEXT context);

const char*seDescription(const DWORD &code);

const void DUMP_REGISTERS(PCONTEXT context);

LONG WINAPI CEInternalExceptionHandler(struct _EXCEPTION_POINTERS*ExceptionInfo);
