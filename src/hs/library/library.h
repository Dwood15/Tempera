#pragma once

#include "../cseries/base.h"

namespace Yelo::Enums {
	enum {
		_hs_function_begin,
		_hs_function_begin_random,
		_hs_function_if,
		_hs_function_cond,
		_hs_function_set,
		_hs_function_and,
		_hs_function_or,
		_hs_function_add,
		_hs_function_subtract,
		_hs_function_multiply,
		_hs_function_divide,
		_hs_function_min,
		_hs_function_max,
		_hs_function_equal,
		_hs_function_not_equal,
		_hs_function_gt,
		_hs_function_lt,
		_hs_function_gte,
		_hs_function_lte,
		_hs_function_sleep,
		_hs_function_sleep_until,
		_hs_function_wake,
		_hs_function_inspect,
		_hs_function_object_to_unit,
		_hs_function_ai_debug_communication_suppress,
		_hs_function_ai_debug_communication_ignore,
		_hs_function_ai_debug_communication_focus,

		_hs_function_logical__first = _hs_function_and,
		_hs_function_logical__last  = _hs_function_or,

		_hs_function_arithmetic__first = _hs_function_add,
		_hs_function_arithmetic__last  = _hs_function_max,

		_hs_function_equality__first = _hs_function_equal,
		_hs_function_equality__last  = _hs_function_not_equal,

		_hs_function_inequality__first = _hs_function_gt,
		_hs_function_inequality__last  = _hs_function_lte,

		_hs_function_object_cast_up__first = _hs_function_object_to_unit,
		_hs_function_object_cast_up__last  = _hs_function_object_to_unit,

		_hs_function_debug_string__first = _hs_function_ai_debug_communication_suppress,
		_hs_function_debug_string__last  = _hs_function_ai_debug_communication_focus,
	};
};

namespace Yelo::blam {
	void __cdecl hs_object_orient(datum_index object_index, long cutscene_flag_index, bool teleport, bool set_facing);

	// Spawn an effect tag on the specified object's marker
	void __cdecl hs_effect_new_from_object_marker(datum_index effect_definition_index, datum_index object_index, const char *  marker_name = "");
};
