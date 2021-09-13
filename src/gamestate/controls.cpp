#include "controls.h"
#include "../CurrentEngine.h"

void Control::HandleActionOverride(ushort idx, s_unit_control_data * from) {
	auto override = CurrentEngine->GetPlayerActionOverride(idx, from);

	from->control_flags = override.control_flagsA;
	from->throttle.x = override.throttle_leftright;
	from->throttle.y = override.throttle_forwardback;

	from->weapon_index = override.desired_weapon_index;

	from->primary_trigger = override.primary_trigger;

	from->grenade_index = override.desired_grenade_index;
	//CurrentEngine->ClampIndex(idx);
}

static int ucUdates = 0;
void Control::UnitControl(ushort unit_idx, s_unit_control_data *from, int client_update_idx) {
	ucUdates++;
	PrintLn("[%d] Updating player with unit index: 0x%x", ucUdates, unit_idx);

	auto to = reinterpret_cast<s_unit_datum *>(CurrentEngine->GetGenericObject(short(unit_idx)));

	auto playerHandle = to->unit.controlling_player_index.handle;

	if(playerHandle == static_cast<uint>(-1)) {
		PrintLn("player: 0x%x player index controlling is -1 on the unit, exiting UnitControl method", unit_idx);
		return;
	}

	auto playerIdx = to->unit.controlling_player_index.index;

	if (!CurrentEngine->IsPlayerSpawned(playerIdx) || !CurrentEngine->IsPlayerValid(playerIdx)) {
		PrintLn("Player: [%d] is either not spawned or is not valid. Waiting.", playerIdx);
		return;
	}

	//For when we're still in a cutscene and the player still hasn't spawned yet
	if (to->unit_object.position.x == 0 &&  to->unit_object.position.y == 0 && to->unit_object.position.z == 0) {
		PrintLn("Player unit object is at level origin. Waiting until player spawns.");
		return;
	}

	PrintLn("Executing HandleActionOverride");
	HandleActionOverride(playerIdx, from);
	PrintLn("HandleActionOverride Completed");

	auto connType = *CurrentEngine->main_globals_game_connection_type;
	PrintLn("[%d] Unit Control for game connection type: [%d]", ucUdates, connType);

	if (connType == 2) {
		PrintLn("game connection type -- non-local !?");

		if ((from->control_flags.control_flags_a >> 8) & 0b00101000) {
			to->unit.pad11_networkpcOnly = true;
		} else {
			to->unit.pad11_networkpcOnly = false;
		}
	}

	PrintLn("[%d] transposing controls onto the unit");

	auto unit = &to->unit;

	unit->control_flags.control_flags.jump_button = 1;

	unit->throttle        = from->throttle;
	unit->primary_trigger = from->primary_trigger;
	unit->aiming_speed    = (byte)from->aiming_speed;

	if (from->weapon_index != -1) {
		unit->next_weapon_index = from->weapon_index;
	}

	PrintLn("Unit control update grenade index");

	if (from->grenade_index != -1) {
		unit->next_grenade_index = static_cast<sbyte>(from->grenade_index);
	}

	unit->desired_zoom_level = static_cast<byte>(from->zoom_index);

	//Heh. Vertical throttle, anyone?
	unit->throttle.z = 0;

	unit->control_flags = from->control_flags;

	unit->looking_angles = from->looking_vector;

	unit->desired_aiming_vector = from->aiming_vector;

	unit->desired_facing_vector = from->facing_vector;

	if (client_update_idx == -1) {
		unit->last_completed_client_update_id_valid = false;
	} else {
		unit->last_completed_client_update_id = client_update_idx;
		unit->last_completed_client_update_id_valid = true;
	}

	PrintLn("[%d] Unit control update completed", ucUdates);
}
