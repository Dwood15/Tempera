#pragma once

#include "base.h"
#include "../../ce_base_types.h"

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

	static_assert(sizeof(TagReference<NONE>) == 0x10);

	template <>
	inline bool TagReference<static_cast<tag>(NONE)>::IsValid() const { return true; }

	// Template'd tag block for allowing more robust code.
	template <typename T>
	struct TagBlock {
		typedef T       *iterator;
		typedef const T *const_iterator;
		typedef T value_type;
		typedef T       &reference;
		typedef const T &const_reference;
		typedef T       *pointer;
		typedef const T *const_pointer;

		// Element count for this block
		long Count;

		// Anonymous union for allowing less code for converting, and less 
		// "#pragma warning" code entries
		union {
			// Pointer to the first tag block definition element
			void *Address;

			// Pointer to a specific block of object(s) of type T
			T *Definitions;
		};

		// definition pointer for this block instance
		const class tag_block_definition *BlockDefinition;

		// Using the class's template 'T' parameter, calculates the total
		// size, in bytes, the elements assume in memory
		size_t SizeOf() const { return sizeof(T) * Count; }

		// tag_block *to_tag_block() { return reinterpret_cast<tag_block *>(&this->Count); }

		// const tag_block *to_tag_block() const { return reinterpret_cast<const tag_block *>(&this->Count); }

		// Indexer for getting a definition reference via the definition's index in the block
		T &operator [](long index) {
			// assert(index >= 0 && index < Count);
			return this->Definitions[index];
		}

		// Indexer for getting a (const) definition reference via the definition's index in the block
		const T &operator [](long index) const {

			// assert(index >= 0 && index < Count);
			return this->Definitions[index];
		}

		//index not defined if not editor
		// T *get_element(long element) { return blam::tag_block_get_element(*this, index); }
		//
		// void delete_element(long element) { blam::tag_block_delete_element(*this, element); }
		//
		// bool delete_all_elements() { return blam::tag_block_delete_all_elements(*this); }
		//
		// long add_element() { return blam::tag_block_add_element(*this); }
		//
		// bool resize(long element_count) { return blam::tag_block_resize(*this, element_count); }
		//
		// T *add_and_get_element() { return blam::tag_block_add_and_get_element(*this); }

		//////////////////////////////////////////////////////////////////////////
		// STL-like APIs
		const_iterator begin() const { return Definitions; }

		iterator begin() { return Definitions; }

		const_iterator const_begin() const { return Definitions; }

		const_iterator const_begin() { return Definitions; }

		const_iterator end() const { return Definitions + Count; }

		iterator end() { return Definitions + Count; }

		const_iterator const_end() const { return Definitions + Count; }

		const_iterator const_end() { return Definitions + Count; }

		bool empty() const { return Count == 0; }

		size_t size() const { return CAST(size_t, Count); }
	};

	static_assert(sizeof(TagBlock<char>) == 0xC);

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

		void resize(size_t new_size = 0)	{ blam::tag_data_resize(this, new_size); }
	};

	static_assert(sizeof(TagData<byte>) == 0x14);
	namespace blam {
		template<typename T>
		bool tag_data_resize(TagData<T>& data, size_t new_size = 0)
		{
			return tag_data_resize(data.to_tag_data(), new_size);
		}
	};
};
