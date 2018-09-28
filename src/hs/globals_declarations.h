#pragma once

#include "function_declarations.h"
#include "../ai/ai_yelo/AI.Transform.inl"
#include "../render/rasterizer/postprocessing/Scripting.h"
#include "../render/rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "../render/rasterizer/GBuffer.hpp"

namespace Yelo::Scripting {
#define HS_GLOBAL2(name, type, valuece, valueded) Yelo::Scripting::hs_global_definition global_##name##_definition = {#name, HS_TYPE(type), 0, CAST_PTR(void*,valuece), FLAG(Yelo::Flags::_hs_access_enabled_bit)}
	//////////////////////////////////////////////////////////////////////////
	// Globals
	// Yelo::Scripting::hs_global_definition global_rasterizer_rt_display_definition = {
	// 	"rasterizer_rt_display", Enums::_hs_type_short, 0, reinterpret_cast<void *>(&Yelo::dx9::c_gbuffer_system::g_debug_index), (1 << (Yelo::Flags::_hs_access_enabled_bit))
	// };
	Yelo::Scripting::hs_global_definition global_rasterizer_rt_display_definition              = {
		"rasterizer_rt_display", Enums::_hs_type_short, 0, reinterpret_cast<void *>(&Yelo::DX9::c_gbuffer_system::g_debug_index),
		(1 << (Yelo::Flags::_hs_access_enabled_bit))
	};
	HS_GLOBAL2(rasterizer_gbuffer_enabled, bool, &DX9::c_gbuffer_system::g_system_enabled, nullptr);

	HS_GLOBAL2(pp_external_post_processes_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_External(), nullptr);
	HS_GLOBAL2(pp_internal_post_processes_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_Internal(), nullptr);
	HS_GLOBAL2(pp_fxaa_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_FXAA(), nullptr);
	HS_GLOBAL2(pp_motion_blur_enabled, bool, &Rasterizer::PostProcessing::Scripting::Globals::Enabled_MotionBlur(), nullptr);
	HS_GLOBAL2(pp_motion_blur_amount, real, &Rasterizer::PostProcessing::Scripting::Globals::MotionBlur_Amount(), nullptr);

	HS_GLOBAL2(ai_transforms_enabled, bool, &AI::Transform::TransformsEnabled(), nullptr);

	HS_GLOBAL2(rasterizer_model_normal_mapping, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_normal_mapping, nullptr);
	HS_GLOBAL2(rasterizer_model_detail_normal_mapping, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_detail_normal_mapping,
				  nullptr);
	HS_GLOBAL2(rasterizer_model_specular_lights, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_specular_lights, nullptr);
	HS_GLOBAL2(rasterizer_model_specular_map, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_specular_map, nullptr);
	Yelo::Scripting::hs_global_definition global_rasterizer_environment_dlm_diffuse_definition = {
		"rasterizer_environment_dlm_diffuse",
		Enums::_hs_type_bool,
		0,
		(reinterpret_cast<void *>(&Rasterizer::ShaderExtension::Environment::g_rasterizer_environment_dlm_diffuse)),
		(1 << (Yelo::Flags::_hs_access_enabled_bit))
	};
	HS_GLOBAL2(rasterizer_environment_dlm_specular, bool, &Rasterizer::ShaderExtension::Environment::g_rasterizer_environment_dlm_specular,
				  nullptr);
	HS_GLOBAL2(rasterizer_effect_depth_fade, bool, &Rasterizer::ShaderExtension::Effect::g_rasterizer_effect_depth_fade, nullptr);

	//////////////////////////////////////////////////////////////////////////
	// Globals
	static hs_global_definition *hs_yelo_globals[] = {

		&Yelo::Scripting::global_rasterizer_rt_display_definition,
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
	static_assert(::std::size(hs_yelo_globals) == Yelo::Enums::k_hs_global_enumeration_count);

#pragma region script globals related

	static const long K_HS_YELO_GLOBALS_COUNT = ::std::size(hs_yelo_globals);

	static hs_global_definition **hs_external_globals;

#pragma endregion

};
