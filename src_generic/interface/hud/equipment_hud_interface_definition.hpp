#pragma once

#include <precompile.h>

namespace Yelo::TagGroups {
	struct equipment_hud_interface_definition {
		enum { k_group_tag = 'eqhi' };

		tag_reference:8 * sizeof(tag_reference) * 1;

		struct {
			unsigned short flags;

			unsigned short : 16;
			short:8 * sizeof(short) * 4;
			long:8 * sizeof(long) * 8;
		} flash_cutoffs;

		s_hud_absolute_placement placement;

	}; //static_assert( sizeof(equipment_hud_interface_definition) == 0x );
};
