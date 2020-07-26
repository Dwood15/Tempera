#pragma once

#include "macros_generic.h"
#include "enums/hs_enums.h"
#include "enums/generic_enums.h"
#include "enums/hud_enums.h"
#include "enums/ai_enums.h"
#include "../memory/datum_index.h"

namespace Yelo::Scripting {

	union script_idx {
		short script_index;
		short global_index;
	};

	struct s_hs_script_container_datum_index {
		script_idx idx;
		//			short script_container_index;

		bool IsNull() const;

		static s_hs_script_container_datum_index CreateIndex(short index, short script_container_index = 0) {
			return s_hs_script_container_datum_index{{index}};
		}
	};
	STAT_ASSERT(s_hs_script_container_datum_index, 2)


	struct s_hs_value_union {
		union {
			uint bits;
			void      *pointer;

			bool       boolean;
			real       real;
			short      shurt;
			long       lung;
			const char *str;

			s_hs_script_container_datum_index script;
			//			hs_global_index_t global;

			game_difficulty_level            difficulty_level;
			game_team                        team;
			Yelo::Enums::actor_default_state ai_default_state;
			Yelo::Enums::actor_type          actor_type;
			Yelo::Enums::hud_anchor          hud_corner;

			Yelo::datum_index tag_index;
			Yelo::datum_index datum;

			string_id string_id;
		};

		datum_index ToExpressionIndex() const { return datum; }

		datum_index ToTagIndex() const { return datum; }

		datum_index ToObjectIndex() const { return datum; }
	};

	STAT_ASSERT(s_hs_value_union, sizeof(void *));
	static const s_hs_value_union k_null_value_union = {{0}};
	static const s_hs_value_union k_none_value_union = {{0}};

}
