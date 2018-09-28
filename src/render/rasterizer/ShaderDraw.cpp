/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/ShaderDraw.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/shaders/shader_definitions.hpp>

#include "Memory/MemoryInterface.hpp"

#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

namespace Yelo { namespace Rasterizer { namespace ShaderDraw
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADERDRAW
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADERDRAW
#include "Memory/_EngineLayout.inl"

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Environment shader lightmap draw. </summary>
	///
	/// <param name="shader_pointer">	[in,out] If non-null, the shader pointer. </param>
	/// <param name="arg2">			 	[in,out] The second stock argument. </param>
	/// <param name="arg3">			 	[in,out] The third stock argument. </param>
	/// <param name="arg4">			 	[in,out] The fourth stock argument. </param>
	/// <param name="arg5">			 	[in,out] The fifth stock argument. </param>
	/// <param name="arg6">			 	[in,out] The sixth stock argument. </param>
	static void PLATFORM_API Environment_ShaderLightmapDiffuseDraw(const TagGroups::s_shader_definition* shader, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6)
	{
		typedef void (PLATFORM_API *shader_draw_func_t)(const TagGroups::s_shader_definition*, void*, void*, void*, void*, void*);

		DX9::c_gbuffer_system::RenderGBuffer() = true;

		ShaderExtension::Environment::SetupLightmapShader(shader, Flags::_shader_extension_usage_bit_directional_lightmaps_diff);

		static shader_draw_func_t STOCK_DRAW_FUNC = CAST_PTR(shader_draw_func_t, GET_FUNC_VPTR(RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP));
		STOCK_DRAW_FUNC(shader, arg2, arg3, arg4, arg5, arg6);

		DX9::c_gbuffer_system::RenderGBuffer() = false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Model shader environment draw. </summary>
	///
	/// <param name="shader_pointer">	[in,out] If non-null, the shader pointer. </param>
	/// <param name="arg2">			 	[in,out] The second stock argument. </param>
	/// <param name="arg3">			 	[in,out] The third stock argument. </param>
	/// <param name="arg4">			 	[in,out] The fourth stock argument. </param>
	/// <param name="arg5">			 	[in,out] The fifth stock argument. </param>
	/// <param name="arg6">			 	[in,out] The sixth stock argument. </param>
	/// <param name="arg7">			 	[in,out] The seventh stock argument. </param>
	static void PLATFORM_API Model_ShaderEnvironmentDraw(void* shader_pointer, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7)
	{
		typedef void (PLATFORM_API *shader_draw_func_t)(void*, void*, void*, void*, void*, void*, void*);

		DX9::c_gbuffer_system::RenderGBuffer() = true;

		Rasterizer::ShaderExtension::Model::SetModelNormSpec(shader_pointer);

		static shader_draw_func_t STOCK_DRAW_FUNC = CAST_PTR(shader_draw_func_t, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_ENVIRONMENT));
		STOCK_DRAW_FUNC(shader_pointer, arg2, arg3, arg4, arg5, arg6, arg7);

		DX9::c_gbuffer_system::RenderGBuffer() = false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Model shader model draw. </summary>
	///
	/// <param name="shader_pointer">	[in,out] If non-null, the shader pointer. </param>
	/// <param name="arg2">			 	[in,out] The second stock argument. </param>
	/// <param name="arg3">			 	[in,out] The third stock argument. </param>
	/// <param name="arg4">			 	[in,out] The fourth stock argument. </param>
	/// <param name="arg5">			 	[in,out] The fifth stock argument. </param>
	/// <param name="arg6">			 	[in,out] The sixth stock argument. </param>
	/// <param name="arg7">			 	[in,out] The seventh stock argument. </param>
	static void PLATFORM_API Model_ShaderModelDraw(void* shader_pointer, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7)
	{
		typedef void (PLATFORM_API *shader_draw_func_t)(void*, void*, void*, void*, void*, void*, void*);

		DX9::c_gbuffer_system::RenderGBuffer() = true;

		Rasterizer::ShaderExtension::Model::SetModelNormSpec(shader_pointer);

		static shader_draw_func_t STOCK_DRAW_FUNC = CAST_PTR(shader_draw_func_t, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_MODEL));
		STOCK_DRAW_FUNC(shader_pointer, arg2, arg3, arg4, arg5, arg6, arg7);

		DX9::c_gbuffer_system::RenderGBuffer() = false;
	}

	/// <summary>	Hooks the shader draw functions. </summary>
	void Initialize()
	{
		GET_PTR(RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP_SHADER_DRAW__DRAW_SHADER_LIGHTMAP_PTR) = &Environment_ShaderLightmapDiffuseDraw;

		GET_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_ENVIRONMENT_PTR) = &Model_ShaderEnvironmentDraw;
		GET_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_MODEL_PTR) = &Model_ShaderModelDraw;
	}

	/// <summary>	Unused. </summary>
	void Dispose()
	{

	}
};};};
#endif