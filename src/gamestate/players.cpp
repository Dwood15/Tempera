#pragma once

#include <enums/weapon_enums.h>
#include <macros_generic.h>
#include "../memory/datum_index.h"
#include "players.h"
#include "../RuntimeManager.h"

const Yelo::TagGroups::scenario_player *get_scenario_location(Yelo::TagGroups::scenario *scen, ushort idx) {
	if (idx < scen->player_starting_locations.Count) {
		return &scen->player_starting_locations.Definitions[idx];
	}
	return nullptr;
}

void players_set_local_player_unit(unsigned short requested_plyr_idx, datum_index player_unit) {
	auto control      = CurrentRuntime->GetPlayerControl(requested_plyr_idx);
	auto current_unit = control->unit_index;

	static auto set_actively_controlled = CurrentRuntime->getFunctionBegin("unit_set_actively_controlled");

	if (!set_actively_controlled) {
		return;
	}

	if (!current_unit.IsNull()) {
		reinterpret_cast<s_unit_datum *>(CurrentRuntime->GetGenericObject(current_unit.index))->unit.controlling_player_index = datum_index::null();
#ifndef __GNUC__
		//MS-style inline assembly is not available: Unable to find targets for this triple (no targets are registered)
		//Bullshit CLion stuff
		__asm xor cl, cl
		calls::DoCall<Convention::m_cdecl, void, datum_index>(*set_actively_controlled, current_unit);
#else
		NOT_IMPLEMENTED;
#endif
	}
}

void unit_set_actively_controlled(datum_index u, bool is_controlled) {
	auto unit = reinterpret_cast<s_unit_datum *>(CurrentRuntime->GetGenericObject(u.index));

	bool isAi;
	if (!unit->unit.actor_index.IsNull() || !unit->unit.swarm_actor_index.IsNull() || !unit->unit.controlling_player_index.IsNull()) {
		isAi = true;
	}

	long_flags newFlags;
	if (unit->unit_object.damage.flags & 4 || !isAi) {
		//Todo: Properly label these enums
		unit->unit.flags &= 0xFFFFFFFE;
		newFlags = unit->unit.flags & 0xFFFFFFBF;
	} else {
		unit->unit.flags |= 1;
		newFlags = unit->unit.flags | 0x40;
	}
	unit->unit.flags = newFlags;

	static std::optional<uintptr_t> item_in_unit_inventory = CurrentRuntime->getFunctionBegin("item_in_unit_inventory");

	if (item_in_unit_inventory) {
		for (uint i = 0; i < MAX_WEAPONS_PER_UNIT; i++) {
			auto current = unit->unit.weapon_object_indices[i];

			if (!current.IsNull()) {
				//TODO: double check the value of param # 2
				::calls::DoCall<Convention::m_fastcall, void, ushort, datum_index>(*item_in_unit_inventory, static_cast<ushort>(current.index), current);
			}
		}
	}

	static auto unit_update_driver_and_gunner = CurrentRuntime->getFunctionBegin("unit_update_driver_and_gunner");

#ifndef __GNUC__
	__asm mov ax, u.index
	::calls::DoCall(*unit_update_driver_and_gunner);
#else
	NOT_IMPLEMENTED;
#endif
}
