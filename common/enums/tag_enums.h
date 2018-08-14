#pragma once
#include "hs_enums.h"

namespace Yelo::Enums {
	/// <summary>	when true, all 'model' references are loaded or get as gbxmodels </summary>
	constexpr bool g_gbxmodel_group_enabled = true;
	constexpr int	k_tag_string_length  = 31; // character count in a [tag_string] type
	constexpr int	k_long_string_length = 255; // character count in a [long_string] type
	constexpr int	k_string_id_length = 127;
	constexpr int	k_string_64_length  = 63;
	constexpr int	k_string_128_length = 127;
	constexpr int	k_string_256_length = 255;

	constexpr int k_max_tag_name_length   = 255;
	constexpr int k_string_id_yelo_length = k_max_tag_name_length;
	constexpr int k_tag_block_format_buffer_size = 512;
	constexpr int k_maximum_tags_per_tag_chain = 4;
	constexpr int k_maximum_children_per_tag   = 16;
	constexpr int k_tag_group_loading_error_string_length = k_max_tag_name_length * 10;

	enum {
		k_maximum_field_byte_swap_codes = 1024,

		// these chars should all match the TAG_FIELD_MARKUP_* defines in tag_groups_structures_macros.hpp

		k_tag_field_markup_character_advanced     = '!',
		k_tag_field_markup_character_help_prefix  = '#',
		k_tag_field_markup_character_read_only    = '*',
		k_tag_field_markup_character_units_prefix = ':',
		k_tag_field_markup_character_block_name   = '^',
	};

	enum field_type : short {
		_field_string,
		_field_char_integer,
		_field_short_integer,
		_field_long_integer,
		_field_angle,
		_field_tag,
		_field_enum,
		_field_long_flags,
		_field_word_flags,
		_field_byte_flags,
		_field_point_2d,
		_field_rectangle_2d,
		_field_rgb_color,
		_field_argb_color,
		_field_real,
		_field_real_fraction,
		_field_real_point_2d,
		_field_real_point_3d,
		_field_real_vector_2d,
		_field_real_vector_3d,
		_field_real_quaternion,
		_field_real_euler_angles_2d,
		_field_real_euler_angles_3d,
		_field_real_plane_2d,
		_field_real_plane_3d,
		_field_real_rgb_color,
		_field_real_argb_color,
		_field_real_hsv_color,
		_field_real_ahsv_color,
		_field_short_bounds,
		_field_angle_bounds,
		_field_real_bounds,
		_field_real_fraction_bounds,
		_field_tag_reference,
		_field_block,
		_field_short_block_index,
		_field_long_block_index,
		_field_data,
		_field_array_start,
		_field_array_end,
		_field_pad,
		_field_skip,
		_field_explanation,
		_field_custom,
		_field_terminator,

		k_number_of_tag_field_types,
	};

	// Note: AFAICT, the engine code doesn't actually do the postprocess setup this way.
	// They have what is essentially a boolean parameter that could be considered as 'bool for_editor'
	enum tag_postprocess_mode : unsigned char {
		// In this mode, the tag is being postprocessed for runtime values (automatically fill fields, etc)
			_tag_postprocess_mode_for_runtime = 0,
		// In this mode we're opening for tag editing (eg, tool process or guerilla) and should skip the postprocessing
		// code which prepares the tag for use in-game (Sapien and when building a cache)
			_tag_postprocess_mode_for_editor  = 1,
	};
};

namespace Yelo::Flags {
	enum {
		// Never streamed, unless the tag is loaded with _tag_load_for_editor_bit
		// Used by the sound tag for delay loading the actual sample data. So, eg, when tool goes to build a cache
		// it has to use tag_data_load on the sound samples. Maybe a better name is just 'lazy_loaded'
			_tag_data_never_streamed_bit = 0,
			_tag_data_is_text_data_bit,
		// ie, 'debug data'
			_tag_data_not_streamed_to_cache_bit,
			k_number_of_tag_data_definition_flags,

		// checked in the tag reference solving code.
		// last condition checked after an assortment of conditionals
		// and if this is FALSE, it won't resolve
		// NOTE: I think this a deprecated flag for loading the 'default' definition of a group.
		// This flag would cause a call of tag_load(group_tag, NULL, 0) to occur. However,
		// tag_load asserts name != NULL.
		// Flag isn't checked in H2EK's code (so more than likely deprecated)
			_tag_reference_unknown0_bit = 0,
			_tag_reference_non_resolving_bit,
			k_number_of_tag_group_tag_reference_flags,

		// set when block/data/reference fields are found during initialization
			_tag_block_has_children_bit = 0,
			k_number_of_tag_block_definition_flags,

		// tag_instance of the tag group will have their file_checksum CRC'd into the resulting cache tag header's crc field
			_tag_group_include_in_tags_checksum_bit = 0,
		// only seen preferences_network_game use this
			_tag_group_unknown1_bit,
		// haven't seen this used at all
			_tag_group_unknown2_bit,
		// majority of tags have this set
			_tag_group_reloadable_bit,

		// YELO ONLY! tag_group is not meant for use in runtime builds
			_tag_group_debug_only_yelo_bit,
		// YELO ONLY! tag_group is non-standard (ie, official). This is applied to those defined in CheApe.map
		// Set at startup.
			_tag_group_non_standard_yelo_bit,
		// YELO ONLY! tag_group is referenced in other groups via a tag_reference or as a parent group
		// doesn't consider tag_references which can reference ANYTHING (eg, tag_collection's field).
		// Set at startup.
			_tag_group_referenced_yelo_bit,

		// When this is set, implies _tag_postprocess_mode_for_editor, else _for_runtime
			_tag_load_for_editor_bit = 0,
		// Load the tag from the file system, not from a cache/index
			_tag_load_from_file_system_bit,
		// If set: child references of the tag being loaded are not loaded themselves
		// Else, child references are loaded from disk
			_tag_load_non_resolving_references_bit,

		k_protected_group_tag = 'prot', // HEK+: this overrides the scenario's group tag in the tag index.

	};
};

namespace Yelo::_string_id {
	enum {
		k_index_bit_count  = 19,
		k_id_set_bit_count = 6,

		k_index_bit_mask = ( (unsigned)(1 << (k_index_bit_count)) - (unsigned)1 ),
		k_id_bit_mask    =( (unsigned)(1 << (k_id_set_bit_count)) - (unsigned)1 ),

		k_index_bit_shift = 0,
		k_id_bit_shift    = k_index_bit_count,

		k_maximum_set_values  = (1 << k_index_bit_count) >> 7,
		k_maximum_set_storage = k_maximum_set_values * (Enums::k_string_id_length + 1),

		_string_id_invalid = 0,
	};

	enum {
		_set_global,
		// instead of appending the cache's string ids to the 'global' set, we set aside an entire set
		_set_cache,
		_set_gui,
		_set_gui_alert,
		_set_gui_dialog,
		_set_properties,
		_set_components,
		_set_game_engine,
		_set_incident,
		_set_os_settings,
		_set_unused10,
		_set_unused11,
		_set_unused12,
		_set_unused13,
		_set_unused14,
		_set_unused15,
		_set_unused16,
		_set_unused17,
		_set_unused18,
		_set_unused19,

		k_number_of_sets,

		k_last_valid_set = _set_unused10 - 1,
	};
};
