#pragma once

#include <memory>
#include "../../cseries/yelo_base.h"
#include "groups_structures.hpp"
#include "tag_groups.h"
#include "../../memory/memory_yelo.hpp"
#include "../../memory/data.h"

namespace Yelo::TagGroups {
	/// <summary>	when true, all 'model' references are loaded or get as gbxmodels </summary>
	static inline const bool g_gbxmodel_group_enabled = true;
}

// #define TREF_SIZEOF sizeof(tag_reference);

template <typename T>
class tag_block {
	typedef T *iter;

	union {
		// element count for this block
		long count;

		//size -> in bytes.
		long size;
	};

	union {
		void *address;
		T    *definitions;
	};

	Yelo::tag_block_definition *definition;

	T *Elements() { return reinterpret_cast<T *>(address); }

	size_t get_element_size() {
		return definition->element_size;
	}

	long constexpr Count() { return size / sizeof(T); }

	[[maybe_unused]]
	iter begin() { return definitions; }

	iter end() { return definitions + Count(); }
};

static_assert(sizeof(tag_block<void>) == 0xC);


namespace Yelo {
	// Halo1's editor allocates 256 characters for all tag_reference names, even if they're empty
	typedef char *tag_reference_name_reference;

	struct tag_reference {
		enum {
			k_debug_data_size = sizeof(tag_reference_name_reference) + sizeof(long),
		};

		// group tag identifier for this reference
		tag                          group_tag;
		// path, without tag group extension, to the tag reference
		tag_reference_name_reference name;
		// length of the reference name
		long                         name_length;
		// datum index of this reference in the tag index
		datum_index                  tag_index;

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
	};

	static_assert(sizeof(tag_reference) == 0x10);

	struct tag_data {
		// byte count of this data blob
		long                             size;
		// unknown
		long_flags                       flags;
		// offset in the source tag file (relative to the start of the definition bytes)
		long                             stream_position;
		// data blob bytes pointer
		void                             *address;
		// definition pointer of this data blob instance
		const struct tag_data_definition *definition;

		// Returns a [T] pointer that is the same as [address].
		// Just makes coding a little more cleaner
		template <typename T>
		T *Elements() { return reinterpret_cast<T *>(address); }

		// Returns byte pointer that is the same as [address]
		// Just makes coding a little more cleaner
		byte *Bytes() { return reinterpret_cast<byte *>(address); }

		bool resize(long new_size = 0);
	};

	static_assert(sizeof(tag_data) == 0x14);

	void __cdecl tag_reference_clear(tag_reference &reference);

#define pad_tag_reference PAD32 PAD32 PAD32 PAD32

#define pad_tag_block PAD32 PAD32 PAD32
#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32

	namespace blam {
		// Clear the values of a tag reference so that it references no tag
		void __cdecl tag_reference_clear(tag_reference &reference);

		void __cdecl tag_reference_set(tag_reference &reference, tag group_tag, const char *name);

		template <typename T>
		void tag_reference_set(tag_reference &reference, const char *name);

		datum_index __cdecl tag_reference_try_and_get(const tag_reference *reference);

		bool __cdecl tag_reference_resolve(_Inout_ tag_reference *reference);

		// non-standard overload of the above resolve()
		bool tag_reference_resolve(_Inout_ tag_reference &reference, tag expected_group_tag);

		template <typename T>
		bool tag_reference_resolve(_Inout_ tag_reference &reference);

		// Get the address of a block element which exists at [element_index]
		template<typename T>
		void *__cdecl tag_block_get_element(T *block, long element_index);

		// Add a new block element and return the index which
		// represents the newly added element
		template<typename T>
		long __cdecl tag_block_add_element(T *block) {
			return nullptr;
		}

		// Resize the block to a new count of elements, returning the
		// success result of the operation

		template<typename T>
		bool __cdecl tag_block_resize(T *block, long element_count) {
			return false;
		}

		template<typename T>
		bool __cdecl tag_data_resize(T *data, long new_size) {
			return false;
		}

		template<typename T>
		void *__cdecl tag_data_get_pointer(T &data, long offset, long size) {
			//YELO_ASSERT(size >= 0);
			//YELO_ASSERT(offset >= 0 && offset + size <= data.size);

			return data.Bytes() + offset;
		}

		template <typename T>
		inline
		T *tag_data_get_pointer(tag_data &data, long offset, long index = 0) {
			return reinterpret_cast<T *>(tag_data_get_pointer(data, offset + (sizeof(T) * index), sizeof(T)));
		}

		datum_index __cdecl tag_new(tag group_name, const char *name);

	};

	namespace TagGroups {
		// just an endian swap
		static void TagSwap(tag &x);

		static tag string_to_group_tag(const char *name);

		// Returns true if the tag is an instance of the group_tag or is a child group of it.
		// Returns false if not, or tag_index is invalid.
		bool TagIsInstanceOf(datum_index tag_index, tag group_tag);

		// template <typename T>
		// inline bool TagIsInstanceOf(datum_index tag_index) {
		// 	return TagIsInstanceOf(tag_index, T::k_group_tag);
		// }

		// 'Unsafe' in that it returns the tag as non-const and doesn't do any bounds checking
		// Useful when you're using tag_iterator and known you're getting some good input
		template <typename T>
		inline T *TagGetUnsafe(datum_index tag_index) {
			extern void *TagGetUnsafeImpl(datum_index tag_index);

			return reinterpret_cast<T *>(TagGetUnsafeImpl(tag_index));
		}

		// Union hack to use a group tag as a string
		union group_tag_to_string {
			struct {
				tag group;
				unsigned char : 8; // null terminator
			};
			char str[sizeof(tag) + 1];

			group_tag_to_string &Terminate() { str[4] = '\0';			return *this; }

			group_tag_to_string &TagSwap() 	{ TagGroups::TagSwap(group);return *this; }

			const char *ToString() {
				return Terminate().TagSwap().str;
			}
		};

		struct s_tag_iterator {
			Yelo::Memory::s_data_iterator instances_iterator;
			tag                           group_tag_filter;
		};
		static_assert(sizeof(s_tag_iterator) == 0x14);
	};
};

