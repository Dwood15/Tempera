/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/DxWrapper.hpp"

#if PLATFORM_IS_USER && defined(DX_WRAPPER)
static void* orig_DirectInput8Create;

__declspec(naked) void WINAPI Yelo_DirectInput8Create()
{ _asm { jmp orig_DirectInput8Create }; }

bool LoadDXProxy(HMODULE* mod)
{
	char path[MAX_PATH];
	GetSystemDirectory(path, sizeof(path));
	strcat(path, "\\dinput8.dll");

	*mod = LoadLibraryA(path);

	if(!*mod) return false;

	orig_DirectInput8Create = GetProcAddress(*mod,"DirectInput8Create");

	if(!orig_DirectInput8Create)
		return false;

	return true;
}

void FreeDXProxy(HMODULE mod)
{
	FreeLibrary(mod);
}
#endif
