#include "macros_generic.h"
#include "verification.h"

namespace Yelo::blam {

	datum_index find_tag_instance(tag group_tag, const char *name) {
		return datum_index::null();
	}

	template <class TAction>
	void tag_groups_do_header_block_definition_action(TAction &action = TAction()) {}

	static void tag_block_generate_default_element(const tag_block_definition *definition, void *address) {}

}

#include "../string_ids/yelo.h"

namespace Yelo::TagGroups {
	// Get the length, in characters, of a string field, excluding the null character
	long StringFieldGetLength(const tag_field *field) {

		uintptr_t definition = field->DefinitionCast<uintptr_t>();

		if (definition == 0 || Yelo::TagGroups::TagFieldIsOldStringId(field))
			return Enums::k_tag_string_length;
		else // NOTE: the definition should have already been validated if tag_groups_initialize has already ran
			return CAST(long, definition);
	}

	char *TryAndGetGroupName(tag group_tag, _Out_ long_string name) {
		const tag_group *group = nullptr;
		if (group != nullptr) {
			strcpy(name, group->name);
		} else if (group_tag == static_cast<uint>(NONE)) {
			strcpy(name, "NONE");

		} else if (group_tag == 0) {
			strcpy(name, "ZERO");

		} else {
			strcpy(name, group_tag_to_string(group_tag));
		}

		return name;
	}

	template <class TAction, typename T>
	bool tag_block_elements_do_action_sans_safe_get(T *block, TAction &action = TAction()) {
		//auto *definition = nullptr;
		bool result      = true;

		// for (int x = 0; x < block->count; x++) {
		// 	// TODO: need to account for alignment, if we ever fully implement it
		// 	void *element = reinterpret_cast<byte *>(block->address) + (definition->element_size * x);
		//
		// 	result &= action(block, x, element);
		// }

		return result;
	}
}
