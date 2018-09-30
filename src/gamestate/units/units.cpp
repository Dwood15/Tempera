#pragma once
#include <enums/unit_enums.h>
#include <enums/scenario_enums.h>
#include "units.h"
#include "../objects.h"
#include "../../game/objects/objects.h"

namespace Yelo::Objects {
	const TagGroups::unit_extensions *GetUnitExtensionDefinition(const datum_index unit_index) {
		auto *unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(unit_index);
		if (!unit_datum) {
			return nullptr;
		}

		auto *unit_definition = blam::tag_get<TagGroups::s_unit_definition>(unit_datum->object.definition_index);
		if (!unit_definition) {
			return nullptr;
		}

		if (unit_definition->unit.extensions.Count != 1) {
			return nullptr;
		}

		return &unit_definition->unit.extensions[0];
	}

	const TagGroups::unit_seat *GetSeatDefinition(const datum_index unit_index, const int16 seat_index) {
		if (seat_index == NONE) {
			return nullptr;
		}

		auto *unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(unit_index);
		if (!unit_datum) {
			return nullptr;
		}

		auto *unit_definition = blam::tag_get<TagGroups::s_unit_definition>(unit_datum->object.definition_index);
		if (!unit_definition) {
			return nullptr;
		}

		if (unit_definition->unit.seats.Count <= seat_index) {
			return nullptr;
		}

		return &unit_definition->unit.seats[seat_index];
	}

	const TagGroups::unit_seat_extensions *GetSeatExtensionDefinition(const datum_index unit_index, const int16 seat_index) {
		auto *seat = GetSeatDefinition(unit_index, seat_index);
		if (seat && (seat->seat_extensions.Count != 0)) {
			return &seat->seat_extensions[0];
		}

		return nullptr;
	}

	datum_index GetUnitInSeat(const datum_index unit_index, const int16 seat_index) {
		// Traverse the object's children to find a unit in the matching seat
		auto *unit_datum = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		for (datum_index current = unit_datum->object.first_object_index;
			  !current.IsNull();
			  current = blam::object_get(current)->next_object_index) {
			auto *current_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(current);
			if (current_datum && (current_datum->unit.vehicle_seat_index == seat_index)) {
				return current;
			}
		}

		return datum_index::null;
	}

	static int16
	GetSeatWeaponAnimation(const TagGroups::model_animation_graph &animation_graph, const int16 seat_index, const int16 weapon_index,
								  const Enums::weapon_class_animation animation_class) {
		if ((seat_index == NONE) || (weapon_index == NONE)) {
			return NONE;
		}

		// Get the animation index for the seats unarmed melee animation
		auto &units_block   = animation_graph.units[seat_index];
		auto &weapons_block = units_block.weapons[weapon_index];

		if (weapons_block.animations.Count <= animation_class) {
			return NONE;
		}

		return weapons_block.animations[animation_class];
	}

	void PLATFORM_API

	BipedSeatedMelee(const datum_index unit_index) {
		auto *biped_datum = blam::object_try_and_get_and_verify_type<s_biped_datum>(unit_index);
		if (!biped_datum || (biped_datum->unit.vehicle_seat_index == NONE)) {
			return;
		}

		auto *parent_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(biped_datum->object.parent_object_index);
		if (!parent_datum) {
			return;
		}

		auto *parent_definition = blam::tag_get<TagGroups::s_unit_definition>(parent_datum->object.definition_index);
		if (!parent_definition) {
			return;
		}

		auto &seat = parent_definition->unit.seats[biped_datum->unit.vehicle_seat_index];
		if (!TEST_FLAG(seat.flags, Flags::_unit_seat_allows_melee_bit)) {
			return;
		}

		// If a melee is in progress handle its timer
		if (biped_datum->biped.melee_animation_time > 0) {
			if (biped_datum->biped.melee_animation_time == biped_datum->biped.melee_cause_damage_time) {
				Units::UnitCausePlayerSeatedMelee(unit_index);
			}
			biped_datum->biped.melee_animation_time--;

			return;
		}

		if (biped_datum->unit.controlling_player_index.IsNull()) {
			return;
		}

		if (!TEST_FLAG(biped_datum->unit.control_flags, Flags::_unit_control_melee_bit)) {
			return;
		}

		// If an animation is in progress, skip the melee
		if (!blam::unit_animation_state_interruptable(biped_datum->unit.animation, Enums::_unit_animation_state_melee)) {
			return;
		}

		sbyte melee_animation_length = 0;
		sbyte melee_damage_keyframe  = 0;

		auto weapon_index = blam::unit_inventory_get_weapon(unit_index, biped_datum->unit.current_weapon_index);
		if (weapon_index.IsNull()) {
			// If the unit is unarmed, get the unarmed melee animation
			auto *unit_definition = blam::tag_get<TagGroups::s_unit_definition>(biped_datum->object.definition_index);
			if (unit_definition->object.references.animations.tag_index.IsNull()) {
				return;
			}
			auto &animations_definition = *blam::tag_get<TagGroups::model_animation_graph>(
				unit_definition->object.references.animations.tag_index);

			auto animation_index = GetSeatWeaponAnimation(animations_definition, biped_datum->unit.animation.seat_index,
																		 biped_datum->unit.animation.seat_weapon_index, Enums::_weapon_class_animation_melee);

			if (animation_index == NONE) {
				return;
			}

			// Play a random permutation of the animation
			blam::object_start_interpolation(unit_index, 6);
			animation_index = blam::animation_choose_random_permutation_internal(Enums::_animation_update_kind_affects_game_state,
																										unit_definition->object.references.animations.tag_index,
																										animation_index);

			auto &animation = animations_definition.animations[animation_index];
			melee_animation_length = (sbyte) animation.frame_count;
			melee_damage_keyframe  = (sbyte) animation.key_frame_index;

			// Replace the current animation with the melee animation
			biped_datum->unit.animation.replacement_animation.animation_index = animation_index;
			biped_datum->unit.animation.replacement_animation.frame_index     = 0;
			biped_datum->unit.animation.replacement_animation_state           = Enums::_unit_replacement_animation_state_melee;
		} else {
			if (blam::weapon_prevents_melee_attack(weapon_index)) {
				return;
			}

			// Play the weapons melee animation
			blam::unit_animation_start_action(unit_index, Enums::_unit_replacement_animation_state_melee);
			blam::weapon_stop_reload(weapon_index);

			melee_animation_length = (sbyte) blam::weapon_get_first_person_animation_time(weapon_index, 0,
																													Enums::_first_person_weapon_animation_melee, NONE);
			melee_damage_keyframe  = (sbyte) blam::weapon_get_first_person_animation_time(weapon_index, 1,
																													Enums::_first_person_weapon_animation_melee, NONE);
		}
		blam::first_person_weapon_message_from_unit(unit_index, 4);

		biped_datum->biped.melee_animation_time    = melee_animation_length;
		biped_datum->biped.melee_cause_damage_time = melee_animation_length - melee_damage_keyframe;
	}

	void UnitCanEnterSeatMultiteam(const datum_index unit_index, const datum_index target_unit_index, const int16 target_seat_index,
											 _Out_opt_ datum_index

	* return_unit_in_seat
	,
	bool &result
	) {
	datum_index unit_in_seat = datum_index::null;

	auto unit        = blam::object_get_and_verify_type<s_unit_datum>(unit_index);
	auto target_unit = blam::object_get_and_verify_type<s_unit_datum>(target_unit_index);

	if (unit_index == target_unit_index) {
	result = false;
}

for (
datum_index child_object_index = target_unit->object.first_object_index;
!child_object_index.

IsNull();

child_object_index             = blam::object_get(child_object_index)->next_object_index
)
{
auto child_object = blam::object_try_and_get_and_verify_type<s_unit_datum>(child_object_index);

if (child_object)
{
// Check if the child_object is in the seat_index that unit_index is trying to enter
if (child_object->unit.vehicle_seat_index == target_seat_index)
{
	unit_in_seat = child_object_index;
	result       = false;
}
// If multiteam vehicles is prohibited, test unit and target unit teams
else if (TEST_FLAG (Scenario::GetYelo()
->gameplay.flags, Flags::_project_yellow_gameplay_prohibit_multiteam_vehicles_bit))
{
if (unit->unit.controlling_player_index.IsNull() || !blam::game_team_is_enemy(child_object->object.owner_team, unit->object.owner_team)) {
result = true;
}
else
{
result = false;
}
}
}
}

if (return_unit_in_seat != nullptr)
{
*
return_unit_in_seat = unit_in_seat;
}
}

#pragma region Animation
namespace Units {
	namespace Animations {
		typedef void (*animation_state_keyframe_handler_t)(const datum_index, const unit_animation_keyframe);

		static animation_state_keyframe_handler_t g_animation_state_handlers[] =
																	{
																		//_unit_animation_state_yelo_seat_boarding
																		{nullptr},
																		//_unit_animation_state_yelo_seat_ejecting
																		{nullptr},
																		//_unit_animation_state_yelo_unit_mounted
																		{nullptr},
																		//_unit_animation_state_yelo_unit_transforming
																		{nullptr},
																	};
		static_assert(std::size(g_animation_state_handlers == (_unit_animation_state_yelo - _unit_animation_state));

		void AnimationStateDefinedKeyframe(const datum_index unit_index, const Enums::unit_animation_state state) {
			auto &handler = g_animation_state_handlers[state - Enums::_unit_animation_state];
			if (handler) {
				auto &unit_datum           = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
				auto &animation_definition = *blam::tag_get<TagGroups::model_animation_graph>(unit_datum.unit_object.animation.definition_index);
				auto &animation            = animation_definition.animations[unit_datum.unit_object.animation.state.animation_index];

				if (unit_datum.unit_object.animation.state.frame_index == animation.key_frame_index) {
					handler(unit_index, Enums::_unit_animation_keyframe_primary);
				} else if (unit_datum.unit_object.animation.state.frame_index == animation.second_key_frame_index) {
					handler(unit_index, Enums::_unit_animation_keyframe_secondary);
				}
			}
		}

		void AnimationStateFinalKeyframe(const datum_index unit_index, const Enums::unit_animation_state state) {
			auto &handler = g_animation_state_handlers[state - Enums::_unit_animation_state];
			if (handler) {
				handler(unit_index, Enums::_unit_animation_keyframe_final);
			}
		}

		void SetAnimationStateKeyframeHandler(const Enums::unit_animation_state state, animation_state_keyframe_handler_t handler) {
			g_animation_state_handlers[state - Enums::_unit_animation_state] = handler;
		}
	};
};
};
#pragma endregion
