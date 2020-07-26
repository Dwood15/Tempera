/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		struct s_simple_circular_queue
		{
			long max_queue_size;	// 0x0
			uint entry_size;		// 0x4
			void** user_data;		// 0x8, references to user_data structures
			long start;			// 0xC
			long end;				// 0x10
		}; static_assert( sizeof(s_simple_circular_queue) == 0x14 );
	};
};
