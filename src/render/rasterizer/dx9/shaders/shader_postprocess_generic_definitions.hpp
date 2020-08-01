/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "../../../../tags/group/markup.h"
#include "../../../../bitmaps/bitmap_group.h"

namespace Yelo
{
	namespace Enums
	{
		enum parameter_runtime_value_bool : short
		{
			_parameter_runtime_value_bool_none,
			//_parameter_runtime_value_bool_target_is_enemy, //not yet implemented
			//_parameter_runtime_value_bool_weapon_zoomed, //not yet implemented
			_parameter_runtime_value_bool
		};
		enum parameter_runtime_value_int : short
		{
			_parameter_runtime_value_int_none,
			_parameter_runtime_value_int
		};
		enum parameter_runtime_value_float : short
		{
			_parameter_runtime_value_float_none,
			//_parameter_runtime_value_float_weapon_zoom_amount, //not yet implemented
			_parameter_runtime_value_float
		};
		enum parameter_runtime_value_float2 : short
		{
			_parameter_runtime_value_float2_none,
			_parameter_runtime_value_float2
		};
		enum parameter_runtime_value_float3 : short
		{
			_parameter_runtime_value_float3_none,
			_parameter_runtime_value_float3
		};
		enum parameter_runtime_value_float4 : short
		{
			_parameter_runtime_value_float4_none,
			_parameter_runtime_value_float4
		};
		enum parameter_runtime_value_color : short
		{
			_parameter_runtime_value_color_none,
			_parameter_runtime_value_color_player_team_color,
			_parameter_runtime_value_color
		};
	};

	namespace TagGroups
	{
		union s_shader_postprocess_value_union
		{
			static const size_t k_sizeof = 36;
			byte pad_typek_sizeof[k_sizeof];

			struct s_base {
				t_shader_variable_base handle;
			}base;

			struct s_bitmap {
				t_shader_variable_texture handle;
				ushort bitmap_index;
				short : 16;
			}bitmap;

			struct s_bool {
				t_shader_variable_bool handle;

				bool enabled; // byte_flags
				bool inverse; // inverse value of 'enabled
				short : 16;
			}boolean;

			struct s_integer32 {
				t_shader_variable_int handle;

				int32 lower_bound;
				int32 upper_bound;
			}integer32;

			struct s_real32 {
				t_shader_variable_real handle;

				real lower_bound;
				PAD(byte, 12);
				real upper_bound;
			}real32;

			struct s_real_vector2d {
				t_shader_variable_real2d handle;

				real_vector2d lower_bound;
				PAD(byte, 8);
				real_vector2d upper_bound;
			}vector2d;

			struct s_real_vector3d {
				t_shader_variable_real3d handle;

				real_vector3d lower_bound;
				PAD(byte, 4);
				real_vector3d upper_bound;
			}vector3d;

			struct s_real_vector4d {
				t_shader_variable_real4d handle;

				real_quaternion lower_bound;
				real_quaternion upper_bound;
			}vector4d;

			struct s_real_color4d {
				t_shader_variable_color handle;

				real_argb_color lower_bound;
				real_argb_color upper_bound;
			}color4d;

		}; STAT_ASSERT(s_shader_postprocess_value_union, s_shader_postprocess_value_union::k_sizeof );

		struct s_shader_postprocess_value_runtime_override
		{
			short value;
			struct _flags {
				unsigned char invert_bit:1;
			}flags;
		};

		struct s_shader_postprocess_value_animation_function
		{
			short function;
			struct _flags {
				byte inverted_bit:1;
				byte multichannel_noise_bit:1;
				byte ignore_alpha_bit:1;
			}flags;
			STAT_ASSERT(_flags, sizeof(byte_flags) );
			unsigned char:8;

			real animation_duration;
			real animation_rate;
		};
		STAT_ASSERT(s_shader_postprocess_value_animation_function, 0xC);

		struct s_shader_postprocess_value_base
		{
			tag_string value_name;

			// set by tag post-processing code
			shader_variable_type value_type;

			s_shader_postprocess_value_union value;
			s_shader_postprocess_value_runtime_override runtime_value;
			s_shader_postprocess_value_animation_function animation_function;
		};

		struct s_shader_postprocess_bitmap : s_shader_postprocess_value_base
		{
			byte pad_type12[12];
			tag_reference bitmap;
		};
		STAT_ASSERT(s_shader_postprocess_bitmap, 0x1C + sizeof(s_shader_postprocess_value_base));

		struct s_shader_postprocess_parameter : s_shader_postprocess_value_base
		{
			struct
			{
				tag_reference bitmap;
				
				union {
					struct {
						TagGroups::s_bitmap_data* bitmap;
						PAD32;
					}_internal;	// We use a '_' prefix so intelli-sense doesn't get confused

					struct {
						char* source;
						IDirect3DTexture9* texture_2d;
					}external;
				}runtime;

				struct {
					unsigned char is_loaded_bit:1;
					unsigned char is_external_bit:1;
				}flags;
				short : 16;
			}bitmap_value;

			void				GetBoundingValues(void*& lower_ref, void*& upper_ref);
			void				SetVariableInterp(LPD3DXEFFECT effect, const void* lower_data, const void* upper_data, const real* interp_values);

			// [fixup_argb_color_hack] - internal use only, use default when calling externally
			void				SetVariable(LPD3DXEFFECT effect, void* data, const bool fixup_argb_color_hack = true);

			//bitmap only functions
			HRESULT				LoadBitmap(IDirect3DDevice9* pDevice);
			void				ReleaseBitmap();

			IDirect3DTexture9*	GetTexture();

			void SetParameter(const s_shader_postprocess_value_base* value);
			void SetParameter(const s_shader_postprocess_bitmap* value);
		};

		struct s_shader_postprocess_implementation
		{
			Yelo::TagBlock<s_shader_postprocess_bitmap> bitmaps;
			Yelo::TagBlock<s_shader_postprocess_value_base> bools;
			Yelo::TagBlock<s_shader_postprocess_value_base> integers;
			Yelo::TagBlock<s_shader_postprocess_value_base> floats;
			Yelo::TagBlock<s_shader_postprocess_value_base> float2s;
			Yelo::TagBlock<s_shader_postprocess_value_base> float3s;
			Yelo::TagBlock<s_shader_postprocess_value_base> float4s;
			Yelo::TagBlock<s_shader_postprocess_value_base> colors;
		}; STAT_ASSERT(s_shader_postprocess_implementation, 0x60);

		struct s_shader_postprocess_generic : s_shader_postprocess_definition
		{
			enum { k_group_tag = 'shpg' };
			short : 16;
			short : 16;

			tag_reference base_shader;
			Yelo::TagBlock<s_shader_postprocess_parameter> parameters;
			s_shader_postprocess_implementation implementation;
			byte pad_type36[36];
		};
		STAT_ASSERT(s_shader_postprocess_generic,  0x44 + sizeof(s_shader_postprocess_definition) + sizeof(s_shader_postprocess_implementation));
	};
};
