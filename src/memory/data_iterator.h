#pragma once
#include <cstring>
#include <macros_generic.h>
#include "datum_index.h"
#include "data_array.h"
namespace Yelo::Memory {
	struct s_data_iterator {
		s_data_array *data;
		short        next_index;
		short        finished_flag; // actually alignment, unused by the engine
		datum_index  index;
		tag          signature;

		//////////////////////////////////////////////////////////////////////////
		// stl end() hacks
		enum tag { k_end_hack_signature = 'hack' };

		inline void SetEndHack() { signature = k_end_hack_signature; }

		inline bool IsEndHack() const { return signature == k_end_hack_signature; }

		// HACK: don't use this unless the s_data_iterator was created in the OS codebase!
		// engine's iterator_new doesn't initialize 'finished_flag' and we use it for end() hacks
		bool operator !=(const s_data_iterator &other) const {
			if (other.IsEndHack()) {
				return !this->finished_flag;
			}

			if (this->IsEndHack()) {
				return !other.finished_flag;
			}

			return this->index != other.index;
		}
	};

	STAT_ASSERT(s_data_iterator, 0x10);
};

namespace Yelo::TagGroups {
	struct s_tag_iterator {
		Yelo::Memory::s_data_iterator instances_iterator;
		tag                           group_tag_filter;
	};
	STAT_ASSERT(s_tag_iterator, 0x14);
};

namespace Yelo::blam {
	static void data_iterator_new(Memory::s_data_iterator &iterator, Yelo::Memory::s_data_array *data);
	Yelo::Memory::s_data_iterator* data_iterator_next(Yelo::Memory::s_data_iterator *iterator);
};
