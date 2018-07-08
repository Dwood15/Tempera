#pragma once

#include <macros_generic.h>
#include "../../cseries/base.h"
#include "../../effects/effects_constants.h"
#include "../../render/rasterizer/geometry.h"
#include "../../render/constants.h"

// How much more memory we're adding.
// 50%
static constexpr float K_MEMORY_UPGRADE_INCREASE_AMOUNT = 1.5f;
namespace Yelo::Enums {
	constexpr int k_game_state_allocation_size = 0x00440000;

	enum {
		//////////////////////////////////////////////////////////////////////////
		// physical_memory_map

		k_physical_memory_base_address = 0x40000000,

		// Original allocation size
			k_physical_memory_map_allocation_size = 0x01B40000,

		// Default address of the game state in memory
			k_game_state_base_address = k_physical_memory_base_address,
		// Default address of the tag cache in memory (comes right after the game state memory by default)
			k_tag_cache_base_address  = k_game_state_base_address + k_game_state_allocation_size,

		//////////////////////////////////////////////////////////////////////////
		// resource caches

		k_maximum_number_of_cached_sounds   = 512,
		k_maximum_number_of_cached_textures = 4096,

		//////////////////////////////////////////////////////////////////////////
		// cache files

		// Original maximum cache size
			k_max_cache_size = 0x018000000,

		k_max_cache_vertex_y_index_buffer_size = 0x2000000,

		k_number_of_cached_map_files            = 6,
		k_number_of_cache_read_buffers          = 8, // NUMBER_OF_READ_BUFFERS
		k_number_of_cache_write_buffers         = 1, // NUMBER_OF_WRITE_BUFFERS
		k_number_of_cache_overlapped_structures = 11, // NUMBER_OF_OVERLAPPED_STRUCTURES
		k_maximum_simultaneous_cache_requests   = 0x200,
	};

	// Max amount of memory addressable by the game state. After this comes tag memory
	constexpr auto k_game_state_cpu_size = k_game_state_allocation_size;

	constexpr uint k_maximum_hs_dynamic_source_data_bytes = 1024; // HS_MAXIMUM_DYNAMIC_SOURCE_DATA_BYTES

	constexpr uint k_maximum_hs_syntax_nodes_per_scenario = 19001;
	constexpr uint k_maximum_hs_string_data_per_scenario  = 256 * 1024;

	constexpr uint k_maximum_hs_scripts_per_scenario    = 512;
	constexpr uint k_maximum_hs_globals_per_scenario    = 128;
	constexpr uint k_maximum_hs_references_per_scenario = 256;

	constexpr uint k_maximum_hs_source_files_per_scenario     = 8;
	constexpr uint k_maximum_number_of_hs_function_parameters = 32;
	constexpr uint k_maximum_number_of_hs_globals             = 1024;

	constexpr uint k_hs_inspect_buffer_size          = 1024;
	constexpr uint k_maximum_hs_source_data_per_file = 0x40000;

	// Our upgraded memory allocation size.
	constexpr uint k_physical_memory_map_allocation_size_upgrade = static_cast<uint>(k_physical_memory_map_allocation_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	static_assert(k_physical_memory_map_allocation_size_upgrade >= k_physical_memory_map_allocation_size);

	constexpr uint k_maximum_simultaneous_tag_instances         = 0x00001400; // 5120
	constexpr uint k_maximum_simultaneous_tag_instances_upgrade = static_cast<uint>(Yelo::Enums::k_maximum_simultaneous_tag_instances * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	// 0x40440000
	constexpr uint k_tag_base_address = k_physical_memory_base_address + k_game_state_allocation_size;

	constexpr uint k_tag_allocation_size         = 0x01700000;
	constexpr uint k_tag_allocation_size_upgrade = static_cast<uint>(Enums::k_tag_allocation_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	// Highest Tag Memory Address
	// 0x41B40000
	constexpr uint k_tag_max_address         = k_tag_base_address + k_tag_allocation_size;
	constexpr uint k_tag_max_address_upgrade = k_tag_base_address + k_tag_allocation_size_upgrade;

	constexpr uint k_multiplayer_maximum_additional_players = 0;

	constexpr uint k_multiplayer_maximum_players_upgrade = 16 + k_multiplayer_maximum_additional_players;

	constexpr uint k_maximum_network_machine_count_upgrade = k_multiplayer_maximum_players_upgrade / MAX_PLAYER_COUNT_LOCAL + k_multiplayer_maximum_additional_players;

	//////////////////////////////////////////////////////////////////////////
	// effects upgrades
	constexpr uint k_maximum_number_of_particles_per_map_upgrade = k_maximum_number_of_particles_per_map + k_maximum_number_of_particles_per_map_upgrade_amount;

	constexpr uint k_maximum_number_of_effects_per_map_upgrade          = k_maximum_number_of_effects_per_map + k_maximum_number_of_effects_per_map_upgrade_amount;
	constexpr uint k_maximum_number_of_effect_locations_per_map_upgrade = k_maximum_number_of_effect_locations_per_map + k_maximum_number_of_effect_locations_per_map_upgrade_amount;

	constexpr uint k_maximum_number_of_particle_system_particles_per_map_upgrade = k_maximum_number_of_particle_system_particles_per_map + k_maximum_number_of_particle_system_particles_per_map_upgrade_amount;
	//////////////////////////////////////////////////////////////////////////

	constexpr uint k_hs_functions_count        = 543; // the normal hs_function count in the engine under an editor build
	constexpr uint k_hs_external_globals_count = 495; // the normal hs external globals count in the engine under an editor build

	constexpr uint k_maximum_hs_syntax_nodes_per_scenario_upgrade = static_cast<uint>(k_maximum_hs_syntax_nodes_per_scenario * K_MEMORY_UPGRADE_INCREASE_AMOUNT);
	constexpr uint k_maximum_hs_string_data_per_scenario_upgrade  = static_cast<uint>(k_maximum_hs_string_data_per_scenario * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	constexpr uint k_hs_script_functions_count_upgrade = 1024;
	constexpr uint k_hs_external_globals_count_upgrade = 1024 - k_maximum_hs_globals_per_scenario;

	static_assert(k_maximum_hs_syntax_nodes_per_scenario_upgrade >= k_maximum_hs_syntax_nodes_per_scenario);
	static_assert(k_maximum_hs_string_data_per_scenario_upgrade >= k_maximum_hs_string_data_per_scenario);

	constexpr uint k_rasterizer_maximum_dynamic_triangles_upgrade = static_cast<uint>(k_rasterizer_maximum_dynamic_triangles * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	static_assert(k_rasterizer_maximum_dynamic_triangles_upgrade >= k_rasterizer_maximum_dynamic_triangles);

	constexpr uint k_maximum_rendered_objects_upgrade   = k_maximum_rendered_objects + k_maximum_rendered_objects_upgrade_amount;
	constexpr uint k_maximum_rendered_triangles_upgrade = static_cast<uint>(k_maximum_rendered_triangles * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	// Our upgraded max cache size.
	constexpr uint k_max_cache_size_upgrade = static_cast<uint>(k_max_cache_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	constexpr uint k_max_cache_vertex_y_index_buffer_size_upgrade = static_cast<uint>(k_max_cache_vertex_y_index_buffer_size * K_MEMORY_UPGRADE_INCREASE_AMOUNT);

	static_assert(k_max_cache_size_upgrade >= k_max_cache_size);
	static_assert(k_max_cache_vertex_y_index_buffer_size_upgrade >= k_max_cache_vertex_y_index_buffer_size);

	enum hs_type : short {
		_hs_unparsed = 0,
		// script or global
		_hs_special_form,
		_hs_function_name,
		_hs_passthrough,
		_hs_type_void,
		_hs_type_bool,
		_hs_type_real,
		_hs_type_short,
		_hs_type_long,
		_hs_type_string,
		_hs_type_script,

		_hs_type_trigger_volume,
		_hs_type_cutscene_flag,
		_hs_type_cutscene_camera_point,
		_hs_type_cutscene_title,
		_hs_type_cutscene_recording,
		_hs_type_device_group,
		_hs_type_ai,
		_hs_type_ai_command_list,
		_hs_type_starting_profile,
		_hs_type_conversation,

		_hs_type_navpoint,
		_hs_type_hud_message,

		_hs_type_object_list,

		_hs_type_sound,
		_hs_type_effect,
		_hs_type_damage,
		_hs_type_looping_sound,
		_hs_type_animation_graph,
		_hs_type_actor_variant,
		_hs_type_damage_effect,
		_hs_type_object_definition,

		_hs_type_enum_game_difficulty,
		_hs_type_enum_team,
		_hs_type_enum_ai_default_state,
		_hs_type_enum_actor_type,
		_hs_type_enum_hud_corner,

		_hs_type_object,
		_hs_type_unit,
		_hs_type_vehicle,
		_hs_type_weapon,
		_hs_type_device,
		_hs_type_scenery,

		_hs_type_object_name,
		_hs_type_unit_name,
		_hs_type_vehicle_name,
		_hs_type_weapon_name,
		_hs_type_device_name,
		_hs_type_scenery_name,

		k_number_of_hs_types,

		//_hs_type_string_id = k_number_of_hs_types,
		//_hs_type_any_tag,
		//_hs_type_any_tag_not_resolving,
			k_number_of_hs_types_yelo = k_number_of_hs_types,

		// faux HS type, only for the sake of OS code which exposes 8-bit integers
		// NOTE: we treat sbyte as a byte still!
			_hs_type_byte = _hs_type_bool,

		_hs_type_data__first = _hs_type_bool,
		_hs_type_data__last  = k_number_of_hs_types - 1,
		_hs_type_data__count = (_hs_type_data__last - _hs_type_data__first) + 1,

		_hs_type_tag_reference__first = _hs_type_sound,
		_hs_type_tag_reference__last  = _hs_type_object_definition,
		_hs_type_tag_reference__count = (_hs_type_tag_reference__last - _hs_type_tag_reference__first) + 1,

		_hs_type_enum__first = _hs_type_enum_game_difficulty,
		_hs_type_enum__last  = _hs_type_enum_hud_corner,
		_hs_type_enum__count = (_hs_type_enum__last - _hs_type_enum__first) + 1,

		_hs_type_object__first = _hs_type_object,
		_hs_type_object__last  = _hs_type_scenery,
		_hs_type_object__count = (_hs_type_object__last - _hs_type_object__first) + 1,

		_hs_type_object_name__first = _hs_type_object_name,
		_hs_type_object_name__last  = _hs_type_scenery_name,
		_hs_type_object_name__count = (_hs_type_object_name__last - _hs_type_object_name__first) + 1,
	};

	enum hs_object_type : short {
		_hs_object_type_any     = _hs_type_object - _hs_type_object__first,
		_hs_object_type_unit    = _hs_type_unit - _hs_type_object__first,
		_hs_object_type_vehicle = _hs_type_vehicle - _hs_type_object__first,
		_hs_object_type_weapon  = _hs_type_weapon - _hs_type_object__first,
		_hs_object_type_device  = _hs_type_device - _hs_type_object__first,
		_hs_object_type_scenery = _hs_type_scenery - _hs_type_object__first,

		k_number_of_hs_object_types
	};

	enum hs_script_type : short {
		_hs_script_startup,
		_hs_script_dormant,
		_hs_script_continuous,
		_hs_script_static,
		_hs_script_stub,

		k_number_of_hs_script_types,
	};

	enum {
		k_tag_string_length  = 31, // character count in a [tag_string] type
		k_long_string_length = 255, // character count in a [long_string] type

		k_string_id_length = 127,

		k_string_64_length  = 63,
		k_string_128_length = 127,
		k_string_256_length = 255,
	};
};

namespace Yelo::Flags {
	enum {
		_alignment_16_bit = 1,
		_alignment_32_bit,
		_alignment_64_bit,
		_alignment_128_bit,
	};
};
