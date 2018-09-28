/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <macros_generic.h>
#include <enums/bitmaps.h>
#include "../tags/group/markup.h"
#include "../memory/datum_index.h"
#include "../math/real_math.h"
#include "../math/int_math.h"
#include "../tags/group/tagblock.h"
#include "../tags/group/tagdata.h"
#include "../cseries/MacrosCpp.h"

namespace Yelo::TagGroups {

	struct s_bitmap_group_sprite {
		int16 bitmap_index;
		short : 16;
		int   : 32;
		real         left;
		real         right;
		real         top;
		real         bottom;
		real_point2d registration_point;
	};

	static_assert(sizeof(s_bitmap_group_sprite) == 0x20); // max count: 64

	struct s_bitmap_group_sequence {
		tag_string name;
		int16      first_bitmap_index;
		int16      bitmap_count;
		int :32;
		int :32;
		int :32;
		int :32;
		TagBlock<s_bitmap_group_sprite> sprites;
	};

	static_assert(sizeof(s_bitmap_group_sequence) == 0x40); // max count: 256

	struct s_bitmap_data {
		struct _flags {
			unsigned short power_of_two_dimensions_bit:1;
			unsigned short compressed_bit:1;
			unsigned short palettized_bit:1;
			unsigned short swizzled_bit:1;
			unsigned short linear_bit:1;
			unsigned short v16u16_bit:1;
			unsigned short orphan_bit:1; // this bitmap and its pixel data are allocated outside of the tag system
			unsigned short cached_bit:1; // _bitmap_cached_bit
			unsigned short in_data_file_bit:1; // data is in the bitmaps data file, not the cache file
		};
		static_assert(sizeof(_flags) == sizeof(unsigned short));

		tag     signature;
		int16   width;
		int16   height;
		int16   depth;
		short   type;
		short   format; // "determines how pixels are represented internally"
		_flags  flags;
		point2d registration_point;
		int16   mipmap_count;
		short : 16;
		int32 pixels_offset;

		unsigned long:32; //Unknown
		datum_index owner_tag_index;
		datum_index texture_cache_index;
		void        *hardware_format; // IDirect3DBaseTexture9*
		void        *base_address;
	};

	static_assert(sizeof(s_bitmap_data) == 0x30); // max count: 2048

	struct s_bitmap_group {
		enum { k_group_tag = 'bitm' };

		////////////////////////////////////////////////////////////////
		// type
		// Type controls bitmap 'geometry'. All dimensions must be a power of two except for SPRITES and INTERFACE BITMAPS:
		//
		// * 2D TEXTURES: Ordinary, 2D textures will be generated.
		// * 3D TEXTURES: Volume textures will be generated from each sequence of 2D texture 'slices'.
		// * CUBE MAPS: Cube maps will be generated from each consecutive set of six 2D textures in each sequence, all faces of a cube map must be square and the same size.
		// * SPRITES: Sprite texture pages will be generated.
		// * INTERFACE BITMAPS: Similar to 2D TEXTURES, but without mipmaps and without the power of two restriction.
		Enums::bitmap_group_type type;

		////////////////////////////////////////////////////////////////
		// format
		// Format controls how pixels will be stored internally:
		//
		// * COMPRESSED WITH COLOR-KEY TRANSPARENCY: DXT1 compression, uses 4 bits per pixel. 4x4 blocks of pixels are reduced to 2 colors and interpolated, alpha channel uses color-key transparency instead of alpha from the plate (all zero-alpha pixels also have zero-color).
		// * COMPRESSED WITH EXPLICIT ALPHA: DXT2/3 compression, uses 8 bits per pixel. Same as DXT1 without the color key transparency, alpha channel uses alpha from plate quantized down to 4 bits per pixel.
		// * COMPRESSED WITH INTERPOLATED ALPHA: DXT4/5 compression, uses 8 bits per pixel. Same as DXT2/3, except alpha is smoother. Better for smooth alpha gradients, worse for noisy alpha.
		// * 16-BIT COLOR: Uses 16 bits per pixel. Depending on the alpha channel, bitmaps are quantized to either r5g6b5 (no alpha), a1r5g5b5 (1-bit alpha), or a4r4g4b4 (>1-bit alpha).
		// * 32-BIT COLOR: Uses 32 bits per pixel. Very high quality, can have alpha at no added cost. This format takes up the most memory, however. Bitmap formats are x8r8g8b8 and a8r8g8b.
		// * MONOCHROME: Uses either 8 or 16 bits per pixel. Bitmap formats are a8 (alpha), y8 (intensity), ay8 (combined alpha-intensity) and a8y8 (separate alpha-intensity).
		//
		// Note: Height maps (a.k.a. bump maps) should use 32-bit color; this is internally converted to a palettized format which takes less memory.
		Enums::bitmap_group_format format;

		////////////////////////////////////////////////////////////////
		// usage
		// Usage controls how mipmaps are generated:
		//
		// * ALPHA BLEND: Pixels with zero alpha are ignored in mipmaps, to prevent bleeding the transparent color.
		// * DEFAULT: Downsampling works normally, as in Photoshop.
		// * HEIGHT MAP: The bitmap (normally grayscale) is a height map which gets converted to a bump map. Uses <bump height> below. Alpha is passed through unmodified.
		// * DETAIL MAP: Mipmap color fades to gray, controlled by <detail fade factor> below. Alpha fades to white.
		// * LIGHT MAP: Generates no mipmaps. Do not use!
		// * VECTOR MAP: Used mostly for special effects; pixels are treated as XYZ vectors and normalized after downsampling. Alpha is passed through unmodified.
		Enums::bitmap_group_usage usage;
		unsigned short            flags;

		////////////////////////////////////////////////////////////////
		// post-processing
		// These properties control how mipmaps are post-processed.
		real_fraction detail_fade_factor;
		real_fraction sharpen_amount;
		real_fraction bump_height;

		////////////////////////////////////////////////////////////////
		// sprite processing
		// When creating a sprite group, specify the number and size of textures that the group is allowed to occupy. During importing, you'll receive feedback about how well the alloted space was used.
		Enums::sprite_budget sprite_budget_size;
		int16                sprite_budget_count;

		////////////////////////////////////////////////////////////////
		// color plate
		// The original TIFF file used to import the bitmap group.
		int16    color_plate_width;
		int16    color_plate_height;
		tag_data compressed_color_plate_data;

		////////////////////////////////////////////////////////////////
		// processed pixel data
		// Pixel data after being processed by the tool.
		tag_data processed_pixel_data;

		////////////////////////////////////////////////////////////////
		// miscellaneous
		real  blur_filter_size;
		real  alpha_bias;
		int16 mipmap_count;

		////////////////////////////////////////////////////////////////
		// ...more sprite processing
		// Sprite usage controls the background color of sprite plates.
		Enums::sprite_usage sprite_usage;
		int16               sprite_spacing;
		short : 16;
		Yelo::TagBlock<s_bitmap_group_sequence> sequences;
		Yelo::TagBlock<s_bitmap_data>           bitmaps;

		bool ResourcesAreSharable() const {
			return
				!TEST_FLAG(flags, Flags::_bitmap_group_never_share_resources_yelo_bit) &&
				// it makes no sense to ever store lightmaps in a shared cache
				usage != Enums::_bitmap_group_usage_light_map;
		}
	};

	STAT_ASSERT(s_bitmap_group, 0x6C);
};
