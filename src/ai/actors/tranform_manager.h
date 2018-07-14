#pragma once
/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <precompile.h>

#include "../../game/allegiance.h"
#include "../../memory/datum_index.h"
#include "../../game/objects/units/unit_transform_definition.hpp"
#include "../../models/models.hpp"
#include "../../hs/library/library.h"

namespace Yelo::Enums {
	enum game_team : short;
	enum unit_animation_keyframe : short;
};

namespace Yelo::AI::Transform {
	/// <summary>	Manager for actor variant transforms. </summary>
	class c_actor_variant_transform_manager final {
		enum { k_max_concurrent_transforms = 20 };

		enum transform_stage : byte {
			_transform_stage_begin,
			_transform_stage_transform_out_begun,
			_transform_stage_transform_out_ended,
			_transform_stage_transform_in_begun,
			_transform_stage_transform_in_ended,
		};

		struct s_actor_variant_transform_state {
			datum_index::index_t m_unit_index;
			Enums::game_team     m_instigator_team;
			datum_index          m_instigator_encounter;
			short                m_instigator_squad;
			sbyte                m_transform_entry_index;
			sbyte                m_transform_index;
			sbyte                m_target_index;
			transform_stage      m_transform_stage;
		};

		bool m_transforms_enabled;
		unsigned char : 8;
		unsigned short                                           : 16;
		s_actor_variant_transform_state                          *m_transform_states;
		TagGroups::actor_variant_transform_collection_definition *m_transform_collection;

	public:
		/// <summary>	Default constructor. </summary>
		c_actor_variant_transform_manager();

	private:
#pragma region Validation

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns whether a unit is valid for transforming. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		///
		/// <returns>	true if valid, false if not. </returns>
		bool UnitIsValid(const datum_index unit_index) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns whether an actor is valid for transforming. </summary>
		///
		/// <param name="unit_index">	Datum index of the actor. </param>
		///
		/// <returns>	true if valid, false if not. </returns>
		bool ActorIsValid(const datum_index actor_index) const;

#pragma endregion

#pragma region Find Transform

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tests the vitality transform criteria for a unit. </summary>
		///
		/// <param name="unit_index">   	Datum index of the unit. </param>
		/// <param name="transform_out">	The transform out. </param>
		///
		/// <returns>	true if the test passes, false if the test fails. </returns>
		bool TestVitalityCriteria(const datum_index unit_index, const TagGroups::actor_variant_transform_out_definition &transform_out_definition) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a transform using the provided selection function. </summary>
		///
		/// <param name="transformations">	The transformations. </param>
		/// <param name="select_func">	  	The select function. </param>
		///
		/// <returns>	The found transform. </returns>
		sbyte FindTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform> &transformations,
								  std::function<bool(const TagGroups::actor_variant_transform_collection_transform &)> select_func) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Searches for a matching actor variant transform definition based on damage received.
		/// </summary>
		///
		/// <param name="transformations">			[in] The transformations list. </param>
		/// <param name="unit_index">				Datum index of the unit. </param>
		/// <param name="instigator_unit_index">	Datum index of the instigator unit. </param>
		/// <param name="damage_is_melee">			Whether the damage is melee damage. </param>
		///
		/// <returns>	Index of the found transform. Returns NONE if no transform was found. </returns>
		sbyte FindDamageTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform> &transformations, const datum_index unit_index, const datum_index instigator_unit_index,
										  const bool damage_is_melee) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Searches for the first transform matching the criteria for a unit update. </summary>
		///
		/// <param name="transformations">	The transformations. </param>
		/// <param name="unit_index">	  	Datum index of the unit. </param>
		///
		/// <returns>	The found update transform. </returns>
		sbyte FindUpdateTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform> &transformations, const datum_index unit_index) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Searches for the first transforms entry for an actor variant tag. </summary>
		///
		/// <param name="tag_index">	Datum index of the actor variant tag. </param>
		///
		/// <returns>	The found transforms entry. </returns>
		sbyte FindTransformsEntry(const datum_index tag_index) const;

#pragma endregion

#pragma region Tag Options

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Overrides a units health and shield values. </summary>
		///
		/// <param name="operation"> 	The override operation. </param>
		/// <param name="unit_datum">	[in] If non-null, the old unit's datum. </param>
		/// <param name="health">	 	The health value. </param>
		/// <param name="shield">	 	The shield value. </param>
		void HandleVitality(const Enums::actor_variant_transform_in_vitality_handling operation, Objects::s_unit_datum *unit_datum, const real health, const real shield) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Gets a team based on whether it is from the attacked, attacker or an override.
		/// </summary>
		///
		/// <param name="option">			The team handling option. </param>
		/// <param name="attacked_team">	The attacked team. </param>
		/// <param name="attacker_team">	The attacker team. </param>
		/// <param name="override_team">	The override team. </param>
		///
		/// <returns>	The chosen team. </returns>
		Enums::game_team
		HandleTeam(const Yelo::Enums::actor_variant_transform_team_handling option, const Enums::game_team attacked_team, const Enums::game_team attacker_team,
					  const Enums::game_team override_team) const;

#pragma endregion

#pragma region Unit Creation

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates the unit's actor. </summary>
		///
		/// <param name="unit_index">			  	Datum index of the unit. </param>
		/// <param name="target">				  	The transform target definition. </param>
		/// <param name="source_actor_index">	  	Datum index of the source actor. </param>
		/// <param name="instigator_encounter">   	Datum index of the instigators encounter. </param>
		/// <param name="instigator_squad">		  	Index of the instigators squad. </param>
		void CreateUnitActor(const datum_index unit_index, const TagGroups::actor_variant_transform_in_target &target, const datum_index source_actor_index, const datum_index instigator_encounter,
									const short instigator_squad) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new unit reusing the old unit. </summary>
		///
		/// <param name="unit_index">	  	Datum index of the old unit. </param>
		/// <param name="unit_datum">	  	[in] The old unit's datum. </param>
		/// <param name="team_option">	  	The team handling option. </param>
		/// <param name="instigator_team">	The instigator team. </param>
		/// <param name="override_team">  	The override team. </param>
		///
		/// <returns>	The new unit index. </returns>
		datum_index
		CreateUnitReuse(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::actor_variant_transform_team_handling team_option, const Enums::game_team instigator_team,
							 const Enums::game_team override_team) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new unit. </summary>
		///
		/// <param name="unit_index">	  	Datum index of the old unit. </param>
		/// <param name="unit_datum">	  	[in] The old unit's datum. </param>
		/// <param name="new_unit_type">  	Type of the new unit. </param>
		/// <param name="team_option">	  	The team handling option. </param>
		/// <param name="instigator_team">	The instigator team. </param>
		/// <param name="State">		  	The state. </param>
		///
		/// <returns>	The new unit index. </returns>
		datum_index CreateUnitNew(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const datum_index new_unit_type, const Enums::actor_variant_transform_team_handling team_option,
										  const Enums::game_team instigator_team, const Enums::game_team override_team) const;

#pragma endregion

#pragma region Transform State

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Searches for a transform state. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		///
		/// <returns>	null if it fails, else the found transform state. </returns>
		s_actor_variant_transform_state *FindTransformState(const datum_index::index_t unit_index) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Searches for a transform state. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		///
		/// <returns>	null if it fails, else the found transform state. </returns>
		s_actor_variant_transform_state *FindTransformState(const datum_index unit_index) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Allocate a transform state entry. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		///
		/// <returns>	null if it fails, else a s_actor_variant_transform_state*. </returns>
		s_actor_variant_transform_state *AllocateTransformState(const datum_index unit_index);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Frees the transform state described by unit_index. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		void FreeTransformState(const datum_index unit_index);

#pragma endregion

#pragma region Attachments

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Destroys attachments on an object that match those in the transform definition.
		/// </summary>
		///
		/// <param name="unit_index">		   	The unit datum. </param>
		/// <param name="transform_definition">	[in] The transform out definition. </param>
		void DestroyAttachments(const datum_index unit_index, const TagGroups::actor_variant_transform_out_definition &transform_definition) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Attach the transform definition's attachment objects to the target unit at the specific
		/// 	marker permutation.
		/// </summary>
		///
		/// <param name="unit_index">			   	Datum index of the unit. </param>
		/// <param name="unit_datum">			   	The unit datum. </param>
		/// <param name="object_type">			   	The type of object. </param>
		/// <param name="object_marker_name">	   	The object marker to attach at. </param>
		/// <param name="destination_marker_name"> 	The destination marker name. </param>
		/// <param name="destination_marker_index">	The index of the destination marker. </param>
		/// <param name="attachment_team">		   	The attachment's team. </param>
		/// <param name="attachment_scale">		   	The attachment's scale. </param>
		void AttachObject(const datum_index unit_index, const Objects::s_unit_datum *unit_datum, const datum_index object_type, const tag_string &object_marker_name,
								const tag_string &destination_marker_name, const long destination_marker_index, const Enums::game_team attachment_team, const real attachment_scale) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Attach the transform definition's attachment objects to the target unit. </summary>
		///
		/// <param name="unit_index">		   	Datum index of the unit. </param>
		/// <param name="unit_datum">		   	The unit datum. </param>
		/// <param name="instigator_team">	   	The instigator team. </param>
		/// <param name="transform_definition">	[in] The transform out definition. </param>
		void AttachObjects(const datum_index unit_index, const Objects::s_unit_datum *unit_datum, const Enums::game_team instigator_team,
								 const TagGroups::actor_variant_transform_out_definition &transform_definition) const;

#pragma endregion

#pragma region Transform Stages

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Starts the transform out stage on the target unit. </summary>
		///
		/// <param name="unit_index">			   	Datum index of the unit. </param>
		/// <param name="unit_datum">			   	[in] The unit datum. </param>
		/// <param name="instigator_team">		   	The instigator team. </param>
		/// <param name="transform_out_definition">	[in] The transform out definition. </param>
		/// <param name="transform_in_definition"> 	[in] The transform in definition. </param>
		void TransformOut(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::game_team instigator_team,
								const TagGroups::actor_variant_transform_out_definition &transform_out_definition, const TagGroups::actor_variant_transform_in_definition &transform_in_definition,
								s_actor_variant_transform_state &transform_state) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Start's the transform in stage on the target unit. </summary>
		///
		/// <param name="unit_index">	  	Datum index of the unit. </param>
		/// <param name="unit_datum">	  	[in] The unit datum. </param>
		/// <param name="transform_state">	State of the transform. </param>
		void TransformIn(const datum_index unit_index, Objects::s_unit_datum *unit_datum, s_actor_variant_transform_state &transform_state) const;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Ends the transform by making the created actor vulnerable. </summary>
		///
		/// <param name="unit_datum">	[in] If non-null, the unit datum. </param>
		void TransformEnd(const datum_index unit_index, Objects::s_unit_datum *unit_datum) const;

#pragma endregion

#pragma region Animation

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the transform keyframe action. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="action">	 	The action. </param>
		void DoTransformKeyframeAction(const datum_index unit_index, const TagGroups::actor_variant_transform_keyframe_action &action);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Transforming out keyframe handler. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="unit_datum">	[in] If non-null, the unit datum. </param>
		/// <param name="keyframe">  	The keyframe. </param>
		void TransformingOutKeyframe(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::unit_animation_keyframe keyframe);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Transforming in keyframe handler. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="unit_datum">	[in] If non-null, the unit datum. </param>
		/// <param name="keyframe">  	The keyframe. </param>
		void TransformingInKeyframe(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::unit_animation_keyframe keyframe);

#pragma endregion

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets/Sets whether transforms are enabled. </summary>
		///
		/// <returns>	true if enabled, false if not. </returns>
		bool &TransformsEnabled();

#pragma region Transform State

		/// <summary>	Allocates game state memory. </summary>
		void AllocateGameStateMemory();

		/// <summary>	Clears the in progress transforms. </summary>
		void ClearInProgressTransforms();

#pragma endregion

#pragma region Game State

		/// <summary>	Loads the actor variant transform collection tag. </summary>
		void LoadActorVariantTransforms();

		/// <summary>	Unloads the actor variant transform collection tag. </summary>
		void UnloadActorVariantTransforms();

#pragma endregion

#pragma region Transform Trigger/Update

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Attempts to start a transform on a damaged unit if applicable. </summary>
		///
		/// <param name="unit_index"> 	Datum index of the unit. </param>
		/// <param name="damage_data">	Information describing the damage. </param>
		void UnitDamaged(const datum_index unit_index, const Objects::s_damage_data *damage_data);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Unit transform update. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		void UnitUpdate(const datum_index unit_index);

#pragma endregion

#pragma region Animation

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Triggers a unit transform keyframe. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="keyframe">  	The keyframe. </param>
		void TriggerUnitTransformKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe);

#pragma endregion

#pragma region Scripting

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Transforms an actor to the specified transform. </summary>
		///
		/// <param name="unit_index">	Datum index of the unit. </param>
		/// <param name="transform">  	The transform name. </param>
		/// <param name="target">	  	The target name. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool TransformActor(const datum_index unit_index, const char * transform_name, const char * target_name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Transforms a list of actors to the specified transform. </summary>
		///
		/// <param name="unit_index_list">	Datum index of the unit list. </param>
		/// <param name="transform_name"> 	The transform name. </param>
		/// <param name="target_name">	  	The target name. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool TransformActors(const datum_index unit_index_list, const char * transform_name, const char * target_name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Transforms actors in a list of a particular type to the specified transform.
		/// </summary>
		///
		/// <param name="unit_index_list">	Datum index of the unit list. </param>
		/// <param name="tag_index">	  	Datum index of the actor variant tag. </param>
		/// <param name="transform_name"> 	The transform name. </param>
		/// <param name="target_name">	  	The target name. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool TransformActorsByType(const datum_index unit_index_list, const datum_index tag_index, const char * transform_name, const char * target_name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns whether an actor is transforming. </summary>
		///
		/// <param name="unit_index">	Datum index of the actor. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool ActorIsTransforming(const datum_index unit_index);

#pragma endregion
	};
};

namespace Yelo::AI::Transform {
	c_actor_variant_transform_manager::c_actor_variant_transform_manager() : m_transforms_enabled(true), m_transform_states(nullptr) {}

	bool &c_actor_variant_transform_manager::TransformsEnabled() {
		return m_transforms_enabled;
	}

#pragma region Validation

	bool c_actor_variant_transform_manager::UnitIsValid(const datum_index unit_index) const {
		auto *unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		if (!unit_datum) {
			return false;
		}

		// Do not allow transforms whilst in a seat
		if (unit_datum->unit.vehicle_seat_index != NONE) {
			return false;
		}

		// If the unit is dead or already transforming, return
		if (TEST_FLAG(unit_datum->object.damage.flags, Flags::_object_killed_bit)
			 || TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)
			 || TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			return false;
		}

		// Transforms only apply to actors
		if (unit_datum->unit.actor_index.IsNull()) {
			return false;
		}

		auto *actor_datum = AI::Actors()[unit_datum->unit.actor_index];
		if (!actor_datum) {
			return false;
		}

		return true;
	}

	bool c_actor_variant_transform_manager::ActorIsValid(const datum_index actor_index) const {
		if (actor_index.IsNull()) {
			return false;
		}

		auto *actor_datum = Actors()[actor_index];
		if (!actor_datum) {
			return false;
		}

		if (actor_datum->meta.unit_index.IsNull()) {
			return false;
		}

		auto *unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(actor_datum->meta.unit_index);
		if (!unit_datum) {
			return false;
		}

		// Do not allow transforms whilst in a seat
		if (unit_datum->unit.vehicle_seat_index != NONE) {
			return false;
		}

		// If the unit is dead or already transforming, return
		if (TEST_FLAG(unit_datum->object.damage.flags, Flags::_object_killed_bit)
			 || TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)
			 || TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			return false;
		}

		return true;
	}

#pragma endregion

#pragma region Find Transform

	bool c_actor_variant_transform_manager::TestVitalityCriteria(const datum_index unit_index, const TagGroups::actor_variant_transform_out_definition &transform_out_definition) const {
		auto &unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);

		// Test whether the transform is valid based on the unit's vitality
		bool health_within_range = (unit_datum.object.damage.health >= transform_out_definition.health_range.lower) && (unit_datum.object.damage.health <= transform_out_definition.health_range.upper);
		bool shield_within_range = (unit_datum.object.damage.shield >= transform_out_definition.shield_range.lower) && (unit_datum.object.damage.shield <= transform_out_definition.shield_range.upper);

		bool do_transform = true;
		if (TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_health_range_bit)
			 && !health_within_range) {
			do_transform = false;
		}

		if (TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_shield_range_bit)
			 && !shield_within_range) {
			do_transform = false;
		}

		return do_transform;
	}

	sbyte c_actor_variant_transform_manager::FindTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform> &transformations,
																			 std::function<bool(const TagGroups::actor_variant_transform_collection_transform &)> select_func) const {
		// Populate the transform index list
		std::vector<sbyte> transform_indices;
		for (sbyte         index = 0; index < transformations.Count; index++) {
			// Ignore transforms that are scripting only
			if (TEST_FLAG(transformations[index].flags, Flags::_actor_variant_transform_collection_transform_flags_scripted_only)) {
				continue;
			}

			// Randomly decide whether to include this transform
			auto difficulty = GameState::GameGlobals()->options.difficulty_level;

			real random = Random::GetReal(0.0f, 1.0f);
			if (random > transformations[index].selection_chance[difficulty]) {
				continue;
			}

			// Determine whether the inclusion criteria are met
			// Include the transform if they are
			if (select_func(transformations[index])) {
				transform_indices.push_back(index);
			}
		}

		// Pick a random transform index from the transforms list
		if (transform_indices.size()) {
			return transform_indices[Random::GetInt16(0, transform_indices.size() - 1)];
		}

		return NONE;
	}

	sbyte c_actor_variant_transform_manager::FindDamageTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform> &transformations, const datum_index unit_index,
																					 const datum_index instigator_unit_index, const bool damage_is_melee) const {
		auto &unit_datum            = *blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		auto *instigator_unit_datum = (instigator_unit_index.IsNull() ? nullptr : blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(instigator_unit_index));

		auto                                                                                 test_transform = [&](const TagGroups::actor_variant_transform_collection_transform &transform) -> bool {
			auto &transform_out_definition = *transform.transform_out_ptr;

			// If this transform doesn't occur when damaged, return false
			if (!TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_damage_only_bit)) {
				return false;
			}

			// If we should ignore friendly fire, do so
			if (instigator_unit_datum) {
				if (TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_ignore_friendly_fire_bit)
					 && !blam::game_team_is_enemy(unit_datum.object.owner_team, instigator_unit_datum->object.owner_team)) {
					return false;
				}
			}

			auto &unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);

			if (!TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_health_range_bit)
				 && !TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_shield_range_bit)) {
				return false;
			}

			return TestVitalityCriteria(unit_index, transform_out_definition);
		};

		std::function<bool(const TagGroups::actor_variant_transform_collection_transform &)> select_func;
		if (instigator_unit_datum) {
			// If the instigator is not null select entries that match the instigator and damage type's
			select_func = [&](const TagGroups::actor_variant_transform_collection_transform &entry) -> bool {
				if (test_transform(entry)) {
					for (auto &instigator : entry.transform_out_ptr->instigators) {
						// Skip if the damage type does not match
						if (((instigator.damage_type == Enums::_actor_variant_transform_out_damage_type_weapon_only) && damage_is_melee)
							 || ((instigator.damage_type == Enums::_actor_variant_transform_out_damage_type_melee_only) && !damage_is_melee)) {
							return false;
						}

						// Skip if the instigator type does not match
						if (instigator.unit.tag_index == instigator_unit_datum->object.definition_index) {
							return true;
						}
					}
				}
				return false;
			};
		} else {
			// If the instigator is null select entries that have no instigators defined
			select_func = [&](const TagGroups::actor_variant_transform_collection_transform &entry) -> bool {
				return (entry.transform_out_ptr->instigators.Count == 0) && test_transform(entry);
			};
		}

		return FindTransform(transformations, select_func);
	}

	sbyte c_actor_variant_transform_manager::FindUpdateTransform(const TagBlock<TagGroups::actor_variant_transform_collection_transform> &transformations, const datum_index unit_index) const {
		return FindTransform(transformations,
									[&](const TagGroups::actor_variant_transform_collection_transform &transform) -> bool {
										auto &transform_out_definition = *transform.transform_out_ptr;
										auto &unit_datum               = *blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);
										auto &actor_datum              = *Actors()[unit_datum.unit.actor_index];

										// Ignore transforms that are on unit damage only
										if (TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_damage_only_bit)
											 || transform.transform_out_ptr->instigators.Count) {
											return false;
										}

										if (TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_actor_action_bit)
											 && !TEST_FLAG(transform_out_definition.actor_action, actor_datum.state.action)) {
											return false;
										}

										if (TEST_FLAG(transform_out_definition.criteria_flags, Flags::_actor_variant_transform_out_criteria_flags_transform_on_actor_state_bit)
											 && !TEST_FLAG(transform_out_definition.actor_state, actor_datum.current_state)) {
											return false;
										}

										return TestVitalityCriteria(unit_index, transform_out_definition);
									}
		);
	}

	sbyte c_actor_variant_transform_manager::FindTransformsEntry(const datum_index tag_index) const {
		// Find an entry with a matching actor variant tag index
		auto &actor_variant_transforms = m_transform_collection->actor_variant_transforms;
		auto found_entry               = std::find_if(actor_variant_transforms.begin(), actor_variant_transforms.end(),
																	 [&](const TagGroups::actor_variant_transform_collection_entry &entry) {
																		 return entry.actor_variant.tag_index == tag_index;
																	 }
		);

		if (found_entry == actor_variant_transforms.end()) {
			return NONE;
		}

		return found_entry - actor_variant_transforms.begin();
	}

#pragma endregion

#pragma region Tag Options

	void c_actor_variant_transform_manager::HandleVitality(const Enums::actor_variant_transform_in_vitality_handling operation, Objects::s_unit_datum *unit_datum, const real health,
																			 const real shield) const {
		switch (operation) {
			case Enums::_actor_variant_transform_in_vitality_handling_both:
				unit_datum->object.damage.health = health;
				unit_datum->object.damage.shield = shield;
				break;
			case Enums::_actor_variant_transform_in_vitality_handling_shield_only:
				unit_datum->object.damage.shield = shield;
				break;
			case Enums::_actor_variant_transform_in_vitality_handling_health_only:
				unit_datum->object.damage.health = health;
				break;
		}
	}

	Enums::game_team c_actor_variant_transform_manager::HandleTeam(const Enums::actor_variant_transform_team_handling option, const Enums::game_team attacked_team, const Enums::game_team attacker_team,
																						const Enums::game_team override_team) const {
		switch (option) {
			case Enums::_actor_variant_transform_team_handling_inherit_from_old_actor:
				return attacked_team;
			case Enums::_actor_variant_transform_team_handling_inherit_from_attacker:
				return attacker_team;
			case Enums::_actor_variant_transform_team_handling_override:
				return override_team;
		}

		return Enums::_game_team_none;
	}

#pragma endregion

#pragma region Transform State

	c_actor_variant_transform_manager::s_actor_variant_transform_state *c_actor_variant_transform_manager::FindTransformState(const datum_index::index_t unit_index) const {
		// Find the first entry with a matching unit index
		auto entry = std::find_if(m_transform_states, &m_transform_states[k_max_concurrent_transforms],
										  [&](const s_actor_variant_transform_state &entry) -> bool {
											  return entry.m_unit_index == unit_index;
										  }
		);

		if (entry != &m_transform_states[k_max_concurrent_transforms]) {
			return entry;
		}

		return nullptr;
	}

	c_actor_variant_transform_manager::s_actor_variant_transform_state *c_actor_variant_transform_manager::FindTransformState(const datum_index unit_index) const {
		return FindTransformState(unit_index.index);
	}

	c_actor_variant_transform_manager::s_actor_variant_transform_state *c_actor_variant_transform_manager::AllocateTransformState(const datum_index unit_index) {
		// Find the first free state entry
		auto entry = FindTransformState(NONE);
		if (entry) {
			entry->m_unit_index      = unit_index.index;
			entry->m_transform_stage = _transform_stage_begin;
			return entry;
		}

		return nullptr;
	}

	void c_actor_variant_transform_manager::FreeTransformState(const datum_index unit_index) {
		// Find a matching state and NONE its memory to free it
		auto entry = FindTransformState(unit_index);
		if (entry) {
			memset(entry, NONE, sizeof(s_actor_variant_transform_state));
		}
	}

	void c_actor_variant_transform_manager::AllocateGameStateMemory() {
		if (!GameState::YeloGameStateEnabled()) {
			return;
		}

		m_transform_states = GameState::GameStateMalloc<s_actor_variant_transform_state>(true, k_max_concurrent_transforms);

		YELO_ASSERT_DISPLAY(m_transform_states, "Failed to allocate actor transform gamestate memory");

		ClearInProgressTransforms();
	}

	void c_actor_variant_transform_manager::ClearInProgressTransforms() {
		if (m_transform_states) {
			memset(m_transform_states, 0xFF, sizeof(s_actor_variant_transform_state) * k_max_concurrent_transforms);
		}
	}

#pragma endregion

#pragma region Attachments

	void c_actor_variant_transform_manager::DestroyAttachments(const datum_index unit_index, const TagGroups::actor_variant_transform_out_definition &transform_definition) const {
		// Delete all of the units children that match the types in the attachments list
		for (auto &attachment : transform_definition.attachments) {
			Yelo::Objects::DestroyChildrenByDefinition(unit_index, attachment.object_type.tag_index);
		}
	}

	void c_actor_variant_transform_manager::AttachObject(const datum_index unit_index, const Objects::s_unit_datum *unit_datum, const datum_index object_type, const tag_string &object_marker_name,
																		  const tag_string &destination_marker_name, const long destination_marker_index, const Enums::game_team attachment_team,
																		  const real attachment_scale) const {
		// If there is only 1 marker do not append a marker index to the name
		const char * destination_marker = destination_marker_name;

		tag_string destination_marker_buffer;
		if (destination_marker_index != NONE) {
			sprintf_s(destination_marker_buffer, "%s%d", destination_marker_name, destination_marker_index);

			destination_marker = destination_marker_buffer;
		}

		// If the object has no model or the target marker does not exist, skip the attachment
		auto *object_definition = blam::tag_get<TagGroups::s_object_definition>(unit_datum->object.definition_index);
		if (object_definition->object.references.render_model.tag_index.IsNull()) {
			return;
		}

		if (blam::model_find_marker(object_definition->object.references.render_model.tag_index, destination_marker) == NONE) {
			return;
		}

		// Set up the placement data to match the target object's
		Objects::s_object_placement_data attachment_placement_data;
		blam::object_placement_data_new(attachment_placement_data, object_type);

		attachment_placement_data.position   = unit_datum->object.position;
		attachment_placement_data.owner_team = attachment_team;

		// Create the object
		datum_index attachment_object;
		attachment_object = blam::object_new(attachment_placement_data);

		// Scale the attachment as required
		auto *attachment_datum = blam::object_get(attachment_object);
		attachment_datum->scale = attachment_scale;

		// Attach the object
		blam::object_attach_to_marker(unit_index, destination_marker_name, attachment_object, object_marker_name);
	}

	void c_actor_variant_transform_manager::AttachObjects(const datum_index unit_index, const Objects::s_unit_datum *unit_datum, const Enums::game_team instigator_team,
																			const TagGroups::actor_variant_transform_out_definition &transform_definition) const {
		for (auto &attachment : transform_definition.attachments) {
			// Get the attachments team and random scale
			Enums::game_team attachment_team = HandleTeam(attachment.team_handling, unit_datum->object.owner_team, instigator_team, attachment.team_override);

			real attachment_scale = Yelo::Random::GetReal(attachment.attachment_scale);

			// Create attachment instances for all markers
			if (attachment.destination_marker_count == 1) {
				AttachObject(unit_index, unit_datum, attachment.object_type.tag_index, attachment.object_marker, attachment.destination_marker, NONE, attachment_team, attachment_scale);
			} else {
				for (long marker_index = 0; marker_index < attachment.destination_marker_count; marker_index++) {
					AttachObject(unit_index, unit_datum, attachment.object_type.tag_index, attachment.object_marker, attachment.destination_marker, marker_index, attachment_team, attachment_scale);
				}
			}
		}
	}

#pragma endregion

#pragma region Unit Creation

	void c_actor_variant_transform_manager::CreateUnitActor(const datum_index unit_index, const TagGroups::actor_variant_transform_in_target &target, const datum_index source_actor_index,
																			  const datum_index instigator_encounter, const short instigator_squad) const {
		// Set up the unit's inventory
		blam::actor_customize_unit(target.actor_variant, unit_index);

		// Inherit the encounter and squad if desired
		datum_index encounter_index = datum_index::null();
		short       squad_index     = NONE;

		const auto actor_data = Yelo::AI::Actors()[source_actor_index];

		switch (target.encounter_squad_handling) {
			case Enums::_actor_variant_transform_in_encounter_squad_handling_inherit_from_old_actor:
				encounter_index = actor_data->meta.encounter_index.index;
				squad_index     = actor_data->meta.squad_index;
				break;
			case Enums::_actor_variant_transform_in_encounter_squad_handling_inherit_from_attacker:
				encounter_index = instigator_encounter;
				squad_index     = instigator_squad;
				break;
		}

		// Get the intended actor states
		Enums::actor_default_state initial_state = Enums::_actor_default_state_none;
		switch (target.initial_state_handling) {
			case Enums::_actor_variant_transform_in_actor_state_handling_inherit:
				initial_state = actor_data->initial_state;
				break;
			case Enums::_actor_variant_transform_in_actor_state_handling_override:
				initial_state = target.initial_state_override;
				break;
		}

		// 0xFFFF triggers getting the actor default in actor_create_for_unit
		Enums::actor_default_state return_state = (Enums::actor_default_state) 0xFFFF;
		switch (target.return_state_handling) {
			case Enums::_actor_variant_transform_in_actor_state_handling_inherit:
				return_state = actor_data->return_state;
				break;
			case Enums::_actor_variant_transform_in_actor_state_handling_override:
				return_state = target.return_state_override;
				break;
		}

		// Create the actor
		blam::actor_create_for_unit(false, unit_index, target.actor_variant, encounter_index, squad_index, 0, NONE, false, initial_state, return_state, NONE, 0);
	}

	datum_index c_actor_variant_transform_manager::CreateUnitReuse(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::actor_variant_transform_team_handling team_option,
																						const Enums::game_team instigator_team, const Enums::game_team override_team) const {
		SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit, false);

		// Set the existing unit's team
		unit_datum->object.owner_team = HandleTeam(team_option, unit_datum->object.owner_team, instigator_team, override_team);

		return unit_index;
	}

	datum_index c_actor_variant_transform_manager::CreateUnitNew(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const datum_index new_unit_type,
																					 const Enums::actor_variant_transform_team_handling team_option, const Enums::game_team instigator_team,
																					 const Enums::game_team override_team) const {
		// Create the new unit based on the target_unit's data
		Objects::s_object_placement_data unit_placement_data;
		PlacementDataNewAndCopy(unit_placement_data, unit_index, new_unit_type);

		// Set the to-be-created unit's team
		unit_placement_data.owner_team = HandleTeam(team_option, unit_datum->object.owner_team, instigator_team, override_team);

		return blam::object_new(unit_placement_data);
	}

#pragma endregion

#pragma region Transform Sequence

	void c_actor_variant_transform_manager::TransformOut(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::game_team instigator_team,
																		  const TagGroups::actor_variant_transform_out_definition &transform_out_definition,
																		  const TagGroups::actor_variant_transform_in_definition &transform_in_definition, s_actor_variant_transform_state &transform_state) const {
		// Freeze the actor
		blam::actor_braindead(unit_datum->unit.actor_index, true);

		// Play the transform-out animation
		blam::unit_start_user_animation(unit_index, unit_datum->object.animation.definition_index, transform_out_definition.transform_out_anim, false);
		unit_datum->unit.animation.state = Enums::_unit_animation_state_yelo_unit_transforming;

		// Attach the transform objects
		AttachObjects(unit_index, unit_datum, instigator_team, transform_out_definition);

		if (TEST_FLAG(transform_out_definition.flags, Flags::_actor_variant_transform_out_flags_invicible_during_transform_out_bit)) {
			SET_FLAG(unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, true);
		}

		SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit, true);

		transform_state.m_transform_stage = _transform_stage_transform_out_begun;
	}

	void c_actor_variant_transform_manager::TransformIn(const datum_index unit_index, Objects::s_unit_datum *unit_datum, s_actor_variant_transform_state &transform_state) const {
		auto &transform_entry          = m_transform_collection->actor_variant_transforms[transform_state.m_transform_entry_index];
		auto &transform                = transform_entry.transforms[transform_state.m_transform_index];
		auto &transform_in_definition  = *transform.transform_in_ptr;
		auto &transform_out_definition = *transform.transform_out_ptr;

		// If the target index is NONE select a random target, otherwise use the existing index (set by script)
		if (transform_state.m_target_index == NONE) {
			real test_value = Random::GetReal(0.0f, 1.0f);

			real       lower_value = 0.0f;
			for (sbyte index       = 0; index < transform_in_definition.targets.Count; index++) {
				auto game_difficulty = GameState::GameGlobals()->options.difficulty_level;

				auto &target = transform_in_definition.targets[index];
				if ((lower_value + target.selection_chance[game_difficulty]) > test_value - K_REAL_EPSILON) {
					transform_state.m_target_index = index;
					break;
				}

				lower_value += target.selection_chance[game_difficulty];
			}
		} else {
			transform_state.m_target_index = transform_state.m_target_index;
		}
		auto &transform_target = transform_in_definition.targets[transform_state.m_target_index];

		// Drop weapon if needed
		if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_drop_weapon)) {
			blam::unit_drop_current_weapon(unit_index, true);
		}

		// Delete child actors if needed
		if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_delete_attached_actors)) {
			Objects::DeleteChildActors(unit_index);
		}

		// Create the new unit, or reuse the existing unit if the flag is set and it matches the type in the actor variant tag
		auto *actor_variant_definition = blam::tag_get<TagGroups::s_actor_variant_definition>(transform_target.actor_variant.tag_index);

		DestroyAttachments(unit_index, transform_out_definition);

		bool delete_unit    = false;
		auto new_unit_index = datum_index::null();
		if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_try_to_use_existing_unit)
			 && (unit_datum->object.definition_index == actor_variant_definition->unit.tag_index)) {
			new_unit_index = CreateUnitReuse(unit_index, unit_datum, transform_target.team_handling, transform_state.m_instigator_team, transform_target.team_override);
			Objects::DetachChildActors(unit_index);
		} else {
			new_unit_index = CreateUnitNew(unit_index, unit_datum, actor_variant_definition->unit.tag_index, transform_target.team_handling, transform_state.m_instigator_team,
													 transform_target.team_override);

			if (TEST_FLAG(transform_target.flags, Flags::_actor_variant_transform_in_target_flags_inherit_seated_units)) {
				auto       &unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(unit_datum->object.definition_index);
				for (short index            = 0; index < unit_definition.unit.seats.Count; index++) {
					auto seated_unit_index = Objects::GetUnitInSeat(unit_index, index);
					if (!seated_unit_index.IsNull()) {
						blam::unit_exit_seat_end(seated_unit_index, false, true, false);
						blam::unit_enter_seat(seated_unit_index, new_unit_index, index);
						blam::unit_animation_set_state(seated_unit_index, Enums::_unit_animation_state_idle);
					}
				}
			}

			delete_unit = true;
		}
		auto *new_unit_datum = blam::object_get_and_verify_type<Objects::s_unit_datum>(new_unit_index);

		// Force the base seat animation to "stand", if we don't do this the unit could be "flaming" and a weapon might not be added
		new_unit_datum->unit.animation.base_seat = Enums::_unit_base_seat_stand;

		// Creating the units actor sets the units vitality so take a copy
		real old_unit_health = unit_datum->object.damage.health;
		real old_unit_shield = unit_datum->object.damage.shield;

		// Create the units actor
		CreateUnitActor(new_unit_index, transform_target, unit_datum->unit.actor_index, transform_state.m_instigator_encounter, transform_state.m_instigator_squad);

		// Freeze the actor during the transform in stage
		blam::actor_braindead(new_unit_datum->unit.actor_index, true);

		// Force the unit to ready it's weapon (weapons such as the stock energy sword would instantly detonate otherwise)
		blam::unit_ready_desired_weapon(new_unit_index, true);

		// Inherit unit vitality
		HandleVitality(transform_target.vitality_inheritance, new_unit_datum, old_unit_health, old_unit_shield);

		// Override unit vitality
		HandleVitality(transform_target.vitality_override, new_unit_datum, transform_target.health_override, transform_target.shield_override);

		SET_FLAG(new_unit_datum->object.damage.flags, Flags::_object_shield_depleted_bit, new_unit_datum->object.damage.shield <= 0.0f);

		// Delete the existing unit if necessary
		if (delete_unit) {
			Objects::DetachChildActors(unit_index);

			blam::actor_delete(unit_datum->unit.actor_index);
			blam::object_delete(unit_index);
		}

		// Play the transform-in animation
		blam::unit_start_user_animation(new_unit_index, new_unit_datum->object.animation.definition_index, transform_target.transform_in_anim, false);
		new_unit_datum->unit.animation.state = Enums::_unit_animation_state_yelo_unit_transforming;

		SET_FLAG(new_unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, true);
		SET_FLAG(new_unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit, true);

		transform_state.m_unit_index      = new_unit_index.index;
		transform_state.m_transform_stage = _transform_stage_transform_in_begun;
	}

	void c_actor_variant_transform_manager::TransformEnd(const datum_index unit_index, Objects::s_unit_datum *unit_datum) const {
		// Unfreeze the actor
		blam::actor_braindead(unit_datum->unit.actor_index, false);

		SET_FLAG(unit_datum->object.damage.flags, Flags::_object_cannot_take_damage_bit, false);
		SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit, false);

		if (unit_datum->unit.animation.state == Enums::_unit_animation_state_yelo_unit_transforming) {
			blam::unit_animation_set_state(unit_index, Enums::_unit_animation_state_idle);
		}
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

		m_transform_collection = TagGroups::TagGetForModify<TagGroups::actor_variant_transform_collection_definition>(tag_index);

		// Populate the entry pointers
		for (auto &actor_variant_entry : m_transform_collection->actor_variant_transforms) {
			for (auto &transform_entry : actor_variant_entry.transforms) {
				auto *transform_out_ptr = TagGroups::TagGetForModify<TagGroups::actor_variant_transform_out_definition>(transform_entry.transform_out.tag_index);
				auto *transform_in_ptr  = TagGroups::TagGetForModify<TagGroups::actor_variant_transform_in_definition>(transform_entry.transform_in.tag_index);

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
		auto &unit_datum  = *blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		auto &actor_datum = *Actors()[unit_datum.unit.actor_index];

		// If the actor variant has no transforms defined, return
		auto transforms_entry_index = FindTransformsEntry(actor_datum.meta.actor_variant_definition_index);
		if (transforms_entry_index == NONE) {
			return;
		}
		auto &transform_entry = m_transform_collection->actor_variant_transforms[transforms_entry_index];

		// Try to find a transform for the damage instigator If no match is found, look for a non-instigator transform
		// Return if no match is found
		bool is_melee        = (unit_datum.unit.damage_result.category == Enums::_damage_category_melee);
		auto transform_index = FindDamageTransform(transform_entry.transforms, unit_index, damage_data->responsible_unit_index, is_melee);

		if (transform_index == NONE) {
			transform_index = FindDamageTransform(transform_entry.transforms, unit_index, datum_index::null, is_melee);

			if (transform_index == NONE) {
				return;
			}
		}
		auto &transform = transform_entry.transforms[transform_index];

		// Get the instigators team
		auto *instigator_unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(damage_data->responsible_unit_index);
		auto instigator_team        = (instigator_unit_datum ? instigator_unit_datum->object.owner_team : Enums::_game_team_none);

		// Get a free transform state entry, if an entry is unavailable skip the transform
		auto *transform_state = AllocateTransformState(unit_index);
		if (transform_state) {
			// Start transforming the unit, using the found transform
			TransformOut(unit_index, &unit_datum, instigator_team, *transform.transform_out_ptr, *transform.transform_in_ptr, *transform_state);

			// Store the instigators encounter and squad for later use
			s_actor_datum *actor_datum = nullptr;
			if (instigator_unit_datum) {
				if (!instigator_unit_datum->unit.swarm_actor_index.IsNull()) {
					actor_datum = AI::Actors()[instigator_unit_datum->unit.swarm_actor_index];
				} else if (!instigator_unit_datum->unit.actor_index.IsNull()) {
					actor_datum = AI::Actors()[instigator_unit_datum->unit.actor_index];
				}
			}

			if (actor_datum) {
				transform_state->m_instigator_encounter = actor_datum->meta.encounter_index;
				transform_state->m_instigator_squad     = actor_datum->meta.squad_index;
			} else {
				transform_state->m_instigator_encounter = NONE;
				transform_state->m_instigator_squad     = NONE;
			}

			transform_state->m_instigator_team       = instigator_team;
			transform_state->m_transform_entry_index = transforms_entry_index;
			transform_state->m_transform_index       = transform_index;
			transform_state->m_target_index          = NONE;
		}
	}

	void c_actor_variant_transform_manager::UnitUpdate(const datum_index unit_index) {
		if (!m_transform_states || !m_transform_collection) {
			return;
		}

		auto *unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		if (!unit_datum) {
			return;
		}

		// If the unit was transforming but is now dead or had it's actor removed, stop the transform
		if (TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)
			 || TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			if (TEST_FLAG(unit_datum->object.damage.flags, Flags::_object_killed_bit) || unit_datum->unit.actor_index.IsNull()) {
				SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit, false);
				SET_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit, false);

				auto &transform_state          = *FindTransformState(unit_index);
				auto &transform_entry          = m_transform_collection->actor_variant_transforms[transform_state.m_transform_entry_index];
				auto &transform                = transform_entry.transforms[transform_state.m_transform_index];
				auto &transform_out_definition = *transform.transform_out_ptr;

				if (TEST_FLAG(transform_out_definition.attachment_flags, Flags::_actor_variant_transform_out_attachment_flags_destroy_attachments_on_death)) {
					DestroyAttachments(unit_index, transform_out_definition);
				}

				if (unit_datum->unit.animation.state == Enums::_unit_animation_state_yelo_unit_transforming) {
					blam::unit_animation_set_state(unit_index, Enums::_unit_animation_state_idle);
				}

				FreeTransformState(unit_index);
				return;
			}
		}

		if (TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)) {
			// If the transform out animation has finished playing start the transform in stage
			auto &transform_state = *FindTransformState(unit_index);
			if (transform_state.m_transform_stage == _transform_stage_transform_out_ended) {
				// Prevent creating a new actor whilst ai is disabled
				if (AI::AIGlobals()->ai_active) {
					TransformIn(unit_index, unit_datum, *FindTransformState(unit_index));
				}
			}
		} else if (TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			// If the transform in animation has finished playing end the transformation
			auto &transform_state = *FindTransformState(unit_index);
			if (transform_state.m_transform_stage == _transform_stage_transform_in_ended) {
				TransformEnd(unit_index, unit_datum);

				// The transform state data is no longer needed
				FreeTransformState(unit_index);
			}
		} else if (UnitIsValid(unit_index)) {
			auto &unit_datum  = *blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
			auto &actor_datum = *Actors()[unit_datum.unit.actor_index];

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
				TransformOut(actor_datum.meta.unit_index, &unit_datum, Enums::_game_team_none, *transform.transform_out_ptr, *transform.transform_in_ptr, *transform_state);

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

	void c_actor_variant_transform_manager::DoTransformKeyframeAction(const datum_index unit_index, const TagGroups::actor_variant_transform_keyframe_action &action) {
		auto &unit_datum      = *blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		auto &unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(unit_datum.object.definition_index);

		std::vector<datum_index> riders;
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
			damage_data.responsible_units_team   = unit_datum.object.owner_team;

			switch (action.target) {
				case Enums::_actor_variant_transform_keyframe_effect_target_self: {
					damage_data.location        = unit_datum.object.location;
					damage_data.damage_position = unit_datum.object.position;

					blam::object_cause_damage(damage_data, unit_index);
				}
					break;
				case Enums::_actor_variant_transform_keyframe_effect_target_riders:
					for (auto &rider : riders) {
						auto &rider_unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(rider);

						damage_data.location        = rider_unit_datum.object.location;
						damage_data.damage_position = rider_unit_datum.object.position;

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

	void c_actor_variant_transform_manager::TransformingOutKeyframe(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::unit_animation_keyframe keyframe) {
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

	void c_actor_variant_transform_manager::TransformingInKeyframe(const datum_index unit_index, Objects::s_unit_datum *unit_datum, const Enums::unit_animation_keyframe keyframe) {
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

	void c_actor_variant_transform_manager::TriggerUnitTransformKeyframe(const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) {
		if (!m_transform_states || !m_transform_collection) {
			return;
		}

		auto *unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		if (!unit_datum) {
			return;
		}

		if (TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)) {
			TransformingOutKeyframe(unit_index, unit_datum, keyframe);
		} else if (TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			TransformingInKeyframe(unit_index, unit_datum, keyframe);
		}
	}

#pragma endregion

#pragma region Scripting

	bool c_actor_variant_transform_manager::TransformActor(const datum_index unit_index, const char * transform_name, const char * target_name) {
		if (!m_transform_states) {
			return false;
		}

		if (unit_index.IsNull()) {
			return false;
		}

		auto *unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		if (!unit_datum) {
			return false;
		}

		// Cannot overlap transforms
		if (TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)
			 || TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
			return false;
		}

		// Transforms only apply to actors
		if (unit_datum->unit.actor_index.IsNull()) {
			return false;
		}
		auto *actor_datum = AI::Actors()[unit_datum->unit.actor_index];

		// If the actor variant has no transforms defined, return false
		auto transform_entry_index = FindTransformsEntry(actor_datum->meta.actor_variant_definition_index);
		if (transform_entry_index == NONE) {
			return false;
		}
		auto &transform_entry = m_transform_collection->actor_variant_transforms[transform_entry_index];

		// If the transform name is empty get a random non-instigator transform, otherwise find a matching transform
		sbyte transform_index = NONE;
		if (!is_null_or_empty(transform_name)) {
			auto found_transform = std::find_if(transform_entry.transforms.begin(), transform_entry.transforms.end(),
															[&](const TagGroups::actor_variant_transform_collection_transform &entry) {
																return strcmp(entry.transform_name, transform_name) == 0;
															}
			);

			if (found_transform == transform_entry.transforms.end()) {
				return false;
			}

			transform_index = found_transform - transform_entry.transforms.begin();
		} else {
			// Find a random non-instigator transform
			std::vector<sbyte> transform_indices;
			for (sbyte         index = 0; index < transform_entry.transforms.Count; index++) {
				if (transform_entry.transforms[index].transform_out_ptr->instigators.Count == 0) {
					transform_indices.push_back(index);
				}
			}

			// Pick a random transform from the transforms list
			if (transform_indices.size() == 0) {
				return false;
			}

			transform_index = transform_indices[Random::GetInt16(0, transform_indices.size() - 1)];
		}
		auto &transform = transform_entry.transforms[transform_index];

		// If the target name is null, use a random target, otherwise look for a matching target name
		// Setting the target index to NONE will cause a random target selection at the start of the transform in stage
		sbyte target_index = NONE;
		if (!is_null_or_empty(target_name)) {
			auto &transform_in = *transform.transform_in_ptr;
			auto found_target  = std::find_if(transform_in.targets.begin(), transform_in.targets.end(),
														 [&](const TagGroups::actor_variant_transform_in_target &entry) {
															 return strcmp(entry.target_name, target_name) == 0;
														 }
			);

			if (found_target == transform_in.targets.end()) {
				return false;
			}

			target_index = found_target - transform_in.targets.end();
		}

		// Try to allocate a transform state entry, return false if an entry is not available
		auto *transform_state = AllocateTransformState(unit_index);
		if (!transform_state) {
			return false;
		}

		// Start transforming the unit, using the found transform
		TransformOut(unit_index, unit_datum, Enums::_game_team_none, *transform.transform_out_ptr, *transform.transform_in_ptr, *transform_state);

		transform_state->m_instigator_encounter  = NONE;
		transform_state->m_instigator_squad      = NONE;
		transform_state->m_instigator_team       = Enums::_game_team_none;
		transform_state->m_transform_entry_index = transform_entry_index;
		transform_state->m_transform_index       = transform_index;
		transform_state->m_target_index          = target_index;

		return true;
	}

	bool c_actor_variant_transform_manager::TransformActors(const datum_index unit_index_list, const char * transform_name, const char * target_name) {
		bool success                                  = false;

		// Try to transform all objects in the list, return true if any objects are transformed
		for (datum_index list_reference, current_unit = blam::object_list_get_first(unit_index_list, list_reference);
			  !current_unit.IsNull();
			  current_unit = blam::object_list_get_next(unit_index_list, list_reference)) {
			success |= TransformActor(current_unit, transform_name, target_name);
		}

		return success;
	}

	bool c_actor_variant_transform_manager::TransformActorsByType(const datum_index unit_index_list, const datum_index tag_index, const char * transform_name, const char * target_name) {
		if (tag_index.IsNull()) {
			return false;
		}

		bool success                                  = false;

		// Try to transform all objects in the list, return true if any objects are transformed
		for (datum_index list_reference, current_unit = blam::object_list_get_first(unit_index_list, list_reference);
			  !current_unit.IsNull();
			  current_unit = blam::object_list_get_next(unit_index_list, list_reference)) {
			// Only transform actors with a matching actor variant type
			auto *unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(current_unit);
			if (unit_datum && !unit_datum->unit.actor_index.IsNull()) {
				auto *actor_datum = AI::Actors()[unit_datum->unit.actor_index];
				if (actor_datum->meta.actor_variant_definition_index != tag_index) {
					continue;
				}

				success |= TransformActor(current_unit, transform_name, target_name);
			}
		}

		return success;
	}

	bool c_actor_variant_transform_manager::ActorIsTransforming(const datum_index unit_index) {
		auto *unit_datum = blam::object_try_and_get_and_verify_type<Objects::s_unit_datum>(unit_index);
		if (unit_datum) {
			if (TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_out_bit)
				 || TEST_FLAG(unit_datum->object.flags, Flags::_object_yelo_is_transforming_in_bit)) {
				return true;
			}
		}

		return false;
	}

#pragma endregion
};
