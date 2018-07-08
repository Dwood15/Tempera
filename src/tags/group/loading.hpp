/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <precompile.h>

/* TODO:
 * use a class to wrap all loading operations, storing all state information as members instead of globals
 * use a producer & consumer model to do asynchronous tag loading
 * tag_get and tag_loaded will need to be modified to block until async loading completes
 * possibly use a new tag_load flag to tell the loader to use synchronous loading, for cases in stock editor code where async loading introduces bugs
*/

namespace Yelo::Enums {
	enum {
		k_tag_group_loading_error_string_length = k_max_tag_name_length * 10,
	};
};

namespace Yelo {
	// tags are terminated by a new line character
	namespace blam {
		bool tag_block_read_recursive(const tag_block_definition *definition, tag_block *block, long *position_reference, unsigned long read_flags,
			// NOTE: nonstandard parameters
												datum_index tag_index);

		static void tag_group_loading_add_non_loaded_tag(tag group_tag, const char * name) {
			char *cursor = *tag_group_loading_error_string_cursor;

			if ((cursor - tag_group_loading_error_string) >= Enums::k_tag_group_loading_error_string_length)
				return;

			int chars = _snprintf_s(tag_group_loading_error_string, Enums::k_tag_group_loading_error_string_length + 1,
											&tag_group_loading_error_string[Enums::k_tag_group_loading_error_string_length] - cursor,
											"\t%s.%s\n", name, tag_group_get(group_tag)->name);

			*tag_group_loading_error_string_cursor += chars;
		}

		const char * tag_load_error_string_get() {
			return tag_group_loading_error_string;
		}

		// clear the error string used to track tags which failed to load
		void tag_load_error_string_clear() {
			*tag_group_loading_error_string_cursor = tag_group_loading_error_string;
			*tag_group_loading_error_string        = '\0';
		}

		// print the tag names which failed to load to the normal error output (as warnings)
		void tag_load_error_string_print() {
			YELO_WARN("need to get the following tags:");
			// print the problem tags by changing the new line char to a NULL char, then back again when finished printing
			for (char *error_string = tag_group_loading_error_string; error_string;) {
				char *new_line = strchr(error_string, '\n');
				if (new_line != nullptr)
					*new_line = '\0';

				YELO_WARN("%s", error_string);

				// no more new lines means no more tags are listed, stop printing
				if (new_line == nullptr)
					break;

				error_string = new_line + 1;
				*new_line = '\n';
			}
		}

		bool __cdecl tag_data_load(void *block_element, tag_data *data, void *address) {
			YELO_ASSERT(block_element);
			YELO_ASSERT(data && data->definition);
			auto *data_definition = data->definition;

			if (address == nullptr) {
				YELO_ASSERT(data->address == nullptr);
				address = data->address = TAG_DATA_NEW(data->definition, data->size);

				if (address == nullptr) {
					YELO_WARN("couldn't allocate memory for #%d bytes %s data (load)",
								 data->size, data_definition->name); // NOTE: added this warning

					return false;
				}
			}

			//YELO_ASSERT( data_definition ); // engine does this, but I don't see why

			if (!tag_file_read(data->stream_position, data->size, address)) {
				YELO_WARN("couldn't read #%d bytes for %s data",
							 data->size, data_definition->name); // NOTE: added this warning

				return false;
			}

			if (data_definition->byte_swap_proc != nullptr)
				data_definition->byte_swap_proc(block_element, address, data->size);

			return true;
		}

		void __cdecl tag_data_unload(void *block_element, tag_data *data) {
			YELO_ASSERT(data && data->definition);
			YELO_ASSERT(data->address);

			TAG_DATA_DELETE(*data);
		}

		static bool tag_data_read_recursive(tag_data_definition *data_definition, void *block_element, tag_data *data,
														long *position_reference, unsigned long read_flags) {
			YELO_ASSERT(data_definition);
			void *data_address = nullptr;
			bool success       = false;
			data->stream_position = *position_reference;
			data->definition      = data_definition;

			if (TEST_FLAG(data_definition->flags, Flags::_tag_data_never_streamed_bit) == false ||
				 TEST_FLAG(read_flags, Flags::_tag_load_for_editor_bit)) {
				int size = data->size;
				if (size < 0 || size > data_definition->maximum_size) {
					YELO_WARN("tag data '%s' size out of range. #%d not in [0,#%d]",
								 data_definition->name, data->size, data_definition->maximum_size); // NOTE: added bounds info to warning
				} else if (!(data_address = TAG_DATA_NEW(data_definition, size))) {
					YELO_WARN("couldn't allocate #%d tag data for '%s'",
								 size, data_definition->name); // NOTE: added size info to warning
				} else if (size > 0 && !tag_data_load(block_element, data, data->address = data_address)) {
					// tag_data_load provides warnings, so we don't provide any here
					TAG_DATA_DELETE(*data);
					data_address = nullptr;
				} else
					success = true;
			} else
				success = true;

			*position_reference += data->size;
			data->address = data_address;
			return success;
		}

		static bool tag_references_resolve_recursive(tag_block_definition *definition, tag_block *block, unsigned long read_flags) {
			bool success = true;

			for (auto element : *block) {
				for (auto field : TagGroups::c_tag_field_scanner(definition->fields, element.address)
					.AddFieldType(Enums::_field_block)
					.AddFieldType(Enums::_field_tag_reference)) {
					bool read_result;

					switch (field.GetType()) {
						case Enums::_field_block:
							read_result = tag_references_resolve_recursive(field.DefinitionAs<tag_block_definition>(), field.As<tag_block>(), read_flags);
							break;

						case Enums::_field_tag_reference:
							read_result = tag_reference_resolve_recursive(field.DefinitionAs<tag_reference_definition>(), field.As<tag_reference>(), read_flags);
							break;

							YELO_ASSERT_CASE_UNREACHABLE();
					}

					if (!read_result)
						success = read_result;
				}
			}

			return success;
		}

		static bool verify_tag_string_field(TagGroups::c_tag_field_scanner::s_iterator_result field,
														tag_block_definition *block_definition, long element_index) {
			auto  &string       = *field.As<tag_string>();
			long string_length = field.GetStringFieldLength(); // For supporting non-standard tag_string lengths

			if (strnlen_s(string, string_length) == string_length &&
				 string[string_length] != '\0') {
				YELO_WARN("fixed corrupt tag string field (%s) in %s #%d.",
							 field.GetName(), block_definition->name, element_index); // NOTE: added block info

				string[string_length] = '\0';
				return false;
			}

			// engine turns non-printable characters into underscores and nulls trailing spaces
			// TODO: shouldn't we note when we correct non-printable characters?
			bool     non_space_character_found;
			for (int x          = CAST(int, strlen(string)) - 1; x >= 0; x--) {
				char &c = string[x];

				if (!isprint(c))
					c = '_';

				if (isspace(c)) {
					if (!non_space_character_found)
						c = '\0';
				} else
					non_space_character_found = true;
			}

			return true;
		}

		static bool tag_reference_read_recursive(tag_reference_definition *definition, tag_reference *reference,
															  long *position_reference, unsigned long read_flags) {
			// NOTE: engine doesn't ASSERT anything

			if (reference->group_tag == 0) // handles cases were tag_reference fields were added to old useless padding
				reference->group_tag = definition->group_tag;

			reference->tag_index = datum_index::null();

			if (!(reference->name = reinterpret_cast<char *>(YELO_MALLOC(Enums::k_max_tag_name_length + 1, false)))) {
				YELO_WARN("couldn't allocate name memory for tag_reference @%p", definition); // NOTE: added this warning

				return false;
			}

			reference->name[0] = '\0';
			int name_length = reference->name_length;
			if (name_length == 0) {
				return true;
			} else if (name_length < 0 || name_length > Enums::k_max_tag_name_length) {
				YELO_WARN("tag reference name too large #%d (this tag is corrupted).", name_length); // NOTE: added length info to warning

				return false;
			} else if (!tag_file_read(*position_reference, name_length + 1, reference->name)) {
				YELO_WARN("couldn't read #%d characters for @%p tag reference name", name_length + 1, definition); // NOTE: added this warning

				return false;
			} else if (strlen(reference->name) != name_length) {
				// NOTE: engine uses the 'too large' warning for this case
				YELO_WARN("tag reference name @%X doesn't match length #%d (this tag is corrupted).", *position_reference, name_length); // NOTE: added length info to warning

				return false;
			}

			_strlwr(reference->name);
			*position_reference += name_length + 1;
			return true;
		}

		template <typename TEnum>
		static bool verify_enum_field(TagGroups::c_tag_field_scanner::s_iterator_result field,
												tag_block_definition *block_definition, long element_index) {
			auto  &value = *field.As<TEnum>();
			long count  = field.DefinitionAs<string_list>()->count;
			if (value >= 0 && value < count)
				return true;

			YELO_WARN("fixed corrupt tag enum field (%s) in %s #%d.", field.GetName(), block_definition->name, element_index); // NOTE: added block info

			value = 0;
			return false;
		}

		template <typename TFlags>
		static bool verify_flags_field(TagGroups::c_tag_field_scanner::s_iterator_result field,
												 tag_block_definition *block_definition, long element_index) {
			auto  &flags    = *field.As<TFlags>();
			long bit_count = field.DefinitionAs<string_list>()->count;
			if (bit_count >= BIT_COUNT(TFlags) || (flags >> bit_count) == 0)
				return true;

			YELO_WARN("fixed corrupt tag flags field (%s) in %s #%d.", field.GetName(), block_definition->name, element_index); // NOTE: added block info

			flags &= MASK(bit_count);
			return false;
		}

		template <typename TIndex>
		static bool verify_block_index_field(TagGroups::c_tag_field_scanner::s_iterator_result field,
														 tag_block_definition *block_definition, long element_index) {
			auto &index                    = *field.As<TIndex>();
			auto *indexed_block_definition = field.DefinitionAs<tag_block_definition>();
			if (index >= NONE && index < indexed_block_definition->maximum_element_count)
				return true;

			YELO_WARN("fixed corrupt tag block index field (%s) in %s #%d (reset to NONE).", field.GetName(), block_definition->name, element_index); // NOTE: added block info

			index = NONE;
			return false;
		}

		bool tag_block_read_children_recursive(const tag_block_definition *definition, void *address, long count,
															long *position_reference, unsigned long read_flags,
			// NOTE: nonstandard parameters
															datum_index tag_index) {
			bool success = true;
			if (count == 0)
				return success;

			for (int x = 0; x < count; x++) {
				void      *block_element = reinterpret_cast<byte *>(address) + (definition->element_size * x);
				for (auto field : TagGroups::c_tag_field_scanner(definition->fields, block_element)
					.AddFieldType(Enums::_field_block)
					.AddFieldType(Enums::_field_data)
					.AddFieldType(Enums::_field_tag_reference)) {
					bool read_result;

					switch (field.GetType()) {
						case Enums::_field_data:
							read_result =
								tag_data_read_recursive(field.DefinitionAs<tag_data_definition>(), block_element,
																field.As<tag_data>(),
																position_reference, read_flags);
							break;

						case Enums::_field_block:
							read_result =
								tag_block_read_recursive(field.DefinitionAs<tag_block_definition>(),
																 field.As<tag_block>(),
																 position_reference, read_flags,
																 tag_index);
							break;

						case Enums::_field_tag_reference:
							read_result =
								tag_reference_read_recursive(field.DefinitionAs<tag_reference_definition>(),
																	  field.As<tag_reference>(),
																	  position_reference, read_flags);
							break;

							YELO_ASSERT_CASE_UNREACHABLE();
					}

					if (!read_result)
						success = read_result;
				}
			}

			return success;
		}

		static bool tag_block_postprocess(const tag_block_definition *definition, tag_block *block,
													 Enums::tag_postprocess_mode mode,
			// NOTE: nonstandard parameters
													 datum_index tag_index) {
			auto proc    = definition->postprocess_proc;
			bool success = true;

			if (proc != nullptr) {
				for (int x = 0; x < block->count; x++) {
					void *element = tag_block_get_element(block, x);
					if (!proc(element, mode, tag_index)) {
						YELO_WARN("%s: failed to postprocess element #%d element for %s block",
									 tag_try_get_name(tag_index),
									 x, definition->name); // NOTE: added this warning message

						success = false;
					}
				}
			}

			return success;
		}

		static bool tag_block_verify_recursive(tag_block_definition *definition, tag_block *block) {
			bool valid = true;

			for (int x = 0; x < block->count; x++) {
				void *element = reinterpret_cast<byte *>(block->address) +
									 (definition->element_size * x); // NOTE: engine doesn't use tag_block_get_element here

				for (auto field : TagGroups::c_tag_field_scanner(definition->fields, element)
					.AddAllFieldTypes()) {
					switch (field.GetType()) {
						case Enums::_field_string:
							if (!verify_tag_string_field(field, definition, x)) valid = false;
							break;

							// NOTE: technically the engine treats enum fields as signed
						case Enums::_field_enum:
							if (!verify_enum_field<short>(field, definition, x)) valid = false;
							break;

							// NOTE: engine only verified long_flags, we added support for the others
						case Enums::_field_long_flags:
							if (!verify_flags_field<long_flags>(field, definition, x)) valid = false;
							break;
						case Enums::_field_word_flags:
							if (!verify_flags_field<unsigned short>(field, definition, x)) valid = false;
							break;
						case Enums::_field_byte_flags:
							if (!verify_flags_field<byte_flags>(field, definition, x)) valid = false;
							break;

						case Enums::_field_pad:
							std::memset(field.GetAddress(), 0, field.GetSize());
							break;

						case Enums::_field_block:
							if (!tag_block_verify_recursive(field.DefinitionAs<tag_block_definition>(), field.As<tag_block>()))
								valid = false;
							break;

							// NOTE: engine doesn't verify block indices, we added support for them
							// TODO: should we enable this as an option via XML setting instead?
						case Enums::_field_short_block_index:
							if (!verify_block_index_field<short>(field, definition, x)) valid = false;
							break;
						case Enums::_field_long_block_index:
							if (!verify_block_index_field<long>(field, definition, x)) valid = false;
							break;
					}
				}
			}

			return valid;
		}

		static void tag_group_postprocess_instance(datum_index tag_index, Enums::tag_postprocess_mode mode) {
			for (const tag_group *group = TagGroups::TagGetGroup(tag_index);
				  group != nullptr;
				  group = tag_group_get(group->parent_group_tag)) {
				if (group->child_count > 0 && group->header_block_definition->postprocess_proc != nullptr) {
					group->header_block_definition->postprocess_proc(tag_get(NONE, tag_index), mode,
																					 tag_index // NOTE: non-standard parameter
					);
				}

				auto proc = group->postprocess_proc;
				if (proc != nullptr)
					proc(tag_index, mode);
			}
		}

		static void tag_instance_setup_groups_and_name(s_tag_instance *instance, const tag_group *group, const char * name) {
			strncpy_s(instance->filename, name, Enums::k_max_tag_name_length);
			instance->filename[Enums::k_max_tag_name_length] = '\0';
			instance->group_tag = group->group_tag;

			if ((instance->parent_group_tags[0] = group->parent_group_tag) != NONE)
				instance->parent_group_tags[1] = tag_group_get(group->parent_group_tag)->parent_group_tag;
			else
				instance->parent_group_tags[1] = NONE;
		}

		static bool tag_reference_resolve_recursive(tag_reference_definition *definition, tag_reference *reference, unsigned long read_flags) {
			datum_index tag_index = datum_index::null();
			bool        success   = true;

			if (TagGroups::g_gbxmodel_group_enabled && reference->group_tag == TagGroups::model_definition::k_group_tag)
				reference->group_tag = TagGroups::gbxmodel_definition::k_group_tag;

			if (reference->group_tag != NONE &&
				 TEST_FLAG(read_flags, Flags::_tag_load_non_resolving_references_bit) == false &&
				 TEST_FLAG(definition->flags, Flags::_tag_reference_non_resolving_bit) == false
				) {
				if (*reference->name != '\0' ||
					 (TEST_FLAG(read_flags, Flags::_tag_load_for_editor_bit) == false &&
					  TEST_FLAG(definition->flags, Flags::_tag_reference_unknown0_bit))
					) {
					tag_index = tag_load(reference->group_tag, reference->name, 0);
					success   = !tag_index.IsNull();
				}
			}

			reference->tag_index = tag_index;
			return success;
		}

		static bool tag_block_read_recursive(const tag_block_definition *definition, tag_block *block,
														 long *position_reference, unsigned long read_flags,
			// NOTE: nonstandard parameters
														 datum_index tag_index) {
			int count = block->count;
			block->address    = nullptr;
			block->definition = definition;
			if (count < 0 || count > definition->maximum_element_count) {
				YELO_WARN("%s block has invalid element count: #%d not in [0,#%d]",
							 definition->name, count, definition->maximum_element_count);

				block->count = 0;
				return false;
			} else if (count == 0)
				return true;

			size_t elements_size = definition->element_size * count;
			if (!(block->address = TAG_BLOCK_NEW(definition, count))) {
				YELO_WARN("couldn't allocate memory for #%d element %s block",
							 block->count, definition->name);

				return false;
			}

			if (!tag_file_read(*position_reference, elements_size, block->address)) {
				YELO_WARN("couldn't read #%d elements for %s block",
							 block->count, definition->name); // NOTE: added count info to error

				return false;
			}

			byte_swap_data_explicit(definition->name, definition->element_size, definition->byte_swap_codes,
											block->count, block->address);

			*position_reference += elements_size;

			if (TEST_FLAG(definition->flags, Flags::_tag_block_has_children_bit)) {
				if (!tag_block_read_children_recursive(definition, block->address, block->count,
																	position_reference, read_flags, tag_index))
					return false;
			}

			// engine doesn't do this logic, but they also don't use an enum for 'postprocess_mode'
			auto postprocess_mode = TEST_FLAG(read_flags, Flags::_tag_load_for_editor_bit)
											? Enums::_tag_postprocess_mode_for_editor
											: Enums::_tag_postprocess_mode_for_runtime;
			if (!tag_block_postprocess(definition, block, postprocess_mode, tag_index))
				return false; // postprocess warns about failures, so we don't YELO_WARN here

			return true;
		}

		datum_index __cdecl tag_new(tag group_tag, const char * name) {
			YELO_ASSERT(name);

			tag_group *group = tag_group_get(group_tag);
			// engine just asserts here: YELO_ASSERT(group);
			YELO_ASSERT_DISPLAY(group != nullptr, "the group tag '%s' does not exist (can't create '%s')",
									  TagGroups::group_tag_to_string{group_tag}.ToString(), name);

			// NOTE: engine asserts child_count, but this will force guerilla to crash
			// Originally, we NOP'd the assert in CheApe
			if (group->child_count != 0) {
				YELO_WARN("can't create the parent tag %s, not creating %s", group->name, name);
				return datum_index::null();
			}

			datum_index tag_index = find_tag_instance(group_tag, name);
			if (!tag_index.IsNull()) {
				YELO_WARN("there is already a %s tag named '%s' loaded",
							 group->name, name);
				return datum_index::null();
			}

			tag_index = TagGroups::TagInstances().New();
			if (tag_index.IsNull()) {
				YELO_WARN("there are no more free tag slots for new %s tag file '%s'.",
							 group->name, name);
				return datum_index::null();
			}

			tag_block_definition *root_definition = group->header_block_definition;
			void                 *root_element    = TAG_BLOCK_NEW(root_definition, 1);
			do { // 'break' on error

				if (root_element == nullptr) {
					YELO_WARN("couldn't allocate memory for new %s tag '%s'.",
								 group->name, name);

					break;
				}

				auto *instance = TagGroups::TagInstances()[tag_index];
				tag_instance_setup_groups_and_name(instance, group, name);
				instance->is_read_only          = tag_file_read_only(group_tag, name);
				instance->reload_index          = datum_index::null();
				instance->root_block.count      = 1;
				instance->root_block.address    = root_element;
				instance->root_block.definition = root_definition;
				tag_block_generate_default_element(root_definition, root_element);

				long position = root_definition->element_size;
				if (!tag_block_read_children_recursive(root_definition, root_element, 1,
																	&position, FLAG(Flags::_tag_load_for_editor_bit), tag_index)) {
					YELO_WARN("couldn't create new %s tag '%s'.",
								 group->name, name); // NOTE: included group name

					break;
				}

				tag_group_postprocess_instance(tag_index, Enums::_tag_postprocess_mode_for_editor);
				return tag_index;

			} while (false);

			TagGroups::TagInstances().Delete(tag_index);

			if (root_element != nullptr)
				TAG_BLOCK_DELETE_ELEMENTS(root_definition, root_element);

			return datum_index::null();
		}

		datum_index __cdecl tag_load(tag group_tag, const char * name, unsigned long flags) {
			YELO_ASSERT(name);

			tag_group *group = tag_group_get(group_tag);
			if (group == nullptr) {
				YELO_WARN("the group tag '%s' does not exist (can't load '%s')",
							 TagGroups::group_tag_to_string{group_tag}.ToString(), name);
				return datum_index::null();
			}

			// NOTE: where tool does the zoners model upgrade check
			if (TagGroups::g_gbxmodel_group_enabled && group_tag == TagGroups::model_definition::k_group_tag)
				group_tag = TagGroups::gbxmodel_definition::k_group_tag;

			datum_index tag_index = find_tag_instance(group_tag, name);
			do {
				if (!tag_index.IsNull())
					break;

				bool   is_readonly;
				uint checksum;
				if (!tag_file_open(group_tag, name, &is_readonly, &checksum, TEST_FLAG(flags, Flags::_tag_load_from_file_system_bit))) {
					tag_group_loading_add_non_loaded_tag(group_tag, name);

					break;
				}

				tag_index = TagGroups::TagInstances().New();
				if (tag_index.IsNull()) {
					YELO_WARN("there are no more free tag slots to load %s tag file '%s'.",
								 group->name, name); // NOTE: added tag info to message

					break;
				}

				auto *instance = TagGroups::TagInstances()[tag_index];
				tag_instance_setup_groups_and_name(instance, group, name);

				instance->is_verified = TEST_FLAG(flags, Flags::_tag_load_for_editor_bit);
				// engine doesn't do this logic, but they also don't use an enum for 'postprocess_mode'
				auto postprocess_mode = TEST_FLAG(flags, Flags::_tag_load_for_editor_bit)
												? Enums::_tag_postprocess_mode_for_editor
												: Enums::_tag_postprocess_mode_for_runtime;
				instance->reload_index          = datum_index::null();
				instance->is_read_only          = is_readonly;
				instance->file_checksum         = checksum;
				instance->root_block.count      = 1;
				instance->root_block.address    = nullptr;
				instance->root_block.definition = group->header_block_definition;

				const char * failed_to_load_reason = nullptr;
				long   position              = 0;
				if (!tag_block_read_recursive(group->header_block_definition, &instance->root_block, &position, flags, tag_index))
					failed_to_load_reason = "read error";
				else if (!tag_references_resolve_recursive(group->header_block_definition, &instance->root_block, flags))
					failed_to_load_reason = "resolve children error";

				if (failed_to_load_reason != nullptr) {
					YELO_WARN("failed to load %s tag '%s' (%s)",
								 group->name, name, failed_to_load_reason);

					TagGroups::TagInstances().Delete(tag_index);

					tag_index = datum_index::null();
					break;
				}

				if (TEST_FLAG(flags, Flags::_tag_load_for_editor_bit) &&
					 !tag_block_verify_recursive(group->header_block_definition, &instance->root_block)) {
					YELO_WARN("the %s tag '%s' may be corrupt.",
								 group->name, name);
				}

				tag_group_postprocess_instance(tag_index, postprocess_mode);
			} while (false);

			tag_files_flush();
			return tag_index;
		}

		void __cdecl tag_unload(datum_index tag_index) {
			auto *instance = TagGroups::TagInstances()[tag_index];

			if (instance->root_block.count > 0 && !instance->is_reload)
				tag_block_delete_element(&instance->root_block, 0);

			TagGroups::TagInstances().Delete(tag_index);
		}

		datum_index __cdecl tag_reload(tag group_tag, const char * name) {
			tag_group   *group    = tag_group_get(group_tag);
			datum_index tag_index = datum_index::null();

			if (!TEST_FLAG(group->flags, Flags::_tag_group_reloadable_bit))
				return tag_index;

			tag_index = find_tag_instance(group_tag, name);
			if (tag_index.IsNull())
				return tag_load(group_tag, name, 0);

			auto *instance = TagGroups::TagInstances()[tag_index];
			instance->is_orphan = true;

			// this instance was already reloaded once before, clear its old reloaded instance
			if (!instance->reload_index.IsNull())
				tag_unload(instance->reload_index);
			instance->reload_index = datum_index::null();

			datum_index reload_tag_index = tag_load(group_tag, name,
																 instance->is_verified ? FLAG(Flags::_tag_load_for_editor_bit) : 0);
			if (reload_tag_index.IsNull()) {
				// TODO: shouldn't we log here?

				instance->is_orphan = false;
				return datum_index::null();
			}

			auto *reloaded_instance = TagGroups::TagInstances()[reload_tag_index];
			tag_block_delete_element(&instance->root_block, 0);
			std::memcpy(&instance->root_block, &reloaded_instance->root_block, sizeof(instance->root_block));
			reloaded_instance->is_reload = true;
			instance->is_orphan          = false;
			instance->reload_index       = reload_tag_index;

			return tag_index;
		}
	};
};
