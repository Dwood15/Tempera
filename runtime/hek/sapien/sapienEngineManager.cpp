#pragma once

#include <precompile.h>
#include "sapienEngineManager.h"
#include "../../../src/lua/script_manager.h"
#include "../../../CurrentEngine.h"

using namespace feature_management::engines;

void Sapien::WriteHooks() {
	constexpr uint game_tick_hook = 0x51F219;
	calls::WriteSimpleHook(game_tick_hook, &game_tick);

	//main_setup_connection
	constexpr uint post_init_hook = 0x5149AE;
	calls::WriteSimpleHook(post_init_hook, &main_setup_connection_init);
}


Core Sapien::GetCoreAddressList() {
	Core CurrentCore;

	return CurrentCore;
}
