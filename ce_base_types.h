/*
	Project: haloforge
	File: addlog.cpp
	Copyright � 2009 SilentK, Abyll
	Copyright � 2018 Dwood15

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "headers/macros_generic.h"
#include "headers/real_math.h"

typedef short        int16;
typedef unsigned int uintptr_t;
typedef char         tag_string[32];
typedef const char* cstring;



/// <summary>	Handle to data allocated by the engine's data-array construct. </summary>
struct datum_index {
	typedef short t_index;
	typedef short t_salt;

	union {
		unsigned long handle;

		struct {
			t_index index;
			t_salt  salt;
		};
	};
}; STAT_ASSRT(datum_index, 0x4);

struct tag_reference {
	unsigned int group_tag;
	const char* name;
	int         name_length;
	datum_index tag_index;
};
STAT_ASSRT(tag_reference, 0x10);
//This is guaranteed to be accurate b/c of stat_assrt macro.
#define TREF_SIZEOF sizeof(tag_reference);

template<typename T>
	struct tag_block {
		typedef T* iter;

		//size -> in bytes.
		long size;
		union {
			void* address;
			T   * definitions;
		};

		long constexpr Count() { return size / sizeof(T); }

		[[maybe_unused]]
		iter begin() { return definitions; }

		iter end() { return definitions + Count( ); }

		struct tag_block_definition* definition;
	};
