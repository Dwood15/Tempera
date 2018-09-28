/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum rasterizer_vertex_shader : long_enum
		{
			_rasterizer_vs_convolution,
			_rasterizer_vs_debug,
			_rasterizer_vs_decal,
			_rasterizer_vs_detail_object_type0,
			_rasterizer_vs_detail_object_type1,
			_rasterizer_vs_effect,
			_rasterizer_vs_effect_multitexture,
			_rasterizer_vs_effect_multitexture_screenspace,
			_rasterizer_vs_effect_zsprite,
			_rasterizer_vs_environment_diffuse_light,
			_rasterizer_vs_environment_diffuse_light_ff,
			_rasterizer_vs_environment_fog,
			_rasterizer_vs_environment_fog_screen,
			_rasterizer_vs_environment_lightmap,
			_rasterizer_vs_environment_reflection_bumped,
			_rasterizer_vs_environment_reflection_flat,
			_rasterizer_vs_environment_reflection_lightmap_mask,
			_rasterizer_vs_environment_reflection_mirror,
			_rasterizer_vs_environment_reflection_radiosity,
			_rasterizer_vs_environment_shadow,
			_rasterizer_vs_environment_specular_light,
			_rasterizer_vs_environment_specular_spot_light,
			_rasterizer_vs_environment_specular_lightmap,
			_rasterizer_vs_environment_texture,
			_rasterizer_vs_lens_flare,
			_rasterizer_vs_model_fogged,
			_rasterizer_vs_model,
			_rasterizer_vs_model_ff,
			_rasterizer_vs_model_fast,
			_rasterizer_vs_model_scenery,
			_rasterizer_vs_model_active_camouflage,
			_rasterizer_vs_model_active_camouflage_ff,
			_rasterizer_vs_model_fog_screen,
			_rasterizer_vs_model_shadow,
			_rasterizer_vs_model_zbuffer,
			_rasterizer_vs_screen,
			_rasterizer_vs_screen2,
			_rasterizer_vs_transparent_generic,
			_rasterizer_vs_transparent_generic_lit_m,
			_rasterizer_vs_transparent_generic_m,
			_rasterizer_vs_transparent_generic_object_centered,
			_rasterizer_vs_transparent_generic_object_centered_m,
			_rasterizer_vs_transparent_generic_reflection,
			_rasterizer_vs_transparent_generic_reflection_m,
			_rasterizer_vs_transparent_generic_screenspace,
			_rasterizer_vs_transparent_generic_screenspace_m,
			_rasterizer_vs_transparent_generic_viewer_centered,
			_rasterizer_vs_transparent_generic_viewer_centered_m,
			_rasterizer_vs_transparent_glass_diffuse_light,
			_rasterizer_vs_transparent_glass_diffuse_light_m,
			_rasterizer_vs_transparent_glass_reflection_bumped,
			_rasterizer_vs_transparent_glass_reflection_bumped_m,
			_rasterizer_vs_transparent_glass_reflection_flat,
			_rasterizer_vs_transparent_glass_reflection_flat_m,
			_rasterizer_vs_transparent_glass_reflection_mirror,
			_rasterizer_vs_transparent_glass_tint,
			_rasterizer_vs_transparent_glass_tint_m,
			_rasterizer_vs_transparent_meter,
			_rasterizer_vs_transparent_meter_m,
			_rasterizer_vs_transparent_plasma_m,
			_rasterizer_vs_transparent_water_opacity,
			_rasterizer_vs_transparent_water_opacity_m,
			_rasterizer_vs_transparent_water_reflection,
			_rasterizer_vs_transparent_water_reflection_m,

			_rasterizer_vertex_shader,

			_rasterizer_vs_invalid = CAST(long_enum, NONE),
		};
	};

	namespace DX9
	{
		struct s_rasterizer_dx9_pixel_shader
		{
			char name[127+1]; // PS_%s_ps_%d_%d, e.g., "PS_EnvironmentLightmapNormal_ps_2_0"
			IDirect3DPixelShader9* pixel_shader;
		}; BOOST_STATIC_ASSERT( sizeof(s_rasterizer_dx9_pixel_shader) == 0x84 );
		struct s_rasterizer_dx9_effect_shaders
		{
			char name[127+1]; // e.g., "environment_lightmap_normal"
			int32 pixel_shader_count;
			s_rasterizer_dx9_pixel_shader* pixel_shaders;
		}; BOOST_STATIC_ASSERT( sizeof(s_rasterizer_dx9_effect_shaders) == 0x88 );

		struct s_rasterizer_dx9_effect_collection
		{
			int32 count;
			s_rasterizer_dx9_effect_shaders* effect_shaders;
		};
		s_rasterizer_dx9_effect_collection* EffectCollections();

		struct s_effect_shader_entry
		{
			s_rasterizer_dx9_effect_shaders* effect_shaders;
			UNUSED_TYPE(int32);
			Enums::rasterizer_vertex_shader vertex_shader;
			cstring name;
		};
		s_effect_shader_entry* EffectShaders(); // i.e., Pixel shaders

		struct s_vsf_entry
		{
			IDirect3DVertexShader9* shader;
			cstring name;
		};
		s_vsf_entry* VertexShaders();
	};
};