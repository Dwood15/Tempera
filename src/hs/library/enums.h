#pragma once
namespace Yelo::Enums {

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

		_hs_function_scenario_faux_zones_reset,					// depreceated
		_hs_function_scenario_faux_zone_current_switch_variant,	// depreceated
		_hs_function_scenario_faux_zone_switch_variant,			// depreceated
		_hs_function_scenario_faux_zone_switch_sky,				// depreceated

		_hs_function_structure_bsp_set_lightmap_set,
		_hs_function_structure_bsp_set_sky_set,

		_hs_function_ai_transform_actor,
		_hs_function_ai_transform_actors,
		_hs_function_ai_transform_actors_by_type,
		_hs_function_ai_actor_is_transforming,

		//////////////////////////////////////////////////////////////////////////
		// everything after is runtime-only, ie not defined in the CheApe scripting definitions

		_hs_function_vehicle_remapper_enabled,

		_hs_function_sv_httpserver_set_thread_count,
		_hs_function_sv_httpserver_set_root,
		_hs_function_sv_httpserver_set_throttle,
		_hs_function_sv_httpserver_set_ports,
		_hs_function_sv_httpserver_show_config,
		_hs_function_sv_httpserver_start,
		_hs_function_sv_httpserver_stop,
		_hs_function_sv_httpserver_log_enable,
		_hs_function_sv_httpserver_set_connection_ban,
		_hs_function_sv_httpserver_banlist,
		_hs_function_sv_httpserver_banlist_file,
		_hs_function_sv_httpserver_ban_ip,
		_hs_function_sv_httpserver_unban_ip,

		_hs_function_sv_mapdownload_start_server,
		_hs_function_sv_mapdownload_stop_server,
		_hs_function_sv_mapdownload_set_part_definitions_path,
		_hs_function_sv_mapdownload_set_host,
		_hs_function_sv_mapdownload_reload_map_part_definitions,

		_hs_function_data_array_info,

		_hs_function_test_networking,

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
}
