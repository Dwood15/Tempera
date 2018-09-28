/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace Effect
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION_EFFECT
#include "Memory/_EngineLayout.inl"

	struct s_pixel_shader_variables {
		real m_far_clip_distance;
		real m_near_clip_distance;
		real m_depth_fade_distance;
		real m_camera_fade_distance;
	};
	
	const uint32 g_feature_mix_list[] = {
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 0,
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 1
	};
	
	bool					g_rasterizer_effect_depth_fade = true;
	static uint32			g_current_shader_offset = 0;
	static _enum			g_extension_usage_mask;

	void SetShaderEffectVariables(const TagGroups::s_shader_effect* shader);

	__declspec(naked) void Hook_ShaderEffect_ParticleSystem()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_EFFECT_PARTICLE_SYSTEM_RETN);

		_asm{
			mov		eax, [ebx + 8]
			add		esp, 4

			push	eax
			push	ecx
			push	edx

			push	eax
			call	SetShaderEffectVariables;

			pop		edx
			pop		ecx
			pop		eax

			jmp		RETN_ADDRESS;
		};
	}

	__declspec(naked) void Hook_ShaderEffect_Particle()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_EFFECT_PARTICLE_RETN);

		_asm{
			push	eax
			push	edx

			push	edi
			call	SetShaderEffectVariables;

			pop		edx
			pop		eax

			mov		al, [edi+28h]
			mov		ecx, [ebx]
			jmp		RETN_ADDRESS;
		};
	}

	__declspec(naked) void Hook_ShaderEffect_Contrail()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_EFFECT_CONTRAIL_RETN);

		_asm{
			push	eax
			push	ecx
			push	edx

			push	edi
			call	SetShaderEffectVariables;

			pop		edx
			pop		ecx
			pop		eax

			mov		[eax+8], ecx
			mov		[eax+0Ch], edi
			jmp		RETN_ADDRESS
		};
	}

	__declspec(naked) void Hook_ShaderEffectPixelShaderOffset()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_EFFECT_PS_INDEX_NO_RETN);

		_asm{
			push	ebx
			add		eax, g_current_shader_offset
			pop		ebx

			mov		eax, [eax + 80h]
			jmp		RETN_ADDRESS
		};
	}

	static const uint32 SetupShaderEffect(const TagGroups::s_shader_effect& shader, s_pixel_shader_variables& variables)
	{
		_enum feature_usage = shader.shader.extension_usage & g_extension_usage_mask;

		if(TEST_FLAG(feature_usage, Flags::_shader_extension_usage_bit_depth_fade))
		{
			if(shader.effect.shader_extension.Count == 1)
			{
				// If the shader has a shader extension block set the effect softness from the extension block
				auto& extension = shader.effect.shader_extension[0];
		
				variables.m_far_clip_distance = Render::RenderGlobals()->frustum.z_far;
				variables.m_near_clip_distance = Render::RenderGlobals()->frustum.z_near;
				variables.m_depth_fade_distance = extension.depth_fade_distance;
				variables.m_camera_fade_distance = extension.camera_fade_distance;

				auto& render_device = *DX9::Direct3DDevice();
				render_device.SetTexture(4, DX9::c_gbuffer_system::GBuffer().m_rt_depth.texture);

				render_device.SetSamplerState(4, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
				render_device.SetSamplerState(4, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
				render_device.SetSamplerState(4, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				render_device.SetSamplerState(4, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
				render_device.SetSamplerState(4, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

				return g_feature_mix_list[1];
			}
		}
		
		return g_feature_mix_list[0];
	}
	
	void SetShaderEffectVariables(const TagGroups::s_shader_effect* shader)
	{
		g_current_shader_offset = 0;

		if(!g_rasterizer_effect_depth_fade)
		{
			return;
		}

		if((g_ps_support <= _ps_support_2_0) || !g_extensions_enabled || !shader)
		{
			return;
		}

		if(!DX9::c_gbuffer_system::g_system_enabled || !DX9::c_gbuffer_system::GBuffer().m_rt_depth.IsEnabled())
		{
			return;
		}

		s_pixel_shader_variables pixel_shader_variables = { 0.0f, 0.0f };

		g_current_shader_offset = SetupShaderEffect(*shader, pixel_shader_variables);

		// Set the custom pixel shader values
		DX9::Direct3DDevice()->SetPixelShaderConstantF(8, (float*)&pixel_shader_variables, 1);
	}

	void ApplyHooks()
	{
		Memory::WriteRelativeJmp(&Hook_ShaderEffectPixelShaderOffset, GET_FUNC_VPTR(RASTERIZER_EFFECT_PS_INDEX_NO_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ShaderEffect_ParticleSystem, GET_FUNC_VPTR(RASTERIZER_EFFECT_PARTICLE_SYSTEM_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ShaderEffect_Particle, GET_FUNC_VPTR(RASTERIZER_EFFECT_PARTICLE_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ShaderEffect_Contrail, GET_FUNC_VPTR(RASTERIZER_EFFECT_CONTRAIL_HOOK), true);
	}

	void SetDepthFadeEnabled(bool value)
	{
		g_extension_usage_mask &= ~Flags::_shader_extension_usage_depth_fade;
		g_extension_usage_mask |= (value ? Flags::_shader_extension_usage_depth_fade : 0);
	}

	bool GetDepthFadeEnabled()
	{
		return TEST_FLAG(g_extension_usage_mask, Flags::_shader_extension_usage_bit_depth_fade);
	}
};
