#pragma once

#include <cstring>
#include "macros_generic.h"
#include "enums/tag_enums.h"
#include "../../memory/datum_index.h"
#include "tagdata.h"
#include "tagblock.h"

namespace Yelo {
	// Template'd tag_reference that is nifty, and allows for better markup in the code
	template <unsigned long K_TAG>
	struct TagReference {
		// group tag identifier for this reference
		tag                                      GroupTag;

		// path, without tag group extension, to the tag reference
		tag_reference_name_reference Name;
		// length of the reference name
		long                        NameLength;

		// datum index of this reference in the tag index
		datum_index TagIndex;

		operator datum_index() { return TagIndex; }

		inline bool operator ==(const uint& rhs) { return this->TagIndex == rhs ; }
		// Returns whether or not this reference has a valid group reference
		bool IsValid() const { return GroupTag == K_TAG; }

		// Sets this reference to null
		void Reset() {
			GroupTag = K_TAG;
			TagIndex = datum_index::null();
		}

		tag Tag() const { return K_TAG; }
	};

	static_assert(sizeof(TagReference<0>) == 0x10);

	template <>
	inline bool TagReference<static_cast<unsigned long>(NONE)>::IsValid() const { return true; }

	typedef char *tag_reference_name_reference;

	struct s_tag_ref {
		tag group_tag;
		char * name;
		long name_length;
		datum_index tag_index;
	}; 	STAT_ASSERT(s_tag_ref, 0x10);


	struct tag_reference : s_tag_ref {
		enum {
			k_debug_data_size = sizeof(tag_reference_name_reference) + sizeof(long),
		};

//		// group tag identifier for this reference
//		tag                          group_tag;
//		// path, without tag group extension, to the tag reference
//		tag_reference_name_reference name;
//		// length of the reference name
//		long                         name_length;
//		// datum index of this reference in the tag index
//		datum_index                  tag_index;

		operator datum_index() const { return tag_index; }

		void clear() {
			auto reference = (*this);

			std::memset(reference.name, 0, Enums::k_max_tag_name_length + 1);
			reference.name_length = 0;
			reference.group_tag   = NONE;
			reference.tag_index   = datum_index::null();
		}

		void set(tag group_tag, const char *name) {

		}

		template <typename T>
		void set(const char *name) {
			this->set(T::k_group_tag, name);
		}
	}; STAT_ASSERT(tag_reference, 0x10);

	namespace blam {
		extern void tag_reference_clear(tag_reference &reference);
		extern void tag_reference_set(tag_reference &reference, tag group_tag, const char *name);

		// non-standard overload of the above resolve()
		extern bool tag_reference_resolve(_Inout_ tag_reference &reference, tag expected_group_tag);
	}
};

#define pad_tag_reference PAD32 PAD32 PAD32 PAD32
