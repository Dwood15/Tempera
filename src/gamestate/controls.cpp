#include "controls.h"
#include "../RuntimeManager.h"

static int ucUdates = 0;
void Control::UnitControl(ushort unit_idx, s_unit_control_data *from, int client_update_idx) {
	ucUdates++;

	auto to = reinterpret_cast<s_unit_datum *>(CurrentRuntime->GetGenericObject(short(unit_idx)));

	auto playerHandle = to->unit.controlling_player_index.handle;

	if(playerHandle == static_cast<uint>(-1)) {
		PrintLn("player: 0x%x player index controlling is -1 on the unit, exiting UnitControl method", unit_idx);
		return;
	}

	auto playerIdx = to->unit.controlling_player_index.index;

	if (!CurrentRuntime->IsPlayerSpawned(playerIdx) || !CurrentRuntime->IsPlayerValid(playerIdx)) {
		PrintLn("Player: [%d] is either not spawned or is not valid. Waiting.", playerIdx);
		return;
	}

	if (playerIdx > 0) {
		PrintLn("Handling player idx: [%d]", playerIdx);
	}

	//For when we're still in a cutscene and the player still hasn't spawned yet
	if (to->unit_object.position.x == 0 &&  to->unit_object.position.y == 0 && to->unit_object.position.z == 0) {
		PrintLn("Player unit object is at level origin. Waiting until player spawns.");
		return;
	}

	auto connType = *CurrentRuntime->main_globals_game_connection_type;
	PrintLn("[%d] UnitControl, game connType: [%d], playerIdx: [%d], unitIdx: [0x%x]",
			ucUdates, connType, playerIdx, unit_idx);

	s_unit_control_data override = *from;

//	PrintLn("HandleActionOverride Completed");

	if (connType == 2) {
		static bool printed;
		if (!printed) {
			printed = true;
			PrintLn("game connection type -- networked");
		}

		if ((override.control_flags.control_flags_a >> 8) & 0b00101000) {
			to->unit.pad11_networkpcOnly = true;
		} else {
			to->unit.pad11_networkpcOnly = false;
		}
	}

//	PrintLn("[%d] transposing controls onto the unit");

	auto unit = &to->unit;

	unit->throttle        = override.throttle;
	unit->primary_trigger = override.primary_trigger;
	unit->aiming_speed    = (byte)override.aiming_speed;

	if (from->weapon_index != -1) {
		unit->next_weapon_index = override.weapon_index;
	}

	//PrintLn("Unit control update grenade index");

	if (from->grenade_index != -1) {
		unit->next_grenade_index = static_cast<sbyte>(override.grenade_index);
	}

	unit->desired_zoom_level = static_cast<byte>(override.zoom_index);
	unit->control_flags = override.control_flags;
	unit->looking_angles = override.looking_vector;

	unit->desired_aiming_vector = override.aiming_vector;
	unit->desired_facing_vector = override.facing_vector;

	if (client_update_idx == -1) {
		unit->last_completed_client_update_id_valid = false;
	} else {
		unit->last_completed_client_update_id = client_update_idx;
		unit->last_completed_client_update_id_valid = true;
	}

	//PrintLn("[%d] Unit control update completed", ucUdates);
}
