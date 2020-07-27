#include "controls.h"
#include "../CurrentEngine.h"

void Control::HandleActionOverride(ushort idx, s_unit_control_data * from) {
	if (CurrentEngine.ShouldOverrideAction(idx)) {

		auto override = CurrentEngine.GetPlayerActionOverride(idx, from);

		from->control_flags = override.control_flagsA;
		from->throttle.x = override.throttle_leftright;
		from->throttle.y =  override.throttle_forwardback;

		from->weapon_index = override.desired_weapon_index;

		from->primary_trigger = override.primary_trigger;

		from->grenade_index = override.desired_grenade_index;

		CurrentEngine.ResetOverride();
	}
}

#include "../../common/addlog.h"

void Control::UnitControl(ushort unit_idx, s_unit_control_data *from, int client_update_idx) {
	PrintLn("Updating player: 0x%x", unit_idx);

	auto to = reinterpret_cast<s_unit_datum *>(CurrentEngine.GetGenericObject(unit_idx));

	if(to->unit.controlling_player_index.handle != static_cast<uint>(-1))	{
		HandleActionOverride(to->unit.controlling_player_index.index, from);
	}

	PrintLn("HandleActionOverride");

	if (*CurrentEngine.main_globals_game_connection_type == 2) {
		PrintLn("game connection type -- non local !?");

		if ((from->control_flags.control_flags_a >> 8) & 0b00101000) {
			to->unit.pad11_networkpcOnly = true;
		} else {
			to->unit.pad11_networkpcOnly = false;
		}
	}

	PrintLn("reading unit");
	auto unit = &to->unit;

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

	PrintLn("Unit control update completed");

}
