#pragma once
namespace Yelo::TagGroups {
	struct s_shader_postprocess_bloom_definition {
		real          size;
		real          exposure;
		real_fraction mix_amount;

		real_rgb_color minimum_color;
		real_rgb_color maximum_color;
	};

	struct s_shader_postprocess_globals_bloom {
		struct _flags {
			unsigned short is_enabled_bit:1;
			unsigned short apply_after_hud_bit:1;
		} flags;
		short : 16;

		s_shader_postprocess_bloom_definition bloom;
	};
	STAT_ASSERT(s_shader_postprocess_globals_bloom, 0x28);

	struct s_shader_postprocess_globals {
		enum { k_group_tag = 'sppg' };

		short : 16;
		short : 16;
		union {
			tag_block<s_shader_postprocess_globals_bloom> uPad[20]; // Up to to 20 blocks for subsystem globals

			struct {
				Yelo::TagBlock<s_shader_postprocess_globals_bloom> bloom_globals;
			};
		};
	};
	STAT_ASSERT(s_shader_postprocess_globals, 0xF4);
};
