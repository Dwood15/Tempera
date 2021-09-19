#pragma once

#include "effects_enums.h"
#include "geometry_enums.h"
#include "player_enums.h"



namespace Yelo::Enums {
		// How many values we allow in the runtime data game state for each type (ie, integers, real, etc)
		constexpr int k_runtime_data_max_values_count = 64;

		// This is a partially made up constant, though when dev-mode is >= this, it outputs messages to the console
		constexpr int k_developer_mode_level_debug_output = 4;
}

namespace Yelo::Flags {
	enum hs_yelo_definition_flags : unsigned short {
		_hs_yelo_definition_internal_bit,
		// changes to the global are sync'd
		_hs_yelo_definition_is_synchronized_bit,
		// hs_function is actually an hs_function_definition_yelo
		_hs_yelo_definition_is_yelo_function_bit,
	};

	enum hs_syntax_node_flags : unsigned short {
		_hs_syntax_node_primitive_bit,
		_hs_syntax_node_script_index_bit,
		_hs_syntax_node_global_index_bit,
		_hs_syntax_node_dont_gc_bit,

		/// <summary>	node is an index to a script parameter. </summary>
			_hs_syntax_node_script_parameter_index_yelo_bit,
		/// <summary>	node is an index to a script's local variable. </summary>
			_hs_syntax_node_script_local_index_yelo_bit,
		/// <summary>	node is an index to a constant. </summary>
			_hs_syntax_node_const_index_yelo_bit,
	};

	enum hs_access_flags : unsigned short {
		_hs_access_enabled_bit,
		_hs_access_sent_to_server_bit,   ///< automatically sent to server
		_hs_access_rconable_bit,      ///< can be executed from an rcon command
		_hs_access_client_bit,         ///< only a client connection can execute it
		_hs_access_server_bit,         ///< only a server connection can execute it
	};

	enum {
		_hs_thread_datum_has_new_stack_frame_bit,
		_hs_thread_datum_sleeping_bit,
	};

	enum {
		_alignment_16_bit = 1,
		_alignment_32_bit,
		_alignment_64_bit,
		_alignment_128_bit,
	};
}

namespace Yelo::Enums {
	enum {
		k_hs_thread_stack_size = 0x200,

		_hs_thread_datum_sleep_forever = 0,
		_hs_thread_datum_sleep_dormant = 0 - 1,
	};

	enum hs_thread_type : short {
		_hs_thread_type_script,
		_hs_thread_type_global_initialize, // script global initializer thunk
		_hs_thread_type_runtime_evaluate, // console_command in stock engine

		k_number_of_hs_thread_types,
	};

	enum actor_action : short;

	enum {
		k_maximum_number_of_units_per_swarm = 16,
		k_maximum_number_of_active_swarms   = 32,
	};

	enum actor_default_state : short {
		_actor_default_state_none,
		_actor_default_state_sleeping,
		_actor_default_state_alert,
		_actor_default_state_moving_repeat_same_position,
		_actor_default_state_moving_loop,
		_actor_default_state_moving_loop_back_and_forth,
		_actor_default_state_moving_loop_randomly,
		_actor_default_state_moving_randomly,
		_actor_default_state_guarding,
		_actor_default_state_guarding_at_guard_position,
		_actor_default_state_searching,
		_actor_default_state_fleeing,

		k_number_of_actor_default_states,
	};

	enum actor_fire_target : short {
		_actor_fire_target_prop         = 1,
		_actor_fire_target_manual_point = 2,
	};

	enum actor_acknowledgement : short {
		_actor_acknowledgement_never,
		_actor_acknowledgement_combat,
		_actor_acknowledgement_instant,
		_actor_acknowledgement_searching,
		_actor_acknowledgement_definite,
	};

	enum actor_knowledge_type : short {
		_actor_knowledge_type_noncombat0,
		_actor_knowledge_type_guard1,
		_actor_knowledge_type_guard2,
		_actor_knowledge_type_noncombat3,

		k_number_of_actor_knowledge_types,
	};

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

		_hs_type_data_first = _hs_type_bool,
		_hs_type_data_last  = k_number_of_hs_types - 1,
		_hs_type_data_count = (_hs_type_data_last - _hs_type_data_first) + 1,

		_hs_type_tag_reference_first = _hs_type_sound,
		_hs_type_tag_reference_last  = _hs_type_object_definition,
		_hs_type_tag_reference_count = (_hs_type_tag_reference_last - _hs_type_tag_reference_first) + 1,

		_hs_type_enum_first = _hs_type_enum_game_difficulty,
		_hs_type_enum_last  = _hs_type_enum_hud_corner,
		_hs_type_enum_count = (_hs_type_enum_last - _hs_type_enum_first) + 1,

		_hs_type_object_first = _hs_type_object,
		_hs_type_object_last  = _hs_type_scenery,
		_hs_type_object_count = (_hs_type_object_last - _hs_type_object_first) + 1,

		_hs_type_object_name_first = _hs_type_object_name,
		_hs_type_object_name_last  = _hs_type_scenery_name,
		_hs_type_object_name_count = (_hs_type_object_name_last - _hs_type_object_name_first) + 1,
	};

	enum hs_object_type : short {
		_hs_object_type_any     = _hs_type_object - _hs_type_object_first,
		_hs_object_type_unit    = _hs_type_unit - _hs_type_object_first,
		_hs_object_type_vehicle = _hs_type_vehicle - _hs_type_object_first,
		_hs_object_type_weapon  = _hs_type_weapon - _hs_type_object_first,
		_hs_object_type_device  = _hs_type_device - _hs_type_object_first,
		_hs_object_type_scenery = _hs_type_scenery - _hs_type_object_first,

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

	enum hs_function_enumeration : short {
		//_hs_function_null,

		_hs_function_volume_test_player_team,
		_hs_function_volume_test_player_team_all,
		_hs_function_player_team_teleport,
		_hs_function_player_team_players,

		_hs_function_player_data_get_integer,
		_hs_function_player_team_data_get_integer,
		_hs_function_player_data_get_object,
		_hs_function_player_data_get_real,
		_hs_function_player_data_set_real,
		_hs_function_player_local_get,

		_hs_function_objects_distance_to_object,
		_hs_function_object_data_get_real,
		_hs_function_object_data_set_real,
		_hs_function_weapon_data_get_real,
		_hs_function_weapon_data_set_real,
		_hs_function_weapon_data_magazine_get_integer,
		_hs_function_weapon_data_magazine_set_integer,
		_hs_function_weapon_data_trigger_set_real,
		_hs_function_unit_data_get_object,
		_hs_function_unit_data_get_integer,
		_hs_function_unit_data_set_integer,
		_hs_function_unit_data_get_real,
		_hs_function_unit_data_set_real,

		_hs_function_physics_get_gravity,
		_hs_function_physics_set_gravity,
		_hs_function_physics_constants_reset,

		_hs_function_runtime_integers_reset,
		_hs_function_runtime_integer_get,
		_hs_function_runtime_integer_set,
		_hs_function_runtime_integer_inc,
		_hs_function_runtime_integer_dec,

		_hs_function_game_change_version_id,
		_hs_function_game_engine_data_get_integer,

		_hs_function_machine_is_host,
		_hs_function_machine_is_dedi,

		_hs_function_abs_integer,
		_hs_function_abs_real,

		_hs_function_pp_load,
		_hs_function_pp_unload,

		_hs_function_pp_get_effect_instance_index_by_name,
		_hs_function_pp_set_effect_instance_active,
		_hs_function_pp_set_effect_instance_fade,
		_hs_function_pp_get_effect_instance_current_fade,
		_hs_function_pp_get_effect_instance_fade_direction,
		_hs_function_pp_get_effect_index_by_name,
		_hs_function_pp_get_effect_is_valid,
		_hs_function_pp_get_effect_shader_variable_index_by_name,
		_hs_function_pp_set_effect_shader_variable_boolean,
		_hs_function_pp_set_effect_shader_variable_integer,
		_hs_function_pp_set_effect_shader_variable_real,
		_hs_function_pp_set_effect_shader_instance_active,
		_hs_function_pp_bloom_set_size,
		_hs_function_pp_bloom_set_exposure,
		_hs_function_pp_bloom_set_mix_amount,
		_hs_function_pp_bloom_set_minimum_color,
		_hs_function_pp_bloom_set_maximum_color,

		_hs_function_bitwise_and,
		_hs_function_bitwise_or,
		_hs_function_bitwise_xor,
		_hs_function_bitwise_lhs,
		_hs_function_bitwise_rhs,
		_hs_function_bit_test,
		_hs_function_bit_toggle,
		_hs_function_bit_flags_test,
		_hs_function_bit_flags_toggle,
		_hs_function_hex_string_to_long,

		_hs_function_runtime_integer_operation,
		_hs_function_runtime_vectors_reset,
		_hs_function_runtime_vector_get_element,
		_hs_function_runtime_vector_set_element,
		_hs_function_runtime_vector_set,
		_hs_function_runtime_vector_operation,
		_hs_function_runtime_vector_to_string,
		_hs_function_object_data_set_vector,
		_hs_function_object_data_save_vector,

		_hs_function_display_scripted_ui_widget,
		_hs_function_play_bink_movie,

		_hs_function_scenario_faux_zones_reset,               // deprecated
		_hs_function_scenario_faux_zone_current_switch_variant,   // deprecated
		_hs_function_scenario_faux_zone_switch_variant,         // deprecated
		_hs_function_scenario_faux_zone_switch_sky,            // deprecated

		_hs_function_structure_bsp_set_lightmap_set,
		_hs_function_structure_bsp_set_sky_set,

		_hs_function_ai_transform_actor,
		_hs_function_ai_transform_actors,
		_hs_function_ai_transform_actors_by_type,
		_hs_function_ai_actor_is_transforming,

		//////////////////////////////////////////////////////////////////////////
		// everything after is runtime-only, ie not defined in the CheApe scripting definitions

		_hs_function_vehicle_remapper_enabled,


		_hs_function_data_array_info,

		k_hs_function_enumeration_count,
	};

	//////////////////////////////////////////////////////////////////////////
	// Globals
	enum hs_global_enumeration : short {

		_hs_global_rasterizer_rt_display,
		_hs_global_rasterizer_gbuffer_enabled,

		_hs_global_pp_external_post_processes_enabled,
		_hs_global_pp_internal_post_processes_enabled,
		_hs_global_pp_fxaa_enabled,
		_hs_global_pp_motion_blur_enabled,
		_hs_global_pp_motion_blur_amount,

		_hs_global_ai_transforms_enabled,

		_hs_global_rasterizer_model_normal_mapping,
		_hs_global_rasterizer_model_detail_normal_mapping,
		_hs_global_rasterizer_model_specular_lights,
		_hs_global_rasterizer_model_specular_map,
		_hs_global_rasterizer_environment_dlm_diffuse,
		_hs_global_rasterizer_environment_dlm_specular,
		_hs_global_rasterizer_effect_depth_fade,

		k_hs_global_enumeration_count,
	};

	// stock sizes


	constexpr unsigned int _hs_global_index_is_external_mask = 1 << 15;
	constexpr unsigned int _hs_global_index_mask             = _hs_global_index_is_external_mask - 1;

	static_assert(0x8000 == _hs_global_index_is_external_mask);
	static_assert(0x7FFF == _hs_global_index_mask);

	//MEMORY_UPGRADES
	typedef unsigned int uint;

	// How much more memory we're adding.
	// +50%

	static constexpr double K_MEMORY_UPGRADE_INCREASE_MULTIPLIER = 1.5f;
	constexpr int          k_game_state_allocation_size     = 0x00440000;

	//////////////////////////////////////////////////////////////////////////
	// physical_memory_map

	constexpr uint k_physical_memory_base_address = 0x40000000;

	// Original allocation size
	constexpr uint k_physical_memory_map_allocation_size = 0x01B40000;

	// Default address of the game state in memory
	constexpr uint k_game_state_base_address = k_physical_memory_base_address;
	// Default address of the tag cache in memory (comes right after the game state memory by default)
	constexpr uint k_tag_cache_base_address  = k_game_state_base_address + k_game_state_allocation_size;


	//////////////////////////////////////////////////////////////////////////
	// resource caches

	constexpr uint k_maximum_number_of_cached_sounds   = 512;
	constexpr uint k_maximum_number_of_cached_textures = 4096;

	//////////////////////////////////////////////////////////////////////////
	// cache files

	// Original maximum cache size
	constexpr uint k_max_cache_size = 0x018000000;

	constexpr uint k_max_cache_vertex_y_index_buffer_size = 0x2000000;

	constexpr uint k_number_of_cached_map_files            = 6;
	constexpr uint k_number_of_cache_read_buffers          = 8; // NUMBER_OF_READ_BUFFERS
	constexpr uint k_number_of_cache_write_buffers         = 1; // NUMBER_OF_WRITE_BUFFERS
	constexpr uint k_number_of_cache_overlapped_structures = 11; // NUMBER_OF_OVERLAPPED_STRUCTURES
	constexpr uint k_maximum_simultaneous_cache_requests   = 0x200;

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
	constexpr uint k_physical_memory_map_allocation_size_upgrade = static_cast<uint>(k_physical_memory_map_allocation_size * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);

	static_assert(k_physical_memory_map_allocation_size_upgrade >= k_physical_memory_map_allocation_size);

	constexpr uint k_maximum_simultaneous_tag_instances         = 0x00001400; // 5120
	constexpr uint k_maximum_simultaneous_tag_instances_upgrade = static_cast<uint>(Yelo::Enums::k_maximum_simultaneous_tag_instances * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);

	// 0x40440000
	constexpr uint k_tag_base_address = k_physical_memory_base_address + k_game_state_allocation_size;

	constexpr uint k_tag_allocation_size         = 0x01700000;
	constexpr uint k_tag_allocation_size_upgrade = static_cast<uint>(Enums::k_tag_allocation_size * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);

	// Highest Tag Memory Address
	// 0x41B40000
	constexpr uint k_tag_max_address         = k_tag_base_address + k_tag_allocation_size;
	constexpr uint k_tag_max_address_upgrade = k_tag_base_address + k_tag_allocation_size_upgrade;

	constexpr uint k_multiplayer_maximum_additional_players = 0;

	constexpr uint k_multiplayer_maximum_players_upgrade = 16 + k_multiplayer_maximum_additional_players;

	constexpr uint k_maximum_network_machine_count_upgrade = k_multiplayer_maximum_players_upgrade / 1 + k_multiplayer_maximum_additional_players;

	//////////////////////////////////////////////////////////////////////////
	// effects upgrades
	constexpr uint k_maximum_number_of_particles_per_map_upgrade = k_maximum_number_of_particles_per_map + k_maximum_number_of_particles_per_map_upgrade_amount;

	constexpr uint k_maximum_number_of_effects_per_map_upgrade          = k_maximum_number_of_effects_per_map + k_maximum_number_of_effects_per_map_upgrade_amount;
	constexpr uint k_maximum_number_of_effect_locations_per_map_upgrade = k_maximum_number_of_effect_locations_per_map + k_maximum_number_of_effect_locations_per_map_upgrade_amount;

	constexpr uint k_maximum_number_of_particle_system_particles_per_map_upgrade =
							k_maximum_number_of_particle_system_particles_per_map + k_maximum_number_of_particle_system_particles_per_map_upgrade_amount;
	//////////////////////////////////////////////////////////////////////////

	constexpr uint k_hs_functions_count        = 543; // the normal hs_function count in the engine under an editor build
	constexpr uint k_hs_external_globals_count = 495; // the normal hs external globals count in the engine under an editor build

	constexpr uint k_maximum_hs_syntax_nodes_per_scenario_upgrade = static_cast<uint>(k_maximum_hs_syntax_nodes_per_scenario * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);
	constexpr uint k_maximum_hs_string_data_per_scenario_upgrade  = static_cast<uint>(k_maximum_hs_string_data_per_scenario * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);

	constexpr uint k_hs_script_functions_count_upgrade = 1024;
	constexpr uint k_hs_external_globals_count_upgrade = 1024 - k_maximum_hs_globals_per_scenario;

	//sizeof(Yelo::Memory::s_data_array) == 0x38

	constexpr uint k_rasterizer_maximum_dynamic_triangles_upgrade = static_cast<uint>(k_rasterizer_maximum_dynamic_triangles * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);


	constexpr uint k_maximum_rendered_objects_upgrade   = k_maximum_rendered_objects + k_maximum_rendered_objects_upgrade_amount;
	constexpr uint k_maximum_rendered_triangles_upgrade = static_cast<uint>(k_maximum_rendered_triangles * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);

	// Our upgraded max cache size.
	constexpr uint k_max_cache_size_upgrade = static_cast<uint>(k_max_cache_size * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);

	constexpr uint k_max_cache_vertex_y_index_buffer_size_upgrade = static_cast<uint>(k_max_cache_vertex_y_index_buffer_size * K_MEMORY_UPGRADE_INCREASE_MULTIPLIER);

	constexpr uint k_total_scenario_hs_syntax_data         = 0x38 + (0x14 * k_maximum_hs_syntax_nodes_per_scenario);
	constexpr uint k_total_scenario_hs_syntax_data_upgrade        = 0x38 + (0x14 * k_maximum_hs_syntax_nodes_per_scenario_upgrade);//sizeof(Scripting::hs_syntax_node)

	static_assert(k_maximum_hs_syntax_nodes_per_scenario_upgrade >= k_maximum_hs_syntax_nodes_per_scenario);
	static_assert(k_maximum_hs_string_data_per_scenario_upgrade >= k_maximum_hs_string_data_per_scenario);
	static_assert(k_rasterizer_maximum_dynamic_triangles_upgrade >= k_rasterizer_maximum_dynamic_triangles);

	static_assert(k_max_cache_size_upgrade >= k_max_cache_size);
	static_assert(k_max_cache_vertex_y_index_buffer_size_upgrade >= k_max_cache_vertex_y_index_buffer_size);
}

