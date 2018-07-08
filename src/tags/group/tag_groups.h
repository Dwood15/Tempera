#pragma once

#include "base.h"
#include "groups_structures.hpp"
#include "../../memory/datum_index.h"
#include "../../memory/data.h"
#include "../../memory/upgrades/blam_memory_upgrades.hpp"
#include "../../memory/array.h"
#include "tag_groups_base_yelo.hpp"


namespace Yelo::Enums {
	enum {
		k_protected_group_tag = 'prot', // HEK+: this overrides the scenario's group tag in the tag index.
	};
};

namespace Yelo {
	struct s_tag_instance;
	namespace TagGroups {
		struct s_tag_iterator;
	}
}
namespace Yelo::TagGroups {
	typedef Memory::DataArray<Yelo::s_tag_instance, Enums::k_maximum_simultaneous_tag_instances, Enums::k_maximum_simultaneous_tag_instances_upgrade> tag_instance_data_t;
	tag_instance_data_t &TagInstances();
};

namespace Yelo::blam {
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialize a tag instance iterator for the given group tag </summary>
	///
	/// <param name="iter">			   	[out] The iterator to initialize </param>
	/// <param name="group_tag_filter">
	/// 	(Optional) the group tag to filter results by. Use [NONE] for [group_tag_filter] to
	/// 	iterate all tag groups.
	/// </param>
	static void __cdecl tag_iterator_new(Yelo::TagGroups::s_tag_iterator &iter, const tag group_tag_filter = NONE) {
		data_iterator_new(iter.instances_iterator, &TagGroups::TagInstances().Header);
		iter.group_tag_filter = group_tag_filter;
	}

	template <typename T>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialize a tag instance iterator for the given group tag </summary>
	///
	/// <param name="iter">	[out] The iterator to initialize </param>
	inline static void tag_iterator_new(TagGroups::s_tag_iterator &iter) {
		tag_iterator_new(iter, T::k_group_tag);
	}

	// Get the group definition that follows [group], or the first group if it is NULL
	tag_group *__cdecl tag_group_get_next(const tag_group *group = nullptr);

	// Get the group definition based on a four-character code
	tag_group *__cdecl tag_group_get(tag group_tag);

}

namespace Yelo::TagGroups {
	// Patches stock tag_groups with new fields where they once had useless padding
	// Called before group definitions have been verified (but after group tags have) and group parents are built
	// We do this to ensure we don't fuck anything up, but also because a parent group may get modified
	// Defined in CheApe's TagFieldSetReplacements.cpp
	// TODO: perhaps instead we should run this -after- initial verification, then in true debug builds
	// run group verification again after this executes. Or we could perform verification in the fieldset replacement code
	void InitializeFieldSetReplacements();

	// Patches stock tag groups for various *fixes* (eg, to correct behavior), not additions or the like
	// Called after the group definitions have been verified
	// Defined in CheApe's TagGroups.cpp
	void InitializeFixes();

	extern const s_tag_field_definition k_tag_field_definitions[];



	// Get the length, in characters, of a string field, excluding the null character
	long StringFieldGetLength(const tag_field *field) {
		assert(field->type == Enums::_field_string);

		uintptr_t definition = field->DefinitionCast<uintptr_t>();

		if (definition == 0 || TagFieldIsOldStringId(field))
			return Enums::k_tag_string_length;
		else // NOTE: the definition should have already been validated if tag_groups_initialize has already ran
			return CAST(long, definition);
	}

	// Get the size, in characters, of a string field, inclusive of the null character
	long StringFieldGetSize(const tag_field *field) {
		return StringFieldGetLength(field) + 1;
	}

	tag_group *FindTagGroupByName(const char * name);

	char *TryAndGetGroupName(tag group_tag, _Out_ long_string name) {
		const tag_group *group = Yelo::blam::tag_group_get(group_tag);
		if (group != nullptr)
			strcpy(name, group->name);
		else if (group_tag == NONE)
			strcpy(name, "NONE");
		else if (group_tag == 0)
			strcpy(name, "ZERO");
		else
			strcpy(name, group_tag_to_string{group_tag}.ToString());

		return name;
	}

	// Convenience function to handle deleting all of the data in tag_data field.
	// Use [terminator_size] for tag_data which HAS to have a specific amount of
	// bytes no matter what. IE, text data requires 1 or 2 bytes (ascii or unicode)
	// for the null terminator.
	void tag_data_delete(tag_data *data, size_t terminator_size = 0) {
		blam::tag_data_resize(data, terminator_size);
	}

	template <typename T>
	inline void tag_data_delete(TagData<T> &data, size_t terminator_size = 0) {
		tag_data_delete(data.to_tag_data(), terminator_size);
	}

	bool tag_block_delete_all_elements(tag_block *block) {
		return block->count > 0 ? blam::tag_block_resize(block, 0) : true;
	}

	template <typename T>
	inline
	bool tag_block_delete_all_elements(TagBlock<T> &block) {
		return tag_block_delete_all_elements(block.to_tag_block());
	}

	// Note: when used in range based for loops this will create an unnecessary copy operation, but with SSE2 it shouldn't be that bad
	class c_tag_iterator {
		s_tag_iterator m_state;
		datum_index    m_tag_index;

		c_tag_iterator(const void *endHackDummy) {
			//std::memset(&m_state, 0, sizeof(m_state));
			m_state.instances_iterator.SetEndHack();
		}

	public:
		c_tag_iterator(const tag group_tag_filter) : m_tag_index(datum_index::null()) {
			blam::tag_iterator_new(m_state, group_tag_filter);
		}

		template <typename T>
		c_tag_iterator() :
			m_tag_index(datum_index::null()) {
			blam::tag_iterator_new<T>(m_state);
		}

		// Get an iterator that doesn't have any specific group_tag filter
		static c_tag_iterator all() {
			return c_tag_iterator(NONE);
		}

		datum_index Next() {
			return m_tag_index = blam::tag_iterator_next(m_state);
		}

		bool operator !=(const c_tag_iterator &other) const {
			// NOTE: we intentionally don't compare the group_tag filter
			return m_state.instances_iterator != other.m_state.instances_iterator;
		}

		c_tag_iterator &operator ++() {
			Next();
			return *this;
		}

		datum_index operator *() const {
			return m_tag_index;
		}

		c_tag_iterator &begin() /*const*/
		{
			this->Next();
			return *this;
		}

		static const c_tag_iterator end() /*const*/
		{
			return c_tag_iterator(nullptr);
		}
	};
};



namespace Yelo::blam {
	void __cdecl tag_groups_initialize();

	void __cdecl tag_groups_dispose();

	void __cdecl tag_groups_initialize_for_new_map();

	void __cdecl tag_groups_dispose_from_old_map();

	void __cdecl tag_groups_dump_memory();

	uint __cdecl tag_groups_checksum();

	datum_index __cdecl find_tag_instance(tag group_tag, const char * name) {
		for (auto instance : Yelo::TagGroups::TagInstances()) {
			if (instance->is_orphan || instance->is_reload || instance->group_tag != group_tag)
				continue;

			if (strcmp(instance->filename, name) == 0)
				return instance.index;
		}
		return datum_index::null();
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Increment a tags instance iterator to the next instance </summary>
	///
	/// <param name="iter">	[in,out] The iterator to increment </param>
	///
	/// <returns>	Returns the next datum's index or [datum_index::null] when finished iterating </returns>
	datum_index __cdecl tag_iterator_next(TagGroups::s_tag_iterator &iter) {
		tag group_tag_filter = iter.group_tag_filter;

		while (const void *datum = data_iterator_next(iter.instances_iterator)) {
			auto instance = reinterpret_cast<const s_tag_instance *>(datum);

			if (group_tag_filter == NONE ||
				 instance->group_tag == group_tag_filter ||
				 instance->parent_group_tags[0] == group_tag_filter ||
				 instance->parent_group_tags[1] == group_tag_filter) {
				return iter.instances_iterator.index;
			}
		}

		return datum_index::null();
	}


	template <typename T>
	inline tag_group *tag_group_get() {
		return tag_group_get(T::k_group_tag);
	}

	// Rename the tag definition [tag_index] to [new_name]
	void __cdecl tag_rename(datum_index tag_index, const char * new_name);

	tag_block *__cdecl tag_block_index_resolve(datum_index tag_index, tag_field *block_index_field, long index);

	// Get the size in bytes of how much memory the tag definition [tag_index]
	// consumes with all of its child data too
	uint __cdecl tag_size(datum_index tag_index);

	// Get the size in bytes of how much memory [block] consumes with all
	// of its child data too
	uint __cdecl tag_block_size(tag_block *block);

	// Insert a new block element at [index] and return the index
	// of the inserted element
	long __cdecl tag_block_insert_element(tag_block *block, long index);

	template <typename T>
	inline
	T *tag_block_insert_element(TagBlock<T> &block, long index) {
		return reinterpret_cast<T *>(tag_block_insert_element(block.to_tag_block(), index));
	}

	// Duplicate the block element at [element_index] and return the index which
	// represents the duplicated element
	long __cdecl tag_block_duplicate_element(tag_block *block, long element_index);

	template <typename T>
	inline
	long tag_block_duplicate_element(TagBlock<T> &block, long element_index) {
		return tag_block_duplicate_element(block.to_tag_block(), element_index);
	}

	static void __cdecl tag_block_generate_default_element(const tag_block_definition *definition, void *address) {
		memset(address, 0, definition->element_size);

		for (auto field : TagGroups::c_tag_field_scanner(definition->fields, address)
			.AddFieldType(Enums::_field_short_block_index)
			.AddFieldType(Enums::_field_long_block_index)) {
			switch (field.GetType()) {
				case Enums::_field_short_block_index:
					*field.As<short>() = NONE;
					break;

				case Enums::_field_long_block_index:
					*field.As<long>() = NONE;
					break;
			}
		}
	}
};

namespace Yelo::TagGroups {
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the tag_group for a specific tag definition </summary>
	inline tag_group *TagGetGroup(datum_index tag_index) {
		return blam::tag_group_get(blam::tag_get_group_tag(tag_index));
	}

	// Visit all the registered tag_groups and perform an action on them
	// TAction: void operator()([const] tag_group* group)
	template <class TAction>
	void tag_groups_do_action(TAction &action = TAction()) {
		for (tag_group *group = blam::tag_group_get_next(); group; group = blam::tag_group_get_next(group))
			action(group);
	}

	// Visit all the registered tag_groups and perform an action on their header_block_definition
	// TAction: void operator()(tag_block_definition* header_block_definition)
	template <class TAction>
	void tag_groups_do_header_block_definition_action(TAction &action = TAction()) {
		for (tag_group *group = blam::tag_group_get_next(); group; group = blam::tag_group_get_next(group))
			action(group->header_block_definition);
	}

	// TAction: bool operator()([const] tag_block* block, long element_index, [const] void* element)
	template <class TAction>
	bool tag_block_elements_do_action_sans_safe_get(tag_block *block, TAction &action = TAction()) {
		auto *definition = block->definition;
		bool result      = true;

		for (int x = 0; x < block->count; x++) {
			// TODO: need to account for alignment, if we ever fully implement it
			void *element = reinterpret_cast<byte *>(block->address) +
								 (definition->element_size * x);

			result &= action(block, x, element);
		}

		return result;
	}
};
};
namespace Yelo {
	void tag_reference::set(tag group_tag, const char * name) {
		blam::tag_reference_set(*this, group_tag, name);
	}

	void *tag_block::get_element(long element) {
		return blam::tag_block_get_element(this, element);
	}

	void tag_block::delete_element(long element) {
		blam::tag_block_delete_element(this, element);
	}

	long tag_block::add_element() {
		return blam::tag_block_add_element(this);
	}

	bool tag_block::resize(long element_count) {
		return blam::tag_block_resize(this, element_count);
	}

	void *tag_block::add_and_get_element() {
		return blam::tag_block_add_and_get_element(this);
	}

	bool tag_data::resize(long new_size) {
		return blam::tag_data_resize(this, new_size);
	}

	namespace TagGroups {
		// just an endian swap
		void TagSwap(tag &x) {
			x = (x >> 24) |
				 ((x << 8) & 0x00FF0000) |
				 ((x >> 8) & 0x0000FF00) |
				 (x << 24);
		}

		tag string_to_group_tag(const char * name) {
			const uint _group = *((const uint *) name);
			return (tag) ((_group >> 24) | ((_group >> 8) & 0xFF00) | (((_group << 8) & 0xFF0000) | ((_group << 24) & 0xFF000000)));
		}

		group_tag_to_string &group_tag_to_string::Terminate() {
			str[4] = '\0';
			return *this;
		}

		group_tag_to_string &group_tag_to_string::TagSwap() {
			TagGroups::TagSwap(group);
			return *this;
		}
	};

	namespace blam {
		void *tag_block_add_and_get_element(tag_block *block) {
			long index = tag_block_add_element(block);
			return tag_block_get_element(block, index);
		}
	};
};
