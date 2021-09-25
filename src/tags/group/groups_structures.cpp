#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "groups_structures.h"
#include "addlog.h"
namespace Yelo {
	// Searches the definition for a field of type [field_type] with a name which starts
	// with [name] characters. Optionally starts at a specific field index.
	// Returns NONE if this fails.
	long tag_block_definition::FindFieldIndex(short field_type, const char *name, long start_index) {
		// YELO_ASSERT(this);
		// YELO_ASSERT(this->fields);
		// YELO_ASSERT(IN_RANGE_ENUM(field_type, Enums::k_number_of_tag_field_types));
		// YELO_ASSERT(name);

		if (start_index == NONE) start_index = 0;

		size_t name_length = strlen(name);

		for (const tag_field *cur = this->fields + start_index; cur->type != Enums::_field_terminator; cur++) {
			if (cur->type == field_type && !strncmp(cur->name, name, name_length))
				return cur - this->fields;
		}

		return NONE;
	}

	tag_field *tag_block_definition::FindField(short field_type, const char *name, long start_index) {
		long index = FindFieldIndex(field_type, name, start_index);

		if (index == NONE) {
			Print<false>("failed to find a %s field named %s in %s", TagGroups::k_tag_field_definitions[field_type].name, name, this->name);
		}

		return &this->fields[index];
	}

	tag_block_definition * tag_block_definition::FindBlockField(const char *name, long start_index) {
		tag_field *block_field = FindField(Enums::_field_block, name, start_index);

		return block_field->Definition<tag_block_definition>();
	}




	// Mainly a visitor for startup/shutdown processes, performs an action (via a functor) on a root block definition
	// then repeats the action for all child blocks (etc)
	// Uses CRT assert()'s since it is assumed this is used after the group definitions have been verified
	// TAction: void operator()([const] tag_block_definition* block_definition)
	template <class TAction>
	void tag_block_definition::DoRecursiveAction(TAction &action) {
		action(this); // perform the action on the root first

		size_t field_index = 0;

		do {
			const tag_field &field = fields[field_index];
			// assert(field.type >= 0 && field.type < Enums::k_number_of_tag_field_types);

			if (field.type == Enums::_field_block) {
				// assert(field.definition != this);

				action(field.Definition<tag_block_definition>());
			}

		} while (fields[field_index++].type != Enums::_field_terminator);
	}

};
