/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <d3dx9.h>
#include <enums/postprocessing.h>
#include "../../../../tags/group/tagdata.h"
#include "../../../../math/int_math.h"
#include "../../../../models/bsp_definitions.h"

namespace Yelo::TagGroups {
	struct shader_variable_type {
		short type;      // Enums::shader_variable_base_type
		int16 count;   // # of [shader_variable_base_type]s
	};

	template <Enums::shader_variable_base_type TType, size_t TCount = 1>
	struct s_shader_postprocess_shader_variable {
		struct {
			D3DXHANDLE dx_handle;
		} runtime;

#if !PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI

		bool IsUsed() { return runtime.dx_handle != NULL; }

		void ClearHandles() { runtime.dx_handle = NULL; }

		void Initialize(LPD3DXEFFECT effect, const char *id, bool semantic = false);

		template <typename TValueType>
		void GetValues(TValueType *values_out, const TValueType *values1, const TValueType *values2, const size_t count,
							const real *interp_values);

		void GetValues(BOOL *values_out, const bool *values1, const bool *values2, const size_t count, const real *interp_values);

		void GetValues(D3DXCOLOR *values_out, const FLOAT *values1, const FLOAT *values2, const size_t count, const real *interp_values);

		void SetVariableInterp(LPD3DXEFFECT effect, const void *data1, const void *data2, const real *interp_values);

		// [fixup_argb_color_hack] - internal use only, use default when calling externally
		void SetVariable(LPD3DXEFFECT effect, void *data, const bool fixup_argb_color_hack = true);

#endif
	};

	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type>            t_shader_variable_base;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_boolean>    t_shader_variable_bool;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_integer>    t_shader_variable_int;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float>      t_shader_variable_real;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float, 2>   t_shader_variable_real2d;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float, 3>   t_shader_variable_real3d;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float, 4>   t_shader_variable_real4d;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_argb_color> t_shader_variable_color;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_matrix>     t_shader_variable_matrix;
	typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_texture>    t_shader_variable_texture;

	//////////////////////////////////////////////////////////////////////////
	// shader_postprocess base
	struct s_pass_definition {
		tag_string name;

		struct _flags {
			unsigned short clear_target_bit:1;
			unsigned short copy_scene_to_target_bit:1;
			unsigned short clear_buffer_texture_bit:1;
		}          flags;

		STAT_ASSERT(_flags, sizeof(unsigned short));
		short render_chain;

		int32: 32;
		int32: 32;
		int32: 32;
	};
	STAT_ASSERT(s_pass_definition, 0x30);

	struct s_technique_definition {
		tag_string name;

		struct _flags {
			unsigned short sm_1_0_bit:1;
			unsigned short sm_2_0_bit:1;
			unsigned short sm_3_0_bit:1;
		}          shader_model;
		STAT_ASSERT(_flags, sizeof(unsigned short));

		short : 16;

		int32 :32;
		int32 :32;
		int32 :32;
		int32 :32;

		Yelo::TagBlock<s_pass_definition> passes;
	};
	STAT_ASSERT(s_technique_definition, 0x40);

	struct s_shader_postprocess_definition {
		enum { k_group_tag = 'shpp' };
		short : 16;
		struct _flags {
			unsigned short shader_is_binary_bit:1;
		}     flags;

		tag_data shader_code_text;

		tag_data shader_code_binary;

		t_shader_variable_matrix  ortho_wvp_matrix;   // Orthographic projection matrix handle
		t_shader_variable_real2d  scene_size;         // Handle to the scene size float2 value
		t_shader_variable_real2d  screen_fov;         // Handle to the screen fov float2 value
		t_shader_variable_real2d  hud_scale;         // Handle to the hud scale float2 value
		t_shader_variable_real2d  pixel_size;         // Handle to the pixel size float2 value
		t_shader_variable_real    near_clip_dist;      // Handle to the near clip float value
		t_shader_variable_real    far_clip_dist;      // Handle to the far clip float value
		t_shader_variable_real    frame_time;         // Handle to the frame time float value
		t_shader_variable_texture tex_source;         // Handle to the post-process source texture
		t_shader_variable_texture tex_scene;         // Handle to the saved scene texture
		t_shader_variable_texture tex_buffer;         // Handle to the secondary buffer

		int32 : 32;
		int32 : 32;
		int32 : 32;
		int32 : 32;
		int32 : 32;

		Yelo::TagBlock<s_technique_definition> techniques;
		Yelo::TagBlock<predicted_resource>     predicted_resources;

		struct _runtime {
			struct _postprocess_flags {
				unsigned short valid_shader_bit:1;
				unsigned short uses_gbuffer_bit:1;
			} flags;
			short : 16;

			STAT_ASSERT(_postprocess_flags, sizeof(unsigned short));

			LPD3DXEFFECT           dx_effect;
			D3DXHANDLE             postprocess_handle;
			s_technique_definition *active_technique;
			int32 : 32;
			int32 : 32;
			int32 : 32;
			int32 : 32;
		} runtime;
	};
	STAT_ASSERT(s_shader_postprocess_definition, 0xA4);


	//////////////////////////////////////////////////////////////////////////
	// effect_postprocess base
	struct s_effect_postprocess_custom_vertex_data {
		real_bounds x_data;
		real_bounds y_data;
		real_bounds z_data;
		real_bounds w_data;
	};
	STAT_ASSERT(s_effect_postprocess_custom_vertex_data, 0x20);

	struct s_effect_postprocess_quad_definition {
		point2d                                                 tessellation;
		real_bounds                                             x_bounds;
		real_bounds                                             y_bounds;
		Yelo::TagBlock<s_effect_postprocess_custom_vertex_data> custom_vertex_data; // not implemented
	};

	struct s_effect_postprocess_definition {
		enum { k_group_tag = 'shpe' };
		byte padA[12];

		s_effect_postprocess_quad_definition quad_definition;

		struct _runtime {
			struct _postprocess_flags {
				unsigned short valid_effect_bit:1;
				unsigned short is_active_bit:1;
				unsigned short uses_gbuffer_bit:1;
			} flags;
			short : 16;

			byte padB[12];
		}                                    runtime;

	};
	STAT_ASSERT(s_effect_postprocess_definition, 0x3C);
};

#include "shader_postprocess_runtime.inl"
#include "shader_postprocess_globals_definitions.hpp"
#include "shader_postprocess_generic_definitions.hpp"
#include "effect_postprocess_generic_definitions.hpp"
#include "effect_postprocess_collection_definitions.hpp"
