#pragma once

#include <optional>
#include "function_definitions.h"
#include "../CurrentEngine.h"

namespace Yelo::Scripting {
	static Yelo::Scripting::proc_hs_parse macro_parse_fun;

	template <Enums::hs_type hsT, short numParams, Enums::hs_type ...hsArgTypes>
	static hs_function_definition generate_definition(const char *name, const char *info, const char *param) {
		return {
			hsT,
			0,
			name,
			macro_parse_fun,
			nullptr,
			info,
			param,
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			numParams,
			{hsArgTypes...}
		};
	}

	void InitializeHSFunctionDefinitions() {
		static std::optional<uintptr_t> proc_parse_hs;

		if (!proc_parse_hs) {
			proc_parse_hs = CurrentEngine.getFunctionBegin("hs_macro_function_parse");
			if (!proc_parse_hs) {
				throw "Can't find hs macro function parse. HS scripting extensions will not work.";
			}
		}

		macro_parse_fun = reinterpret_cast<Yelo::Scripting::proc_hs_parse>(*proc_parse_hs);

		function_physics_get_gravity_definition = {
			Enums::_hs_type_real, 0, "physics_get_gravity",
			macro_parse_fun,
			nullptr,
			"",
			nullptr,
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			0
		};

		function_physics_set_gravity_definition = {
			Enums::_hs_type_void, 0, "physics_set_gravity",
			macro_parse_fun,
			nullptr, "", "gravity fraction",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Yelo::Enums::hs_type::_hs_type_real}
		};

		function_physics_constants_reset_definition = {
			Enums::_hs_type_void,
			0,
			"physics_constants_reset",
			macro_parse_fun,
			nullptr,
			"",
			nullptr,
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			0
		};

		// #include "Game/ScriptLibrary.Definitions.RuntimeData.inl"
		{
			using namespace Yelo::Enums;
			function_game_change_version_id_definition = generate_definition<_hs_type_bool, 2, _hs_type_bool, _hs_type_string>(
				"game_change_version_id",
				"returns whether the change was successful or not",
				"<also-change-game-build-string> <version-string>");
		}

		function_machine_is_host_definition = {
			HS_TYPE(bool),
			0,
			"machine_is_host",
			macro_parse_fun,
			nullptr,
			"returns whether or not the local machine is the host",
			nullptr,
			FLAG(Yelo::Flags::_hs_access_enabled_bit),
			0
		};
		function_machine_is_dedi_definition = {
			Enums::_hs_type_bool,
			0,
			"machine_is_dedi",
			macro_parse_fun,
			nullptr,
			"returns whether or not the local machine is a dedicated server",
			nullptr,
			FLAG(Yelo::Flags::_hs_access_enabled_bit),
			0
		};

		function_display_scripted_ui_widget_definition = {
			Enums::_hs_type_bool,
			0,
			"display_scripted_ui_widget",
			macro_parse_fun,
			nullptr,
			"",
			"<name>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		function_play_bink_movie_definition = {
			Enums::_hs_type_void,
			0,
			"play_bink_movie",
			macro_parse_fun,
			nullptr,
			"",
			"<filename>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		// HS_FUNCTION(scenario_faux_zones_reset, void, "depreceated, do not use");
		function_switch_variant_definition = {
			Enums::_hs_type_bool,
			0,
			"scenario_faux_zone_current_switch_variant",
			macro_parse_fun,
			nullptr,
			"depreceated, do not use",
			"<variant-name>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		{
			using namespace Yelo::Enums;
			function_scenario_faux_zone_switch_variant_definition = generate_definition<_hs_type_bool, 2, _hs_type_string, _hs_type_string>(
				"scenario_faux_zone_switch_variant",
				"depreceated, do not use",
				"<zone-name> <variant-name>");
		}

		function_scenario_faux_zone_switch_sky_definition = {
			Enums::_hs_type_bool,
			0,
			"scenario_faux_zone_switch_sky",
			macro_parse_fun,
			nullptr,
			"depreceated, do not use",
			"<zone-sky-name>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		{
			using namespace Yelo::Enums;
			function_structure_bsp_set_lightmap_set_definition = generate_definition<_hs_type_bool, 2, _hs_type_short, _hs_type_string>(
				"structure_bsp_set_lightmap_set",
				"Sets a bsp's lightmap set",
				"<bsp-index> <set-name>");

			function_structure_bsp_set_sky_set_definition = generate_definition<_hs_type_bool, 2, _hs_type_short, _hs_type_string>(
				"structure_bsp_set_sky_set",
				"Sets a bsp's sky set",
				"<bsp-index> <set-name>");

		};

		function_vehicle_remapper_enabled_definition = {
			Enums::_hs_type_bool,
			0,
			"vehicle_remapper_enabled",
			macro_parse_fun,
			nullptr,
			"returns the remapper state before the function call",
			"state_name",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		function_data_array_info_definition = {
			Enums::_hs_type_void,
			0,
			"data_array_info",
			macro_parse_fun,
			nullptr,
			"",
			"<data-array-name>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		{
			using namespace Yelo::Enums;
			function_ai_transform_actor_definition = generate_definition<_hs_type_bool, 3, Enums::_hs_type_object, Enums::_hs_type_string, Enums::_hs_type_string>(
				"ai_transform_actor",
				"Transforms an actor into the specified target. Returns false if it fails. Empty names causes random selection.",
				"<object> <transform_name> <target_name>");

			function_ai_transform_actors_definition = generate_definition<_hs_type_bool, 3, Enums::_hs_type_object_list, Enums::_hs_type_string, Enums::_hs_type_string>(
				"ai_transform_actors",
				"Transforms a list of actors into the specified target. Returns false if it fails. Empty names causes random selection.",
				"<objects> <transform_name> <target_name>");

			function_ai_transform_actors_by_type_definition = generate_definition<_hs_type_bool, 4,
																										 Enums::_hs_type_object_list,
																										 Enums::_hs_type_actor_variant,
																										 Enums::_hs_type_string,
																										 Enums::_hs_type_string>(
				"ai_transform_actors_by_type",
				"Transforms actors in a list of a specific type into the specified target. Returns false if it fails. Empty names causes random selection.",
				"<objects> <actor_variant> <transform_name> <target_name>");
		}

		function_ai_actor_is_transforming_definition = {
			Enums::_hs_type_bool,
			0,
			"ai_actor_is_transforming",
			macro_parse_fun,
			nullptr,
			"Returns true if the specified actor is transforming.",
			"<object>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_object}
		};
		//////////////////////////////////////////////////////////////////////////
		// hs definitions
		function_debug_pvs_definition                = {
			Enums::_hs_type_void, 0, "debug_pvs",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_bool}
		};

		function_radiosity_start_definition = {
			Enums::_hs_type_void,
			0,
			"radiosity_start",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_bool}
		};

		function_radiosity_save_definition        = {
			Enums::_hs_type_void,
			0,
			"radiosity_save",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_bool}
		};
		function_radiosity_debug_point_definition = {
			Enums::_hs_type_void,
			0,
			"radiosity_debug_point",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_bool}
		};

		{
			using namespace Yelo::Enums;

			function_debug_teleport_player_definition = generate_definition<_hs_type_void, 2, _hs_type_short, _hs_type_short>(
				"debug_teleport_player", "", "");

			function_hud_team_icon_set_pos_definition = generate_definition<_hs_type_void, 2, _hs_type_long, _hs_type_long>(
				"hud_team_icon_set_pos", "", "");

			function_hud_team_icon_set_scale_definition = generate_definition<_hs_type_void, 2, _hs_type_real, _hs_type_real>(
				"hud_team_icon_set_scale", "", "");

			function_hud_team_background_set_pos_definition = generate_definition<_hs_type_void, 2, _hs_type_long, _hs_type_long>(
				"hud_team_background_set_pos", "", "");

			function_hud_team_background_set_scale_definition = generate_definition<_hs_type_void, 2, _hs_type_real, _hs_type_real>(
				"hud_team_background_set_scale", "", "");
		};

		function_reload_shader_transparent_chicago_definition = {
			Enums::_hs_type_void,
			0,
			"reload_shader_transparent_chicago",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_bool}
		};
		function_rasterizer_reload_effects_definition         = {
			Enums::_hs_type_void,
			0,
			"rasterizer_reload_effects",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_bool}
		};

		function_oid_watch_definition  = {
			Enums::_hs_type_void,
			0,
			"oid_watch",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};
		function_oid_dump_definition   = {
			Enums::_hs_type_void,
			0,
			"oid_dump",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};
		function_oid_status_definition = {
			Enums::_hs_type_void, 0, "oid_status",
			macro_parse_fun,
			nullptr, "", "",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		function_runtime_integers_reset_definition = {
			Enums::_hs_type_void,
			0,
			"runtime_integers_reset",
			macro_parse_fun,
			nullptr,
			"",
			"",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_void}
		};

		function_runtime_integer_get_definition = {
			Enums::_hs_type_long,
			0,
			"runtime_integer_get",
			macro_parse_fun,
			nullptr,
			"",
			"<value-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_short}
		};
		function_runtime_integer_set_definition = {
			Enums::_hs_type_long,
			0,
			"runtime_integer_set",
			macro_parse_fun,
			nullptr,
			"",
			"<value-index> <value>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_short, Enums::_hs_type_long}
		};

		function_runtime_integer_inc_definition = {
			Enums::_hs_type_long,
			0,
			"runtime_integer_inc",
			macro_parse_fun,
			nullptr,
			"",
			"<value-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_short}
		};

		function_runtime_integer_dec_definition = {
			Enums::_hs_type_long,
			0,
			"runtime_integer_dec",
			macro_parse_fun,
			nullptr,
			"",
			"<value-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_short}
		};

		{
			using namespace Yelo::Enums;function_runtime_integer_operation_definition = generate_definition<_hs_type_long, 3, _hs_type_short, _hs_type_string, _hs_type_long>(
				"runtime_integer_operation", "", "<value-index> <operation-name> <operation-value>");
		};

		function_runtime_vectors_reset_definition = {
			Enums::_hs_type_void,
			0,
			"runtime_vectors_reset",
			CAST_PTR(Yelo::Scripting::proc_hs_parse, nullptr),
			nullptr,
			"",
			nullptr,
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			0
		};

		{
			using namespace Yelo::Enums;function_runtime_vector_get_element_definition = generate_definition<_hs_type_real, 2, _hs_type_short, _hs_type_short>(
				"runtime_vector_get_element", "", "<value-index> <element-index>");

			function_runtime_vector_set_element_definition = generate_definition<_hs_type_bool, 4, _hs_type_short, _hs_type_short, _hs_type_string, _hs_type_real>(
				"runtime_vector_set_element", "", "<value-index> <element-index> <operation-name> <operation-value>");

			function_runtime_vector_set_definition = generate_definition<_hs_type_bool, 5, _hs_type_short, _hs_type_string, _hs_type_real, _hs_type_real, _hs_type_real>(
				"runtime_vector_set", "", "<value-index> <operation-name> <value-x> <value-y> <value-z>");

			function_runtime_vector_operation_definition = generate_definition<_hs_type_bool, 3, _hs_type_short, _hs_type_string, _hs_type_short>(
				"runtime_vector_operation", "", "<value-index> <operation-name> <operation-vector-index>");
		};

		function_runtime_vector_to_string_definition = {
			Enums::_hs_type_string,
			0,
			"runtime_vector_to_string",
			macro_parse_fun,
			nullptr,
			"",
			"<value-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_short,}
		};

		function_pp_load_definition = {
			Enums::_hs_type_void,
			0,
			"pp_load",
			macro_parse_fun,
			nullptr,
			"loads post processing",
			nullptr,
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			0
		};

		function_pp_unload_definition = {
			Enums::_hs_type_void,
			0,
			"pp_unload",
			macro_parse_fun,
			nullptr,
			"unloads post processing",
			nullptr,
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			0
		};

		function_pp_get_effect_instance_index_by_name_definition = {
			Enums::_hs_type_short,
			0,
			"pp_get_effect_instance_index_by_name",
			macro_parse_fun,
			nullptr,
			"returns the index of the effect instance named <instance-name>",
			"<instance-name>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		{
			using namespace Yelo::Enums;

			function_pp_set_effect_instance_active_definition = generate_definition<_hs_type_void, 2, _hs_type_short, _hs_type_bool>(
				"pp_set_effect_instance_active",
				"instantly set an effect instance to on or off",
				"<instance-index> <instance-enabled>");

			function_pp_set_effect_instance_fade_definition = generate_definition<_hs_type_void, 4, _hs_type_short, _hs_type_real, _hs_type_real, _hs_type_real>(
				"pp_set_effect_instance_fade",
				"fade an effect instance in or out",
				"<instance-index> <fade-start> <fade-end> <fade-time>");
		};

		function_pp_get_effect_instance_current_fade_definition = {
			Enums::_hs_type_real,
			0,
			"pp_get_effect_instance_current_fade",
			macro_parse_fun,
			nullptr,
			"returns current fade amount of an effect instance",
			"<instance-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_short}
		};

		function_pp_get_effect_instance_fade_direction_definition = {
			Enums::_hs_type_short,
			0,
			"pp_get_effect_instance_fade_direction",
			macro_parse_fun,
			nullptr,
			"returns fade direction of an effect instance, -1 = no fade, 0 = fade out, 1 = fade in",
			"<instance-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_short}
		};

		function_pp_get_effect_index_by_name_definition = {
			Enums::_hs_type_short,
			0,
			"pp_get_effect_index_by_name",
			macro_parse_fun,
			nullptr,
			"returns the index of the effect named <effect-name>",
			"<effect-name>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};

		function_pp_get_effect_is_valid_definition = {
			Enums::_hs_type_bool,
			0,
			"pp_get_effect_is_valid",
			macro_parse_fun,
			nullptr,
			"returns whether an effect is valid and ready to use",
			"<effect-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_short}
		};


		{
			using namespace Yelo::Enums;

			function_pp_get_effect_shader_variable_index_by_name_definition = generate_definition<_hs_type_short, 2, _hs_type_short, _hs_type_string>(
				"pp_get_effect_shader_variable_index_by_name", "gets the index of an exposed post processing effect variable named <name>",
				"<effect-index> <variable-name>");

			function_pp_set_effect_shader_variable_boolean_definition = generate_definition<_hs_type_void, 4, _hs_type_short, _hs_type_short, _hs_type_bool, _hs_type_real>(
				"pp_set_effect_shader_variable_boolean", "sets the value of an exposed boolean variable in a post processing effect",
				"<effect-index> <variable-index> <boolean> <real>");

			function_pp_set_effect_shader_variable_integer_definition = generate_definition<_hs_type_void, 4, _hs_type_short, _hs_type_short, _hs_type_long, _hs_type_real>(
				"pp_set_effect_shader_variable_integer", "sets the value of an exposed integer variable in a post processing effect",
				"<effect-index> <variable-index> <long> <real>");

			function_pp_set_effect_shader_variable_real_definition = generate_definition<_hs_type_void, 7, _hs_type_short, _hs_type_short, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real>(
				"pp_set_effect_shader_variable_real", "sets the value of an exposed float/vector/color variable in a post processing effect",
				"<effect-index> <variable-index> <real> <real> <real> <real> <real>");

			function_pp_set_effect_shader_instance_active_definition = generate_definition<_hs_type_void, 3, _hs_type_short, _hs_type_short, _hs_type_bool>(
				"pp_set_effect_shader_instance_active", "disables/enables a shader instance in an effect",
				"<effect-index> <shader-instance-index> <boolean>");

			function_pp_bloom_set_size_definition = generate_definition<_hs_type_void, 2, _hs_type_real, _hs_type_real>(
				"pp_bloom_set_size", "changes the bloom size over a period of time", "<real> <real>");

			function_pp_bloom_set_exposure_definition = generate_definition<_hs_type_void, 2, _hs_type_real, _hs_type_real>(
				"pp_bloom_set_exposure", "changes the bloom exposure over a period of time", "<real> <real>");

			function_pp_bloom_set_mix_amount_definition = generate_definition<_hs_type_void, 2, _hs_type_real, _hs_type_real>(
				"pp_bloom_set_mix_amount", "changes the bloom mix amount over a period of time", "<real> <real>");

			function_pp_bloom_set_minimum_color_definition = generate_definition<_hs_type_void, 4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real>(
				"pp_bloom_set_minimum_color", "changes the bloom minimum color over a period of time", "<real> <real> <real> <real>");

			function_pp_bloom_set_maximum_color_definition = generate_definition<_hs_type_void, 4, _hs_type_real, _hs_type_real, _hs_type_real, _hs_type_real>(
				"pp_bloom_set_maximum_color", "changes the bloom maximum color over a period of time", "<real> <real> <real> <real>");

			function_volume_test_player_team_definition = generate_definition<_hs_type_bool, 2, _hs_type_trigger_volume, _hs_type_short>(
					"volume_test_player_team", "returns true if any players of the specified team are within the specified volume.",
					"<volume> <team-index>");

			function_volume_test_player_team_all_definition = generate_definition<_hs_type_bool, 2, _hs_type_trigger_volume, _hs_type_short>(
					"volume_test_player_team_all", "returns true if all players of the specified team are within the specified volume.",
					"<volume> <team-index>");

			function_player_team_teleport_definition = generate_definition<_hs_type_void, 2, _hs_type_short, _hs_type_cutscene_flag>(
					"player_team_teleport", "moves the specified team to the specified flag.", "<team-index> <cutscene-flag>");

			function_player_team_players_definition = generate_definition<_hs_type_object_list, 1, _hs_type_short>(
					"player_team_players", "returns a list of players on the specified team", "<team-index>");
			}


		// 				#define HS_TYPE(typen) _hs_type_##typen##
// #define HS_FUNCTION_WITH_PARAMS(name, ret, info, param, paramCount, ...) { \
//    using namespace Yelo::Enums; \
//    function_##name##_definition = generate_definition<HS_TYPE(ret), paramCount, __VA_ARGS__> ( \
//    #name, \
//    info, \
//    param); }

		HS_FUNCTION(physics_get_gravity, real, "");
		HS_FUNCTION(physics_constants_reset, void, "");
		HS_FUNCTION(player_local_get, short, "");

		{
			using namespace Yelo::Enums;function_player_data_get_integer_definition = generate_definition<_hs_type_long, 2, _hs_type_short, _hs_type_string>(
				"player_data_get_integer", "", "<player-list-index> <data-name>");

			function_player_team_data_get_integer_definition = generate_definition<_hs_type_long, 2, _hs_type_short, _hs_type_string>(
				"player_team_data_get_integer", "", "<team-index> <data-name>");

			function_player_data_get_object_definition = generate_definition<_hs_type_object, 2, _hs_type_short, _hs_type_string>(
				"player_data_get_object", "", "<player-list-index> <data-name>");

			function_player_data_get_real_definition = generate_definition<_hs_type_real, 2, _hs_type_short, _hs_type_string>(
				"player_data_get_real", "", "<player-list-index> <data-name>");

			function_player_data_set_real_definition = generate_definition<_hs_type_void, 3, _hs_type_short, _hs_type_string, _hs_type_real>(
				"player_data_set_real", "", "<player-list-index> <data-name> <data-value>");

			function_objects_distance_to_object_definition = generate_definition<_hs_type_real, 2, _hs_type_object_list, _hs_type_object>(
				"objects_distance_to_object", "", "<objects> <destination-object>");

			function_object_data_get_real_definition = generate_definition<_hs_type_real, 3, _hs_type_object, _hs_type_string, _hs_type_string>(
				"object_data_get_real", "", "<object> <data-name> <subdata-name>");

			function_object_data_set_real_definition = generate_definition<_hs_type_void, 4, _hs_type_object, _hs_type_string, _hs_type_string, _hs_type_real>(
				"object_data_set_real", "", "<object> <data-name> <subdata-name> <data-value>");

			function_object_data_set_vector_definition = generate_definition<_hs_type_bool, 3, _hs_type_object, _hs_type_string, _hs_type_short>(
				"object_data_set_vector", "", "<object> <data-name> <vector-index>");

			function_object_data_save_vector_definition = generate_definition<_hs_type_bool, 3, _hs_type_object, _hs_type_string, _hs_type_short>(
				"object_data_save_vector", "", "<object> <data-name> <vector-index>");

			function_weapon_data_get_real_definition = generate_definition<_hs_type_real, 2, _hs_type_object, _hs_type_string>(
				"weapon_data_get_real", "", "<weapon> <data-name>");

			function_weapon_data_set_real_definition = generate_definition<_hs_type_void, 3, _hs_type_object, _hs_type_string, _hs_type_real>(
				"weapon_data_set_real", "", "<weapon> <data-name> <data-value>");

			function_weapon_data_magazine_get_integer_definition = generate_definition<_hs_type_long, 4, _hs_type_object, _hs_type_long, _hs_type_string, _hs_type_string>(
				"weapon_data_magazine_get_integer", "", "<weapon> <magazine-index> <data-name> <subdata-name>");

			function_weapon_data_magazine_set_integer_definition = generate_definition<_hs_type_void, 5, _hs_type_object, _hs_type_long, _hs_type_string, _hs_type_string, _hs_type_long>(
				"weapon_data_magazine_set_integer", "", "<weapon> <magazine-index> <data-name> <subdata-name> <data-value>");

			function_weapon_data_trigger_set_real_definition = generate_definition<_hs_type_void, 5, _hs_type_object, _hs_type_long, _hs_type_string, _hs_type_string, _hs_type_real>(
				"weapon_data_trigger_set_real", "", "<weapon> <trigger-index> <data-name> <subdata-name> <data-value>");

			function_unit_data_get_object_definition = generate_definition<_hs_type_object, 2, _hs_type_unit, _hs_type_string>(
				"unit_data_get_object", "", "<unit> <data-name>");

			function_unit_data_get_integer_definition = generate_definition<_hs_type_long, 2, _hs_type_unit, _hs_type_string>(
				"unit_data_get_integer", "", "<unit> <data-name>");

			function_unit_data_set_integer_definition = generate_definition<_hs_type_void, 3, _hs_type_unit, _hs_type_string, _hs_type_long>(
				"unit_data_set_integer", "", "<unit> <data-name> <data-value>");

			function_unit_data_get_real_definition = generate_definition<_hs_type_real, 2, _hs_type_unit, _hs_type_string>(
				"unit_data_get_real", "", "<unit> <data-name>");

			function_unit_data_set_real_definition = generate_definition<_hs_type_void, 3, _hs_type_unit, _hs_type_string, _hs_type_real>(
				"unit_data_set_real", "", "<unit> <data-name> <data-value>");


			function_physics_set_gravity_definition = generate_definition<_hs_type_void, 1, _hs_type_real>(
				"physics_set_gravity", "", "gravity fraction");
		};

	#include "Game/ScriptLibrary.Definitions.RuntimeData.inl"

		{
			using namespace Yelo::Enums;

			function_game_change_version_id_definition = generate_definition<_hs_type_bool, 2, _hs_type_bool, _hs_type_string>(
				"game_change_version_id", "returns whether the change was successful or not",
				"<also-change-game-build-string> <version-string>");

			function_game_engine_data_get_integer_definition = generate_definition<_hs_type_long, 1, _hs_type_string>(
				"game_engine_data_get_integer", "", "<data-name>");
		};

		function_abs_integer_definition = {
			Enums::_hs_type_long,
			0,
			"abs_integer",
			macro_parse_fun,
			nullptr,
			"",
			"<value>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_long}
		};

		function_abs_real_definition = {
			Enums::_hs_type_real,
			0,
			"abs_real",
			macro_parse_fun,
			nullptr,
			"",
			"<value>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_real}
		};

		function_bitwise_and_definition = {
			Enums::_hs_type_long,
			0,
			"bitwise_and",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <flags>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_long, Enums::_hs_type_long}
		};

		function_bitwise_or_definition = {
			Enums::_hs_type_long,
			0,
			"bitwise_or",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <flags>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_long, Enums::_hs_type_long}
		};

		function_bitwise_xor_definition = {
			Enums::_hs_type_long,
			0,
			"bitwise_xor",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <flags>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_long, Enums::_hs_type_long}
		};

		function_bitwise_lhs_definition = {
			Enums::_hs_type_long,
			0,
			"bitwise_lhs",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <bit-count>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_long, Enums::_hs_type_long}
		};

		function_bitwise_rhs_definition = {
			Enums::_hs_type_long,
			0,
			"bitwise_rhs",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <bit-count>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_long, Enums::_hs_type_long}
		};

		function_bit_test_definition = {
			Enums::_hs_type_bool,
			0,
			"bit_test",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <bit-index>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_long, Enums::_hs_type_short}
		};

		function_bit_toggle_definition = {
			Enums::_hs_type_long,
			0,
			"bit_toggle",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <bit-index> <on-or-off>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			3,
			{Enums::_hs_type_long, Enums::_hs_type_short, Enums::_hs_type_bool}
		};

		function_bit_flags_test_definition = {
			Enums::_hs_type_bool,
			0,
			"bit_flags_test",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <flags>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			2,
			{Enums::_hs_type_long, Enums::_hs_type_long}
		};

		function_bit_flags_toggle_definition = {
			Enums::_hs_type_long,
			0,
			"bit_flags_toggle",
			macro_parse_fun,
			nullptr,
			"",
			"<value> <flags> <add-or-remove>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			3,
			{Enums::_hs_type_long, Enums::_hs_type_long, Enums::_hs_type_bool}
		};

		function_hex_string_to_long_definition = {
			Enums::_hs_type_long,
			0,
			"hex_string_to_long",
			macro_parse_fun,
			nullptr,
			"",
			"<value>",
			(1 << (Yelo::Flags::_hs_access_enabled_bit)),
			1,
			{Enums::_hs_type_string}
		};
	}
};
