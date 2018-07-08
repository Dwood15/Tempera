/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

namespace
{
	using namespace Yelo;
	using namespace Yelo::Enums;

	static byte_swap_code_t k_string_byte_swap_codes[] =
		{ sizeof(tag_string), 0 };

	static byte_swap_code_t k_char_integer_byte_swap_codes[] =
		{ _bs_code_1byte, 0 };
	static byte_swap_code_t k_short_integer_byte_swap_codes[] =
		{ _bs_code_2byte, 0 };
	static byte_swap_code_t k_long_integer_byte_swap_codes[] =
		{ _bs_code_4byte, 0 };

	static byte_swap_code_t k_point2d_byte_swap_codes[] =
		{ _bs_code_2byte, _bs_code_2byte, 0 };

	static byte_swap_code_t k_rectangle2d_byte_swap_codes[] =
		{ _bs_code_2byte, _bs_code_2byte, _bs_code_2byte, _bs_code_2byte, 0 };

	static byte_swap_code_t k_real_point2d_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_point3d_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_rgb_color_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_argb_color_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_quaternion_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_real_plane3d_byte_swap_codes[] =
		{ _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0 };

	static byte_swap_code_t k_tag_reference_byte_swap_codes[] = 
		{ _bs_code_4byte, sizeof(tag_reference_name_reference), _bs_code_4byte, sizeof(datum_index), 0 };

	static byte_swap_code_t k_tag_block_byte_swap_codes[] =
		{  _bs_code_4byte, _bs_code_4byte, sizeof(tag_block_definition*), 0 };

	static byte_swap_code_t k_tag_data_byte_swap_codes[] =
		{  _bs_code_4byte, sizeof(long)*2, sizeof(void*)*2, 0 };

	static byte_swap_code_t k_start_array_byte_swap_codes[] =
		{ _bs_code_array_start, 0 };

	static byte_swap_code_t k_end_array_byte_swap_codes[] =
		{ _bs_code_array_end, 0 };

	static byte_swap_code_t k_pad_byte_swap_codes[] =
		{ 0 };

	static byte_swap_code_t k_explanation_byte_swap_codes[] =
		{ 0 };

	static byte_swap_code_t k_custom_byte_swap_codes[] =
		{ 0 };

	static byte_swap_code_t k_terminator_byte_swap_codes[] =
		{ _bs_code_array_end, 0 };
};

namespace Yelo { namespace TagGroups {
	const s_tag_field_definition k_tag_field_definitions[] = {
		{ sizeof(tag_string),			"string",				k_string_byte_swap_codes,
			"tag_string" },
		{ sizeof(sbyte),				"char integer",			k_char_integer_byte_swap_codes,
			"byte" },
		{ sizeof(short),				"short integer",		k_short_integer_byte_swap_codes,
			"short" },
		{ sizeof(long),				"long integer",			k_long_integer_byte_swap_codes,
			"long" },
		{ sizeof(angle),				"angle",				k_long_integer_byte_swap_codes,
			"angle" },
		{ sizeof(tag),					"tag",					k_long_integer_byte_swap_codes,
			"tag" },
		{ sizeof(short),				"enum",					k_short_integer_byte_swap_codes,
			"short" },
		{ sizeof(unsigned long),			"long flags",			k_long_integer_byte_swap_codes,
			"long_flags" },
		{ sizeof(unsigned short),			"word flags",			k_short_integer_byte_swap_codes,
			"unsigned short" },
		{ sizeof(byte_flags),			"byte flags",			k_char_integer_byte_swap_codes,
			"byte_flags" },
		{ sizeof(point2d),				"point 2d",				k_point2d_byte_swap_codes,
			"point2d" },
		{ sizeof(rectangle2d),			"rectangle 2d",			k_rectangle2d_byte_swap_codes,
			"rectangle2d" },
		{ sizeof(rgb_color),			"rgb color",			k_long_integer_byte_swap_codes,
			"rgb_color" },
		{ sizeof(argb_color),			"argb color",			k_long_integer_byte_swap_codes,
			"argb_color" },

		{ sizeof(real),					"real",					k_long_integer_byte_swap_codes,
			"real" },
		{ sizeof(real_fraction),		"real fraction",		k_long_integer_byte_swap_codes,
			"real_fraction" },
		{ sizeof(real_point2d),			"real point 2d",		k_real_point2d_byte_swap_codes,
			"real_point2d" },
		{ sizeof(real_point3d),			"real point 3d",		k_real_point3d_byte_swap_codes,
			"real_point3d" },
		{ sizeof(real_vector2d),		"real vector 2d",		k_real_point2d_byte_swap_codes,
			"real_vector2d" },
		{ sizeof(real_vector3d),		"real vector 3d",		k_real_point3d_byte_swap_codes,
			"real_vector3d" },
		{ sizeof(real_quaternion),		"real quaternion",		k_real_quaternion_byte_swap_codes,
			"real_quaternion" },
		{ sizeof(real_euler_angles2d),	"real euler angles 2d",	k_real_point2d_byte_swap_codes,
			"real_euler_angles2d" },
		{ sizeof(real_euler_angles3d),	"real euler angles 3d",	k_real_point3d_byte_swap_codes,
			"real_euler_angles3d" },
		{ sizeof(real_plane2d),			"real plane 2d",		k_real_point3d_byte_swap_codes,
			"real_plane2d" },
		{ sizeof(real_plane3d),			"real plane 3d",		k_real_plane3d_byte_swap_codes,
			"real_plane3d" },
		{ sizeof(real_rgb_color),		"real rgb color",		k_real_rgb_color_byte_swap_codes,
			"real_rgb_color" },
		{ sizeof(real_argb_color),		"real argb color",		k_real_argb_color_byte_swap_codes,
			"real_argb_color" },

		{ sizeof(real_rgb_color),		"real hsv color",		k_real_rgb_color_byte_swap_codes,
			/*"real_hsv_color"*/ },
		{ sizeof(real_argb_color),		"real ahsv color",		k_real_argb_color_byte_swap_codes,
			/*"real_ahsv_color"*/ },

		{ sizeof(short_bounds),			"short integer bounds",	k_point2d_byte_swap_codes,
			"short_bounds" },
		{ sizeof(angle_bounds),			"angle bounds",			k_real_point2d_byte_swap_codes,
			"angle_bounds" },
		{ sizeof(real_bounds),			"real bounds",			k_real_point2d_byte_swap_codes,
			"real_bounds" },
		{ sizeof(real_fraction_bounds),	"fraction bounds",		k_real_point2d_byte_swap_codes,
			"real_fraction_bounds" },

		{ sizeof(tag_reference),		"tag reference",		k_tag_reference_byte_swap_codes,
			"tag_reference" },
		{ sizeof(tag_block),			"block",				k_tag_block_byte_swap_codes,
			"tag_block" },
		{ sizeof(short),				"short block index",	k_short_integer_byte_swap_codes,
			"short" },
		{ sizeof(long),				"long block index",		k_long_integer_byte_swap_codes,
			"long" },
		{ sizeof(tag_data),				"data",					k_tag_data_byte_swap_codes,
			"tag_data" },

		{ 0,							"array start",			k_start_array_byte_swap_codes },
		{ 0,							"array end",			k_end_array_byte_swap_codes },
		{ 0,							"pad",					k_pad_byte_swap_codes },
		{ 0,							"skip",					k_pad_byte_swap_codes },
		{ 0,							"explanation",			k_explanation_byte_swap_codes },
		{ 0,							"custom",				k_custom_byte_swap_codes },
		{ 0,							"terminator X",			k_terminator_byte_swap_codes },

	}; static_assert( std::size(k_tag_field_definitions) == Enums::k_number_of_tag_field_types );
}; };
