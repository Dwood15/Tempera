#pragma once
#include "shitty_macros.h"
#include "shitty_enums.h"

namespace tags {
	struct tag_field {
		field_type type;
		short __pad;
		cstring name;
		void* definition;
	}; STAT_ASSRT(tag_field, 0xC);

	struct tag_block {
		int count;
		void *address;
		//I have no clue what Kornman was trying to do with blockdefinition in Open Sauce, so we'll just use a void* for now.
		void *_unk_blockdefinition;
	};
}