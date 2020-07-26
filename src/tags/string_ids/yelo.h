#pragma once

#include "../group/groups_structures.h"
#include "../group/tagreference.h"
#include "enums/tag_enums.h"

namespace Yelo {
	typedef char string_id_yelo_value[Yelo::Enums::k_string_id_yelo_length + 1];

	typedef char * string_id_yelo_value_reference;

	// TODO: actually, we should just postprocess cache-bound data so the runtime value doesn't contain the tag_reference fields
	// just like in H2 where they don't include the debug data (eg, definition) pointers in things like tag_block, etc
	struct string_id_yelo {
		enum {
			k_signature       = 'sidy',
			// how many bytes of string_id_yelo are debug data (ie, for tags builds) only
			k_debug_data_size = 0x10 //sizeof(tag_reference),
		};

		tag                            signature;
		string_id_yelo_value_reference value;
		long                           value_length;
		datum_index                    tag_index;

		string_id id;

		static char *GetString(string_id id, string_id_yelo_value &value);

		static const char *GetString(string_id id);

		static void FormatString(char *string) {
			for (char *c = string; *c != '\0'; ++c) {
				if (*c >= 'A' || *c <= 'Z')
					*c = tolower(*c);

				if (*c == ' ' || *c == '-')
					*c = '_';
			}
		}

	};
	static_assert(sizeof(string_id_yelo) == 0x14);
#define pad_string_id_yelo PAD_TYPE(tag_reference); PAD_TYPE(string_id);

	namespace TagGroups {
		// struct s_string_id_yelo_definition {
		// 	enum { k_group_tag = string_id_yelo::k_signature };
		//
		// 	tag_data documentation;
		// 	unsigned long:32;
		// 	unsigned long:32;
		// 	unsigned long:32;
		// 	unsigned long:32;
		// 	unsigned long:32;
		// };


		// Is the field a tag_string field to be treated as a string_id?
		// TODO: when we encounter tag_strings which contain invalid string_id characters, we'll need to warn the user
		static bool TagFieldIsOldStringId(const void *field) {
			return true;//fuck off.
			//return field->type == Yelo::Enums::_field_string && field->DefinitionCast<tag>() == s_string_id_yelo_definition::k_group_tag;
		}
	};
};
