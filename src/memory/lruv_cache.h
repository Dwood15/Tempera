#pragma once

#include <precompile.h>
#include "datum_index.h"
#include "../cseries/base.h"

namespace Yelo::Memory {
	//////////////////////////////////////////////////////////////////////////
	// Least Recently Used Cache

	typedef void (*lruv_block_delete_proc)(datum_index block_index);

	typedef bool (*lruv_locked_block_proc)(datum_index block_index);

	struct s_lruv_cache {
		tag_string             name;
		lruv_block_delete_proc delete_proc;
		lruv_locked_block_proc locked_block_proc;
		long                   page_count;
		long                   page_size_bits;
		long                   frame_index;
		datum_index            first_block_index;
		datum_index            last_block_index;
		s_data_array           *data;
		tag                    signature;
	};

	struct s_lruv_cache_block {
		UNKNOWN_TYPE(long);
		long        page_count;
		datum_index first_page_index;
		datum_index next_block_index;
		datum_index previous_block_index;
		long        last_used_frame_index;
		UNKNOWN_TYPE(long); // 0x18
	};
}
