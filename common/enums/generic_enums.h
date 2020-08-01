#pragma once
#ifndef shitty_enums
#define shitty_enums

#include "macros_generic.h"

//OS uses the byte_flags typedef.
#pragma region enumerations
#pragma region generic tag
#pragma region byteswap shit

namespace Yelo::Flags {
	enum project_yellow_globals_flags {
		_project_yellow_globals_hide_health_when_zoomed_bit,
		_project_yellow_globals_hide_shield_when_zoomed_bit,
		_project_yellow_globals_hide_motion_sensor_when_zoomed_bit,
		_project_yellow_globals_force_game_to_use_stun_jumping_penalty_bit,
		// TODO:
		_project_yellow_globals_allow_grenade_chain_reactions_in_mp_bit,

		k_number_of_project_yellow_globals_flags,
	};
};

namespace Yelo::Enums {
	enum {
		k_data_signature          = 'd@t@',
		k_data_iterator_signature = 'iter',

		k_lruv_cache_signature = 'weee',

		k_memory_pool_signature   = 'pool',
		k_block_header_signature  = 'head',
		k_block_trailer_signature = 'tail',
	};
};

namespace byteswaps {
	enum {
		k_byte_swap_signature = 'bysw',
	};

	// positive numbers are runs (of bytes) to leave untouched - zero is invalid
	enum /*e_bs_code*/ : byte_swap_code_t {
		_bs_code_array_end   = -101, // _end_bs_array
		_bs_code_array_start = -100, // _begin_bs_array

		_bs_code_8byte = 0 - (byte_swap_code_t) sizeof(long long), // _8byte
		_bs_code_4byte = 0 - (byte_swap_code_t) sizeof(long long), // _4byte
		_bs_code_2byte = 0 - (byte_swap_code_t) sizeof(long long), // _2byte

		_bs_code_1byte = sizeof(byte), // _byte
	};

	enum {
		k_maximum_field_byte_swap_codes = 1024,

		// these chars should all match the TAG_FIELD_MARKUP_* defines in tag_groups_structures_macros.hpp

		k_tag_field_markup_character_advanced     = '!',
		k_tag_field_markup_character_help_prefix  = '#',
		k_tag_field_markup_character_read_only    = '*',
		k_tag_field_markup_character_units_prefix = ':',
		k_tag_field_markup_character_block_name   = '^',
	};
};

enum LuaCallbackId {
	invalid                   = -1,
	//not working, but still available to hook into.
	on_load                   = 0,
	on_map_load               = 1,
	before_scenario_tags_load = 2,
	after_scenario_tags_load  = 3,
	// available, should be working
	before_game_tick          = 4,
	after_game_tick           = 5,
	post_initialize           = 6,
	post_dll_init             = 7,
	//end available, working hooks.
	max_callback_id
};

#pragma endregion
#pragma region tag related

#pragma region sbsp
enum collision_surface_flags : byte {
	_collision_surface_two_sided_bit,
	_collision_surface_invisible_bit,
	_collision_surface_climbable_bit,
	_collision_surface_breakable_bit
};
#pragma endregion

namespace obj {
	// Stock game state allocation size for the object memory pool
	// When running in editor tools, this and the max number of objects is increased by 5x
	constexpr int k_object_memory_pool_allocation_size{0x200000};
};

enum objects_find_flags : unsigned long {
	_objects_find_collideable_bit,
	_objects_find_noncollideable_bit,
};

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
};
#pragma endregion
#pragma endregion
#pragma region global gamestate kinda
enum game_difficulty_level : short {
	_game_difficulty_level_easy,
	_game_difficulty_level_normal,
	_game_difficulty_level_hard,
	_game_difficulty_level_impossible,

	k_number_of_game_difficulty_levels
};

enum blip_type : signed char {
	blip_type_local_player,        //0x0    // 1, .5, 0
	blip_type_friendly_team,       //0x1     // 1, 1, 0
	blip_type_enemy_team,           //0x2     // 1, 0, 0
	blip_type_friendly_team_vehicle, //0x3   // 1, 1, 0
	blip_type_enemy_team_vehicle,   //0x4     // 1, 0, 0
	blip_type_objective,           //0x5     // .5, .5, 1
	blip_type_none,               //0x6     // 0, 0, 0
};

#pragma region scenario
enum scenario_type : short {
	_scenario_type_campaign,
	_scenario_type_multiplayer,
	_scenario_type_main_menu,
};
#pragma endregion
#pragma endregion
#pragma region misc
enum networked_datum : int {
	_networked_datum_master,
	_networked_datum_puppet,
	_networked_datum_puppet_controlled_by_local_player,
	_networked_datum_local_only,
};

enum game_team : __int16 {
	_game_team_none    = NONE,
	_game_team_default = 0,
	_game_team_player,
	_game_team_human,
	_game_team_covenant,
	_game_team_flood,
	_game_team_sentinel,
	_game_team_unused6,
	_game_team_unused7,
	_game_team_unused8,
	_game_team_unused9,

	k_number_of_game_teams = 10
};
#pragma endregion
#pragma region objects
enum incoming_object_function {
	_incoming_object_function_a,
	_incoming_object_function_b,
	_incoming_object_function_c,
	_incoming_object_function_d,

	k_number_of_incoming_object_functions
};

enum outgoing_object_function {
	_outgoing_object_function_a,
	_outgoing_object_function_b,
	_outgoing_object_function_c,
	_outgoing_object_function_d,

	k_number_of_outgoing_object_functions
};

enum object_function_reference : short{
	_object_function_reference_none,
	_object_function_reference_a,
	_object_function_reference_b,
	_object_function_reference_c,
	_object_function_reference_d,

	k_number_of_object_function_references,
};

enum object_function_scalar {
	_object_function_scalar_none,

	_object_function_scalar_a_in,
	_object_function_scalar_b_in,
	_object_function_scalar_c_in,
	_object_function_scalar_d_in,

	_object_function_scalar_a_out,
	_object_function_scalar_b_out,
	_object_function_scalar_c_out,
	_object_function_scalar_d_out,

	k_number_of_object_function_scalars,
};

enum object_function_bounds_mode {
	_object_function_bounds_mode_clip,
	_object_function_bounds_mode_clip_and_normalize,
	_object_function_bounds_mode_scale_to_fit,

	k_number_of_object_function_bounds_modes,
};

enum object_change_color : __int16 {
	_object_change_color_a,
	_object_change_color_b,
	_object_change_color_c,
	_object_change_color_d,

	k_number_of_object_change_colors,
};

enum object_change_color_reference : __int16 {
	_object_change_color_reference_none,
	_object_change_color_reference_a,
	_object_change_color_reference_b,
	_object_change_color_reference_c,
	_object_change_color_reference_d,

	k_number_of_object_change_color_references,
};

//TODO: STATIC ASSERT USAGE + CONSTEXPR
enum object_sizes {
	// The base object datum allocates 2 real_orientation3d and 1 real_matrix4x3 FOR EVERY model node
		k_object_size_object        = 0x1F4,
		k_object_size_projectile    = 0xBC + k_object_size_object,
		k_object_size_scenery       = 0x4 + k_object_size_object,
		k_object_size_placeholder   = 0x8 + k_object_size_object,
		k_object_size_sound_scenery = 0x4 + k_object_size_object,

	k_object_size_device        = 0x20 + k_object_size_object,
	k_object_size_machine       = 0x14 + k_object_size_device,
	k_object_size_control       = 0x8 + k_object_size_device,
	k_object_size_light_fixture = 0x18 + k_object_size_device,

	k_object_size_item      = 0x38 + k_object_size_object,
	k_object_size_weapon    = 0x114 + k_object_size_item,
	k_object_size_equipment = 0x68 + k_object_size_item,
	k_object_size_garbage   = 0x18 + k_object_size_item,

	k_object_size_unit    = 0x2D8 + k_object_size_object,
	k_object_size_biped   = 0x84 + k_object_size_unit,
	k_object_size_vehicle = 0xF4 + k_object_size_unit,
};

enum object_type : uint32 {
	_object_type_object = (uint32)(-4),
	_object_type_device = (uint32)-3,
	_object_type_item   = (uint32)-2,
	_object_type_unit   = (uint32)-1,

	_object_type_biped = 0,
	_object_type_vehicle,
	_object_type_weapon,
	_object_type_equipment,
	_object_type_garbage,
	_object_type_projectile,
	_object_type_scenery,
	_object_type_machine,
	_object_type_control,
	_object_type_light_fixture,
	_object_type_placeholder,
	_object_type_sound_scenery,

	_object_type_mask_device = FLAG(_object_type_machine) | FLAG(_object_type_control) | FLAG(_object_type_light_fixture),
	_object_type_mask_item   = FLAG(_object_type_weapon) | FLAG(_object_type_equipment) | FLAG(_object_type_garbage),
	_object_type_mask_unit   = FLAG(_object_type_biped) | FLAG(_object_type_vehicle),

	_object_type_mask_all =
	FLAG(_object_type_projectile) | FLAG(_object_type_scenery) | FLAG(_object_type_placeholder) | FLAG(_object_type_sound_scenery) | _object_type_mask_device | _object_type_mask_item |
	_object_type_mask_unit,
};

enum object_header_flags : unsigned char {
	_object_header_active_bit,
	_object_header_visible_bit,
	_object_header_newly_created_bit, // true until after the first call to the
	_object_header_being_deleted_bit,
	_object_header_child_bit, // attached to another object (at a marker/node)
	_object_header_connected_to_map_bit,
	_object_header_automatic_deactivation_bit,
	_object_header_unk7_bit,
};

enum attachment_type : signed char {
	_attachment_type_invalid = NONE,
	_attachment_type_light   = 0,
	_attachment_type_looping_sound,
	_attachment_type_effect,
	_attachment_type_contrail,
	_attachment_type_particle,
	_attachment_type,
};
#pragma endregion
#pragma region weapons
enum weapon_state : sbyte {
	_weapon_state_idle,

	_weapon_state_fire1, _weapon_state_fire2,
	_weapon_state_chamber1, _weapon_state_chamber2,
	_weapon_state_reload1, _weapon_state_reload2,
	_weapon_state_charged1, _weapon_state_charged2,

	_weapon_state_ready,
	_weapon_state_put_away,

	_weapon_state,
};

enum weapon_trigger_state : byte {
	_weapon_trigger_state_idle,
	_weapon_trigger_state_unknown1, // alternate shot/overload related
	_weapon_trigger_state_charging,
	_weapon_trigger_state_charged,
	_weapon_trigger_state_unknown4, // locked related (tracking)
	_weapon_trigger_state_unknown5, // tracking related
	_weapon_trigger_state_spewing,
	_weapon_trigger_state_frozen_while_triggered,
	_weapon_trigger_state_frozen_timed,

	_weapon_trigger_state,
};

enum weapon_magazine_state : short {
	_weapon_magazine_state_idle,
	_weapon_magazine_state_chambering_start,
	_weapon_magazine_state_chambering_finish,
	_weapon_magazine_state_chambering,
	_weapon_magazine_state,
};
#pragma endregion
#pragma endregion
#endif
