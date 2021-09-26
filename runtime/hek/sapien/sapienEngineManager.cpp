#include <utility>

#include "sapienEngineManager.h"

#include "../../../src/RuntimeManager.h"

using namespace feature_management::engines;

namespace {
	// Why is this necessary? HCE/HPC pass in color via ARG.EAX, and the rest by stack.
	// Sapien is full on __cdecl, so an interface is necessary to keep everything compatible.
	void naked sapien_console_printf_interface() {
		// initial pop edx is to take the function pointer out
		__asm {
			mov edx, dword ptr [esp+4]
			mov ecx, 00506B20h
			push dword ptr [esp+8]
			push edx
			push eax
			call ecx
			add esp, 0Ch
			ret
		}
	}
} // (anonymous)

//__cdecl makes the _caller_ clean up the stack. __stdcall means our function cleans up the stack

namespace sapienStatic {
	void naked OnPlayerActionUpdate() {
		//Clang and gcc are pussy-ass bitches
#if !defined(__GNUC__) && !defined(__CLANG__)
		__asm mov     dword ptr[esp+18h], ecx
		__asm mov     dword ptr[esp+14h], edi
		__asm retn
#endif
	}
}


void Sapien::WriteHooks() {
	constexpr uint game_tick_hook = 0x51F219;
	calls::WriteSimpleHook(game_tick_hook, &game_tick);

	//main_setup_connection
	constexpr uint post_init_hook = 0x5149AE;
	calls::WriteSimpleHook(post_init_hook, &main_setup_connection_init);

	//Originally: 89 4C 24 18
	//					89 7C 24 14
	//Basically just sets some random value to -1. Couldn't tell if it was being used or not.
	calls::patchValue<byte>(0x52C864, 0xE8); //call //0x0

	calls::WriteSimpleHook(0x52C865, sapienStatic::OnPlayerActionUpdate); //6 bytes off.
	calls::patchValue<byte>(0x52C869, 0x90); //
	calls::patchValue<unsigned short>(0x52C86A, 0x9090); //
}

LPCoreAddressList Sapien::GetCoreAddressList() {
	auto add_list = LPCoreAddressList{};

	_core_0 * core0 = new _core_0;
	_core_1 * core1 = new _core_1;

	core0->PlayersGlobals = *(s_players_globals_data **) 0x1057538;
	core0->Teams          = *(data_header<void> **) 0x1057548;
	core0->Players        = *(data_header<player> **) 0x105754C;

	// object_list_header_data
	core1->Object = *(data_header<object_header> **) 0x107F2E4;

	add_list.core_0 = reinterpret_cast<uint>(core0);
	add_list.core_1 = reinterpret_cast<uint>(core1);

	add_list.CONSOLE_TEXT_HOOK_ADDRESS = reinterpret_cast<std::uintptr_t>(&sapien_console_printf_interface);

	add_list.at_main_menu      = 0xDBD909;
	add_list.game_time_globals = 0xBD8A18;

	add_list.player_control_globals_data = 0xDF76B0; // = player_control_globals *

	//Pointers to an address
	add_list.cinematic_globals = 0xCA4574;
	return add_list;
}

defined_functionrange * Sapien::GetFunctionMap() {
#include "function_map.txt"
	return const_cast<defined_functionrange *>(sapien_function_map);
}



