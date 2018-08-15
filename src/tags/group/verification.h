/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
// NOTE: NON-STANDARD ENGINE SOURCE FILE
#pragma once

#include "groups_structures.h"
#include "tag_groups.h"

namespace Yelo {
	namespace TagGroups {
		// called in the 'verify group fields' initialize step on tag_reference_definitions
		static void TagGroupMarkAsReferenced(tag group_tag) { }

		void CheckForUnreferencedGroups() { /*#if false'd in YeloLib*/  }
		
		//////////////////////////////////////////////////////////////////////////
		// Everything below this comment is based on engine code
		// I've left it out of the 'blam' namespace due to the amount of structural changes made
		// and due to the fact that we call them from elsewhere besides the tag_groups_system code

		static void VerifyStringListDefinition(const string_list *definition, const tag_block_definition *block_definition, const char *field_type_name) { }

		template <typename TEnum>
		static void VerifyEnumFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) { }

		template <typename TFlags>
		static void VerifyFlagsFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) { }

		static void VerifyTagReferenceFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) { 	}

		static void VerifyDataFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) { 	}

		// Verify a field that is a child of [block_definition]
		static void VerifyTagField(const tag_block_definition *block_definition, const tag_field &field) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
			switch (field.type) {
				case Enums::_field_string: {
					// uintptr_t definition = reinterpret_cast<uintptr_t>(field.definition);
					//
					// YELO_ASSERT(definition == 0 || definition <= Enums::k_long_string_length || TagFieldIsOldStringId(&field));
				}
					break;

				case Enums::_field_enum:
					VerifyEnumFieldDefinition<short>(field, block_definition);
					break;

				case Enums::_field_long_flags:
					VerifyFlagsFieldDefinition<long_flags>(field, block_definition);
					break;
				case Enums::_field_word_flags:
					VerifyFlagsFieldDefinition<unsigned short>(field, block_definition);
					break;
				case Enums::_field_byte_flags:
					VerifyFlagsFieldDefinition<byte_flags>(field, block_definition);
					break;

				case Enums::_field_tag_reference:
					VerifyTagReferenceFieldDefinition(field, block_definition);
					break;

				case Enums::_field_block: {
					// auto *definition = field.Definition<tag_block_definition>();
					// YELO_ASSERT_DISPLAY(definition, "no definition specified for block field in block %s.", block_definition->name); // NOTE: added owner block name to info

					// VerifyBlockFieldDefinitions(definition);
				}
					break;

				case Enums::_field_data:
					VerifyDataFieldDefinition(field, block_definition);
					break;

				case Enums::_field_short_block_index:
				case Enums::_field_long_block_index: {
					// auto *definition = field.Definition<tag_block_definition>();
					// YELO_ASSERT_DISPLAY(definition, "no definition specified for block index field in block %s.", block_definition->name); // NOTE: added owner block name to info
				}
					break;

				case Enums::_field_array_start:
				case Enums::_field_pad:
				case Enums::_field_skip:;
					break;

				case Enums::_field_explanation: {
					// const char *definition = field.Definition<const char>();
					// // NOTE: added owner block name to info
					// YELO_ASSERT_DISPLAY(definition, "no definition specified for explanation field in block %s.", block_definition->name);
				}
					break;
			}
#pragma clang diagnostic pop
		}

		// Verify [block] and its child fields
		static void VerifyBlockFieldDefinitions(tag_block_definition *block) {

		}

		static void VerifyTagGroupTags() {

		}

		static void VerifyGroupFieldDefinitions() {

		}

		// Run any checks on the tag groups, after they have been fully initialized (parents linked, byte swap codes, etc).
		// However, runtime info won't yet be generated at the time of this call
		void VerifyTagGroupsFinalChecks() {}
	};
};
