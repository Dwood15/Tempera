/* See GPLv3 for licensing information*/
#pragma once
#include <precompile.h>

#include "actors/actor_structures.hpp"

namespace Yelo {
	namespace Enums {
		enum {
			k_maximum_number_of_mounted_weapon_units = 8,
		};

		enum ai_sound_volume {
			k_number_of_ai_sound_volumes = 5,
		};

		enum ai_unit_effect {
			k_number_of_ai_unit_effects = 4,
		};

		enum ai_spatial_effect {
			_ai_spatial_effect_environmental_noise,
			_ai_spatial_effect_weapon_impact,
			_ai_spatial_effect_weapon_detonation,

			k_number_of_ai_spatial_effects
		};
	};

	namespace AI {
		struct s_ai_globals_data;

		s_ai_globals_data *AIGlobals();

		struct s_actor_datum;
		struct s_prop_datum;

		bool __cdecl ActorPropShouldCauseExitVehicle(const s_actor_datum *actor_datum, const s_prop_datum *prop_datum, const bool is_enemy) {
			// If the prop is not an enemy is shouldn't cause the ai to exit
			if (!is_enemy) {
				return false;
			}

			// If the actor isn't in a vehicle...there's no vehicle to exit
			auto *unit_datum = blam::object_get_and_verify_type<Yelo::Objects::s_unit_datum>(actor_datum->meta.unit_index);
			if (unit_datum->object.parent_object_index.IsNull() || unit_datum->unit.vehicle_seat_index == NONE) {
				return false;
			}

			// Don't try and exit the vehicle if there is a unit in a seat that target's the actor's
			auto *seat_extension_definition = Objects::GetSeatExtensionDefinition(unit_datum->object.parent_object_index, unit_datum->unit.vehicle_seat_index);
			if (seat_extension_definition && seat_extension_definition->seat_targeting_seats.Count) {
				for (auto seat_index : seat_extension_definition->seat_targeting_seats) {
					if (Objects::GetUnitInSeat(unit_datum->object.parent_object_index, seat_index) == prop_datum->unit_index) {
						return false;
					}
				}
			}

			return true;
		}

		bool __cdecl ActorShouldIgnoreSeatedProp(const s_prop_datum *prop) {
			auto &actor_datum = *Actors()[prop->owner_actor_index];

			// Return false if the unit is null (will be null if the actor is a swarm actor)
			if (actor_datum.meta.unit_index.IsNull()) {
				return false;
			}

			auto &actor_unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(actor_datum.meta.unit_index);
			auto &prop_unit_datum  = *blam::object_get_and_verify_type<Objects::s_unit_datum>(prop->unit_index);

			// If the prop is not sitting in the same parent, don't ignore it
			if (actor_unit_datum.object.parent_object_index.IsNull() || (prop_unit_datum.unit.vehicle_seat_index == NONE) ||
				 (actor_unit_datum.object.parent_object_index != prop_unit_datum.object.parent_object_index)) {
				return false;
			}

			// If the prop is seated in the same parent, see if the seat should be ignored by other seated AI
			auto *seat_extension_definition = Objects::GetSeatExtensionDefinition(actor_unit_datum.object.parent_object_index, prop_unit_datum.unit.vehicle_seat_index);
			if (!seat_extension_definition) {
				return false;
			}

			auto ignored_by_seated_ai = Flags::_unit_seat_extensions_flags_ignored_by_seated_ai_bit;
			return TEST_FLAG(seat_extension_definition->flags, ignored_by_seated_ai);
		}

		bool __cdecl ActorShouldPanicAboutMountedUnit(const datum_index unit_index) {
			auto &unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);
			if (unit_datum.unit.vehicle_seat_index == NONE) {
				return true;
			}

			auto *seat_extension_definition = Objects::GetSeatExtensionDefinition(unit_datum.object.parent_object_index, unit_datum.unit.vehicle_seat_index);
			if (!seat_extension_definition) {
				return true;
			}

			auto ignored_by_mounted_ai
			Flags::_unit_seat_extensions_flags_ignored_by_mounted_ai_bit
			return !TEST_FLAG(seat_extension_definition->flags, ignored_by_mounted_ai);
		}

	};
};
