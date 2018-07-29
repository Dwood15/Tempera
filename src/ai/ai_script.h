#pragma once

#include <enums/ai_enums.h>
#include "../memory/datum_index.h"
#include "encounters.h"

namespace Yelo::AI {
		struct s_actor_datum;

	using namespace Yelo::Enums;

	struct s_ai_index {
			short encounter_index;
			union {
				sbyte platoon_index;
				sbyte squad_index;
			};
			byte_enum pad0;
			// byte_enum pad1;
		};

		STAT_ASSERT(s_ai_index, 4);

		struct s_ai_index_actor_iterator {
			// these are all absolute, not datum_index
			long encounter_index;
			long squad_index;
			long platoon_index;

			s_encounter_actor_iterator actor_iterator;

			void SetEndHack() {
				encounter_index = squad_index = platoon_index = NONE;
				actor_iterator.SetEndHack();
			}

			bool IsEndHack() const {
				// This should work because encounter_index is initialized to (ai_index & 0xFFFF),
				// meaning valid iterators will always have zeros in the upper 16-bits.
				// However, ai_index_actor_iterator_new does set encounter_index to NONE if the ai_index.type is invalid
				return encounter_index == NONE;
			}
		};

		STAT_ASSERT(s_ai_index_actor_iterator, 0x18);
	};

	namespace Yelo::blam {
		AI::s_ai_index_actor_iterator &__cdecl ai_index_actor_iterator_new(AI::s_ai_index index, _Out_ AI::s_ai_index_actor_iterator &iterator);

		AI::s_actor_datum *__cdecl ai_index_actor_iterator_next(AI::s_ai_index_actor_iterator &iterator);

		// Attaches the specified actor_variant to the unit
		void __cdecl ai_scripting_attach_free(datum_index unit_index, datum_index actor_variant_definition_index);
	};
//
