#include "ceinternal/ceinternal.h"
#include <vector>
#include <Windows.h>

//Call the exception handler first.
#if defined(_MSC_VER)
//Try to let the game handle the exception first...
#define CALL_FIRST 0

void constexpr DUMP_INT_REGISTERS(PCONTEXT context) {
	DEBUG("Edi 0x%X", context->Edi);
	DEBUG("Esi 0x%X", context->Esi);
	DEBUG("Ebx 0x%X", context->Ebx);
	DEBUG("Edx 0x%X", context->Edx);
	DEBUG("Ecx 0x%X", context->Ecx);
	DEBUG("Eax 0x%X", context->Eax);
}

void constexpr DUMP_REGISTERS(PCONTEXT context) {
	switch(context->ContextFlags) {
		case CONTEXT_INTEGER:
			DUMP_INT_REGISTERS (context);
		default:
			return;
	}
}

LONG WINAPI CEInternalExceptionHandler (struct _EXCEPTION_POINTERS *ExceptionInfo) {
	DEBUG ("Logging Exception!");
	#define eirecord ExceptionInfo->ExceptionRecord
	#define eicontext

	auto eCode = eirecord->ExceptionCode;
	DEBUG ("Error Code: 0x%X @ 0x%X", eCode, eirecord->ExceptionAddress);


	auto info = eirecord->ExceptionRecord;
	if ( info != 0x0 ) {
		DEBUG ("Got a nested Exception! Err Code: 0x%X", info->ExceptionCode);
	}

	if (eirecord->ExceptionFlags == 0) {
		DEBUG("THIS EXCEPTION IS CONTINUABLE.");
	}

	DUMP_REGISTERS (ExceptionInfo->ContextRecord);
}

#endif

extern "C" BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	const short dsig[] = { 0x75, 0x2D, 0x68, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0x68, -1, -1, -1, -1, 0x68, -1, -1, -1, -1, 0xE8, -1, -1, -1, -1, 0x68, -1, -1, -1, -1, 0x68, -1, -1, -1, -1, 0x33, 0xC0, 0xE8, -1, -1, -1, -1, 0x83, 0xC4, 0x14, 0x8B, 0x44, 0x24, 0x04, 0x68, -1, -1, -1, -1, 0x68, -1, -1, -1, -1, 0x50 };
//    static bool dedicated = reinterpret_cast<const char *>(sig_scan_s(dsig)) != nullptr;

	static bool loaded = false;
	static auto h;

	void constexpr init (HMODULE reason) {
		InitAddLog (reason);
		h = AddVectoredExceptionHandler (CALL_FIRST, CEInternalExceptionHandler);

		loaded = true;
		spcore::initCEI (reason);
		//initialize_client();
	}

	void constexpr detach () {
		loaded = false;
		spcore::unload ();
//    uninitialize_client();
		ExitAddLog ();
	}

	if ( fdwReason == DLL_PROCESS_ATTACH && !loaded ) {
		init (hinstDLL);
	} else if ( fdwReason == DLL_PROCESS_DETACH && loaded ) {
		detach ();
		RemoveVectoredExceptionHandler (h);
	}
	return true;
}
