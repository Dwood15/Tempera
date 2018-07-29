#include <utility>

#include "sapienEngineManager.h"
#include "../../../src/lua/script_manager.h"
#include "../../../src/CurrentEngine.h"

using namespace feature_management::engines;

// void __declspec(naked) Sapien::OnPlayerActionUpdate() {

//__cdecl makes the _caller_ clean up the stack. __stdcall means our function cleans up the stack

//This is this way, because clang's a pussy-ass bitch
void __declspec(naked) Sapien::OnPlayerActionUpdate() {
	__asm mov     dword ptr[esp+18h], ecx
	__asm mov     dword ptr[esp+14h], edi
	__asm retn
}

const defined_functionrange *Sapien::GetFunctionMap() {
#include "function_map.txt"

	return sapien_function_map;
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
	calls::WriteSimpleHook(0x52C865, &Sapien::OnPlayerActionUpdate); //6 bytes off.
	calls::patchValue<byte>(0x52C869, 0x90); //
	calls::patchValue<unsigned short>(0x52C86A, 0x9090); //

}

Core Sapien::GetCoreAddressList() {
	return Core();
}
