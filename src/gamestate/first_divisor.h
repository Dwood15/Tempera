#pragma once
#include "macros_generic.h"

namespace obj {
	namespace Memory {
		struct s_header_data {
			uint32     allocation_crc;
			char       level[256];
			tag_string version;
			short      player_spawn_count;
			short      game_difficulty;
			uint32     cache_crc;

			//Also known as YELO//Open Saucey territory.
			sbyte _unused[32];

		}; STAT_ASSERT(s_header_data, 0x14C);

		struct s_memory_pool_block {
			typedef void **reference_t;

			long                head;         // 'head'
			long                size;         // size of the block, exclusive of this header, within the pool
			reference_t         reference;      // the pointer referencing the block allocation
			s_memory_pool_block *next;      // linked list: the pool block that follows this one
			s_memory_pool_block *prev;      // linked list: the pool block the comes before this one
			long                tail;         // 'tail'
		};

		struct s_memory_pool {
			long                signature;      // 'head'
			::tag_string        name;         // debug code name for the allocation
			void                *base_address;   // next address to allocate a block at
			long                size;         // total size of the pool
			long                free_size;      // total size left in the pool thats not owned by anything
			s_memory_pool_block *first;      // pointer to the first pool item
			s_memory_pool_block *last;      // pointer to the last pool item
		};
	};

	struct s_objects_pool_data {
		Memory::s_memory_pool header;
		byte                  data[k_object_memory_pool_allocation_size];
	};

};
