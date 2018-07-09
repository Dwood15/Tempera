/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/object_lists.hpp>

namespace Yelo
{
	namespace Scripting
	{
		struct s_object_list_header_datum : Memory::s_datum_base_aligned
		{
			short reference_count;	// how many references there are to this list
			short count;			// how many objects are in the list
			datum_index first;		// handle to the first list object reference
		}; static_assert( sizeof(s_object_list_header_datum) == 0xC );

		struct s_list_object_reference_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;	// handle to the object datum
			datum_index next_reference;	// handle to the next list object reference in the list
		}; static_assert( sizeof(s_list_object_reference_datum) == 0xC );
	};
};
