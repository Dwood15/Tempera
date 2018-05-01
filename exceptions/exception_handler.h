/*
 * Insert GPLv3 license
 */
#pragma once

#include "../ceinternal.h"
#include "../haloforge/headers/addlog.h"
#include <vector>
#include <Windows.h>
#include <dbghelp.h>

#include "../memory_map/memorymap.h"

#ifndef clion_not_be_dumb
#pragma message("eyo there poppet")
#endif

#define MAX_EXCEPTIONS_TO_LOG 5

//Try to let the game handle the exception first...
#define CALL_FIRST 0

static void *h;

const void DUMP_INT_REGISTERS(PCONTEXT context);

const char *seDescription(const DWORD &code);

const void DUMP_REGISTERS(PCONTEXT context);

const void setPageGuard(uintptr_t startAddr, uintptr_t length, void *callback, bool setExecuteBit);

LONG WINAPI CEInternalExceptionHandler(struct _EXCEPTION_POINTERS *ExceptionInfo);




//https://msdn.microsoft.com/en-us/library/windows/desktop/aa366549(v=vs.85).aspx
