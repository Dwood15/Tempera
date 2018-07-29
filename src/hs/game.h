#pragma once

#include "enums/hs_enums.h"
#include "structures.h"

namespace Yelo::Scripting {
	// recorded_animations_data_t &RecordedAnimations();

	void InitializeLibrary();

	void DisposeLibrary();

	static void MemoryUpgradesInitialize() {
		for (auto ptr : K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST) {
			*ptr = Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade;
		}
		for (auto ptr : K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST) {
			*ptr = Enums::k_total_scenario_hs_syntax_data_upgrade;
		}

		// change from 'jz' (0x74) to 'jge' (0x7D)
		// This allows us to support scenarios with original script nodes, or with
		// Yelo based script nodes, which are larger (because of memory upgrades, duh)
		*ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK = Enums::_x86_opcode_jge_short;
	}

	static void MemoryUpgradesDispose() {}

	void __cdecl Update() {}

	void Initialize() {
		MemoryUpgradesInitialize();

		InitializeLibrary();

		// Currently, no code is ran in Update
		Memory::CreateHookRelativeCall(&Update, reinterpret_cast<void *>(HS_UPDATE_HOOK), Enums::_x86_opcode_retn);
	}

	// recorded_animations_data_t &RecordedAnimations() { DPTR_IMP_GET_BYREF(recorded_animations); }

	hs_syntax_data_t &HsSyntax() { DPTR_IMP_GET_BYREF(hs_syntax); }

	object_list_header_data_t &ObjectListHeader() { DPTR_IMP_GET_BYREF(object_list_header); }

	list_object_reference_data_t &ListObjectReference() { DPTR_IMP_GET_BYREF(list_object_reference); }

	hs_thread_data_t &HsThreads() { DPTR_IMP_GET_BYREF(hs_threads); }

	hs_globals_data_t &HsGlobals() { DPTR_IMP_GET_BYREF(hs_globals); }

	void Dispose() {
		Scripting::DisposeLibrary();

		MemoryUpgradesDispose();
	}

};
