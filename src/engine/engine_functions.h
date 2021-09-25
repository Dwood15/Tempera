#include <enums/unit_enums.h>
#include "../math/real_math.h"
#include "../memory/datum_index.h"
#include "../gamestate/objects/objectcontroller.h"
#include "../RuntimeManager.h"

namespace Yelo::blam {
	void unit_animation_set_state(const datum_index unit_index, const unit_animation_state state);
	/*
	 * 0x4CCF76
	 * - E8 75 EF FF FF
	 *
	 * 0x53BF45
	 * - E8 A6 FF F8 FF
	 *
	 * 0x53D018
	 * - E8 D3 EE F8 FF
	 *
	 * 0x53D0B8
	 * - E8 33 EE F8 FF
	 */
	int main_get_current_solo_level_from_name(char level_string[128]);
};
