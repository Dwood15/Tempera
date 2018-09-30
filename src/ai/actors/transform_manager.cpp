#pragma once

#include "tranform_manager.h"

namespace Yelo::AI::Transform {

#pragma region Transform Sequence

	void
	c_actor_variant_transform_manager::TransformOut(const datum_index unit_index, s_unit_datum *unit_datum, const game_team instigator_team,
																	const TagGroups::actor_variant_transform_out_definition &transform_out_definition,
																	const TagGroups::actor_variant_transform_in_definition &transform_in_definition,
																	s_actor_variant_transform_state &transform_state) const {
		NOT_IMPLEMENTED;
		// Freeze the actor
		// blam::actor_braindead(unit_datum->unit.actor_index, true);
		//
		// // Play the transform-out animation
		// blam::unit_start_user_animation(unit_index, unit_datum->object.animation.definition_index,
		// 										  transform_out_definition.transform_out_anim, false);
		// unit_datum->unit.animation.state = Enums::_unit_animation_state_yelo_unit_transforming;
		//
		// // Attach the transform objects
		// AttachObjects(unit_index, unit_datum, instigator_team, transform_out_definition);
		//
		// if (TEST_FLAG(transform_out_definition.flags, Flags::_actor_variant_transform_out_flags_invicible_during_transform_out_bit)) {
		// 	SET_FLAG(unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, true);
		// }
		//
		// SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit, true);
		//
		// transform_state.m_transform_stage = _transform_stage_transform_out_begun;
	}

	void c_actor_variant_transform_manager::TransformIn(const datum_index unit_index, s_unit_datum *unit_datum,
																		 Yelo::AI::Transform::c_actor_variant_transform_manager::s_actor_variant_transform_state &transform_state) const {

		NOT_IMPLEMENTED;

		// auto &transform_entry          = m_transform_collection->actor_variant_transforms[transform_state.m_transform_entry_index];
		// auto &transform                = transform_entry.transforms[transform_state.m_transform_index];
		// auto &transform_in_definition  = *transform.transform_in_ptr;
		// auto &transform_out_definition = *transform.transform_out_ptr;
		// If the target index is NONE select a random target, otherwise use the existing index (set by script)
		// if (transform_state.m_target_index == NONE) {
		// 	real test_value = Random::GetReal(0.0f, 1.0f);
		//
		// 	real       lower_value = 0.0f;
		// 	for (sbyte index       = 0; index < transform_in_definition.targets.Count; index++) {
		// 		auto game_difficulty = GameState::GameGlobals()->options.difficulty_level;
		//
		// 		auto &target = transform_in_definition.targets[index];
		// 		if ((lower_value + target.selection_chance[game_difficulty]) > test_value - K_REAL_EPSILON) {
		// 			transform_state.m_target_index = index;
		// 			break;
		// 		}
		//
		// 		lower_value += target.selection_chance[game_difficulty];
		// 	}
		// } else {
		// 	transform_state.m_target_index = transform_state.m_target_index;
		// }
		// auto &transform_target = transform_in_definition.targets[transform_state.m_target_index];
		//
		// // Drop weapon if needed
		// if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_drop_weapon)) {
		// 	blam::unit_drop_current_weapon(unit_index, true);
		// }
		//
		// // Delete child actors if needed
		// if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_delete_attached_actors)) {
		// 	Objects::DeleteChildActors(unit_index);
		// }
		//
		// // Create the new unit, or reuse the existing unit if the flag is set and it matches the type in the actor variant tag
		// auto *actor_variant_definition = blam::tag_get<TagGroups::s_actor_variant_definition>(transform_target.actor_variant.tag_index);
		//
		// DestroyAttachments(unit_index, transform_out_definition);
		//
		// bool delete_unit    = false;
		// auto new_unit_index = datum_index::null();
		// if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_try_to_use_existing_unit)
		// 	 && (unit_datum->object.definition_index == actor_variant_definition->unit.tag_index)) {
		// 	new_unit_index = CreateUnitReuse(unit_index, unit_datum, transform_target.team_handling, transform_state.m_instigator_team,
		// 												transform_target.team_override);
		// 	Objects::DetachChildActors(unit_index);
		// } else {
		// 	new_unit_index = CreateUnitNew(unit_index, unit_datum, actor_variant_definition->unit.tag_index, transform_target.team_handling,
		// 											 transform_state.m_instigator_team,
		// 											 transform_target.team_override);
		//
		// 	if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_inherit_seated_units)) {
		// 		auto       &unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(unit_datum->object.definition_index);
		// 		for (short index            = 0; index < unit_definition.unit.seats.Count; index++) {
		// 			auto seated_unit_index = Objects::GetUnitInSeat(unit_index, index);
		// 			if (!seated_unit_index.IsNull()) {
		// 				blam::unit_exit_seat_end(seated_unit_index, false, true, false);
		// 				blam::unit_enter_seat(seated_unit_index, new_unit_index, index);
		// 				blam::unit_animation_set_state(seated_unit_index, Enums::_unit_animation_state_idle);
		// 			}
		// 		}
		// 	}
		//
		// 	delete_unit = true;
		// }
		// auto *new_unit_datum = blam::object_get_and_verify_type<s_unit_datum>(new_unit_index);
		//
		// // Force the base seat animation to "stand", if we don't do this the unit could be "flaming" and a weapon might not be added
		// new_unit_datum->unit.animation.base_seat = Enums::_unit_base_seat_stand;
		//
		// // Creating the units actor sets the units vitality so take a copy
		// real old_unit_health = unit_datum->object.damage.health;
		// real old_unit_shield = unit_datum->object.damage.shield;
		//
		// // Create the units actor
		// CreateUnitActor(new_unit_index, transform_target, unit_datum->unit.actor_index, transform_state.m_instigator_encounter,
		// 					 transform_state.m_instigator_squad);
		//
		// // Freeze the actor during the transform in stage
		// blam::actor_braindead(new_unit_datum->unit.actor_index, true);
		//
		// // Force the unit to ready it's weapon (weapons such as the stock energy sword would instantly detonate otherwise)
		// blam::unit_ready_desired_weapon(new_unit_index, true);
		//
		// // Inherit unit vitality
		// HandleVitality(transform_target.vitality_inheritance, new_unit_datum, old_unit_health, old_unit_shield);
		//
		// // Override unit vitality
		// HandleVitality(transform_target.vitality_override, new_unit_datum, transform_target.health_override,
		// 					transform_target.shield_override);
		//
		// SET_FLAG(new_unit_datum->object.damage.flags, Flags::_object_shield_depleted_bit, new_unit_datum->object.damage.shield <= 0.0f);
		//
		// // Delete the existing unit if necessary
		// if (delete_unit) {
		// 	Objects::DetachChildActors(unit_index);
		//
		// 	blam::actor_delete(unit_datum->unit.actor_index);
		// 	blam::object_delete(unit_index);
		// }
		//
		// // Play the transform-in animation
		// blam::unit_start_user_animation(new_unit_index, new_unit_datum->object.animation.definition_index, transform_target.transform_in_anim,
		// 										  false);
		// new_unit_datum->unit.animation.state = Enums::_unit_animation_state_yelo_unit_transforming;
		//
		// SET_FLAG(new_unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, true);
		// SET_FLAG(new_unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit, true);
		//
		// transform_state.m_unit_index      = new_unit_index.index;
		// transform_state.m_transform_stage = _transform_stage_transform_in_begun;
	}

	void c_actor_variant_transform_manager::TransformEnd(const datum_index unit_index, s_unit_datum *unit_datum) const {

		NOT_IMPLEMENTED;
		// Unfreeze the actor
		// blam::actor_braindead(unit_datum->unit.actor_index, false);
		//
		// SET_FLAG(unit_datum->unit_object.damage.flags, Flags::_object_cannot_take_damage_bit, false);
		// SET_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_in_bit, false);
		//
		// if (unit_datum->unit.animation.state == _unit_animation_state_yelo_unit_transforming) {
		// 	unit_animation_set_state(unit_index, _unit_animation_state_idle);
		// }
	}

#pragma endregion

#pragma region Game State

	void c_actor_variant_transform_manager::LoadActorVariantTransforms() {

		TagGroups::c_tag_iterator iterator(TagGroups::actor_variant_transform_collection_definition::k_group_tag);

		// Get the first collection tag as there should be only one
		auto tag_index = iterator.Next();
		if (tag_index.IsNull()) {
			m_transform_collection = nullptr;
			return;
		}

		m_transform_collection = TagGroups::TagGetUnsafe<TagGroups::actor_variant_transform_collection_definition>(tag_index);

		// Populate the entry pointers
		for (auto &actor_variant_entry : m_transform_collection->actor_variant_transforms) {
			for (auto &transform_entry : actor_variant_entry.transforms) {
				auto *transform_out_ptr = TagGroups::TagGetUnsafe<TagGroups::actor_variant_transform_out_definition>(
					transform_entry.transform_out.tag_index);
				auto *transform_in_ptr  = TagGroups::TagGetUnsafe<TagGroups::actor_variant_transform_in_definition>(
					transform_entry.transform_in.tag_index);

				transform_entry.transform_out_ptr = transform_out_ptr;
				transform_entry.transform_in_ptr  = transform_in_ptr;
			}
		}
	}

	void c_actor_variant_transform_manager::UnloadActorVariantTransforms() {
		m_transform_collection = nullptr;
	}

#pragma endregion


#pragma region Transform Trigger/Update

	void c_actor_variant_transform_manager::UnitDamaged(const datum_index unit_index, const Objects::s_damage_data *damage_data) {
		if (!m_transform_states || !m_transform_collection || !m_transforms_enabled) {
			return;
		}

		if (!UnitIsValid(unit_index)) {
			return;
		}
		auto &unit_datum  = *blam::object_try_and_get_and_verify_type<s_unit_datum>(unit_index);
		s_actor_datum * actor_datum = nullptr; //(*Actors())[unit_datum.unit.actor_index.index];
		NOT_IMPLEMENTED;

		// If the actor variant has no transforms defined, return
		auto transforms_entry_index = FindTransformsEntry(actor_datum->meta.actor_variant_definition_index);
		if (transforms_entry_index == NONE) {
			return;
		}
		auto &transform_entry = m_transform_collection->actor_variant_transforms[transforms_entry_index];

		// Try to find a transform for the damage instigator If no match is found, look for a non-instigator transform
		// Return if no match is found
		bool is_melee        = (unit_datum.unit.damage_result.category == Yelo::Enums::damage_category::_damage_category_melee);
		auto transform_index = FindDamageTransform(transform_entry.transforms, unit_index, damage_data->responsible_unit_index, is_melee);

		if (transform_index == NONE) {
			transform_index = FindDamageTransform(transform_entry.transforms, unit_index, datum_index::null(), is_melee);

			if (transform_index == NONE) {
				return;
			}
		}
		auto &transform = transform_entry.transforms[transform_index];

		// Get the instigators team
		auto *instigator_unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(damage_data->responsible_unit_index);
		auto instigator_team        = (instigator_unit_datum ? instigator_unit_datum->unit_object.owner_team : _game_team_none);

		// Get a free transform state entry, if an entry is unavailable skip the transform
		auto *transform_state = AllocateTransformState(unit_index);
		// if (transform_state) {
		// 	// Start transforming the unit, using the found transform
		// 	TransformOut(unit_index, &unit_datum, instigator_team, *transform.transform_out_ptr, *transform.transform_in_ptr,
		// 					 *transform_state);
		//
		// 	// Store the instigators encounter and squad for later use
		// 	if (instigator_unit_datum) {
		// 		if (!instigator_unit_datum->unit.swarm_actor_index.IsNull()) {
		// 			actor_datum = AI::Actors()[instigator_unit_datum->unit.swarm_actor_index.index];
		// 		} else if (!instigator_unit_datum->unit.actor_index.IsNull()) {
		// 			actor_datum = AI::Actors()[instigator_unit_datum->unit.actor_index.index];
		// 		}
		// 	}
		//
		// 	if (actor_datum) {
		// 		transform_state->m_instigator_encounter = actor_datum->meta.encounter_index;
		// 		transform_state->m_instigator_squad     = actor_datum->meta.squad_index;
		// 	} else {
		// 		transform_state->m_instigator_encounter = NONE;
		// 		transform_state->m_instigator_squad     = NONE;
		// 	}
		//
		// 	transform_state->m_instigator_team       = instigator_team;
		// 	transform_state->m_transform_entry_index = transforms_entry_index;
		// 	transform_state->m_transform_index       = transform_index;
		// 	transform_state->m_target_index          = NONE;
		// }
	}

	void c_actor_variant_transform_manager::UnitUpdate(const datum_index unit_index) {
		if (!m_transform_states || !m_transform_collection) {
			return;
		}

		auto *unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(unit_index);
		if (!unit_datum) {
			return;
		}

		// If the unit was transforming but is now dead or had it's actor removed, stop the transform
		if (TEST_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_out_bit)
			 || TEST_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			if (TEST_FLAG(unit_datum->unit_object.damage.flags, Flags::_object_killed_bit) || unit_datum->unit.actor_index.IsNull()) {
				SET_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_out_bit, false);
				SET_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_in_bit, false);

				auto &transform_state          = *FindTransformState(unit_index);
				auto &transform_entry          = m_transform_collection->actor_variant_transforms[transform_state.m_transform_entry_index];
				auto &transform                = transform_entry.transforms[transform_state.m_transform_index];
				auto &transform_out_definition = *transform.transform_out_ptr;

				if (TEST_FLAG(transform_out_definition.attachment_flags,
								  Flags::_actor_variant_transform_out_attachment_flags_destroy_attachments_on_death)) {
					DestroyAttachments(unit_index, transform_out_definition);
				}

				if (unit_datum->unit.animation.state == _unit_animation_state_yelo_unit_transforming) {
					blam::unit_animation_set_state(unit_index, _unit_animation_state_idle);
				}

				FreeTransformState(unit_index);
				return;
			}
		}

		NOT_IMPLEMENTED;
		if (TEST_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_out_bit)) {
			// If the transform out animation has finished playing start the transform in stage
			auto &transform_state = *FindTransformState(unit_index);
			if (transform_state.m_transform_stage == _transform_stage_transform_out_ended) {
				// Prevent creating a new actor whilst ai is disabled
				if (AI::AIGlobals()->ai_active) {
					TransformIn(unit_index, unit_datum, *FindTransformState(unit_index));
				}
			}
		} else if (TEST_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			// If the transform in animation has finished playing end the transformation
			auto &transform_state = *FindTransformState(unit_index);
			if (transform_state.m_transform_stage == _transform_stage_transform_in_ended) {
				TransformEnd(unit_index, unit_datum);

				// The transform state data is no longer needed
				FreeTransformState(unit_index);
			}
		} else if (UnitIsValid(unit_index)) {
			auto &unit_datum  = *blam::object_try_and_get_and_verify_type<s_unit_datum>(unit_index);
			auto &actor_datum = *Actors()[unit_datum.unit.actor_index.index];

			// If the actor variant has no transforms defined, return
			auto transforms_entry_index = FindTransformsEntry(actor_datum.meta.actor_variant_definition_index);
			if (transforms_entry_index == NONE) {
				return;
			}
			auto &transform_entry = m_transform_collection->actor_variant_transforms[transforms_entry_index];

			// Try to find a transform for the actor action
			// Return if no match is found
			auto transform_index = FindUpdateTransform(transform_entry.transforms, unit_index);
			if (transform_index == NONE) {
				return;
			}
			auto &transform = transform_entry.transforms[transform_index];

			// Get a free transform state entry, if an entry is unavailable skip the transform
			auto *transform_state = AllocateTransformState(actor_datum.meta.unit_index);
			if (transform_state) {
				// Start transforming the unit, using the found transform
				TransformOut(actor_datum.meta.unit_index, &unit_datum, Enums::_game_team_none, *transform.transform_out_ptr,
								 *transform.transform_in_ptr, *transform_state);

				transform_state->m_instigator_encounter  = NONE;
				transform_state->m_instigator_squad      = NONE;
				transform_state->m_instigator_team       = Enums::_game_team_none;
				transform_state->m_transform_entry_index = transforms_entry_index;
				transform_state->m_transform_index       = transform_index;
				transform_state->m_target_index          = NONE;
			}
		}
	}
#pragma endregion

#pragma region Animation

	void c_actor_variant_transform_manager::DoTransformKeyframeAction(const datum_index unit_index,
																							const TagGroups::actor_variant_transform_keyframe_action &action) {
		auto &unit_datum      = *blam::object_get_and_verify_type<s_unit_datum>(unit_index);
		auto &unit_definition = *blam::tag_get(TagGroups::s_unit_definition::, unit_datum.object.definition_index);

		::std::vector<datum_index> riders;
		if ((action.target == Enums::_actor_variant_transform_keyframe_effect_target_riders)
			 || (action.rider_handling != Enums::_actor_variant_transform_keyframe_rider_handling_none)) {
			for (short index = 0; index < unit_definition.unit.seats.Count; index++) {
				auto seated_unit_index = Objects::GetUnitInSeat(unit_index, index);
				if (seated_unit_index.IsNull()) {
					continue;
				}

				riders.push_back(seated_unit_index);
			}
		}

		// Apply damage to self or riders
		if (!action.damage_effect.tag_index.IsNull()) {
			Objects::s_damage_data damage_data;
			blam::damage_data_new(damage_data, action.damage_effect.tag_index);

			SET_FLAG(damage_data.flags, Flags::_damage_data_flags_affect_target_only_bit, true);
			damage_data.responsible_player_index = unit_datum.unit.controlling_player_index;
			damage_data.responsible_unit_index   = unit_index;
			damage_data.responsible_units_team   = unit_datum.unit_object.owner_team;

			switch (action.target) {
				case Enums::_actor_variant_transform_keyframe_effect_target_self: {
					damage_data.location        = unit_datum.unit_object.location;
					damage_data.damage_position = unit_datum.unit_object.position;

					blam::object_cause_damage(damage_data, unit_index);
				}
					break;
				case Enums::_actor_variant_transform_keyframe_effect_target_riders:
					for (auto &rider : riders) {
						auto &rider_unit_datum = *blam::object_get_and_verify_type<s_unit_datum>(rider);

						damage_data.location        = rider_unit_datum.unit_object.location;
						damage_data.damage_position = rider_unit_datum.unit_object.position;

						blam::object_cause_damage(damage_data, rider);
					}
					break;
			}
		}

		// Spawn effect on self or riders
		if (!action.effect.tag_index.IsNull()) {
			switch (action.target) {
				case Enums::_actor_variant_transform_keyframe_effect_target_self:
					blam::hs_effect_new_from_object_marker(action.effect.tag_index, unit_index, action.effect_marker);
					break;
				case Enums::_actor_variant_transform_keyframe_effect_target_riders:
					for (auto &rider : riders) {
						blam::hs_effect_new_from_object_marker(action.effect.tag_index, rider, action.effect_marker);
					}
					break;
			}
		}

		// Handle rider ejection or killing
		if (action.rider_handling != Enums::_actor_variant_transform_keyframe_rider_handling_none) {
			for (auto &rider : riders) {
				switch (action.rider_handling) {
					case Enums::_actor_variant_transform_keyframe_rider_handling_kill:
						blam::unit_kill(rider);
						break;
					case Enums::_actor_variant_transform_keyframe_rider_handling_eject:
						blam::unit_exit_seat_end(rider, false, true, false);
						break;
				}
			}
		}
	}


	void c_actor_variant_transform_manager::TransformingOutKeyframe(const datum_index unit_index, s_unit_datum *unit_datum,
																						 const unit_animation_keyframe keyframe) {
		auto &transform_state = *FindTransformState(unit_index);

		auto &transform_entry          = m_transform_collection->actor_variant_transforms[transform_state.m_transform_entry_index];
		auto &transform                = transform_entry.transforms[transform_state.m_transform_index];
		auto &transform_out_definition = *transform.transform_out_ptr;

		for (auto &action : transform_out_definition.keyframe_actions) {
			if (action.keyframe == keyframe) {
				DoTransformKeyframeAction(unit_index, action);
			}
		}

		if (keyframe == Enums::_unit_animation_keyframe_final) {
			transform_state.m_transform_stage = _transform_stage_transform_out_ended;
		}
	}

	void c_actor_variant_transform_manager::TransformingInKeyframe(const datum_index unit_index, s_unit_datum *unit_datum,
																						const unit_animation_keyframe keyframe) {
		auto &transform_state = *FindTransformState(unit_index);

		auto &transform_entry         = m_transform_collection->actor_variant_transforms[transform_state.m_transform_entry_index];
		auto &transform               = transform_entry.transforms[transform_state.m_transform_index];
		auto &transform_in_definition = *transform.transform_in_ptr;
		auto &target_definition       = transform_in_definition.targets[transform_state.m_target_index];

		for (auto &action : target_definition.keyframe_actions) {
			if (action.keyframe == keyframe) {
				DoTransformKeyframeAction(unit_index, action);
			}
		}

		if (keyframe == Enums::_unit_animation_keyframe_final) {
			transform_state.m_transform_stage = _transform_stage_transform_in_ended;
		}
	}

	void c_actor_variant_transform_manager::TriggerUnitTransformKeyframe(const datum_index unit_index,
																								const unit_animation_keyframe keyframe) {
		if (!m_transform_states || !m_transform_collection) {
			return;
		}

		auto *unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(unit_index);
		if (!unit_datum) {
			return;
		}

		if (TEST_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_out_bit)) {
			TransformingOutKeyframe(unit_index, unit_datum, keyframe);
		} else if (TEST_FLAG(unit_datum->unit_object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			TransformingInKeyframe(unit_index, unit_datum, keyframe);
		}
	}
#pragma endregion

};
