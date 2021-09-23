#pragma once
#include <macros_generic.h>

#include "../../memory/datum_index.h"
#include "../../cache/cache_files.h"

namespace Yelo {
	// Template'd tag_data for more robust code dealing with known
	// sub-structures.
	template<typename T>
	struct TagData {
		typedef T *iterator;
		typedef const T *const_iterator;
		typedef T value_type;
		typedef T &reference;
		typedef const T &const_reference;
		typedef T *pointer;
		typedef const T *const_pointer;

		// Size, in bytes, of the data blob
		long Size;

		unsigned long Flags;
		long StreamPosition;

		union {
			// Address of the data blob
			void *Address;

			T *Definitions;
		};

		const struct tag_data_definition *DataDefinition;

		// How many [T] objects are actually in the definition.
		size_t Count() const { return Size / sizeof(T); }

		// Sizeof(T)
		size_t SizeOf() const { return sizeof(T); }

		// tag_data<T> *to_tag_data() { return reinterpret_cast<tag_data<T> *>(&this->Size); }
		//
		// const tag_data *to_tag_data() const { return reinterpret_cast<const tag_data *>(&this->Size); }

		T *operator[](long index) { return &this->Definitions[index]; }

		//////////////////////////////////////////////////////////////////////////
		// STL-like APIs
		const_iterator begin() const { return Definitions; }

		iterator begin() { return Definitions; }

		const_iterator const_begin() const { return Definitions; }

		const_iterator const_begin() { return Definitions; }

		const_iterator end() const { return Definitions + Count(); }

		iterator end() { return Definitions + Count(); }

		const_iterator const_end() const { return Definitions + Count(); }

		const_iterator const_end() { return Definitions + Count(); }

		bool empty() const { return Size == 0; }

		size_t size() const { return static_cast<size_t>(Count()); }

		void resize(size_t new_size = 0) { return; }  //blam::tag_data_resize(this, new_size); }
	};

	STAT_ASSERT(TagData<byte>, 0x14);

	struct tag_data {
		// byte count of this data blob
		long size;
		// unknown
		long_flags flags;
		// offset in the source tag file (relative to the start of the definition bytes)
		long stream_position;
		// data blob bytes pointer
		void *address;
		// definition pointer of this data blob instance
		const struct tag_data_definition *definition;

		// Returns a [T] pointer that is the same as [address].
		// Just makes coding a little cleaner
		template<typename T>
		T *Elements() { return reinterpret_cast<T *>(address); }

		// Returns byte pointer that is the same as [address]
		// Just makes coding a little more cleaner
		byte *Bytes() { return reinterpret_cast<byte *>(address); }

		bool resize(long new_size = 0);
	};

	STAT_ASSERT(tag_data, 0x14);
	STAT_ASSERT(TagData<byte>, sizeof(tag_data));

	namespace blam {
		// Get the address of a block element which exists at [element_index]
		template<typename T>
		void *__cdecl tag_data_get_pointer(T &data, long offset, long size) {
			//YELO_ASSERT(size >= 0);
			//YELO_ASSERT(offset >= 0 && offset + size <= data.size);
			return data.Bytes() + offset;
		}

		template<typename T>
		inline T *tag_data_get_pointer(tag_data &data, long offset, long index = 0) {
			return reinterpret_cast<T *>(tag_data_get_pointer(data, offset + (sizeof(T) * index), sizeof(T)));
		}

		//TODO: relocate this definition.
		Yelo::datum_index tag_new(tag group_name, const char *name);
	}
}
#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32



