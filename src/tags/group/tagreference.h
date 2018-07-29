#pragma once

#include "macros_generic.h"
#include "../../ce_base_types.h"

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
	inline bool TagReference<static_cast<tag>(NONE)>::IsValid() const { return true; }

	namespace blam {
		template<typename T>
		bool tag_data_resize(TagData<T>& data, size_t new_size = 0)
		{
			return tag_data_resize(data.to_tag_data(), new_size);
		}
	};
};
