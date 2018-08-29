#pragma once

#include "script_extensions/hs_base.h"


//////////////////////////////////////////////////////////////////////////
// hs_function
// [name]	Code based name
// [ret]	hs_type this function returns
// [info]	(string) Function's "help" text, short little description
// CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
#define HS_FUNCTION(name, ret, info)											\
	Yelo::Scripting::hs_function_definition function_##name##_definition = {	\
		HS_TYPE(ret),0,															\
		#name,																	\
		CAST_PTR(Yelo::Scripting::proc_hs_parse, nullptr),
		nullptr,																\
		info,																	\
		nullptr,																\
		FLAG(Yelo::Flags::_hs_access_enabled_bit), 0							\
	}

// [name]	Code based name
// [ret]	[hs_type] this function returns
// [info]	(string) Function's "help" text, short little description
// [param]	(string) Function's "parameters" text, lists the parameter names
// [argc]	How many parameters this function takes
// [...]	List of parameter's [hs_type] types
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// hs_external_global
#define HS_GLOBAL(name, type, value)										\
	Yelo::Scripting::hs_global_definition global_##name##_definition = {	\
		#name,																\
		HS_TYPE(type),														\
		0,																	\
		CAST_PTR(void*,value),												\
		FLAG(Yelo::Flags::_hs_access_enabled_bit)							\
	}

#define HS_GLOBAL_EX(name, type, value, flags)								\
	Yelo::Scripting::hs_global_definition global_##name##_definition = {	\
		#name,																\
		HS_TYPE(type),														\
		flags,																\
		CAST_PTR(void*,value),												\
		FLAG(Yelo::Flags::_hs_access_enabled_bit)							\
	}


#define HS_GLOBAL2(name, type, valuece, valueded) \
		Yelo::Scripting::hs_global_definition global_##name##_definition = {#name, HS_TYPE(type), 0, CAST_PTR(void*,valuece), FLAG(Yelo::Flags::_hs_access_enabled_bit)}
//////////////////////////////////////////////////////////////////////////

namespace Yelo::Scripting {

	Yelo::Scripting::hs_function_definition function_physics_get_gravity_definition = {
		Enums::_hs_type_real,
		0,
		"physics_get_gravity",
		(static_cast<Yelo::Scripting::proc_hs_parse>(nullptr)), //CurrentEngine.getFunctionBegin("hs_macro_function_parse"))),
		nullptr,
		"",
		nullptr,
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		0
	};

	Yelo::Scripting::hs_function_definition function_physics_set_gravity_definition = {
		Enums::_hs_type_void,
		0,
		"physics_set_gravity",
		(static_cast<Yelo::Scripting::proc_hs_parse>(nullptr)), //CurrentEngine.getFunctionBegin("hs_macro_function_parse"))),
		nullptr,
		"",
		"gravity fraction",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Yelo::Enums::hs_type::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_physics_constants_reset_definition = {
		Enums::_hs_type_void,
		0,
		"physics_constants_reset",
		(static_cast<Yelo::Scripting::proc_hs_parse>(nullptr)), //CurrentEngine.getFunctionBegin("hs_macro_function_parse"))),
		nullptr,
		"",
		nullptr,
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		0
	};

	// #include "Game/ScriptLibrary.Definitions.RuntimeData.inl"

	Yelo::Scripting::hs_function_definition function_game_change_version_id_definition = {
		Enums::_hs_type_bool,
		0,
		"game_change_version_id",
		(static_cast<Yelo::Scripting::proc_hs_parse>(nullptr)), //CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns whether the change was successful or not",
		"<also-change-game-build-string> <version-string> ",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_bool, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_game_engine_data_get_integer_definition = {
		Enums::_hs_type_long,
		0,
		"game_engine_data_get_integer",
		(static_cast<Yelo::Scripting::proc_hs_parse>(nullptr)), //CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	// HS_FUNCTION(machine_is_host, bool, "returns whether or not the local machine is the host");
	// HS_FUNCTION(machine_is_dedi, bool, "returns whether or not the local machine is a dedicated server");

	Yelo::Scripting::hs_function_definition function_display_scripted_ui_widget_definition = {
		Enums::_hs_type_bool,
		0,
		"display_scripted_ui_widget",
		(static_cast<Yelo::Scripting::proc_hs_parse>(nullptr)), //CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_play_bink_movie_definition = {
		Enums::_hs_type_void,
		0,
		"play_bink_movie",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr),//CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<filename>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	// HS_FUNCTION(scenario_faux_zones_reset, void, "depreceated, do not use");
	Yelo::Scripting::hs_function_definition function_scenario_faux_zone_current
	_switch_variant_definition = {
		Enums::_hs_type_bool,
		0,
		"scenario_faux_zone_current_switch_variant",
			static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"depreceated, do not use",
		"<variant-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};
	Yelo::Scripting::hs_function_definition function_scenario_faux_zone_switch_variant_definition = {
		Enums::_hs_type_bool,
		0,
		"scenario_faux_zone_switch_variant",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"depreceated, do not use",
		"<zone-name> <variant-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_string, Enums::_hs_type_string}
	};
	Yelo::Scripting::hs_function_definition function_scenario_faux_zone_switch_sky_definition = {
		Enums::_hs_type_bool,
		0,
		"scenario_faux_zone_switch_sky",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"depreceated, do not use",
		"<zone-sky-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_structure_bsp_set_lightmap_set_definition = {
		Enums::_hs_type_bool,
		0,
		"structure_bsp_set_lightmap_set",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"Sets a bsp's lightmap set",
		"<bsp-index> <set-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_structure_bsp_set_sky_set_definition = {
		Enums::_hs_type_bool,
		0,
		"structure_bsp_set_sky_set",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"Sets a bsp's sky set",
		"<bsp-index> <set-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_vehicle_remapper_enabled_definition = {
		Enums::_hs_type_bool,
		0,
		"vehicle_remapper_enabled",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns the remapper state before the function call",
		"state_name",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_data_array_info_definition = {
		Enums::_hs_type_void,
		0,
		"data_array_info",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<data-array-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_ai_transform_actor_definition = {
		Enums::_hs_type_bool,
		0,
		"ai_transform_actor",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"Transforms an actor into the specified target. Returns false if it fails. Empty names causes random selection.",
		"<object> <transform_name> <target_name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_object, Enums::_hs_type_string, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_ai_transform_actors_definition = {
		Enums::_hs_type_bool,
		0,
		"ai_transform_actors",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"Transforms a list of actors into the specified target. Returns false if it fails. Empty names causes random selection.",
		"<objects> <transform_name> <target_name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_object_list, Enums::_hs_type_string, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_ai_transform_actors_by_type_definition = {
		Enums::_hs_type_bool,
		0,
		"ai_transform_actors_by_type",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"Transforms actors in a list of a specific type into the specified target. Returns false if it fails. Empty names causes random selection.",
		"<objects> <actor_variant> <transform_name> <target_name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_object_list, Enums::_hs_type_actor_variant, Enums::_hs_type_string, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_ai_actor_is_transforming_definition = {
		Enums::_hs_type_bool,
		0,
		"ai_actor_is_transforming",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"Returns true if the specified actor is transforming.",
		"<object>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_object}
	};

	//////////////////////////////////////////////////////////////////////////
	// Functions
	static hs_function_definition *hs_yelo_functions[] = {
		//&GET_HS_FUNCTION(null),

		// &GET_HS_FUNCTION(volume_test_player_team),
		// &GET_HS_FUNCTION(volume_test_player_team_all),
		// &GET_HS_FUNCTION(player_team_teleport),
		// &GET_HS_FUNCTION(player_team_players),

		// &GET_HS_FUNCTION(player_data_get_integer),
		// &GET_HS_FUNCTION(player_team_data_get_integer),
		// &GET_HS_FUNCTION(player_data_get_object),
		// &GET_HS_FUNCTION(player_data_get_real),
		// &GET_HS_FUNCTION(player_data_set_real),
		// &GET_HS_FUNCTION(player_local_get),

		// &GET_HS_FUNCTION(objects_distance_to_object),
		// &GET_HS_FUNCTION(object_data_get_real),
		// &GET_HS_FUNCTION(object_data_set_real),
		// &GET_HS_FUNCTION(weapon_data_get_real),
		// &GET_HS_FUNCTION(weapon_data_set_real),
		// &GET_HS_FUNCTION(weapon_data_magazine_get_integer),
		// &GET_HS_FUNCTION(weapon_data_magazine_set_integer),
		// &GET_HS_FUNCTION(weapon_data_trigger_set_real),
		// &GET_HS_FUNCTION(unit_data_get_object),
		// &GET_HS_FUNCTION(unit_data_get_integer),
		// &GET_HS_FUNCTION(unit_data_set_integer),
		// &GET_HS_FUNCTION(unit_data_get_real),
		// &GET_HS_FUNCTION(unit_data_set_real),

		&Yelo::Scripting::function_physics_get_gravity_definition,
		&Yelo::Scripting::function_physics_set_gravity_definition,
		&Yelo::Scripting::function_physics_constants_reset_definition,

		// &GET_HS_FUNCTION(runtime_integers_reset),
		// &GET_HS_FUNCTION(runtime_integer_get),
		// &GET_HS_FUNCTION(runtime_integer_set),
		// &GET_HS_FUNCTION(runtime_integer_inc),
		// &GET_HS_FUNCTION(runtime_integer_dec),

		&Yelo::Scripting::function_game_change_version_id_definition,
		&Yelo::Scripting::function_game_engine_data_get_integer_definition,

		// &Yelo::Scripting::function_machine_is_host_definition,
		// &Yelo::Scripting::function_machine_is_dedi_definition,

		// &GET_HS_FUNCTION(abs_integer),
		// &GET_HS_FUNCTION(abs_real),

		//TODO: POSTPROCESSING
		// &GET_HS_FUNCTION(pp_load),
		// &GET_HS_FUNCTION(pp_unload),
		//
		// &GET_HS_FUNCTION(pp_get_effect_instance_index_by_name),
		// &GET_HS_FUNCTION(pp_set_effect_instance_active),
		// &GET_HS_FUNCTION(pp_set_effect_instance_fade),
		// &GET_HS_FUNCTION(pp_get_effect_instance_current_fade),
		// &GET_HS_FUNCTION(pp_get_effect_instance_fade_direction),
		// &GET_HS_FUNCTION(pp_get_effect_index_by_name),
		// &GET_HS_FUNCTION(pp_get_effect_is_valid),
		// &GET_HS_FUNCTION(pp_get_effect_shader_variable_index_by_name),
		// &GET_HS_FUNCTION(pp_set_effect_shader_variable_boolean),
		// &GET_HS_FUNCTION(pp_set_effect_shader_variable_integer),
		// &GET_HS_FUNCTION(pp_set_effect_shader_variable_real),
		// &GET_HS_FUNCTION(pp_set_effect_shader_instance_active),
		// &GET_HS_FUNCTION(pp_bloom_set_size),
		// &GET_HS_FUNCTION(pp_bloom_set_exposure),
		// &GET_HS_FUNCTION(pp_bloom_set_mix_amount),
		// &GET_HS_FUNCTION(pp_bloom_set_minimum_color),
		// &GET_HS_FUNCTION(pp_bloom_set_maximum_color),

		// &GET_HS_FUNCTION(bitwise_and),
		// &GET_HS_FUNCTION(bitwise_or),
		// &GET_HS_FUNCTION(bitwise_xor),
		// &GET_HS_FUNCTION(bitwise_lhs),
		// &GET_HS_FUNCTION(bitwise_rhs),
		// &GET_HS_FUNCTION(bit_test),
		// &GET_HS_FUNCTION(bit_toggle),
		// &GET_HS_FUNCTION(bit_flags_test),
		// &GET_HS_FUNCTION(bit_flags_toggle),
		// &GET_HS_FUNCTION(hex_string_to_long),
		//
		// &GET_HS_FUNCTION(runtime_integer_operation),
		// &GET_HS_FUNCTION(runtime_vectors_reset),
		// &GET_HS_FUNCTION(runtime_vector_get_element),
		// &GET_HS_FUNCTION(runtime_vector_set_element),
		// &GET_HS_FUNCTION(runtime_vector_set),
		// &GET_HS_FUNCTION(runtime_vector_operation),
		// &GET_HS_FUNCTION(runtime_vector_to_string),
		// &GET_HS_FUNCTION(object_data_set_vector),
		// &GET_HS_FUNCTION(object_data_save_vector),

		&Yelo::Scripting::function_display_scripted_ui_widget_definition,
		&Yelo::Scripting::function_play_bink_movie_definition,

		// &Yelo::Scripting::function_scenario_faux_zones_reset_definition,
		// &Yelo::Scripting::function_scenario_faux_zone_current_switch_variant_definition,
		&Yelo::Scripting::function_scenario_faux_zone_switch_variant_definition,
		&Yelo::Scripting::function_scenario_faux_zone_switch_sky_definition,

		&Yelo::Scripting::function_structure_bsp_set_lightmap_set_definition,
		&Yelo::Scripting::function_structure_bsp_set_sky_set_definition,

		&Yelo::Scripting::function_ai_transform_actor_definition,
		&Yelo::Scripting::function_ai_transform_actors_definition,
		&Yelo::Scripting::function_ai_transform_actors_by_type_definition,
		&Yelo::Scripting::function_ai_actor_is_transforming_definition,

		&Yelo::Scripting::function_vehicle_remapper_enabled_definition,

		// &GET_HS_FUNCTION(sv_httpserver_set_thread_count),
		// &GET_HS_FUNCTION(sv_httpserver_set_root),
		// &GET_HS_FUNCTION(sv_httpserver_set_throttle),
		// &GET_HS_FUNCTION(sv_httpserver_set_ports),
		// &GET_HS_FUNCTION(sv_httpserver_show_config),
		// &GET_HS_FUNCTION(sv_httpserver_start),
		// &GET_HS_FUNCTION(sv_httpserver_stop),
		// &GET_HS_FUNCTION(sv_httpserver_log_enable),
		// &GET_HS_FUNCTION(sv_httpserver_set_connection_ban),
		// &GET_HS_FUNCTION(sv_httpserver_banlist),
		// &GET_HS_FUNCTION(sv_httpserver_banlist_file),
		// &GET_HS_FUNCTION(sv_httpserver_ban_ip),
		// &GET_HS_FUNCTION(sv_httpserver_unban_ip),

		// &GET_HS_FUNCTION(sv_mapdownload_start_server),
		// &GET_HS_FUNCTION(sv_mapdownload_stop_server),
		// &GET_HS_FUNCTION(sv_mapdownload_set_part_definitions_path),
		// &GET_HS_FUNCTION(sv_mapdownload_set_host),
		// &GET_HS_FUNCTION(sv_mapdownload_reload_map_part_definitions),

		&GET_HS_FUNCTION(data_array_info),

		// debug functions
		// &GET_HS_FUNCTION(test_networking),
	};

	static const long K_HS_YELO_FUNCTION_COUNT =::std::size(hs_yelo_functions);

	// Validate our definition list has the same amount as our exposed enumeration count
	static_assert(::std::size(hs_yelo_functions) == Enums::k_hs_function_enumeration_count);
	// Validate we haven't gone over our limit of allowed new script functions
	static_assert(Enums::k_hs_function_enumeration_count <= (Enums::k_hs_script_functions_count_upgrade - Enums::k_hs_functions_count));

	// This actually isn't entirely needed. However, the yelo scripting definitions include the definition
	// index and later versions of the blam engine don't use the name to perform look-ups on script definitions,
	// they use the actual definition index. So...I'm just looking to be a little more consistent and who knows
	// maybe some of these may serve a purpose in being implemented? Yeah because we all know how much better
	// lightmaps are when you run radiosity in-game %-)

	//////////////////////////////////////////////////////////////////////////
	// hs definitions
	Yelo::Scripting::hs_function_definition function_debug_pvs_definition = {
		Enums::_hs_type_void, 0, "debug_pvs", reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")), nullptr, "", "",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)), 1, {Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_radiosity_start_definition = {
		Enums::_hs_type_void,
		0,
		"radiosity_start",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_radiosity_save_definition = {
		Enums::_hs_type_void,
		0,
		"radiosity_save",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_bool}
	};
	Yelo::Scripting::hs_function_definition function_radiosity_debug_point_definition = {
		Enums::_hs_type_void,
		0,
		"radiosity_debug_point",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_debug_teleport_player_definition = {
		Enums::_hs_type_void,
		0,
		"debug_teleport_player",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_hud_team_icon_set_pos_definition = {
		Enums::_hs_type_void,
		0,
		"hud_team_icon_set_pos",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};
	Yelo::Scripting::hs_function_definition function_hud_team_icon_set_scale_definition = {
		Enums::_hs_type_void,
		0,
		"hud_team_icon_set_scale",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_real, Enums::_hs_type_real}
	};
	Yelo::Scripting::hs_function_definition function_hud_team_background_set_pos_definition = {
		Enums::_hs_type_void,
		0,
		"hud_team_background_set_pos",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};
	Yelo::Scripting::hs_function_definition function_hud_team_background_set_scale_definition = {
		Enums::_hs_type_void,
		0,
		"hud_team_background_set_scale",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_reload_shader_transparent_chicago_definition = {
		Enums::_hs_type_void,
		0,
		"reload_shader_transparent_chicago",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_bool}
	};
	Yelo::Scripting::hs_function_definition function_rasterizer_reload_effects_definition = {
		Enums::_hs_type_void,
		0,
		"rasterizer_reload_effects",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_oid_watch_definition = {
		Enums::_hs_type_void,
		0,
		"oid_watch",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};
	Yelo::Scripting::hs_function_definition function_oid_dump_definition = {
		Enums::_hs_type_void,
		0,
		"oid_dump",
		static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};
	Yelo::Scripting::hs_function_definition function_oid_status_definition = {
		Enums::_hs_type_void, 0, "oid_status", static_cast<Yelo::Scripting::proc_hs_parse>(nullptr), //reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr, "", "",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)), 1, {Enums::_hs_type_string}
	};

	HS_GLOBAL(radiosity_quality, short, nullptr);
	HS_GLOBAL(radiosity_step_count, short, nullptr);
	HS_GLOBAL(radiosity_lines, bool, nullptr);
	HS_GLOBAL(radiosity_normals, bool, nullptr);
	//////////////////////////////////////////////////////////////////////////

	struct s_fixup_globals {
		struct fixup {
			long index;
			union {
				void *address;

				hs_function_definition *function;
				hs_global_definition   *global;
			};
			bool is_global;
			unsigned char : 8;
			unsigned short : 16;
		};

#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
		fixup fixups[];
#pragma warning( pop )
	}                 _fixup_globals           = {
		{ // fixups
			{0x11C, &Yelo::Scripting::function_debug_pvs_definition},
			{0x11D, &Yelo::Scripting::function_radiosity_start_definition},
			{0x11E, &Yelo::Scripting::function_radiosity_save_definition},
			{0x11F, &Yelo::Scripting::function_radiosity_debug_point_definition},

			{0x170, &Yelo::Scripting::function_debug_teleport_player_definition},

			{0x17D, &Yelo::Scripting::function_hud_team_icon_set_pos_definition},
			{0x17E, &Yelo::Scripting::function_hud_team_icon_set_scale_definition},
			{0x17F, &Yelo::Scripting::function_hud_team_background_set_pos_definition},
			{0x180, &Yelo::Scripting::function_hud_team_background_set_scale_definition},

			{0x1A8, &Yelo::Scripting::function_reload_shader_transparent_chicago_definition},
			{0x1A9, &Yelo::Scripting::function_rasterizer_reload_effects_definition},

			{0x21C, &Yelo::Scripting::function_oid_watch_definition},
			{0x21D, &Yelo::Scripting::function_oid_dump_definition},
			{0x21E, &Yelo::Scripting::function_oid_status_definition},

			{0x112, &Yelo::Scripting::global_radiosity_quality_definition, true},
			{0x113, &Yelo::Scripting::global_radiosity_step_count_definition, true},
			{0x114, &Yelo::Scripting::global_radiosity_lines_definition, true},
			{0x115, &Yelo::Scripting::global_radiosity_normals_definition, true},

			{NONE}
		},
	};

	static void NullifyScriptLibraryFixups() {
		if (!CurrentEngine.IsSapien()) {
			NullifyScriptFunctionWithParams(Yelo::Scripting::function_debug_pvs_definition);
			NullifyScriptFunctionWithParams(Yelo::Scripting::function_radiosity_start_definition);
			NullifyScriptFunctionWithParams(Yelo::Scripting::function_radiosity_save_definition);
			NullifyScriptFunctionWithParams(Yelo::Scripting::function_radiosity_debug_point_definition);
			NullifyScriptFunctionWithParams(Yelo::Scripting::function_oid_watch_definition);
			NullifyScriptFunctionWithParams(Yelo::Scripting::function_oid_dump_definition);
			NullifyScriptFunctionWithParams(Yelo::Scripting::function_oid_status_definition);
		}

		NullifyScriptFunctionWithParams(Yelo::Scripting::function_debug_teleport_player_definition);

		NullifyScriptFunctionWithParams(Yelo::Scripting::function_hud_team_icon_set_pos_definition);
		NullifyScriptFunctionWithParams(Yelo::Scripting::function_hud_team_icon_set_scale_definition);
		NullifyScriptFunctionWithParams(Yelo::Scripting::function_hud_team_background_set_pos_definition);
		NullifyScriptFunctionWithParams(Yelo::Scripting::function_hud_team_background_set_scale_definition);

		NullifyScriptFunctionWithParams(Yelo::Scripting::function_reload_shader_transparent_chicago_definition);
		NullifyScriptFunctionWithParams(Yelo::Scripting::function_rasterizer_reload_effects_definition);
	}

	struct s_upgrade_globals {
		struct {
			long count;
			long capacity;
			hs_function_definition *table[Enums::k_hs_script_functions_count_upgrade];
			long yelo_start_index; // starting index of our custom 'yelo' functions
		} functions;

		struct {
			long count;
			long capacity;
			hs_global_definition *table[Enums::k_hs_external_globals_count_upgrade];
			long yelo_start_index; // starting index of our custom 'yelo' globals
		} globals;
	};

	static s_upgrade_globals _upgrade_globals       = {
		{0, Enums::k_hs_script_functions_count_upgrade},
		{0, Enums::k_hs_external_globals_count_upgrade}
	};

	static void InitializeLibraryFixups() {
		NullifyScriptLibraryFixups();

		//////////////////////////////////////////////////////////////////////////
		// Add back functions/globals which don't appear in the release builds (and the like).
		// The expressions don't do anything...unless you find a reason to implement them...
		s_fixup_globals::fixup *fixups = _fixup_globals.fixups;
		while (fixups->index != NONE) {
			if (!fixups->is_global) {
				_upgrade_globals.functions.table[fixups->index] = fixups->function;
				_upgrade_globals.functions.count++;
			} else {
				_upgrade_globals.globals.table[fixups->index] = fixups->global;
				_upgrade_globals.globals.count++;
			}

			fixups++;
		}
		//////////////////////////////////////////////////////////////////////////
	}




#include "globals_declarations.h"
#include "script_extensions/hs_base.h"
// #include "../../network/server/network_server_manager_structures.hpp"
// #include "../../network/server/network_server_manager.hpp"
// #include "../../network/networking.h"
// #include "../../game/engines/engine.hpp"

	static void MemoryUpgradesInitialize() {
		InitializeLibraryFixups();

		//////////////////////////////////////////////////////////////////////////
		// Add the game's functions/globals to our upgraded memory
		static const rsize_t K_HS_FUNCTION_TABLE_COUNT = 0x211;

		static const rsize_t K_HS_EXTERNAL_GLOBALS_COUNT = 0x1EC - 1; // NOTE: we don't copy the 'rasterizer_frame_drop_ms' definition as its not defined in the tools


		for (rsize_t x = 0, index = 0;
			  x < K_HS_FUNCTION_TABLE_COUNT;
			  index++) {
			if (_upgrade_globals.functions.table[index] == nullptr) {
				_upgrade_globals.functions.table[index] = hs_function_table[x++];
				_upgrade_globals.functions.count++;
			}
		}

		for (rsize_t x = 0, index = 0;
			  x < K_HS_EXTERNAL_GLOBALS_COUNT;
			  index++) {
			if (_upgrade_globals.globals.table[index] == nullptr) {
				hs_global_definition &glob =
												*(_upgrade_globals.globals.table[index] = hs_external_globals[x++]);
				_upgrade_globals.globals.count++;

				// Is this the global who we want to override for exposing opensauce status?
				if (strcmp(glob.name, k_external_global_opensauce_override_name) == 0)
					InitializeExternalGlobalOpenSauceOverride(glob);
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// Add Yelo's functions/globals to our upgraded memory
		{
			_upgrade_globals.functions.yelo_start_index = _upgrade_globals.functions.count;
			for (size_t x = 0; x < K_HS_YELO_FUNCTION_COUNT; x++,
				_upgrade_globals.functions.count++)
				_upgrade_globals.functions.table[_upgrade_globals.functions.count] =
					hs_yelo_functions[x];

			_upgrade_globals.globals.yelo_start_index = _upgrade_globals.globals.count;
			for (size_t x = 0; x < K_HS_YELO_GLOBALS_COUNT; x++,
				_upgrade_globals.globals.count++)
				_upgrade_globals.globals.table[_upgrade_globals.globals.count] =
					hs_yelo_globals[x];
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// Update the game code to use OUR function/global definition tables
		{
			void ** table_references = CurrentEngine.GetHsFunctionTableReferences();
			if (table_references != nullptr) {
				hs_function_definition ****definitions = reinterpret_cast<hs_function_definition ****>(table_references);
				const size_t           k_count         = CurrentEngine.GetNumberOfFunctionTableReferences();

				for (size_t x = 0; x < k_count; x++) {
					*definitions[x] = &_upgrade_globals.functions.table[0];
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
	}

	void* scripting_game_change_version_id_evaluate(void** arguments)
	{
		// struct s_arguments {
		// 	bool and_game_build;
		// 	unsigned char : 8; unsigned short : 16;
		// 	const char *  version_str;
		// }* args = reinterpret_cast<s_arguments *>(arguments);
		// TypeHolder result; result.pointer = nullptr;
		//
		// result.boolean = BuildNumber::ChangeAdvertisedVersion(args->version_str, args->and_game_build);
		//
		// return result.pointer;
	}


	static void* scripting_game_engine_data_get_integer_evaluate(void** arguments)
	{
		struct s_arguments {
			const char *  data_name;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		if( !strcmp(args->data_name,"type") )			result.long = GameEngine::Current() != nullptr ? Yelo::GameEngine::GlobalVariant()->game_engine_index : Yelo::Enums::_game_engine_none;
		else if( !strcmp(args->data_name,"is_teams") )		result.long = CAST(long, Yelo::GameEngine::GlobalVariant()->universal_variant.teams);
		else if( !strcmp(args->data_name,"is_odd_man_out") )result.long = CAST(long, Yelo::GameEngine::GlobalVariant()->universal_variant.odd_man_out);
		else if( !strcmp(args->data_name,"lives") )			result.long = Yelo::GameEngine::GlobalVariant()->universal_variant.lives;
		else if( !strcmp(args->data_name,"score_to_win") )	result.long = Yelo::GameEngine::GlobalVariant()->universal_variant.score_to_win;

		// CTF
		//	else if( !strcmp(args->data_name,"ctf:") )			result.long = GameEngine::GlobalVariant();

		// Oddball
		else if( !strcmp(args->data_name,"oddball:ball_count") )	result.long = Yelo::GameEngine::GlobalVariant()->game_engine_variant.oddball.ball_count;
		//	else if( !strcmp(args->data_name,"oddball:") )		result.long = GameEngine::GlobalVariant();

		else												result.long = NONE;

		return result.pointer;
	}


	static void* scripting_machine_is_host()
	{
		TypeHolder result; result.pointer = nullptr;

		result.boolean = false; //GameState::IsServer();

		return result.pointer;
	}

	static void* scripting_machine_is_dedi()
	{
		TypeHolder result; result.pointer = nullptr;

		result.boolean = false;

		return result.pointer;
	}


	static void* scripting_abs_integer_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		result.long = abs(args->value);

		return result.pointer;
	}
	static void* scripting_abs_real_evaluate(void** arguments)
	{
		struct s_arguments {
			real value;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		result.real = abs(args->value);

		return result.pointer;
	}

	static void* scripting_bitwise_and_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			long flags;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		result.uint = CAST(uint,args->value) & CAST(uint,args->flags);

		return result.pointer;
	}
	static void* scripting_bitwise_or_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			long flags;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		result.uint = CAST(uint,args->value) | CAST(uint,args->flags);

		return result.pointer;
	}
	static void* scripting_bitwise_xor_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			long flags;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		result.uint = CAST(uint,args->value) ^ CAST(uint,args->flags);

		return result.pointer;
	}
	static void* scripting_bitwise_lhs_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			long bit_count;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		if(args->bit_count >= 0 && args->bit_count < BIT_COUNT(long))
			result.uint = CAST(uint,args->value) << args->bit_count;

		return result.pointer;
	}
	static void* scripting_bitwise_rhs_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			long bit_count;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		if(args->bit_count >= 0 && args->bit_count < BIT_COUNT(long))
			result.uint = CAST(uint,args->value) >> args->bit_count;

		return result.pointer;
	}
	static void* scripting_bit_test_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			short bit_index;
			unsigned short : 16;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		if(args->bit_index >= 0 && args->bit_index < BIT_COUNT(long))
			result.boolean = TEST_FLAG(CAST(uint,args->value), args->bit_index);

		return result.pointer;
	}
	static void* scripting_bit_toggle_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			short bit_index;
			unsigned short : 16;
			bool add_or_remove;
			unsigned char : 8; unsigned short : 16;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		if(args->bit_index >= 0 && args->bit_index < BIT_COUNT(long))
		{
			uint value = CAST(uint,args->value);
			result.uint = SET_FLAG(value, args->bit_index, args->add_or_remove);
		}
		else result.uint = CAST(uint,args->value);

		return result.pointer;
	}
	static void* scripting_bit_flags_test_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			long flags;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		result.boolean = (CAST(uint,args->value) & CAST(uint,args->flags)) != 0;

		return result.pointer;
	}
	static void* scripting_bit_flags_toggle_evaluate(void** arguments)
	{
		struct s_arguments {
			long value;
			long flags;
			bool add_or_remove;
			unsigned char : 8; unsigned short : 16;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		{
			uint value = CAST(uint,args->value);
			uint flags = CAST(uint,args->flags);
			result.uint = args->add_or_remove ? value | flags : value & ~flags;
		}

		return result.pointer;
	}
	static void* scripting_hex_string_to_long_evaluate(void** arguments)
	{
		struct s_arguments {
			const char * str;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		sscanf_s(args->str, "%x", &result.uint);

		return result.pointer;
	}


	static void* scripting_display_scripted_ui_widget_evaluate(void** arguments)
	{
		struct s_arguments {
			const char * name;
		}* args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result; result.pointer = nullptr;

		result.boolean = Yelo::UIWidgets::DisplayScriptedWidget(args->name);

		return result.pointer;
	}

	static void* scripting_play_bink_movie_evaluate(void** arguments)
	{
		struct s_arguments {
			const char * name;
		}* args = reinterpret_cast<s_arguments *>(arguments);

		if(GameState::IsLocal())
			Yelo::blam::bink_playback_start(args->name);

		return nullptr;
	}


	static void InitializeMiscFunctions() {
		InitializeScriptFunctionWithParams(Enums::_hs_function_game_change_version_id, scripting_game_change_version_id_evaluate);

		InitializeScriptFunctionWithParams(Enums::_hs_function_game_engine_data_get_integer, scripting_game_engine_data_get_integer_evaluate);

		// InitializeScriptFunction(Enums::_hs_function_machine_is_host, scripting_machine_is_host);
		// InitializeScriptFunction(Enums::_hs_function_machine_is_dedi, scripting_machine_is_dedi);


		//YELO_INIT_SCRIPT_FUNCTION_USER(Enums::_hs_function_pp_load, Rasterizer::PostProcessing::Scripting::HS_Load);
		//YELO_INIT_SCRIPT_FUNCTION_USER(Enums::_hs_function_pp_unload, Rasterizer::PostProcessing::Scripting::HS_Unload);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_instance_index_by_name, Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceIndexByName);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_instance_active, Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectInstanceActive);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_instance_fade, Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectInstanceFade);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_instance_current_fade, Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceCurrentFade);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_instance_fade_direction, Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceFadeDirection);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_index_by_name, Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectIndexByName);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_is_valid, Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectIsValid);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_shader_variable_index_by_name, Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectShaderVariableIndexByName);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_variable_boolean, Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableBoolean);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_variable_integer, Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableInteger);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_variable_real, Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableReal);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_instance_active, Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderInstanceActive);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_size, Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetSize);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_exposure, Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetExposure);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_mix_amount, Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMixAmount);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_minimum_color, Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMinimumColor);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_maximum_color, Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMaximumColor);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_transform_actor, AI::Transform::HS_AITransformActor);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_transform_actors, AI::Transform::HS_AITransformActors);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_transform_actors_by_type, AI::Transform::HS_AITransformActorsByType);
		//YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_actor_is_transforming, AI::Transform::HS_AIActorIsTransforming);

		//////////////////////////////////////////////////////////////////////////
		// Numbers
		InitializeScriptFunctionWithParams(Enums::_hs_function_abs_integer, scripting_abs_integer_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_abs_real, scripting_abs_real_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_and, scripting_bitwise_and_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_or, scripting_bitwise_or_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_xor, scripting_bitwise_xor_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_lhs, scripting_bitwise_lhs_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_rhs, scripting_bitwise_rhs_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bit_test, scripting_bit_test_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bit_toggle, scripting_bit_toggle_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bit_flags_test, scripting_bit_flags_test_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_bit_flags_toggle, scripting_bit_flags_toggle_evaluate);
		InitializeScriptFunctionWithParams(Enums::_hs_function_hex_string_to_long, scripting_hex_string_to_long_evaluate);
		//////////////////////////////////////////////////////////////////////////

		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_display_scripted_ui_widget, scripting_display_scripted_ui_widget_evaluate);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_play_bink_movie, scripting_play_bink_movie_evaluate);


		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_thread_count, Networking::HTTP::Server::HTTPServerSetThreadCount);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_root, Networking::HTTP::Server::HTTPServerSetRoot);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_throttle, Networking::HTTP::Server::HTTPServerSetThrottle);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_ports, Networking::HTTP::Server::HTTPServerSetPorts);
		YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_show_config, Networking::HTTP::Server::HTTPServerShowConfig);
		YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_start, Networking::HTTP::Server::HTTPServerStart);
		YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_stop, Networking::HTTP::Server::HTTPServerStop);

		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_log_enable, Networking::HTTP::Server::HTTPServerLogEnable);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_connection_ban, Networking::HTTP::Server::BanManager::HTTPServerSetConnectionBan);
		YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_banlist, Networking::HTTP::Server::BanManager::HTTPServerBanlist);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_banlist_file, Networking::HTTP::Server::BanManager::HTTPServerBanlistFile);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_ban_ip, Networking::HTTP::Server::BanManager::HTTPServerBanIP);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_unban_ip, Networking::HTTP::Server::BanManager::HTTPServerUnbanIP);

		YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_mapdownload_start_server, Networking::HTTP::Server::MapDownload::MapDownloadStartServer);
		YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_mapdownload_stop_server, Networking::HTTP::Server::MapDownload::MapDownloadStopServer);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_mapdownload_set_part_definitions_path, Networking::HTTP::Server::MapDownload::MapDownloadSetPartDefinitionsPath);
		YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_mapdownload_set_host, Networking::HTTP::Server::MapDownload::MapDownloadSetHost);
		YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_mapdownload_reload_map_part_definitions, Networking::HTTP::Server::MapDownload::MapDownloadReloadMapPartDefinitions);

		// Depreceated
		Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zones_reset);
		Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zone_current_switch_variant);
		Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zone_switch_variant);
		Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zone_switch_sky);
	}

	HS_FUNCTION(runtime_integers_reset, void, "");
	Yelo::Scripting::hs_function_definition function_runtime_integer_get_definition = {
		Enums::_hs_type_long,
		0,
		"runtime_integer_get",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};
	Yelo::Scripting::hs_function_definition function_runtime_integer_set_definition = {
		Enums::_hs_type_long,
		0,
		"runtime_integer_set",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index> <value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_long}
	};
	Yelo::Scripting::hs_function_definition function_runtime_integer_inc_definition = {
		Enums::_hs_type_long,
		0,
		"runtime_integer_inc",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};
	Yelo::Scripting::hs_function_definition function_runtime_integer_dec_definition = {
		Enums::_hs_type_long,
		0,
		"runtime_integer_dec",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};
	Yelo::Scripting::hs_function_definition function_runtime_integer_operation_definition = {
		Enums::_hs_type_long,
		0,
		"runtime_integer_operation",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index> <operation-name> <operation-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_short, Enums::_hs_type_string, Enums::_hs_type_long}
	};

	HS_FUNCTION(runtime_vectors_reset, void, "");

	Yelo::Scripting::hs_function_definition function_runtime_vector_get_element_definition = {
		Enums::_hs_type_real,
		0,
		"runtime_vector_get_element",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index> <element-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_runtime_vector_set_element_definition = {
		Enums::_hs_type_bool,
		0,
		"runtime_vector_set_element",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index> <element-index> <operation-name> <operation-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_string, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_runtime_vector_set_definition = {
		Enums::_hs_type_bool,
		0,
		"runtime_vector_set",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index> <operation-name> <value-x> <value-y> <value-z>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		5,
		{Enums::_hs_type_short, Enums::_hs_type_string, Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_runtime_vector_operation_definition = {
		Enums::_hs_type_bool,
		0,
		"runtime_vector_operation",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index> <operation-name> <operation-vector-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_short, Enums::_hs_type_string, Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_runtime_vector_to_string_definition = {
		Enums::_hs_type_string,
		0,
		"runtime_vector_to_string",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short,}
	};


	HS_FUNCTION(pp_load, void, "loads post processing");
	HS_FUNCTION(pp_unload, void, "unloads post processing");

	Yelo::Scripting::hs_function_definition function_pp_get_effect_instance_index_by_name_definition = {
		Enums::_hs_type_short,
		0,
		"pp_get_effect_instance_index_by_name",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns the index of the effect instance named <instance-name>",
		"<instance-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_pp_set_effect_instance_active_definition = {
		Enums::_hs_type_void,
		0,
		"pp_set_effect_instance_active",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"instantly set an effect instance to on or off",
		"<instance-index> <instance-enabled>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_pp_set_effect_instance_fade_definition = {
		Enums::_hs_type_void,
		0,
		"pp_set_effect_instance_fade",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"fade an effect instance in or out",
		"<instance-index> <fade-start> <fade-end> <fade-time>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_short, Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_get_effect_instance_current_fade_definition = {
		Enums::_hs_type_real,
		0,
		"pp_get_effect_instance_current_fade",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns current fade amount of an effect instance",
		"<instance-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_pp_get_effect_instance_fade_direction_definition = {
		Enums::_hs_type_short,
		0,
		"pp_get_effect_instance_fade_direction",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns fade direction of an effect instance, -1 = no fade, 0 = fade out, 1 = fade in",
		"<instance-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_pp_get_effect_index_by_name_definition = {
		Enums::_hs_type_short,
		0,
		"pp_get_effect_index_by_name",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns the index of the effect named <effect-name>",
		"<effect-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_pp_get_effect_is_valid_definition = {
		Enums::_hs_type_bool,
		0,
		"pp_get_effect_is_valid",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns whether an effect is valid and ready to use",
		"<effect-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_pp_get_effect_shader_variable_index_by_name_definition = {
		Enums::_hs_type_short,
		0,
		"pp_get_effect_shader_variable_index_by_name",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"gets the index of an exposed post processing effect variable named <name>",
		"<effect-index> <variable-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_pp_set_effect_shader_variable_boolean_definition = {
		Enums::_hs_type_void,
		0,
		"pp_set_effect_shader_variable_boolean",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the value of an exposed boolean variable in a post processing effect",
		"<effect-index> <variable-index> <boolean> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_bool, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_set_effect_shader_variable_integer_definition = {
		Enums::_hs_type_void,
		0,
		"pp_set_effect_shader_variable_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the value of an exposed integer variable in a post processing effect",
		"<effect-index> <variable-index> <long> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_long, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_set_effect_shader_variable_real_definition = {
		Enums::_hs_type_void,
		0,
		"pp_set_effect_shader_variable_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the value of an exposed float/vector/color variable in a post processing effect",
		"<effect-index> <variable-index> <real> <real> <real> <real> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		7,
		{Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_set_effect_shader_instance_active_definition = {
		Enums::_hs_type_void,
		0,
		"pp_set_effect_shader_instance_active",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"disables/enables a shader instance in an effect",
		"<effect-index> <shader-instance-index> <boolean>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_pp_bloom_set_size_definition = {
		Enums::_hs_type_void,
		0,
		"pp_bloom_set_size",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"changes the bloom size over a period of time",
		"<real> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_bloom_set_exposure_definition = {
		Enums::_hs_type_void,
		0,
		"pp_bloom_set_exposure",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"changes the bloom exposure over a period of time",
		"<real> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_bloom_set_mix_amount_definition = {
		Enums::_hs_type_void,
		0,
		"pp_bloom_set_mix_amount",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"changes the bloom mix amount over a period of time",
		"<real> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_bloom_set_minimum_color_definition = {
		Enums::_hs_type_void,
		0,
		"pp_bloom_set_minimum_color",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"changes the bloom minimum color over a period of time",
		"<real> <real> <real> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_pp_bloom_set_maximum_color_definition = {
		Enums::_hs_type_void,
		0,
		"pp_bloom_set_maximum_color",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"changes the bloom maximum color over a period of time",
		"<real> <real> <real> <real>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_volume_test_player_team_definition = {
		Enums::_hs_type_bool,
		0,
		"volume_test_player_team",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns true if any players of the specified team are within the specified volume.",
		"<volume> <team-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_trigger_volume, Enums::_hs_type_short}
	};
	Yelo::Scripting::hs_function_definition function_volume_test_player_team_all_definition = {
		Enums::_hs_type_bool,
		0,
		"volume_test_player_team_all",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns true if all players of the specified team are within the specified volume.",
		"<volume> <team-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_trigger_volume, Enums::_hs_type_short}
	};
	Yelo::Scripting::hs_function_definition function_player_team_teleport_definition = {
		Enums::_hs_type_void,
		0,
		"player_team_teleport",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"moves the specified team to the specified flag.",
		"<team-index> <cutscene-flag>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_cutscene_flag}
	};

	Yelo::Scripting::hs_function_definition function_player_team_players_definition = {
		Enums::_hs_type_object_list,
		0,
		"player_team_players",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"returns a list of players on the specified team",
		"<team-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_player_data_get_integer_definition = {
		Enums::_hs_type_long,
		0,
		"player_data_get_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<player-list-index> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_player_team_data_get_integer_definition = {
		Enums::_hs_type_long,
		0,
		"player_team_data_get_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<team-index> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_player_data_get_object_definition = {
		Enums::_hs_type_object,
		0,
		"player_data_get_object",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<player-list-index> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_player_data_get_real_definition = {
		Enums::_hs_type_real,
		0,
		"player_data_get_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<player-list-index> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_short, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_player_data_set_real_definition = {
		Enums::_hs_type_void,
		0,
		"player_data_set_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<player-list-index> <data-name> <data-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_short, Enums::_hs_type_string, Enums::_hs_type_real}
	};
	HS_FUNCTION(player_local_get, short, "");

	Yelo::Scripting::hs_function_definition function_objects_distance_to_object_definition = {
		Enums::_hs_type_real,
		0,
		"objects_distance_to_object",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<objects> <destination-object>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_object_list, Enums::_hs_type_object}
	};

	Yelo::Scripting::hs_function_definition function_object_data_get_real_definition = {
		Enums::_hs_type_real,
		0,
		"object_data_get_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<object> <data-name> <subdata-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_object, Enums::_hs_type_string, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_object_data_set_real_definition = {
		Enums::_hs_type_void,
		0,
		"object_data_set_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<object> <data-name> <subdata-name> <data-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_object, Enums::_hs_type_string, Enums::_hs_type_string, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_object_data_set_vector_definition = {
		Enums::_hs_type_bool,
		0,
		"object_data_set_vector",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<object> <data-name> <vector-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_object, Enums::_hs_type_string, Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_object_data_save_vector_definition = {
		Enums::_hs_type_bool,
		0,
		"object_data_save_vector",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<object> <data-name> <vector-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_object, Enums::_hs_type_string, Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_weapon_data_get_real_definition = {
		Enums::_hs_type_real,
		0,
		"weapon_data_get_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<weapon> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_object, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_weapon_data_set_real_definition = {
		Enums::_hs_type_void,
		0,
		"weapon_data_set_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<weapon> <data-name> <data-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_object, Enums::_hs_type_string, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_weapon_data_magazine_get_integer_definition = {
		Enums::_hs_type_long,
		0,
		"weapon_data_magazine_get_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<weapon> <magazine-index> <data-name> <subdata-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		4,
		{Enums::_hs_type_object, Enums::_hs_type_long, Enums::_hs_type_string, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_weapon_data_magazine_set_integer_definition = {
		Enums::_hs_type_void,
		0,
		"weapon_data_magazine_set_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<weapon> <magazine-index> <data-name> <subdata-name> <data-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		5,
		{Enums::_hs_type_object, Enums::_hs_type_long, Enums::_hs_type_string, Enums::_hs_type_string, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_weapon_data_trigger_set_real_definition = {
		Enums::_hs_type_void,
		0,
		"weapon_data_trigger_set_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<weapon> <trigger-index> <data-name> <subdata-name> <data-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		5,
		{Enums::_hs_type_object, Enums::_hs_type_long, Enums::_hs_type_string, Enums::_hs_type_string, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_unit_data_get_object_definition = {
		Enums::_hs_type_object,
		0,
		"unit_data_get_object",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<unit> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_unit, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_unit_data_get_integer_definition = {
		Enums::_hs_type_long,
		0,
		"unit_data_get_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<unit> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_unit, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_unit_data_set_integer_definition = {
		Enums::_hs_type_void,
		0,
		"unit_data_set_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<unit> <data-name> <data-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_unit, Enums::_hs_type_string, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_unit_data_get_real_definition = {
		Enums::_hs_type_real,
		0,
		"unit_data_get_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<unit> <data-name>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_unit, Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_unit_data_set_real_definition = {
		Enums::_hs_type_void,
		0,
		"unit_data_set_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<unit> <data-name> <data-value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_unit, Enums::_hs_type_string, Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_abs_integer_definition = {
		Enums::_hs_type_long,
		0,
		"abs_integer",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_abs_real_definition = {
		Enums::_hs_type_real,
		0,
		"abs_real",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_real}
	};

	Yelo::Scripting::hs_function_definition function_bitwise_and_definition = {
		Enums::_hs_type_long,
		0,
		"bitwise_and",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <flags>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_bitwise_or_definition = {
		Enums::_hs_type_long,
		0,
		"bitwise_or",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <flags>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_bitwise_xor_definition = {
		Enums::_hs_type_long,
		0,
		"bitwise_xor",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <flags>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_bitwise_lhs_definition = {
		Enums::_hs_type_long,
		0,
		"bitwise_lhs",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <bit-count>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_bitwise_rhs_definition = {
		Enums::_hs_type_long,
		0,
		"bitwise_rhs",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <bit-count>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_bit_test_definition = {
		Enums::_hs_type_bool,
		0,
		"bit_test",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <bit-index>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_bit_toggle_definition = {
		Enums::_hs_type_long,
		0,
		"bit_toggle",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <bit-index> <on-or-off>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_long, Enums::_hs_type_short, Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_bit_flags_test_definition = {
		Enums::_hs_type_bool,
		0,
		"bit_flags_test",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <flags>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		2,
		{Enums::_hs_type_long, Enums::_hs_type_long}
	};

	Yelo::Scripting::hs_function_definition function_bit_flags_toggle_definition = {
		Enums::_hs_type_long,
		0,
		"bit_flags_toggle",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value> <flags> <add-or-remove>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_long, Enums::_hs_type_long, Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_hex_string_to_long_definition = {
		Enums::_hs_type_long,
		0,
		"hex_string_to_long",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"",
		"<value>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	HS_FUNCTION(sv_mapdownload_start_server, void, "starts the map download server");
	HS_FUNCTION(sv_mapdownload_stop_server, void, "stops the map download server");

	Yelo::Scripting::hs_function_definition function_sv_mapdownload_set_part_definitions_path_definition = {
		Enums::_hs_type_void,
		0,
		"sv_mapdownload_set_part_definitions_path",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the path containing the map part definition xmls",
		"<string>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};
	Yelo::Scripting::hs_function_definition function_sv_mapdownload_set_host_definition = {
		Enums::_hs_type_void,
		0,
		"sv_mapdownload_set_host",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the web address where the map parts are being hosted",
		"<string>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};
	HS_FUNCTION(sv_mapdownload_reload_map_part_definitions, void, "re-loads the map part definitions from the specified folder");

	Yelo::Scripting::hs_function_definition function_sv_httpserver_set_thread_count_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_set_thread_count",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the number of http threads the http server starts",
		"<short>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_sv_httpserver_set_root_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_set_root",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the folder to serve files from",
		"<string>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_sv_httpserver_set_throttle_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_set_throttle",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the maximum bandwidth the http server will use in bytes per sec, append k and m for kilo and mega bytes respectively",
		"<string>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_sv_httpserver_set_ports_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_set_ports",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the ports to listen on",
		"<string>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	HS_FUNCTION(sv_httpserver_show_config, void, "displays the current http server config options");

	HS_FUNCTION(sv_httpserver_start, void, "starts the http server");

	HS_FUNCTION(sv_httpserver_stop, void, "stop the http server");

	Yelo::Scripting::hs_function_definition function_sv_httpserver_log_enable_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_log_enable",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"set whether to output http server events into the server log",
		"<bool>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_bool}
	};

	Yelo::Scripting::hs_function_definition function_sv_httpserver_set_connection_ban_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_set_connection_ban",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the variables that controls how many connections an IP can have over time",
		"<max_connections> <connection_cooloff> <forget_connection_time>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		3,
		{Enums::_hs_type_long, Enums::_hs_type_real, Enums::_hs_type_real}
	};

	HS_FUNCTION(sv_httpserver_banlist, void, "prints the current http server ip ban list");

	Yelo::Scripting::hs_function_definition function_sv_httpserver_banlist_file_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_banlist_file",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"sets the http servers ban list suffix",
		"<string>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		1,
		{Enums::_hs_type_string}
	};

	Yelo::Scripting::hs_function_definition function_sv_httpserver_ban_ip_definition = {
		Enums::_hs_type_void,
		0,
		"sv_httpserver_ban_ip",
		reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
		nullptr,
		"adds an ip to the ban list, the version bool is false for ipv4 and true for ipv6",
		"<bool> <short> <short> <short> <short> <short> <short> <short> <short>",
		(1 << (Yelo::Flags::_hs_access_enabled_bit)),
		9,
		{Enums::_hs_type_bool, Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_short, Enums::_hs_type_short}
	};

	Yelo::Scripting::hs_function_definition function_sv_httpserver_unban_ip_definition = {
		Enums::_hs_type_void, 0, "sv_httpserver_unban_ip", reinterpret_cast<Yelo::Scripting::proc_hs_parse>(CurrentEngine.getFunctionBegin("hs_macro_function_parse")), nullptr,
		"removes an ip from the ban list by its index", "<short>", (1 << (Yelo::Flags::_hs_access_enabled_bit)), 1, {Enums::_hs_type_short}
	};
};
