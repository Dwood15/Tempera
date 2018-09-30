#pragma once
namespace Yelo::Enums {
	enum ps_support {
		_ps_support_legacy,
		_ps_support_2_0,
		_ps_support_2_a,
		_ps_support_2_b,
		_ps_support_3_0,

		_ps_support
	};

	enum shader_type : short {
		_shader_type_screen,// sscr
		_shader_type_effect,
		_shader_type_decal,   // sdec

		_shader_type_environment,
		_shader_type_model,
		_shader_type_transparent_generic,
		_shader_type_transparent_chicago,
		_shader_type_transparent_chicago_extended,
		_shader_type_transparent_water,
		_shader_type_transparent_glass,
		_shader_type_transparent_meter,
		_shader_type_transparent_plasma,

		k_number_of_shader_types,
	};
	enum environment_shader_type : short {
		_environment_shader_type_normal,
		_environment_shader_type_blended,
		_environment_shader_type_blended_base_specular,

		_environment_shader_type,
	};
	enum detail_map_function : short {
		_detail_map_function_double_biased_multiply,
		_detail_map_function_multiply,
		_detail_map_function_double_biased_add,

		_detail_map_function,
	};
	enum reflection_type : short {
		_reflection_type_bumped_cube_map,
		_reflection_type_flat_cube_map,
		_reflection_type_bumped_radiosity,

		_reflection_type,
	};
	enum detail_mask : short {
		_detail_mask_none,
		_detail_mask_reflection_mask_inverse,
		_detail_mask_reflection_mask,
		_detail_mask_self_illumination_mask_inverse,
		_detail_mask_self_illumination_mask,
		_detail_mask_change_color_mask_inverse,
		_detail_mask_change_color_mask,
		_detail_mask_multipurpose_map_alpha_inverse,
		_detail_mask_multipurpose_map_alpha,

		_detail_mask,
	};
	enum first_map_type : short {
		_first_map_type__2d_map,
		_first_map_type_first_map_is_reflection_cube_map,
		_first_map_type_first_map_is_object_centered_cube_map,
		_first_map_type_first_map_is_viewer_centered_cube_map,

		_first_map_type,
	};
	enum shader_framebuffer_blend_function : short {
		_shader_framebuffer_blend_function_alpha_blend,
		_shader_framebuffer_blend_function_multiply,
		_shader_framebuffer_blend_function_double_multiply,
		_shader_framebuffer_blend_function_add,
		_shader_framebuffer_blend_function_subtract,
		_shader_framebuffer_blend_function_component_min,
		_shader_framebuffer_blend_function_component_max,
		_shader_framebuffer_blend_function_alpha_multiply_add,

		_shader_framebuffer_blend_function,
	};
	enum shader_framebuffer_fade_mode : short {
		_shader_framebuffer_fade_mode_none,
		_shader_framebuffer_fade_mode_fade_when_perpendicular,
		_shader_framebuffer_fade_mode_fade_when_parallel,

		_shader_framebuffer_fade_mode,
	};
	enum color_function : short {
		_color_function_current,
		_color_function_next_map,
		_color_function_multiply,
		_color_function_double_multiply,
		_color_function_add,
		_color_function_add_signed_current,
		_color_function_add_signed_next_map,
		_color_function_subtract_current,
		_color_function_subtract_next_map,
		_color_function_blend_current_alpha,
		_color_function_blend_current_alpha_inverse,
		_color_function_blend_next_map_alpha,
		_color_function_blend_next_map_alpha_inverse,

		_color_function,
	};
	enum color_input : short {
		_color_input_zero,
		_color_input_one,
		_color_input_one_half,
		_color_input_negative_one,
		_color_input_negative_one_half,
		_color_input_map_color_0,
		_color_input_map_color_1,
		_color_input_map_color_2,
		_color_input_map_color_3,
		_color_input_vertex_color_0__diffuse_light,
		_color_input_vertex_color_1__fade_perpendicular,
		_color_input_scratch_color_0,
		_color_input_scratch_color_1,
		_color_input_constant_color_0,
		_color_input_constant_color_1,
		_color_input_map_alpha_0,
		_color_input_map_alpha_1,
		_color_input_map_alpha_2,
		_color_input_map_alpha_3,
		_color_input_vertex_alpha_0__fade_none,
		_color_input_vertex_alpha_1__fade_perpendicular,
		_color_input_scratch_alpha_0,
		_color_input_scratch_alpha_1,
		_color_input_constant_alpha_0,
		_color_input_constant_alpha_1,

		_color_input,
	};
	enum color_input_mapping : short {
		_color_input_mapping_clamp_x,
		_color_input_mapping__1__clamp_x,
		_color_input_mapping__2,
		_color_input_mapping__1__2,
		_color_input_mapping_clamp_x__1_2,
		_color_input_mapping__1_2__clamp_x,
		_color_input_mapping_x,
		_color_input_mapping_x_1,

		_color_input_mapping,
	};
	enum color_output : short {
		_color_output_discard,
		_color_output_scratch_color_0__final_color,
		_color_output_scratch_color_1,
		_color_output_vertex_color_0,
		_color_output_vertex_color_1,
		_color_output_map_color_0,
		_color_output_map_color_1,
		_color_output_map_color_2,
		_color_output_map_color_3,

		_color_output,
	};
	enum color_output_function : short {
		_color_output_function_multiply,
		_color_output_function_dot_product,

		_color_output_function,
	};
	enum output_mapping : short {
		_output_mapping_identity,
		_output_mapping_scale_by_1_2,
		_output_mapping_scale_by_2,
		_output_mapping_scale_by_4,
		_output_mapping_bias_by_1_2,
		_output_mapping_expand_normal,

		_output_mapping,
	};
	enum alpha_input : short {
		_alpha_input_zero,
		_alpha_input_one,
		_alpha_input_one_half,
		_alpha_input_negative_one,
		_alpha_input_negative_one_half,
		_alpha_input_map_alpha_0,
		_alpha_input_map_alpha_1,
		_alpha_input_map_alpha_2,
		_alpha_input_map_alpha_3,
		_alpha_input_vertex_alpha_0__fade_none,
		_alpha_input_vertex_alpha_1__fade_perpendicular,
		_alpha_input_scratch_alpha_0,
		_alpha_input_scratch_alpha_1,
		_alpha_input_constant_alpha_0,
		_alpha_input_constant_alpha_1,
		_alpha_input_map_blue_0,
		_alpha_input_map_blue_1,
		_alpha_input_map_blue_2,
		_alpha_input_map_blue_3,
		_alpha_input_vertex_blue_0__blue_light,
		_alpha_input_vertex_blue_1__fade_parallel,
		_alpha_input_scratch_blue_0,
		_alpha_input_scratch_blue_1,
		_alpha_input_constant_blue_0,
		_alpha_input_constant_blue_1,

		_alpha_input,
	};
	enum alpha_output : short {
		_alpha_output_discard,
		_alpha_output_scratch_alpha_0__final_alpha,
		_alpha_output_scratch_alpha_1,
		_alpha_output_vertex_alpha_0__fog,
		_alpha_output_vertex_alpha_1,
		_alpha_output_map_alpha_0,
		_alpha_output_map_alpha_1,
		_alpha_output_map_alpha_2,
		_alpha_output_map_alpha_3,

		_alpha_output,
	};

	enum shader_effect_secondary_map_anchor : short {
		_shader_effect_secondary_map_anchor_with_primary,
		_shader_effect_secondary_map_anchor_with_screen_space,
		_shader_effect_secondary_map_anchor_zsprite,

		_shader_effect_secondary_map_anchor,
	};
};

namespace Yelo::Flags
{
	enum shader_extension_usage : unsigned short
	{
		_shader_extension_usage_none						= 0,
		_shader_extension_usage_normal_map					= 1 << 0,
		_shader_extension_usage_detail_normal				= 1 << 1,
		_shader_extension_usage_specular_map				= 1 << 2,
		_shader_extension_usage_specular_lighting			= 1 << 3,
		_shader_extension_usage_directional_lightmaps_diff	= 1 << 4,
		_shader_extension_usage_directional_lightmaps_spec	= 1 << 5,
		_shader_extension_usage_depth_fade					= 1 << 6,

		_shader_extension_usage,
	};

	enum shader_extension_usage_bit : unsigned short
	{
		_shader_extension_usage_bit_normal_map,
		_shader_extension_usage_bit_detail_normal,
		_shader_extension_usage_bit_specular_map,
		_shader_extension_usage_bit_specular_lighting,
		_shader_extension_usage_bit_directional_lightmaps_diff,
		_shader_extension_usage_bit_directional_lightmaps_spec,
		_shader_extension_usage_bit_depth_fade,

		_shader_extension_usage_bit,
	};
};
