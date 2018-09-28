/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace Environment
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION_ENVIRONMENT
#include "Memory/_EngineLayout.inl"

	/// <summary>	The pixel shader variables used by the custom lightmap shader. </summary>
	struct s_pixel_shader_variables {
		real base_normal_map_coefficient;
		real base_normal_map_z_coefficient;
		real spec_color_exponent_mask_toggle;
		PAD32;

		real specular_color_map_exponent;
		real specular_color_map_coefficient;
		real specular_lighting_exponent;
		real specular_lighting_coefficient;
		
		real_rgb_color perpendicular_specular_colour;
		real perpendicular_brightness;
		real_rgb_color parallel_specular_colour;
		real parallel_brightness;
	};

	/// <summary>	The list of feature combinations available. </summary>
	const uint32 g_shader_offsets[] = {
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 0, // Standard
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 1, // Directional Diffuse
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 2, // Directional Diffuse with Normal map
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 3, // Directional Specular
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 4, // Directional Specular with Normal map
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 5, // Directional Specular with Specular Map
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 6  // Directional Specular with Normal map and Specular Map
	};
	
	bool					g_rasterizer_environment_dlm_diffuse = true;
	bool					g_rasterizer_environment_dlm_specular = true;
	static uint32			g_current_shader_offset = 0;
	static _enum			g_extension_usage_mask;

	/// <summary>	Hook to change the used pixel shader to a custom one. </summary>
	__declspec(naked) void Hook_ShaderEnvironmentLightmapPS()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_RETN);

		_asm
		{
			mov		edx, [edx+84h]

			push	ebx
			add		edx, g_current_shader_offset					// Add g_current_shader_offset to edx to point to the custom shader
			pop		ebx

			jmp		RETN_ADDRESS
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Calculates the z channel coefficient. </summary>
	///
	/// <param name="bump_amount">	The bump amount. </param>
	///
	/// <returns>	The calculated z channel coefficient. </returns>
	static real CalculateZCoefficient(const real bump_amount)
	{
		real z_multiplier = 0;
		if (bump_amount > 0)
		{
			z_multiplier = __max(1.0f / bump_amount, 0.0f);
		}
		return __min(z_multiplier, 1.0f);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets up the custom shader variables for a shader environment. </summary>
	///
	/// <param name="shader">			The shader. </param>
	/// <param name="variables">		[out] The pixel variables. </param>
	/// <param name="lightmap_type">	The type of lightmap. </param>
	static void SetupShaderEnvironment(const TagGroups::s_shader_environment_definition& shader,
		s_pixel_shader_variables& variables,
		const _enum lightmap_type)
	{
		if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_directional_lightmaps_spec) &&
			(lightmap_type == Flags::_shader_extension_usage_bit_directional_lightmaps_spec))
		{
			auto& extension = shader.environment.shader_extension[0];

			if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_specular_map))
			{
				SetTexture(DX9::Direct3DDevice(), 6, extension.specular_color_map.tag_index);
				variables.specular_color_map_exponent = extension.specular_color_exponent;
				variables.specular_color_map_coefficient = extension.specular_color_coefficient;

				if(extension.specular_color_flags.alpha_as_exponent_mask_bit)
					variables.spec_color_exponent_mask_toggle = 1.0f;
			}
			
			variables.perpendicular_brightness = extension.perpendicular_brightness;
			variables.perpendicular_specular_colour = extension.perpendicular_tint_color;
			variables.parallel_brightness = extension.parallel_brightness;
			variables.parallel_specular_colour = extension.parallel_tint_color;

			variables.specular_lighting_exponent = extension.specular_lighting_exponent;
			variables.specular_lighting_coefficient = extension.specular_lighting_coefficient;
		}

		if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_normal_map))
		{
			if(shader.environment.shader_extension.Count == 1)
			{
				// If the shader has a shader extension block and a normal map, set the multiplier from the extension block
				auto& extension = shader.environment.shader_extension[0];

				float coefficient = extension.bump_amount;
				variables.base_normal_map_coefficient = coefficient;
				variables.base_normal_map_z_coefficient = CalculateZCoefficient(coefficient);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets up the custom shader variables for a shader model. </summary>
	///
	/// <param name="shader">			The shader. </param>
	/// <param name="variables">		[out] The pixel variables. </param>
	/// <param name="lightmap_type">	The type of lightmap. </param>
	static void SetupShaderModel(const TagGroups::s_shader_model_definition& shader,
		s_pixel_shader_variables& variables,
		const _enum lightmap_type)
	{
		if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_directional_lightmaps_spec) &&
			(lightmap_type == Flags::_shader_extension_usage_bit_directional_lightmaps_spec))
		{
			auto& extension = shader.model.maps.shader_extension[0];

			// Setup specular map
			if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_specular_map))
			{
				SetTexture(Yelo::DX9::Direct3DDevice(), 6, extension.specular_color.map.tag_index);

				variables.specular_color_map_coefficient = extension.specular_color.modifiers.coefficient;
				variables.specular_color_map_exponent = extension.specular_color.modifiers.exponent;

				if(extension.specular_color.modifiers.flags.alpha_as_exponent_mask_bit)
					variables.spec_color_exponent_mask_toggle = 1.0f;
			}

			variables.perpendicular_brightness = extension.perpendicular_brightness;
			variables.perpendicular_specular_colour = extension.perpendicular_tint_color;
			variables.parallel_brightness = extension.parallel_brightness;
			variables.parallel_specular_colour = extension.parallel_tint_color;

			variables.specular_lighting_exponent = extension.specular_lighting_exponent;
			variables.specular_lighting_coefficient = extension.specular_lighting_coefficient;
		}

		if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_normal_map))
		{
			// If the shader has a shader extension block and a normal map set the multiplier from the extension block
			auto& extension = shader.model.maps.shader_extension[0];
			float coefficient = extension.base_normal.modifiers.coefficient;

			variables.base_normal_map_coefficient = coefficient;
			variables.base_normal_map_z_coefficient = CalculateZCoefficient(coefficient);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets directional lightmap. </summary>
	///
	/// <param name="shader">		 	The shader. </param>
	/// <param name="extension_type">	The type of shader extension. </param>
	void SetupLightmapShader(const TagGroups::s_shader_definition* shader, const _enum lightmap_type)
	{
		g_current_shader_offset = 0;

		if(!g_rasterizer_environment_dlm_diffuse && (lightmap_type == Flags::_shader_extension_usage_bit_directional_lightmaps_diff))
		{
			return;
		}

		if(!g_rasterizer_environment_dlm_specular && (lightmap_type == Flags::_shader_extension_usage_bit_directional_lightmaps_spec))
		{
			return;
		}

		if((g_ps_support <= _ps_support_2_0) || !g_extensions_enabled)
		{
			return;
		}
		
		auto& shader_extension_usage = shader->shader.extension_usage; 
		if(!shader || !TEST_FLAG(shader_extension_usage, lightmap_type) || !Render::Lightmaps::HasDirectional())
		{
			return;
		}
		
		// Setup the shader variables
		s_pixel_shader_variables pixel_shader_variables = 
		{
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			{ 1.0f, 1.0f, 1.0f }, 1.0f,
			{ 1.0f, 1.0f, 1.0f }, 1.0f
		};

		switch(shader->shader.shader_type)
		{
		case Enums::_shader_type_environment:
			SetupShaderEnvironment(*CAST_PTR(const TagGroups::s_shader_environment_definition*, shader), pixel_shader_variables, lightmap_type);
			break;
		case Enums::_shader_type_model:
			SetupShaderModel(*CAST_PTR(const TagGroups::s_shader_model_definition*, shader), pixel_shader_variables, lightmap_type);
			break;
		YELO_ASSERT_CASE_UNREACHABLE();
		};

		// Pick the appropriate shader
		if(lightmap_type == Flags::_shader_extension_usage_bit_directional_lightmaps_diff)
		{
			if(!TEST_FLAG(shader_extension_usage, Flags::_shader_extension_usage_bit_normal_map))
			{
				g_current_shader_offset = g_shader_offsets[1];
			}
			else
			{
				g_current_shader_offset = g_shader_offsets[2];
			}
		}
		else
		{
			if(!TEST_FLAG(shader_extension_usage, Flags::_shader_extension_usage_bit_specular_map))
			{
				if(!TEST_FLAG(shader_extension_usage, Flags::_shader_extension_usage_bit_normal_map))
				{
					g_current_shader_offset = g_shader_offsets[3];
				}
				else
				{
					g_current_shader_offset = g_shader_offsets[4];
				}
			}
			else
			{
				if(!TEST_FLAG(shader_extension_usage, Flags::_shader_extension_usage_bit_normal_map))
				{
					g_current_shader_offset = g_shader_offsets[5];
				}
				else
				{
					g_current_shader_offset = g_shader_offsets[6];
				}
			}
		}

		// Set the custom pixel shader values
		DX9::Direct3DDevice()->SetPixelShaderConstantF(6, (float*)&pixel_shader_variables, 4);
	}

	typedef void (PLATFORM_API *structure_render_pass_begin_proc)(void* argument0);
	typedef void (PLATFORM_API *structure_render_pass_draw_proc)(const TagGroups::s_shader_definition* argument0, void* argument1, void* argument2, void* argument3, void* argument4, void* argument5);
	typedef void (PLATFORM_API *structure_render_pass_end_proc)(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Renders a structure bsp pass. </summary>
	///
	/// <param name="begin"> 	The pre-draw process. </param>
	/// <param name="draw">  	The draw process. </param>
	/// <param name="end">   	The post-draw process. </param>
	/// <param name="submit">	Flag for whether to submit or not. </param>
	__declspec(naked) void PLATFORM_API StructureRenderPass(const structure_render_pass_begin_proc begin,
		const structure_render_pass_draw_proc draw,
		const structure_render_pass_end_proc end,
		const bool submit)
	{
		static const uintptr_t dynamic_triangles_count_refptr = GET_FUNC_PTR(STRUCTURE_RENDER_DYNAMIC_TRIANGLES_COUNT_REF);
		static const uintptr_t dynamic_triangles_buffer_refptr = GET_FUNC_PTR(STRUCTURE_RENDER_DYNAMIC_TRIANGLES_BUFFER_REF);
		static const uintptr_t call_address = GET_FUNC_PTR(STRUCTURE_RENDER_PASS_FUNC);

		API_FUNC_NAKED_START()
			// Get dynamic_triangle_buffer_index
			call	Render::StructureRenderGlobals
			mov		ecx, [eax + 4]

			xor		eax, eax
			mov		al, submit
			push	eax
			push	end
			push	draw
			push	begin
			push	ecx				// dynamic_triangle_buffer_index
			mov		eax, dynamic_triangles_count_refptr
			mov		eax, [eax]		// dynamic_triangles_count
			mov		ecx, dynamic_triangles_buffer_refptr
			mov		ecx, [ecx]		// dynamic_triangles_buffer
			call	call_address
			add		esp, 14h
		API_FUNC_NAKED_END(0)
	}

#pragma region Specular Pass

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Rasterizer environment lightmap specular draw. </summary>
	///
	/// <param name="shader">	The shader. </param>
	/// <param name="arg2">  	[in,out] If non-null, the second argument. </param>
	/// <param name="arg3">  	[in,out] If non-null, the third argument. </param>
	/// <param name="arg4">  	[in,out] If non-null, the fourth argument. </param>
	/// <param name="arg5">  	[in,out] If non-null, the fifth argument. </param>
	/// <param name="arg6">  	[in,out] If non-null, the argument 6. </param>
	static void PLATFORM_API RasterizerEnvironmentLightmapSpecularDraw(const TagGroups::s_shader_definition* shader, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6)
	{
		typedef void (PLATFORM_API *shader_draw_func_t)(const TagGroups::s_shader_definition*, void*, void*, void*, void*, void*);

		// Skip drawing if the shader doesn't have specular enabled
		if(!(TEST_FLAG(shader->shader.extension_usage, Flags::_shader_extension_usage_bit_directional_lightmaps_diff) &&
			TEST_FLAG(shader->shader.extension_usage, Flags::_shader_extension_usage_bit_directional_lightmaps_spec)))
		{
			return;
		}

		SetupLightmapShader(shader, Flags::_shader_extension_usage_bit_directional_lightmaps_spec);

		CAST_PTR(shader_draw_func_t, GET_FUNC_VPTR(RASTERIZER_ENVIRONMENT_LIGHTMAP_DRAW_FUNC))(shader, arg2, arg3, arg4, arg5, arg6);
	}

	/// <summary>	Render specular directional lightmaps state setup. </summary>
	static void StructureRenderSpecularDirectionalLightmapsBegin()
	{
		auto* render_device = DX9::Direct3DDevice();
		
		render_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		render_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
		render_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		render_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
		render_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		render_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		render_device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		render_device->SetRenderState(D3DRS_ALPHAREF, 0);
		render_device->SetRenderState(D3DRS_ZENABLE, true);
		render_device->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
		render_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
		render_device->SetRenderState(D3DRS_FOGENABLE, false);

		render_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		render_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		render_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		render_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		render_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		render_device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		render_device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		render_device->SetSamplerState(2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		render_device->SetSamplerState(2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		render_device->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		render_device->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		render_device->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		render_device->SetSamplerState(3, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		render_device->SetSamplerState(3, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		render_device->SetSamplerState(3, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		render_device->SetSamplerState(3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		render_device->SetSamplerState(3, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		render_device->SetSamplerState(3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}

	/// <summary>	Render specular directional lightmaps pass. </summary>
	static void PLATFORM_API StructureRenderSpecularDirectionalLightmaps()
	{
		// Call the lights render specular function
		CAST_PTR(void (*)(void), GET_FUNC_VPTR(LIGHTS_RENDER_SPECULAR_FUNC))();

		if(!g_rasterizer_environment_dlm_diffuse || !g_rasterizer_environment_dlm_specular)
		{
			return;
		}

		// Render the DLM specular pass
		if(TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_directional_lightmaps_spec) &&
			Render::StructureRenderGlobals()->render_bsp)
		{
			auto* structure_bsp = Scenario::StructureBsp();
			if(!structure_bsp->lightmap_bitmaps.tag_index.IsNull() && Render::Lightmaps::HasDirectional())
			{
				StructureRenderSpecularDirectionalLightmapsBegin();

				StructureRenderPass(
					CAST_PTR(structure_render_pass_begin_proc, GET_FUNC_PTR(RASTERIZER_ENVIRONMENT_LIGHTMAP_BEGIN_FUNC)),
					&RasterizerEnvironmentLightmapSpecularDraw,
					CAST_PTR(structure_render_pass_end_proc, GET_FUNC_PTR(RASTERIZER_ENVIRONMENT_LIGHTMAP_END_FUNC)),
					false);
			}
		}
	}
#pragma endregion

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the lightmap texture samplers for the current shader. </summary>
	///
	/// <param name="shader">	The shader. </param>
	void SetLightmapSampler(const TagGroups::s_shader_definition* shader)
	{
		if((g_ps_support <= _ps_support_2_0) || !g_extensions_enabled || !g_rasterizer_environment_dlm_diffuse)
		{
			Render::Lightmaps::SetLightmapSamplers(false);
			return;
		}

		// Use directional lightmaps if enabled by the user and shader, otherwise force the standard lightmap and shader to be used.
		if(TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_directional_lightmaps_diff) &&
			TEST_FLAG(shader->shader.extension_usage, Flags::_shader_extension_usage_bit_directional_lightmaps_diff) &&
			Render::Lightmaps::HasDirectional())
		{
			Render::Lightmaps::SetLightmapSamplers(true);
		}
		else
		{
			Render::Lightmaps::SetLightmapSamplers(false);
			g_current_shader_offset = 0;
		}
	}

	/// <summary>	Applies the hooks for custom shader code. </summary>
	void ApplyHooks()
	{
		Memory::WriteRelativeJmp(&Hook_ShaderEnvironmentLightmapPS, GET_FUNC_VPTR(RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_HOOK), true);
		Memory::WriteRelativeCall(&StructureRenderSpecularDirectionalLightmaps, GET_FUNC_VPTR(LIGHTS_RENDER_SPECULAR_CALL), true);
	}

	void SetDiffuseDLMsEnabled(bool value)
	{
		g_extension_usage_mask &= ~Flags::_shader_extension_usage_directional_lightmaps_diff;
		g_extension_usage_mask |= (value ? Flags::_shader_extension_usage_directional_lightmaps_diff : 0);

		if(!value)
		{
			g_extension_usage_mask &= ~Flags::_shader_extension_usage_directional_lightmaps_spec;
		}
	}

	bool GetDiffuseDLMsEnabled()
	{
		return TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_directional_lightmaps_diff);
	}

	void SetSpecularDLMsEnabled(bool value)
	{
		g_extension_usage_mask &= ~Flags::_shader_extension_usage_directional_lightmaps_spec;
		g_extension_usage_mask |= (value ? Flags::_shader_extension_usage_directional_lightmaps_spec : 0);
	}

	bool GetSpecularDLMsEnabled()
	{
		return TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_directional_lightmaps_spec);
	}

};
