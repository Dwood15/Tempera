#pragma once

#include <macros_generic.h>
#include "groups_structures.h"

namespace Yelo {
	struct tag_block {
		long count;
		void *Address;
		s_tag_block_definition *tagBlockDefinition;
	}; STAT_ASSERT(tag_block, 0xC);

	// Halo1's editor allocates 256 characters for all tag_reference names, even if they're empty
	// Template'd tag block for allowing more robust code.
	template <typename T>
	struct TagBlock {
		typedef T       *iterator;
		typedef const T *const_iterator;
		typedef T       value_type;
		typedef T       &reference;
		typedef const T &const_reference;
		typedef T       *pointer;
		typedef const T *const_pointer;

		// Element count for this block
		union {
			// element count for this block
			long count;

			//size -> in bytes.
			long size;

			long Count;
		};

		// Anonymous union for allowing less code for converting, and less
		// "#pragma warning" code entries
		union {
			void *address;
			T    *definitions;

			// Pointer to the first tag block definition element
			void *Address;

			// Pointer to a specific block of object(s) of type T
			T *Definitions;
		};

		// definition pointer for this block instance
		union {
			const struct tag_block_definition *BlockDefinition;
			Yelo::tag_block_definition       *definition;
		};

		// Using the class's template 'T' parameter, calculates the total
		// size, in bytes, the elements assume in memory
		unsigned int SizeOf() const { return sizeof(T) * Count; }

		unsigned int get_element_size() {
			return definition->element_size;
		}

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

		T *Elements() { return reinterpret_cast<T *>(Address); }

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
		const_iterator begin() const { return definitions; }

		iterator begin() { return definitions; }

		const_iterator const_begin() const { return Definitions; }

		const_iterator const_begin() { return Definitions; }

		iterator end() { return Definitions + Count; }

		const_iterator end() const { return Definitions + Count; }

		const_iterator const_end() { return Definitions + Count; }

		bool empty() const { return Count == 0; }
	};

	STAT_ASSERT(TagBlock<char>, 0xC);
};

#define pad_tag_block PAD32 PAD32 PAD32
