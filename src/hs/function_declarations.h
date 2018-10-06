#pragma once

#include <enums/hs_enums.h>
#include "script_extensions/hs_base.h"
#include "structures.h"
#include "../CurrentEngine.h"


//////////////////////////////////////////////////////////////////////////
// hs_function
// [name]	Code based name
// [ret]	hs_type this function returns
// [info]	(string) Function's "help" text, short little description
// CurrentEngine.getFunctionBegin("hs_macro_function_parse")),
// #define HS_FUNCTION(name, ret, info)                                 \
//    Yelo::Scripting::hs_function_definition function_##name##_definition = {   \
//       HS_TYPE(ret),0,                                             \
//       #name,                                                   \
//       CAST_PTR(Yelo::Scripting::proc_hs_parse, nullptr),   \
//       nullptr,                                                \
//       info,                                                   \
//       nullptr,                                                \
//       FLAG(Yelo::Flags::_hs_access_enabled_bit), 0                     \
//    }

// [name]	Code based name
// [ret]	[hs_type] this function returns
// [info]	(string) Function's "help" text, short little description
// [param]	(string) Function's "parameters" text, lists the parameter names
// [argc]	How many parameters this function takes
// [...]	List of parameter's [hs_type] types
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// hs_external_global
#define HS_GLOBAL(name, type, value)                              \
   Yelo::Scripting::hs_global_definition global_##name##_definition = {   \
      #name,                                                \
      HS_TYPE(type),                                          \
      0,                                                   \
      CAST_PTR(void*,value),                                    \
      FLAG(Yelo::Flags::_hs_access_enabled_bit)                     \
   }

#define HS_GLOBAL_EX(name, type, value, flags)                        \
   Yelo::Scripting::hs_global_definition global_##name##_definition = {   \
      #name,                                                \
      HS_TYPE(type),                                          \
      flags,                                                \
      CAST_PTR(void*,value),                                    \
      FLAG(Yelo::Flags::_hs_access_enabled_bit)                     \
   }

//////////////////////////////////////////////////////////////////////////

// #define GET_HS_FUNCTION(name) Yelo::Scripting::function_##name##_definition
#include "function_definitions.h"

namespace Yelo::Scripting {
	//////////////////////////////////////////////////////////////////////////
	// Functions
	static hs_function_definition *hs_yelo_functions[] = {
		// &GET_HS_FUNCTION(null),

		&Yelo::Scripting::function_volume_test_player_team_definition,
		&Yelo::Scripting::function_volume_test_player_team_definition,
		&Yelo::Scripting::function_volume_test_player_team_all_definition,
		&Yelo::Scripting::function_player_team_teleport_definition,
		&Yelo::Scripting::function_player_team_players_definition,

		&Yelo::Scripting::function_player_data_get_integer_definition,
		&Yelo::Scripting::function_player_team_data_get_integer_definition,
		&Yelo::Scripting::function_player_data_get_object_definition,
		&Yelo::Scripting::function_player_data_get_real_definition,
		&Yelo::Scripting::function_player_data_set_real_definition,
		&Yelo::Scripting::function_player_local_get_definition,

		&Yelo::Scripting::function_objects_distance_to_object_definition,
		&Yelo::Scripting::function_object_data_get_real_definition,
		&Yelo::Scripting::function_object_data_set_real_definition,
		&Yelo::Scripting::function_weapon_data_get_real_definition,
		&Yelo::Scripting::function_weapon_data_set_real_definition,
		&Yelo::Scripting::function_weapon_data_magazine_get_integer_definition,
		&Yelo::Scripting::function_weapon_data_magazine_set_integer_definition,
		&Yelo::Scripting::function_weapon_data_trigger_set_real_definition,
		&Yelo::Scripting::function_unit_data_get_object_definition,
		&Yelo::Scripting::function_unit_data_get_integer_definition,
		&Yelo::Scripting::function_unit_data_set_integer_definition,
		&Yelo::Scripting::function_unit_data_get_real_definition,
		&Yelo::Scripting::function_unit_data_set_real_definition,

		&Yelo::Scripting::function_physics_get_gravity_definition,
		&Yelo::Scripting::function_physics_set_gravity_definition,
		&Yelo::Scripting::function_physics_constants_reset_definition,

		&Yelo::Scripting::function_runtime_integers_reset_definition,
		&Yelo::Scripting::function_runtime_integer_get_definition,
		&Yelo::Scripting::function_runtime_integer_set_definition,
		&Yelo::Scripting::function_runtime_integer_inc_definition,
		&Yelo::Scripting::function_runtime_integer_dec_definition,

		&Yelo::Scripting::function_game_change_version_id_definition,
		&Yelo::Scripting::function_game_engine_data_get_integer_definition,

		&Yelo::Scripting::function_machine_is_host_definition,
		&Yelo::Scripting::function_machine_is_dedi_definition,

		&Yelo::Scripting::function_abs_integer_definition,
		&Yelo::Scripting::function_abs_real_definition,

		//TODO: POSTPROCESSING
		&Yelo::Scripting::function_pp_load_definition,
		&Yelo::Scripting::function_pp_unload_definition,
		//
		&Yelo::Scripting::function_pp_get_effect_instance_index_by_name_definition,
		&Yelo::Scripting::function_pp_set_effect_instance_active_definition,
		&Yelo::Scripting::function_pp_set_effect_instance_fade_definition,
		&Yelo::Scripting::function_pp_get_effect_instance_current_fade_definition,
		&Yelo::Scripting::function_pp_get_effect_instance_fade_direction_definition,
		&Yelo::Scripting::function_pp_get_effect_index_by_name_definition,
		&Yelo::Scripting::function_pp_get_effect_is_valid_definition,
		&Yelo::Scripting::function_pp_get_effect_shader_variable_index_by_name_definition,
		&Yelo::Scripting::function_pp_set_effect_shader_variable_boolean_definition,
		&Yelo::Scripting::function_pp_set_effect_shader_variable_integer_definition,
		&Yelo::Scripting::function_pp_set_effect_shader_variable_real_definition,
		&Yelo::Scripting::function_pp_set_effect_shader_instance_active_definition,
		&Yelo::Scripting::function_pp_bloom_set_size_definition,
		&Yelo::Scripting::function_pp_bloom_set_exposure_definition,
		&Yelo::Scripting::function_pp_bloom_set_mix_amount_definition,
		&Yelo::Scripting::function_pp_bloom_set_minimum_color_definition,
		&Yelo::Scripting::function_pp_bloom_set_maximum_color_definition,

		&Yelo::Scripting::function_bitwise_and_definition,
		&Yelo::Scripting::function_bitwise_or_definition,
		&Yelo::Scripting::function_bitwise_xor_definition,
		&Yelo::Scripting::function_bitwise_lhs_definition,
		&Yelo::Scripting::function_bitwise_rhs_definition,
		&Yelo::Scripting::function_bit_test_definition,
		&Yelo::Scripting::function_bit_toggle_definition,
		&Yelo::Scripting::function_bit_flags_test_definition,
		&Yelo::Scripting::function_bit_flags_toggle_definition,
		&Yelo::Scripting::function_hex_string_to_long_definition,

		&Yelo::Scripting::function_runtime_integer_operation_definition,
		&Yelo::Scripting::function_runtime_vectors_reset_definition,
		&Yelo::Scripting::function_runtime_vector_get_element_definition,
		&Yelo::Scripting::function_runtime_vector_set_element_definition,
		&Yelo::Scripting::function_runtime_vector_set_definition,
		&Yelo::Scripting::function_runtime_vector_operation_definition,
		&Yelo::Scripting::function_runtime_vector_to_string_definition,
		&Yelo::Scripting::function_object_data_set_vector_definition,
		&Yelo::Scripting::function_object_data_save_vector_definition,

		&Yelo::Scripting::function_display_scripted_ui_widget_definition,
		&Yelo::Scripting::function_play_bink_movie_definition,

		&Yelo::Scripting::function_scenario_faux_zones_reset_definition,
		&Yelo::Scripting::function_scenario_faux_zone_current_switch_variant_definition,
		&Yelo::Scripting::function_scenario_faux_zone_switch_variant_definition,
		&Yelo::Scripting::function_scenario_faux_zone_switch_sky_definition,

		&Yelo::Scripting::function_structure_bsp_set_lightmap_set_definition,
		&Yelo::Scripting::function_structure_bsp_set_sky_set_definition,

		&Yelo::Scripting::function_ai_transform_actor_definition,
		&Yelo::Scripting::function_ai_transform_actors_definition,
		&Yelo::Scripting::function_ai_transform_actors_by_type_definition,
		&Yelo::Scripting::function_ai_actor_is_transforming_definition,

		&Yelo::Scripting::function_vehicle_remapper_enabled_definition,

		&Yelo::Scripting::function_data_array_info_definition,

	};

	static const long K_HS_YELO_FUNCTION_COUNT = ::std::size(hs_yelo_functions);

	// Validate our definition list has the same amount as our exposed enumeration count
	// static_assert(::std::size(hs_yelo_functions) == Enums::k_hs_function_enumeration_count);
	// Validate we haven't gone over our limit of allowed new script functions
	// static_assert(Enums::k_hs_function_enumeration_count <= (Enums::k_hs_script_functions_count_upgrade - Enums::k_hs_functions_count));

	// This actually isn't entirely needed. However, the yelo scripting definitions include the definition
	// index and later versions of the blam engine don't use the name to perform look-ups on script definitions,
	// they use the actual definition index. So...I'm just looking to be a little more consistent and who knows
	// maybe some of these may serve a purpose in being implemented? Yeah because we all know how much better
	// lightmaps are when you run radiosity in-game %-)

	HS_GLOBAL(radiosity_quality, short, nullptr);
	HS_GLOBAL(radiosity_step_count, short, nullptr);
	HS_GLOBAL(radiosity_lines, bool, nullptr);
	HS_GLOBAL(radiosity_normals, bool, nullptr);
	//////////////////////////////////////////////////////////////////////////

	union globalOrFunction {
		void *address;

		hs_function_definition *function;
		hs_global_definition   *global;
	};

	struct s_fixup_globals {
		struct fixup {
			long             index;
			globalOrFunction function;
			bool             is_global;
			unsigned char  : 8;
			unsigned short : 16;
		};

#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
		fixup fixups[];
#pragma warning( pop )
	};

	struct s_upgrade_globals {
		struct {
			long                   count;
			long                   capacity;
			hs_function_definition *table[Enums::k_hs_script_functions_count_upgrade];
			long                   yelo_start_index; // starting index of our custom 'yelo' functions
		} functions;

		struct {
			long                 count;
			long                 capacity;
			hs_global_definition *table[Enums::k_hs_external_globals_count_upgrade];
			long                 yelo_start_index; // starting index of our custom 'yelo' globals
		} globals;
	};

#include "globals_declarations.h"
#include "script_extensions/hs_base.h"
#include "../gamestate/game_engine.h"

	// #include "../../network/server/network_server_manager_structures.hpp"
	// #include "../../network/server/network_server_manager.hpp"
	// #include "../../network/networking.h"
	// #include "../../game/engines/engine.hpp"



	void *scripting_game_change_version_id_evaluate(void **arguments) {
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

	static void *scripting_game_engine_data_get_integer_evaluate(void **arguments) {
		struct s_arguments {
			const char *data_name;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		if (!strcmp(args->data_name, "type"))
			result.int32 = Yelo::GameEngine::Current() != nullptr ? Yelo::GameEngine::GlobalVariant()->game_engine_index
																			: Yelo::Enums::_game_engine_none;
		else if (!strcmp(args->data_name, "is_teams")) result.int32 = CAST(long, Yelo::GameEngine::GlobalVariant()->universal_variant.teams);
		else if (!strcmp(args->data_name, "is_odd_man_out"))
			result.int32 = CAST(long, Yelo::GameEngine::GlobalVariant()->universal_variant.odd_man_out);
		else if (!strcmp(args->data_name, "lives")) result.int32 = Yelo::GameEngine::GlobalVariant()->universal_variant.lives;
		else if (!strcmp(args->data_name, "score_to_win")) result.int32 = Yelo::GameEngine::GlobalVariant()->universal_variant.score_to_win;

			// CTF
			//	else if( !strcmp(args->data_name,"ctf:") )			result.long = GameEngine::GlobalVariant();

			// Oddball
		else if (!strcmp(args->data_name, "oddball:ball_count"))
			result.int32 = Yelo::GameEngine::GlobalVariant()->game_engine_variant.oddball.ball_count;
		//	else if( !strcmp(args->data_name,"oddball:") )		result.long = GameEngine::GlobalVariant();

		else  result.int32 = NONE;

		return result.pointer;
	}

	static void *scripting_machine_is_host() {
		TypeHolder result;
		result.pointer = nullptr;

		result.boolean = false; //GameState::IsServer();

		return result.pointer;
	}

	static void *scripting_machine_is_dedi() {
		TypeHolder result;
		result.pointer = nullptr;

		result.boolean = false;

		return result.pointer;
	}

	static void *scripting_abs_integer_evaluate(void **arguments) {
		struct s_arguments {
			long value;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		result.int32 = abs(args->value);

		return result.pointer;
	}

	static void *scripting_abs_real_evaluate(void **arguments) {
		struct s_arguments {
			real value;
		} *args = reinterpret_cast<s_arguments *>(arguments);

		TypeHolder result;
		result.pointer = nullptr;

		result.real = abs(args->value);

		return result.pointer;
	}

	static void *scripting_bitwise_and_evaluate(void **arguments) {
		struct s_arguments {
			long value;
			long flags;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		result.uint32 = CAST(uint, args->value) & CAST(uint, args->flags);

		return result.pointer;
	}

	static void *scripting_bitwise_or_evaluate(void **arguments) {
		struct s_arguments {
			long value;
			long flags;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		result.uint32 = CAST(uint, args->value) | CAST(uint, args->flags);

		return result.pointer;
	}

	static void *scripting_bitwise_xor_evaluate(void **arguments) {
		struct s_arguments {
			long value;
			long flags;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		result.uint32 = CAST(uint, args->value) ^ CAST(uint, args->flags);

		return result.pointer;
	}

	static void *scripting_bitwise_lhs_evaluate(void **arguments) {
		struct s_arguments {
			long value;
			long bit_count;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		if (args->bit_count >= 0 && args->bit_count < BIT_COUNT(long))
			result.uint32 = CAST(uint, args->value) << args->bit_count;

		return result.pointer;
	}

	static void *scripting_bitwise_rhs_evaluate(void **arguments) {
		struct s_arguments {
			long value;
			long bit_count;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		if (args->bit_count >= 0 && args->bit_count < BIT_COUNT(long))
			result.uint32 = CAST(uint, args->value) >> args->bit_count;

		return result.pointer;
	}

	static void *scripting_bit_test_evaluate(void **arguments) {
		struct s_arguments {
			long  value;
			short bit_index;
			unsigned short : 16;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		if (args->bit_index >= 0 && args->bit_index < BIT_COUNT(long))
			result.boolean = TEST_FLAG(CAST(uint, args->value), args->bit_index);

		return result.pointer;
	}

	static void *scripting_bit_toggle_evaluate(void **arguments) {
		struct s_arguments {
			long  value;
			short bit_index;
			unsigned short : 16;
			bool add_or_remove;
			unsigned char : 8;
			unsigned short : 16;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		if (args->bit_index >= 0 && args->bit_index < BIT_COUNT(long)) {
			uint value = CAST(uint, args->value);
			result.uint32 = SET_FLAG(value, args->bit_index, args->add_or_remove);
		} else result.uint32 = CAST(uint, args->value);

		return result.pointer;
	}

	static void *scripting_bit_flags_test_evaluate(void **arguments) {
		struct s_arguments {
			long value;
			long flags;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		result.boolean = (CAST(uint, args->value) & CAST(uint, args->flags)) != 0;

		return result.pointer;
	}

	static void *scripting_bit_flags_toggle_evaluate(void **arguments) {
		struct s_arguments {
			long value;
			long flags;
			bool add_or_remove;
			unsigned char : 8;
			unsigned short : 16;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		{
			uint value = CAST(uint, args->value);
			uint flags = CAST(uint, args->flags);
			result.uint32 = args->add_or_remove ? value | flags : value & ~flags;
		}

		return result.pointer;
	}

	static void *scripting_hex_string_to_long_evaluate(void **arguments) {
		struct s_arguments {
			const char *str;
		} *args = reinterpret_cast<s_arguments *>(arguments);
		TypeHolder result;
		result.pointer = nullptr;

		sscanf_s(args->str, "%x", &result.uint32);

		return result.pointer;
	}

	static void *scripting_display_scripted_ui_widget_evaluate(void **arguments) {
		// struct s_arguments {
		// 	const char * name;
		// }* args = reinterpret_cast<s_arguments *>(arguments);
		// TypeHolder result; result.pointer = nullptr;
		//
		// result.boolean = Yelo::UIWidgets::DisplayScriptedWidget(args->name);
		//
		// return result.pointer;
		return nullptr;
	}

	static void *scripting_play_bink_movie_evaluate(void **arguments) {
		// struct s_arguments {
		// 	const char * name;
		// }* args = reinterpret_cast<s_arguments *>(arguments);
		//
		// if(GameState::IsLocal())
		// 	Yelo::blam::bink_playback_start(args->name);

		return nullptr;
	}

	static void InitializeMiscFunctions() {
		// InitializeScriptFunctionWithParams(Enums::_hs_function_game_change_version_id, scripting_game_change_version_id_evaluate);
		//
		// InitializeScriptFunctionWithParams(Enums::_hs_function_game_engine_data_get_integer, scripting_game_engine_data_get_integer_evaluate);

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
		// InitializeScriptFunctionWithParams(Enums::_hs_function_abs_integer, scripting_abs_integer_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_abs_real, scripting_abs_real_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_and, scripting_bitwise_and_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_or, scripting_bitwise_or_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_xor, scripting_bitwise_xor_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_lhs, scripting_bitwise_lhs_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_rhs, scripting_bitwise_rhs_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bit_test, scripting_bit_test_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bit_toggle, scripting_bit_toggle_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bit_flags_test, scripting_bit_flags_test_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_bit_flags_toggle, scripting_bit_flags_toggle_evaluate);
		// InitializeScriptFunctionWithParams(Enums::_hs_function_hex_string_to_long, scripting_hex_string_to_long_evaluate);
		// //////////////////////////////////////////////////////////////////////////
		//
		// YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_display_scripted_ui_widget, scripting_display_scripted_ui_widget_evaluate);
		// YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_play_bink_movie, scripting_play_bink_movie_evaluate);

		/////////////////////////////////////////////////////////////////////////
		// Depreceated
		// Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zones_reset);
		// Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zone_current_switch_variant);
		// Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zone_switch_variant);
		// Yelo::Scripting::NullifyScriptFunctionWithParams(Yelo::Enums::_hs_function_scenario_faux_zone_switch_sky);
	}
};
