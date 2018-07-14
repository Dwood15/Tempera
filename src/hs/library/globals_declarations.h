#pragma once


#include "function_declarations.h"

namespace Yelo::Scripting {

	//////////////////////////////////////////////////////////////////////////
	// Globals
	Yelo::Scripting::hs_global_definition global_rasterizer_rt_display_definition = {
		"rasterizer_rt_display", Enums::_hs_type_short, 0, reinterpret_cast<void *>(&Yelo::DX9::c_gbuffer_system::g_debug_index), (1 << (Yelo::Flags::_hs_access_enabled_bit))
	};
	HS_GLOBAL2(rasterizer_gbuffer_enabled, bool, &DX9::c_gbuffer_system::g_system_enabled, nullptr);

	HS_GLOBAL2(pp_external_post_processes_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_External(), nullptr);
	HS_GLOBAL2(pp_internal_post_processes_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_Internal(), nullptr);
	HS_GLOBAL2(pp_fxaa_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_FXAA(), nullptr);
	HS_GLOBAL2(pp_motion_blur_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_MotionBlur(), nullptr);
	HS_GLOBAL2(pp_motion_blur_amount, real, &Rasterizer::PostProcessing::Scripting::Globals::MotionBlur_Amount(), nullptr);

	HS_GLOBAL2(ai_transforms_enabled, bool, &AI::Transform::TransformsEnabled(), nullptr);

	HS_GLOBAL2(rasterizer_model_normal_mapping, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_normal_mapping, nullptr);
	HS_GLOBAL2(rasterizer_model_detail_normal_mapping, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_detail_normal_mapping, nullptr);
	HS_GLOBAL2(rasterizer_model_specular_lights, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_specular_lights, nullptr);
	HS_GLOBAL2(rasterizer_model_specular_map, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_specular_map, nullptr);
	HS_GLOBAL2(rasterizer_environment_dlm_diffuse, bool, &Rasterizer::ShaderExtension::Environment::g_rasterizer_environment_dlm_diffuse, nullptr);
	HS_GLOBAL2(rasterizer_environment_dlm_specular, bool, &Rasterizer::ShaderExtension::Environment::g_rasterizer_environment_dlm_specular, nullptr);
	HS_GLOBAL2(rasterizer_effect_depth_fade, bool, &Rasterizer::ShaderExtension::Effect::g_rasterizer_effect_depth_fade, nullptr);

	//////////////////////////////////////////////////////////////////////////
	// Globals
	static hs_global_definition *hs_yelo_globals[] = {

		&GET_HS_GLOBAL(rasterizer_rt_display),
		&GET_HS_GLOBAL(rasterizer_gbuffer_enabled),

		&GET_HS_GLOBAL(pp_external_post_processes_enabled),
		&GET_HS_GLOBAL(pp_internal_post_processes_enabled),
		&GET_HS_GLOBAL(pp_fxaa_enabled),
		&GET_HS_GLOBAL(pp_motion_blur_enabled),
		&GET_HS_GLOBAL(pp_motion_blur_amount),

		&GET_HS_GLOBAL(ai_transforms_enabled),

		&GET_HS_GLOBAL(rasterizer_model_normal_mapping),
		&GET_HS_GLOBAL(rasterizer_model_detail_normal_mapping),
		&GET_HS_GLOBAL(rasterizer_model_specular_lights),
		&GET_HS_GLOBAL(rasterizer_model_specular_map),
		&GET_HS_GLOBAL(rasterizer_environment_dlm_diffuse),
		&GET_HS_GLOBAL(rasterizer_environment_dlm_specular),
		&GET_HS_GLOBAL(rasterizer_effect_depth_fade),
	};
	// Validate our definition list has the same amount as our exposed enumeration count
	static_assert(std::size(hs_yelo_globals) == Yelo::Enums::k_hs_global_enumeration_count);


#pragma region script globals related

	static const long K_HS_YELO_GLOBALS_COUNT = std::size(hs_yelo_globals);
	long HSYeloGlobalCount() { return K_HS_YELO_GLOBALS_COUNT; }
	const hs_global_definition& HSYeloGlobal(short index) { return *hs_yelo_globals[index]; }


	static hs_global_definition** hs_external_globals;
	const hs_global_definition* HSExternalGlobals() DPTR_IMP_GET(hs_external_globals);

#pragma endregion

};
