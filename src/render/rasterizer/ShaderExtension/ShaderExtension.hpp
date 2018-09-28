/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

/*
	User Add-on
		Component: ShaderExtension
		Original Author: FireScythe

	This component adds a normal maps and a specular map to Halo's
	rendering engine, allowing for per pixel lighting.
	shader_model tags can now have a base normal map, two detail
	normal maps and a specular colour map.
	shader_environment tags that have a normal map assigned will
	also have per pixel lighting applied when used on a model.
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/shaders/shader_definitions.hpp>

namespace Yelo
{
	namespace Rasterizer
	{
		namespace ShaderExtension
		{
			enum {
				k_vector_4_count = 3,
				k_shader_constant_offset = 5
			};

			bool		ExtensionsEnabled();

			void		Initialize();
			void		Dispose();

			void		Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
			void		OnLostDevice();
			void		OnResetDevice(D3DPRESENT_PARAMETERS* params);
			void		Render();
			void		Release();

			void		ApplyHooks();
			
			namespace Model
			{
				extern bool g_rasterizer_model_normal_mapping;
				extern bool g_rasterizer_model_detail_normal_mapping;
				extern bool g_rasterizer_model_specular_lights;
				extern bool g_rasterizer_model_specular_map;

				void SetModelNormSpec(void* shader_pointer);

				bool SetViewProj(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount);
				bool SetSpecColor(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount);
				bool SetVertexLight(IDirect3DDevice9* device, CONST float* pConstantData, UINT Vector4fCount);
				
				void SetNormalMapsEnabled(bool value);
				bool GetNormalMapsEnabled();
				void SetDetailNormalMapsEnabled(bool value);
				bool GetDetailNormalMapsEnabled();
				void SetSpecularMapsEnabled(bool value);
				bool GetSpecularMapsEnabled();
				void SetSpecularLightingEnabled(bool value);
				bool GetSpecularLightingEnabled();
			};

			namespace Environment
			{
				extern bool g_rasterizer_environment_dlm_diffuse;
				extern bool g_rasterizer_environment_dlm_specular;

				void SetLightmapSampler(const TagGroups::s_shader_definition* shader);
				void SetupLightmapShader(const TagGroups::s_shader_definition* shader, const _enum extension_type);

				void SetDiffuseDLMsEnabled(bool value);
				bool GetDiffuseDLMsEnabled();
				void SetSpecularDLMsEnabled(bool value);
				bool GetSpecularDLMsEnabled();
			}

			namespace Effect
			{
				extern bool g_rasterizer_effect_depth_fade;

				void SetDepthFadeEnabled(bool value);
				bool GetDepthFadeEnabled();
			}
		};
	};
};
#endif