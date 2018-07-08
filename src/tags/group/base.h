#pragma once

#include <memory>
#include "../../cseries/yelo_base.h"
#include "groups_structures.hpp"
#include "tag_groups.h"
#include "loading.hpp"
#include "../../memory/memory_yelo.hpp"

namespace Yelo::TagGroups {
	/// <summary>	when true, all 'model' references are loaded or get as gbxmodels </summary>
	static inline const bool g_gbxmodel_group_enabled = true;
}

namespace Yelo {

	namespace Enums {
		enum {
			k_max_tag_name_length = 255,

			k_tag_block_format_buffer_size = 512,

			k_maximum_tags_per_tag_chain = 4,
			k_maximum_children_per_tag   = 16,
		};
	};

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

			//assert(reference.name);
			std::memset(reference.name, 0, Enums::k_max_tag_name_length + 1);
			reference.name_length = 0;
			reference.group_tag   = NONE;
			reference.tag_index   = datum_index::null();
		}

		void set(tag group_tag, const char *name) {
			// auto reference = (*this);
			// assert(group_tag == NONE || tag_group_get(group_tag));
			// reference.group_tag = group_tag;
			//
			// size_t name_length = strlen(name);
			// //YELO_ASSERT(name_length <= Enums::k_max_tag_name_length); // NOTE: engine does '<', but I'm pretty sure we want '<='
			//
			// //YELO_ASSERT(reference.name);
			// if (reference.name != name)
			// 	strcpy(reference.name, name);
			//
			// reference.name_length = name_length;
		}

		template <typename T>
		void set(const char *name) {
			this->set(T::k_group_tag, name);
		}
	};

	static_assert(sizeof(tag_reference) == 0x10);

	void __cdecl tag_reference_clear(tag_reference &reference);

#define pad_tag_reference PAD32 PAD32 PAD32 PAD32

	namespace blam {
		// Clear the values of a tag reference so that it references no tag
		void __cdecl tag_reference_clear(tag_reference &reference) {
			// The engine's code will free (ie, YELO_FREE) the reference's name
			// when tag_block_delete_element (which is called by tag_unload) is ran

			//assert(reference.name);
			std::memset(reference.name, 0, Enums::k_max_tag_name_length + 1);
			reference.name_length = 0;
			reference.group_tag   = NONE;
			reference.tag_index   = datum_index::null();
		}

		void __cdecl tag_reference_set(tag_reference &reference, tag group_tag, const char *name) {
			//assert(group_tag == NONE || tag_group_get(group_tag));
			reference.group_tag = group_tag;

			size_t name_length = strlen(name);
			//YELO_ASSERT(name_length <= Enums::k_max_tag_name_length); // NOTE: engine does '<', but I'm pretty sure we want '<='

			//YELO_ASSERT(reference.name);
			if (reference.name != name)
				strcpy(reference.name, name);

			reference.name_length = name_length;
		}

		template <typename T>
		void tag_reference_set(tag_reference &reference, const char *name) {
			return tag_reference_set(reference, T::k_group_tag, name);
		}

		datum_index __cdecl tag_reference_try_and_get(const tag_reference *reference) {
			// //assert(reference);
			//
			// datum_index loaded_tag_index = tag_loaded(reference->group_tag, reference->name);
			// //assert(reference->tag_index == loaded_tag_index);
			//"tag reference \"%s\" and actual index do not match: is %08lX but should be %08lX",
			// 									  reference->name, reference->tag_index,
			// 									  loaded_tag_index

			return datum_index::null();
		}

		bool __cdecl tag_reference_resolve(_Inout_ tag_reference *reference) {
			//assert(reference);

			bool success = false;
			// if (reference->group_tag != NONE && !is_null_or_empty(reference->name)) {
			// 	reference->tag_index = tag_load(reference->group_tag, reference->name, 0);
			// 	success = !reference->tag_index.IsNull();
			// } else
			// 	reference->tag_index = datum_index::null();

			return success;
		}

		// non-standard overload of the above resolve()
		bool tag_reference_resolve(_Inout_ tag_reference &reference, tag expected_group_tag) {
			bool success = false;
			if (reference.group_tag == expected_group_tag && !Yelo::is_null_or_empty(reference.name)) {
				reference.tag_index = tag_load(reference.group_tag, reference.name, 0);
				success = !reference.tag_index.IsNull();
			} else
				reference.tag_index = datum_index::null();

			return success;
		}

		template <typename T>
		bool tag_reference_resolve(_Inout_ tag_reference &reference) {
			return tag_reference_resolve(reference, T::k_group_tag);
		}
	};

	struct tag_block {
		enum {
			k_debug_data_size = sizeof(tag_block_definition *),
		};

		// element count for this block
		long                              count;
		// elements pointer
		void                              *address;
		// definition pointer for this block instance
		const struct tag_block_definition *definition;

		// Returns a [T] pointer that is the same as [address].
		// Just makes coding a little more cleaner
		template <typename T>
		T *Elements() { return reinterpret_cast<T *>(address); }

		void *get_element(long element_index);

		void delete_element(long element_index);

		long add_element();

		bool resize(long element_count);

		void *add_and_get_element();

		size_t get_element_size() const {
			return definition->element_size;
		}

		struct s_iterator_result {
			void *address;
			long index;

			s_iterator_result(void *ptr, long i) : address(ptr), index(i) {}
		};

		// NOTE: Design assumes there's no concurrent element changing (adding or removing)
		struct s_iterator {
			byte   *m_address;
			long   m_element_index;
			size_t m_element_size;
		public:
			s_iterator(tag_block &block, size_t element_size, size_t element_index = 0)
				: m_address(reinterpret_cast<byte *>(block.address) + (element_size * element_index)), m_element_index(element_index), m_element_size(element_size) {
			}

			bool operator !=(const s_iterator &other) const {
				return m_address != other.m_address;
			}

			s_iterator &operator ++() {
				m_address += m_element_size;
				++m_element_index;
				return *this;
			}

			s_iterator_result operator *() const {
				return s_iterator_result(m_address, m_element_index);
			}
		};

		s_iterator begin() { return s_iterator(*this, this->get_element_size()); }

		s_iterator end() { return s_iterator(*this, this->get_element_size(), reinterpret_cast<size_t>(this->count)); }
	};

	static_assert(sizeof(tag_block) == 0xC);
#define pad_tag_block PAD32 PAD32 PAD32

	namespace blam {
		// Get the address of a block element which exists at [element_index]
		void *__cdecl tag_block_get_element(tag_block *block, long element_index);

		const void *__cdecl tag_block_get_element(const tag_block *block, long element_index);

		// Add a new block element and return the index which
		// represents the newly added element
		long __cdecl tag_block_add_element(tag_block *block) {
			// YELO_ASSERT(block && block->definition);
			//
			// auto *definition = block->definition;
			// if (block->count >= definition->maximum_element_count) {
			// 	YELO_WARN("tried to add more elements for a %s @%p #%d than allowed",
			// 				 definition->name, block, block->count);
			// 	return NONE;
			// }
			//
			// int  add_index    = block->count++;
			// void *new_address = TAG_BLOCK_REALLOC(*block, block->count);
			// if (new_address == nullptr) {
			// 	YELO_WARN("failed to allocate new elements for a %s @%p #%d",
			// 				 definition->name, block, block->count);
			// 	return NONE;
			// }
			//
			// void *new_element = blam::tag_block_get_element(block, add_index);
			// blam::tag_block_generate_default_element(definition, new_element);
			//
			// long dummy_position;
			// bool  success     = blam::tag_block_read_children_recursive(definition, new_element, 1, &dummy_position,
			// 																				FLAG(Flags::_tag_load_for_editor_bit), datum_index::null();
			//
			// if (!success) {
			// 	--block->count;
			// 	add_index = NONE;
			// }

			return nullptr;
		}

		// Resize the block to a new count of elements, returning the
		// success result of the operation
		bool __cdecl tag_block_resize(tag_block *block, long element_count) {
			// YELO_ASSERT(block && block->definition);
			// YELO_ASSERT(block->count >= 0);
			//
			// YELO_ASSERT(element_count >= 0);
			//
			// // this is how resize is implemented in the engine. Hey, it handles both cases
			//
			// while (block->count > element_count)
			// 	blam::tag_block_delete_element(block, block->count - 1);
			//
			// while (block->count < element_count)
			// 	if (blam::tag_block_add_element(block) == NONE)
			// 		return false;

			return false;
		}

		// Frees the pointers used in more complex fields (tag_data, etc)
		static void tag_block_delete_element_pointer_data(tag_block *block, long element_index) {
			// auto *definition = block->definition;
			//
			// if (definition->delete_proc != nullptr)
			// 	definition->delete_proc(block, element_index);
			//
			// // NOTE: YELO_FREE/DELETE take the pointers by reference, so that it can NULL them in the process
			//
			// for (auto field : TagGroups::c_tag_field_scanner(definition->fields, blam::tag_block_get_element(block, element_index))
			// 	.AddFieldType(Enums::_field_block)
			// 	.AddFieldType(Enums::_field_data)
			// 	.AddFieldType(Enums::_field_tag_reference)) {
			// 	switch (field.GetType()) {
			// 		case Enums::_field_data:
			// 			// TagGroups::c_tag_data_allocations::Delete(*field.As<tag_data>(), __FILE__, __LINE__)
			// 			// TAG_DATA_DELETE();
			// 			break;
			//
			// 		case Enums::_field_block:
			// 			// engine actually does a while loop here, calling delete_element
			// 			blam::tag_block_resize(field.As<tag_block>(), 0);
			// 			break;
			//
			// 		case Enums::_field_tag_reference:
			// 			YELO_DELETE(field.As<tag_reference>()->name);
			// 			break;
			//
			// 		default:
			// 			//assert(false);
			// 	}
			// }
		}

		// Delete the block element at [element_index]
		void __cdecl tag_block_delete_element(tag_block *block, long element_index) {

			auto *definition = block->definition;
			if (definition->fields == nullptr) {
				//::Print<false>("'%s' block has NULL fields", definition->name);
			}
			tag_block_delete_element_pointer_data(block, element_index);

			// move up the elements that follow the deleted element
			int next_element_index = element_index + 1;
			// engine doesn't actually check if the deleted element is the last
			if (next_element_index < block->count) {
				size_t element_size              = definition->element_size;
				size_t following_elements_offset = element_size * next_element_index;
				size_t following_elements_size   = element_size * ((block->count - element_index) - 1);

				memmove(blam::tag_block_get_element(block, element_index), // elements will start consuming the memory at the deleted element
						  reinterpret_cast<byte *>(block->address) + following_elements_offset,
						  following_elements_size);
			}

			// if (--block->count == 0) // free the elements and clear the pointer if that was the last element
			// {
			// 	TagGroups::c_tag_block_allocations::Delete(*block, __FILE__, __LINE__);
			// }
		}

		void *tag_block_add_and_get_element(tag_block *block);
	};

	struct tag_data {
		enum {
			k_debug_data_size = sizeof(unsigned long) + sizeof(long) + sizeof(struct tag_block_definition *),
		};

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
#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32

	namespace blam {
		bool __cdecl tag_data_resize(tag_data *data, long new_size) {
			// YELO_ASSERT(data->address);
			//
			// bool result = false;
			// if (new_size < 0) {
			// 	YELO_WARN("tried to resize a %s @%p to a negative size %d",
			// 				 data->definition->name, data, new_size);
			// } else if (new_size > data->definition->maximum_size) {
			// 	YELO_WARN("tried to resize a %s @%p to %d which is larger than the max allowed %d",
			// 				 data->definition->name, data, new_size, data->definition->maximum_size);
			// } else if (new_size == 0) {
			// 	data->size = 0;
			// 	result = true;
			// } else {
			// 	Yelo::TagGroups::c_tag_data_allocations::Realloc(*data, new_size, __FILE__, __LINE__);
			// 	result = data->address != nullptr;
			// }
			//
			// return result;
			return false;
		}

		void *__cdecl tag_data_get_pointer(tag_data &data, long offset, long size) {
			//YELO_ASSERT(size >= 0);
			//YELO_ASSERT(offset >= 0 && offset + size <= data.size);

			return data.Bytes() + offset;
		}

		template <typename T>
		inline
		T *tag_data_get_pointer(tag_data &data, long offset, long index = 0) {
			return reinterpret_cast<T *>(tag_data_get_pointer(data, offset + (sizeof(T) * index), sizeof(T)));
		}
	};

	namespace blam {
		tag __cdecl tag_get_group_tag(datum_index tag_index);

		tag_block *__cdecl tag_get_root_block(datum_index tag_index);

		void __cdecl tag_orphan(datum_index tag_index);

		datum_index __cdecl tag_loaded(tag group_tag, const char *name);

		template <typename T>
		inline datum_index tag_loaded(const char *name) {
			return tag_loaded(T::k_group_tag, name);
		}

		const char *__cdecl tag_get_name(datum_index tag_index) {
			const char *name = cache_file_tag_get_instance(tag_index)->name;

			// non-standard behavior, but I believe some cache protectors NULL the tag name
			return name != nullptr ? name : "<unnamed tag>";
		}

		inline const char *tag_try_get_name(datum_index tag_index) {
			return tag_index.IsNull() ? "<unspecified tag>" : tag_get_name(tag_index);
		}

		bool __cdecl tag_read_only(datum_index tag_index);

		// Get the tag definition's address by it's expected group tag and
		// it's tag handle [tag_index]
		void *__cdecl tag_get(tag group_tag, datum_index tag_index) {
			auto *tag_instance = cache_file_tag_get_instance(tag_index);

			// YELO_ASSERT_DISPLAY(tag_instance->MatchesGroup(group_tag), "expected tag group '%s' but got '%s' for %08x", group_tag_to_string{group_tag}.ToString(),
			// 						  group_tag_to_string{tag_instance->group_tag}.ToString(),
			// 						  tag_index);
			//
			// YELO_ASSERT_DISPLAY(tag_instance->base_address != nullptr, "can't get() a tag (%08x) with a base address!", tag_index);

			return tag_instance->base_address;
		}

		template <typename T>
		inline T *tag_get(datum_index tag_index) {
			return reinterpret_cast<T *>(tag_get(T::k_group_tag, tag_index));
		}

		datum_index __cdecl tag_new(tag group_name, const char *name);

		template <typename T>
		inline datum_index tag_new(const char *name) {
			return tag_new(T::k_group_tag, name);
		}

		// Load a tag definition into memory.
		// Returns the tag handle of the loaded tag definition
		datum_index __cdecl tag_load(tag group_tag, const char *name, unsigned long file_flags);

		template <typename T>
		inline datum_index tag_load(const char *name, unsigned long file_flags) {
			return tag_load(T::k_group_tag, name, file_flags);
		}

		datum_index __cdecl tag_reload(tag group_tag, const char *name);

		template <typename T>
		inline datum_index tag_reload(const char *name) {
			return tag_reload(T::k_group_tag, name);
		}

		void __cdecl tag_load_children(datum_index tag_index);

		// Unload a tag definition from memory.
		// [tag_index] will resolve to an invalid index after this returns.
		void __cdecl tag_unload(datum_index tag_index);

		bool __cdecl tag_save(datum_index tag_index);
	};

	namespace TagGroups {
		// just an endian swap
		void TagSwap(tag &x);

		tag string_to_group_tag(const char *name);

		// Returns true if the tag is an instance of the group_tag or is a child group of it.
		// Returns false if not, or tag_index is invalid.
		bool TagIsInstanceOf(datum_index tag_index, tag group_tag);

		// template <typename T>
		// inline bool TagIsInstanceOf(datum_index tag_index) {
		// 	return TagIsInstanceOf(tag_index, T::k_group_tag);
		// }

		// Returns the tag as non-const. Are you sure you want to be writing to tags at runtime?
		template <typename T>
		inline T *TagGetForModify(datum_index tag_index) {
			return const_cast<T *>(blam::tag_get<T>(tag_index));
		}

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

			group_tag_to_string &Terminate();

			group_tag_to_string &TagSwap();

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

	static long __cdecl tag_block_insert_element_impl(tag_block *block, long index) {
		// YELO_ASSERT(block && block->definition); // engine actually does the asserts these after the allocation
		// YELO_ASSERT(index >= 0 && index <= block->count);

		auto *definition              = block->definition;
		int  proceeding_element_index = index + 1;

		// get a new element
		int add_index = blam::tag_block_add_element(block); // engine actually does this after the allocation
		if (add_index == NONE)
			return NONE;
		else if (proceeding_element_index == block->count)
			return add_index; // the element was inserted at the end, nothing needs to be done

		void   *element     = blam::tag_block_get_element(block, add_index);
		size_t element_size = definition->element_size;

		// copy the newly created element data to a temp buffer
		auto element_scratch = std::unique_ptr<void, Yelo::Memory::memory_deleter<void>>(malloc(element_size));
		{
			if (element_scratch == nullptr)
				return NONE;

			std::memcpy(element_scratch.get(), element, element_size);

			size_t following_elements_offset     = element_size * index;
			size_t following_elements_size       = element_size * ((block->count - index) - 1);
			size_t following_elements_new_offset = element_size * (index + 1);

			auto *address = reinterpret_cast<byte *>(block->address);
			std::memmove(address + following_elements_new_offset,
							 address + following_elements_offset,
							 following_elements_size);

			// copy the element we added for insertion. IE, insert it
			std::memcpy(blam::tag_block_get_element(block, index), element_scratch.get(), element_size);
		}

		return index;
	}

	static bool tag_block_duplicate_element_recursive(tag_block *source_block, long source_element_index, tag_block *destination_block, long destination_element_index) {
		// YELO_ASSERT(source_block->definition == destination_block->address); // engine doesn't actually do this first
		//
		// TagGroups::c_tag_field_scanner source(source_block->definition->fields,
		// 												  blam::tag_block_get_element(source_block, source_element_index));
		// source.AddAllFieldTypes();
		//
		// TagGroups::c_tag_field_scanner destination(destination_block->definition->fields, blam::tag_block_get_element(destination_block, destination_element_index));
		// destination.AddAllFieldTypes();
		//
		// while (source.Scan()) {
		// 	destination.Scan();
		// 	YELO_ASSERT(source.GetTagFieldType() == destination.GetTagFieldType());
		//
		// 	switch (source.GetTagFieldType()) {
		// 		default:
		// 			std::memcpy(destination.GetFieldAddress(), source.GetFieldAddress(), source.GetFieldSize());
		// 			break;
		//
		// 		case Enums::_field_data: {
		// 			auto *dest_data = destination.GetFieldAs<tag_data>();
		// 			auto *src_data  = source.GetFieldAs<tag_data>();
		//
		// 			if (src_data->address != nullptr && blam::tag_data_resize(dest_data, src_data->size))
		// 				std::memcpy(dest_data->address, src_data->address, src_data->size);
		// 		}
		// 			break;
		//
		// 		case Enums::_field_block: {
		// 			auto *dest_block = destination.GetFieldAs<tag_block>();
		// 			YELO_ASSERT(dest_block->count == 0);
		// 			auto *src_block = source.GetFieldAs<tag_block>();
		//
		// 			if (blam::tag_block_resize(dest_block, src_block->count)) {
		// 				for (int x = 0; x < src_block->count; x++)
		// 					tag_block_duplicate_element_recursive(src_block, x, destination_block, x);
		// 			}
		// 		}
		// 			break;
		//
		// 		case Enums::_field_tag_reference: {
		// 			auto *dest_ref = destination.GetFieldAs<tag_reference>();
		// 			auto *src_ref  = source.GetFieldAs<tag_reference>();
		//
		// 			blam::tag_reference_set(*dest_ref, src_ref->group_tag, src_ref->name);
		// 			dest_ref->tag_index = src_ref->tag_index;
		// 		}
		// 			break;
		// 	}
		// }

		return false;
	}

	long __cdecl tag_block_duplicate_element_impl(tag_block *block, long element_index) {
		int dup_index = blam::tag_block_add_element(block);
		if (dup_index != NONE) {
			tag_block_duplicate_element_recursive(
				block, element_index,
				block, dup_index);
		}

		return dup_index;
	}

	bool __cdecl tag_block_swap_elements_impl(tag_block *block, long left_element_index, long right_element_index) {
		YELO_ASSERT(block && block->definition); // engine actually does this after the allocation

		auto   *definition  = block->definition;
		size_t element_size = definition->element_size;

		auto element_scratch = YELO_MALLOC_UNIQUE(element_size, false);
		{
			if (element_scratch == nullptr)
				return false;

			void *left_element  = blam::tag_block_get_element(block, left_element_index);
			void *right_element = blam::tag_block_get_element(block, right_element_index);

			std::memcpy(element_scratch.get(), left_element, element_size);
			std::memcpy(left_element, right_element, element_size);
			std::memcpy(right_element, element_scratch.get(), element_size);
		}

		return true;
	}

};

#include "field_definitions.inl"
