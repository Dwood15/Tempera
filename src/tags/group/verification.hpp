/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
// NOTE: NON-STANDARD ENGINE SOURCE FILE
#pragma once

#include <precompile.h>
#include "groups_structures.hpp"
#include "tag_groups.h"

namespace Yelo {
	namespace TagGroups {
		// called in the 'verify group fields' initialize step on tag_reference_definitions
		static void TagGroupMarkAsReferenced(tag group_tag) {
			SET_FLAG(blam::tag_group_get(group_tag)->flags, Flags::_tag_group_referenced_yelo_bit, true);
		}

		void CheckForUnreferencedGroups() { /*#if false'd in YeloLib*/  }


		//////////////////////////////////////////////////////////////////////////
		// Everything below this comment is based on engine code
		// I've left it out of the 'blam' namespace due to the amount of structural changes made
		// and due to the fact that we call them from elsewhere besides the tag_groups_system code

		static void VerifyStringListDefinition(const string_list *definition,
															const tag_block_definition *block_definition, const char * field_type_name) {
			YELO_ASSERT_DISPLAY(definition, "no definition specified for %s field in block %s.",
									  field_type_name, block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT(definition->count >= 0);

			for (int x = 0; x < definition->count; x++) {
				YELO_ASSERT_DISPLAY(definition->strings[x] != nullptr, "%s field in block %s doesn't have enough strings",
										  field_type_name, block_definition->name);
			}
		}

		template <typename TEnum>
		static void VerifyEnumFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) {
			auto *definition = field.Definition<string_list>();
			VerifyStringListDefinition(definition, block_definition, "enum");
		}

		template <typename TFlags>
		static void VerifyFlagsFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) {
			auto *definition = field.Definition<string_list>();
			VerifyStringListDefinition(definition, block_definition, "flags");

			YELO_ASSERT_DISPLAY(definition->count <= BIT_COUNT(TFlags), "invalid string list specified for '%s' in block %s.", field.name, block_definition->name); // NOTE: added owner block name to info
		}

		static void VerifyTagReferenceFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) {
			auto *definition = field.Definition<tag_reference_definition>();
			YELO_ASSERT_DISPLAY(definition, "no definition specified for tag reference field in block %s.",
									  block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT(VALID_FLAGS(definition->flags, Flags::k_number_of_tag_group_tag_reference_flags));

			group_tag_to_string gt_string;
			gt_string.Terminate();

			if (definition->group_tag != NONE) {
				gt_string.group = definition->group_tag;
				YELO_ASSERT_DISPLAY(blam::tag_group_get(definition->group_tag),
										  "invalid group tag '%s' for tag reference field '%s' in block %s",
										  gt_string.TagSwap().str, field.name, block_definition->name);
				YELO_ASSERT(definition->group_tags == nullptr);

				TagGroupMarkAsReferenced(definition->group_tag);
			} else if (definition->group_tags != nullptr) {
				for (auto group_tag : *definition) {
					gt_string.group = group_tag;
					YELO_ASSERT_DISPLAY(blam::tag_group_get(group_tag),
											  "invalid group tag '%s' for variable tag reference field '%s' in block %s",
											  gt_string.TagSwap().str, field.name, block_definition->name);

					TagGroupMarkAsReferenced(group_tag);
				}
			}
		}

		static void VerifyDataFieldDefinition(const tag_field &field, const tag_block_definition *block_definition) {
			auto *definition = field.Definition<tag_data_definition>();

			// NOTE: added owner block name to info
			YELO_ASSERT_DISPLAY(definition, "no definition specified for tag_data field in block %s.", block_definition->name);
			YELO_ASSERT(definition->name);
			YELO_ASSERT(VALID_FLAGS(definition->flags, Flags::k_number_of_tag_data_definition_flags));
			YELO_ASSERT(definition->maximum_size > 0);
		}

		// Verify a field that is a child of [block_definition]
		static void VerifyTagField(const tag_block_definition *block_definition, const tag_field &field) {
			switch (field.type) {
				case Enums::_field_string: {
					uintptr_t definition = reinterpret_cast<uintptr_t>(field.definition);

					YELO_ASSERT(definition == 0 || definition <= Enums::k_long_string_length ||
									TagFieldIsOldStringId(&field));
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
					auto *definition = field.Definition<tag_block_definition>();
					YELO_ASSERT_DISPLAY(definition, "no definition specified for block field in block %s.",
											  block_definition->name); // NOTE: added owner block name to info

					VerifyBlockFieldDefinitions(definition);
				}
					break;

				case Enums::_field_data:
					VerifyDataFieldDefinition(field, block_definition);
					break;

				case Enums::_field_short_block_index:
				case Enums::_field_long_block_index: {
					auto *definition = field.Definition<tag_block_definition>();
					YELO_ASSERT_DISPLAY(definition, "no definition specified for block index field in block %s.",
											  block_definition->name); // NOTE: added owner block name to info
				}
					break;

				case Enums::_field_array_start:
				case Enums::_field_pad:
				case Enums::_field_skip:;
					break;

				case Enums::_field_explanation: {
					const char * definition = field.Definition<const char>();
					// NOTE: added owner block name to info
					YELO_ASSERT_DISPLAY(definition, "no definition specified for explanation field in block %s.", block_definition->name);
				}
					break;
			}
		}

		// Verify [block] and its child fields
		static void VerifyBlockFieldDefinitions(tag_block_definition *block) {
			YELO_ASSERT(block);
			YELO_ASSERT(block->name);
			YELO_ASSERT(VALID_FLAGS(block->flags, Flags::k_number_of_tag_block_definition_flags));
			YELO_ASSERT(block->maximum_element_count >= 0);
			YELO_ASSERT(CAST(int, block->element_size) >= 0);
			YELO_ASSERT(block->fields);

			struct verify_tag_field_action {
				void operator ()(const tag_block_definition *block, const tag_field &field) const {
					VerifyTagField(block, field);
				}
			};

			block->FieldsDoAction<verify_tag_field_action, true>();
		}

		static void VerifyTagGroupTags() {
			struct verify_no_duplicate_group_tags_action {
				group_tag_to_string m_group_string;

				verify_no_duplicate_group_tags_action() {
					m_group_string.Terminate();
				}

				void operator ()(tag_group *group) {
					m_group_string.group = group->group_tag;
					m_group_string.TagSwap();
					YELO_ASSERT_DISPLAY(group == blam::tag_group_get(group->group_tag), "there are two groups using the tag '%s'.",
											  m_group_string.str);
				}
			};

			TagGroups::tag_groups_do_action<verify_no_duplicate_group_tags_action>();
		}

		static void VerifyGroupFieldDefinitions() {
			struct verify_group_field_definitions_action
			{
				bool m_found_group_missing_header_definition;

				verify_group_field_definitions_action() : m_found_group_missing_header_definition(false) { }

				void operator()(const tag_group* group)
				{
					if(group->header_block_definition != nullptr)
						VerifyBlockFieldDefinitions(group->header_block_definition);
					else
					{	// NOTE: added this warning
						YELO_WARN("tag group '%s' doesn't have a definition", group->name);
						m_found_group_missing_header_definition = true;
					}
				}
			};

			auto action = verify_group_field_definitions_action();
			TagGroups::tag_groups_do_action(action);

			if(action.m_found_group_missing_header_definition)
			{
				YELO_ASSERT( !"fix your goddamn tag groups" );
			}
		}

		// Run any checks on the tag groups, after they have been fully initialized (parents linked, byte swap codes, etc).
		// However, runtime info won't yet be generated at the time of this call
		void VerifyTagGroupsFinalChecks() { }
	};
};
