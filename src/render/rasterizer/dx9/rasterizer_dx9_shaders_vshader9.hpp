#pragma once
#include <enums/render.h>

namespace Yelo::DX9 {
	struct s_rasterizer_dx9_pixel_shader {
		char                  name[127 + 1]; // PS_%s_ps_%d_%d, e.g., "PS_EnvironmentLightmapNormal_ps_2_0"
		IDirect3DPixelShader9 *pixel_shader;
	};
	STAT_ASSERT(s_rasterizer_dx9_pixel_shader, 0x84);

	struct s_rasterizer_dx9_effect_shaders {
		char                          name[127 + 1]; // e.g., "environment_lightmap_normal"
		int32                         pixel_shader_count;
		s_rasterizer_dx9_pixel_shader *pixel_shaders;
	};

	STAT_ASSERT(s_rasterizer_dx9_effect_shaders, 0x88);

	struct s_rasterizer_dx9_effect_collection {
		int32                           count;
		s_rasterizer_dx9_effect_shaders *effect_shaders;
	};

	s_rasterizer_dx9_effect_collection *EffectCollections();

	struct s_effect_shader_entry {
		s_rasterizer_dx9_effect_shaders *effect_shaders;
		UNUSED_TYPE(int32);
		Enums::rasterizer_vertex_shader vertex_shader;
		cstring                         name;
	};

	s_effect_shader_entry *EffectShaders(); // i.e., Pixel shaders

	struct s_vsf_entry {
		IDirect3DVertexShader9 *shader;
		cstring                name;
	};

	s_vsf_entry *VertexShaders();
};
