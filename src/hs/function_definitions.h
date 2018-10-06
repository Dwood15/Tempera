#pragma once
#ifdef HS_FUNCTION_DEFINITION
#undef HS_FUNCTION_DEFINITION

#endif 

#define HS_FUNCTION_DEFINITION(name) static hs_function_definition function_##name##_definition

#include "structures.h"
namespace Yelo::Scripting {
	static void InitializeHSFunctionDefinitions();

	HS_FUNCTION_DEFINITION(physics_get_gravity);
	HS_FUNCTION_DEFINITION(physics_set_gravity);
	HS_FUNCTION_DEFINITION(physics_set_gravity_definition);
	HS_FUNCTION_DEFINITION(physics_constants_reset);
	HS_FUNCTION_DEFINITION(game_change_version_id);
	HS_FUNCTION_DEFINITION(game_engine_data_get_integer);
	HS_FUNCTION_DEFINITION(machine_is_host);
	HS_FUNCTION_DEFINITION(machine_is_dedi);
	HS_FUNCTION_DEFINITION(display_scripted_ui_widget);
	HS_FUNCTION_DEFINITION(play_bink_movie);
	HS_FUNCTION_DEFINITION(switch_variant);
	HS_FUNCTION_DEFINITION(scenario_faux_zone_switch_variant);
	HS_FUNCTION_DEFINITION(scenario_faux_zone_switch_sky);
	HS_FUNCTION_DEFINITION(structure_bsp_set_lightmap_set);
	HS_FUNCTION_DEFINITION(structure_bsp_set_sky_set);
	HS_FUNCTION_DEFINITION(vehicle_remapper_enabled);
	HS_FUNCTION_DEFINITION(data_array_info);
	HS_FUNCTION_DEFINITION(ai_transform_actor);
	HS_FUNCTION_DEFINITION(ai_transform_actors);
	HS_FUNCTION_DEFINITION(ai_transform_actors_by_type);
	HS_FUNCTION_DEFINITION(ai_actor_is_transforming);
	HS_FUNCTION_DEFINITION(debug_pvs);
	HS_FUNCTION_DEFINITION(radiosity_start);
	HS_FUNCTION_DEFINITION(radiosity_save);
	HS_FUNCTION_DEFINITION(radiosity_debug_point);
	HS_FUNCTION_DEFINITION(debug_teleport_player);
	HS_FUNCTION_DEFINITION(hud_team_icon_set_pos);
	HS_FUNCTION_DEFINITION(hud_team_icon_set_scale);
	HS_FUNCTION_DEFINITION(hud_team_background_set_pos);
	HS_FUNCTION_DEFINITION(hud_team_background_set_scale);
	HS_FUNCTION_DEFINITION(reload_shader_transparent_chicago);
	HS_FUNCTION_DEFINITION(rasterizer_reload_effects);
	HS_FUNCTION_DEFINITION(oid_watch);
	HS_FUNCTION_DEFINITION(oid_dump);
	HS_FUNCTION_DEFINITION(oid_status);
	HS_FUNCTION_DEFINITION(runtime_integers_reset);
	HS_FUNCTION_DEFINITION(runtime_integer_get);
	HS_FUNCTION_DEFINITION(runtime_integer_set);
	HS_FUNCTION_DEFINITION(runtime_integer_inc);
	HS_FUNCTION_DEFINITION(runtime_integer_dec);
	HS_FUNCTION_DEFINITION(runtime_integer_operation);
	HS_FUNCTION_DEFINITION(runtime_vectors_reset);
	HS_FUNCTION_DEFINITION(runtime_vector_get_element);
	HS_FUNCTION_DEFINITION(runtime_vector_set_element);
	HS_FUNCTION_DEFINITION(runtime_vector_set);
	HS_FUNCTION_DEFINITION(runtime_vector_operation);
	HS_FUNCTION_DEFINITION(runtime_vector_to_string);
	HS_FUNCTION_DEFINITION(pp_load);
	HS_FUNCTION_DEFINITION(pp_unload);
	HS_FUNCTION_DEFINITION(pp_get_effect_instance_index_by_name);
	HS_FUNCTION_DEFINITION(pp_set_effect_instance_active);
	HS_FUNCTION_DEFINITION(pp_set_effect_instance_fade);
	HS_FUNCTION_DEFINITION(pp_get_effect_instance_current_fade);
	HS_FUNCTION_DEFINITION(pp_get_effect_instance_fade_direction);
	HS_FUNCTION_DEFINITION(pp_get_effect_index_by_name);
	HS_FUNCTION_DEFINITION(pp_get_effect_is_valid);
	HS_FUNCTION_DEFINITION(pp_get_effect_shader_variable_index_by_name);
	HS_FUNCTION_DEFINITION(pp_set_effect_shader_variable_boolean);
	HS_FUNCTION_DEFINITION(pp_set_effect_shader_variable_integer);
	HS_FUNCTION_DEFINITION(pp_set_effect_shader_variable_real);
	HS_FUNCTION_DEFINITION(pp_set_effect_shader_instance_active);
	HS_FUNCTION_DEFINITION(pp_bloom_set_size);
	HS_FUNCTION_DEFINITION(pp_bloom_set_exposure);
	HS_FUNCTION_DEFINITION(pp_bloom_set_mix_amount);
	HS_FUNCTION_DEFINITION(pp_bloom_set_minimum_color);
	HS_FUNCTION_DEFINITION(pp_bloom_set_maximum_color);
	HS_FUNCTION_DEFINITION(volume_test_player_team);
	HS_FUNCTION_DEFINITION(volume_test_player_team_all);
	HS_FUNCTION_DEFINITION(player_team_teleport);
	HS_FUNCTION_DEFINITION(player_team_players);
	HS_FUNCTION_DEFINITION(player_data_get_integer);
	HS_FUNCTION_DEFINITION(player_team_data_get_integer);
	HS_FUNCTION_DEFINITION(player_data_get_object);
	HS_FUNCTION_DEFINITION(player_data_get_real);
	HS_FUNCTION_DEFINITION(player_data_set_real);
	HS_FUNCTION_DEFINITION(player_local_get);
	HS_FUNCTION_DEFINITION(objects_distance_to_object);
	HS_FUNCTION_DEFINITION(object_data_get_real);
	HS_FUNCTION_DEFINITION(object_data_set_real);
	HS_FUNCTION_DEFINITION(object_data_set_vector);
	HS_FUNCTION_DEFINITION(object_data_save_vector);
	HS_FUNCTION_DEFINITION(weapon_data_get_real);
	HS_FUNCTION_DEFINITION(weapon_data_set_real);
	HS_FUNCTION_DEFINITION(weapon_data_magazine_get_integer);
	HS_FUNCTION_DEFINITION(weapon_data_magazine_set_integer);
	HS_FUNCTION_DEFINITION(weapon_data_trigger_set_real);
	HS_FUNCTION_DEFINITION(unit_data_get_object);
	HS_FUNCTION_DEFINITION(unit_data_get_integer);
	HS_FUNCTION_DEFINITION(unit_data_set_integer);
	HS_FUNCTION_DEFINITION(unit_data_get_real);
	HS_FUNCTION_DEFINITION(unit_data_set_real);
	HS_FUNCTION_DEFINITION(abs_integer);
	HS_FUNCTION_DEFINITION(abs_real);
	HS_FUNCTION_DEFINITION(bitwise_and);
	HS_FUNCTION_DEFINITION(bitwise_or);
	HS_FUNCTION_DEFINITION(bitwise_xor);
	HS_FUNCTION_DEFINITION(bitwise_lhs);
	HS_FUNCTION_DEFINITION(bitwise_rhs);
	HS_FUNCTION_DEFINITION(bit_test);
	HS_FUNCTION_DEFINITION(bit_toggle);
	HS_FUNCTION_DEFINITION(bit_flags_test);
	HS_FUNCTION_DEFINITION(bit_flags_toggle);
	HS_FUNCTION_DEFINITION(hex_string_to_long);
};

#undef HS_FUNCTION_DEFINITION
