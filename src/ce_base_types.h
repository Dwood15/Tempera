/*
	Project: tempera
   You should have received a copy of the GNU General Public License
   along with tempera. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <macros_generic.h>

typedef short                     int16;
typedef unsigned int              uintptr_t;
typedef unsigned long 				 tag;
typedef char                      tag_string[32];
typedef const char                *cstring;

/// <summary>	Handle to data allocated by the engine's data-array construct. </summary>
struct datum_index {


	union {
		signed long handle;

		struct {
			short index;
			short salt;
		};
	};

	//operator int() { return handle; }

	datum_index(int rhs) {
		this->handle = rhs;
	}

};

STAT_ASSRT(datum_index, 0x4);

struct tag_reference {
	unsigned int group_tag;
	const char   *name;
	int          name_length;
	datum_index  tag_index;
};
STAT_ASSRT(tag_reference, 0x10);
// #define TREF_SIZEOF sizeof(tag_reference);

template <typename T>
struct tag_block {
	typedef T *iter;

	//size -> in bytes.
	long size;
	union {
		void *address;
		T    *definitions;
	};

	long constexpr Count() { return size / sizeof(T); }

	[[maybe_unused]]
	iter begin() { return definitions; }

	iter end() { return definitions + Count(); }

	struct tag_block_definition *definition;
};
