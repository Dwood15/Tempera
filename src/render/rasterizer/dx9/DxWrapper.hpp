/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_USER && defined(DX_WRAPPER)
bool LoadDXProxy(HMODULE* mod);
void FreeDXProxy(HMODULE mod);
#else
void Yelo_DirectInput8Create() {}
#endif