#pragma once
#include "macros_generic.h"
namespace Yelo {
	// Template'd tag_data for more robust code dealing with known
	// sub-structures.
	template <typename T>
	struct TagData {
		typedef T       *iterator;
		typedef const T *const_iterator;
		typedef T value_type;
		typedef T       &reference;
		typedef const T &const_reference;
		typedef T       *pointer;
		typedef const T *const_pointer;

		// Size, in bytes, of the data blob
		long Size;

		unsigned long Flags;
		long      StreamPosition;

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

		T *operator [](long index) { return &this->Definitions[index]; }

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

		size_t size() const { return CAST(size_t, Count()); }

		void resize(size_t new_size = 0)	{ return; }  //blam::tag_data_resize(this, new_size); }
	};

	STAT_ASSERT(TagData<byte>, 0x14);
};
