#include <windows.h>
#include <memoryapi.h>
#include "../memory_interface_base.hpp"

// This memory upgrade design can only work with a DLL override system
static void PhysicalMemoryReInitialize() {
	static const uintptr_t INITIALIZE = 0x445230;

	s_physical_memory_map_globals *globals = PhysicalMemoryMapGlobals();

	VirtualFree(globals->sound_cache_base_address, 0, MEM_RELEASE);
	VirtualFree(globals->texture_cache_base_address, 0, MEM_RELEASE);
	VirtualFree(globals->game_state_base_address, 0, MEM_RELEASE);

	__asm call   INITIALIZE
}

static void MemoryUpgradesInitialize() {
	const uint k_allocation_size = Enums::k_physical_memory_map_allocation_size_upgrade;
	Memory::WriteMemory(GET_DATA_VPTR(PHYSICAL_MEMORY_ALLOCATION_SIZE), static_cast<void *>(k_allocation_size));

	PhysicalMemoryReInitialize();
}

static void MemoryUpgradesDispose() {
}
