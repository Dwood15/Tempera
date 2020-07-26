#pragma once
namespace Yelo::Enums
{
	enum bitmap_type : short
	{
		_bitmap_type__2d_texture,
		_bitmap_type__3d_texture,
		_bitmap_type_cube_map,
		_bitmap_type_white,

		_bitmap_type,
	};
	enum bitmap_pixel_format : short
	{
		_bitmap_pixel_format_a8,
		_bitmap_pixel_format_y8,
		_bitmap_pixel_format_ay8,
		_bitmap_pixel_format_a8y8,
		_bitmap_pixel_format_unused1,
		_bitmap_pixel_format_unused2,
		_bitmap_pixel_format_r5g6b5,
		_bitmap_pixel_format_unused3,
		_bitmap_pixel_format_a1r5g5b5,
		_bitmap_pixel_format_a4r4g4b4,
		_bitmap_pixel_format_x8r8g8b8,
		_bitmap_pixel_format_a8r8g8b8,
		_bitmap_pixel_format_unused4,
		_bitmap_pixel_format_unused5,
		_bitmap_pixel_format_dxt1,
		_bitmap_pixel_format_dxt3,
		_bitmap_pixel_format_dxt5,
		_bitmap_pixel_format_p8_bump,

		_bitmap_pixel_format,
	};
	enum bitmap_group_type : short
	{
		_bitmap_group_type__2d_textures,
		_bitmap_group_type__3d_textures,
		_bitmap_group_type_cube_maps,
		_bitmap_group_type_sprites,
		_bitmap_group_type_interface_bitmaps,

		k_number_of_bitmap_group_types,
	};
	enum bitmap_group_format : short
	{
		_bitmap_group_format_compressed_with_color_key_transparency,
		_bitmap_group_format_compressed_with_explicit_alpha,
		_bitmap_group_format_compressed_with_interpolated_alpha,
		_bitmap_group_format__16_bit_color,
		_bitmap_group_format__32_bit_color,
		_bitmap_group_format_monochrome,

		k_number_of_bitmap_group_formats,
	};
	enum bitmap_group_usage : short
	{
		_bitmap_group_usage_alpha_blend,
		_bitmap_group_usage_default,
		_bitmap_group_usage_height_map,
		_bitmap_group_usage_detail_map,
		_bitmap_group_usage_light_map,
		_bitmap_group_usage_vector_map,

		k_number_of_bitmap_group_usages,

		//_bitmap_group_usage_height_map_blue255,		// Halo2
		//_bitmap_group_usage_embm,						// Halo2. emblem?
		//_bitmap_group_usage_height_map_a8l8,			// Halo2
		//_bitmap_group_usage_height_map_g8b8,			// Halo2
		//_bitmap_group_usage_height_map_g8b8_y_alpha,	// Halo2
	};
	enum sprite_budget : short
	{
		_sprite_budget__32x32,
		_sprite_budget__64x64,
		_sprite_budget__128x128,
		_sprite_budget__256x256,
		_sprite_budget__512x512,

		_sprite_budget,

		//_sprite_budget__1024x1024,	// Halo2. However, sprite processing fields were hidden from view...
	};
	enum sprite_usage : short
	{
		_sprite_usage_blend_add_subtract_max,
		_sprite_usage_multiply_min,
		_sprite_usage_double_multiply,

		_sprite_usage,
	};
};

namespace Yelo::Flags
{
	enum bitmap_group_flags : short
	{
		_bitmap_group_enable_diffusion_dithering_bit,
		_bitmap_group_disable_height_map_compression_bit,
		_bitmap_group_uniform_sprite_sequences_bit,
		_bitmap_group_filthy_sprite_bug_fix_bit,

		k_number_of_bitmap_group_flags,

		_bitmap_group_reserved4_bit =	// Halo2. Use Sharp Bump Filter
		k_number_of_bitmap_group_flags,
		_bitmap_group_unused5_bit,
		_bitmap_group_reserved6_bit,	// Halo2. Use Clamped/Mirrored Bump Filter
		_bitmap_group_reserved7_bit,	// Halo2. Invert Detail Fade
		_bitmap_group_reserved8_bit,	// Halo2. Swap x-y Vector Components
		_bitmap_group_reserved9_bit,	// Halo2. Convert from Signed
		_bitmap_group_reserved10_bit,	// Halo2. Convert to Signed
		_bitmap_group_reserved11_bit,	// Halo2. Import mipmap Chains
		_bitmap_group_reserved12_bit,	// Halo2. Intentionally True Color

		// the bitmap group's data should never be stored in any sort of 'shared' cache file
			_bitmap_group_never_share_resources_yelo_bit,

		k_number_of_bitmap_group_flags_yelo,
	};
};
