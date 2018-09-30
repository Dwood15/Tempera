#pragma once

#include <macros_generic.h>
#include "shader_postprocess_definitions.hpp"

namespace Yelo {
	namespace TagGroups {
		struct s_effect_postprocess_effect_activation_control {
			short state;
			struct _flags {
				unsigned short invert_bit:1;
			}     flags;
		};
		STAT_ASSERT(s_effect_postprocess_effect_activation_control, 0x4);

#if 0
		struct s_effect_postprocess_effect_fade_control
		{
			TAG_FIELD(short, value);
			struct _flags {
				TAG_FLAG16(invert);
			}flags;

			TAG_FIELD(real_bounds, value_map);
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_effect_postprocess_effect_fade_control) == 0x10);
#endif

		struct s_effect_postprocess_generic_effect_instance {
			tag_string                           name;
			int16                                effect_index;
			short render_stage;
			s_effect_postprocess_quad_definition quad_definition;
			short                                activation_operation;
			short : 16;
			PAD32;
			Yelo::TagBlock<s_effect_postprocess_effect_activation_control> activation_controls;
#if 0
			TAG_TBLOCK_(fade_controls, s_effect_postprocess_effect_fade_control);
#else
			tag_block pad_block0;
#endif
			byte pad_type16[16];
		};
		STAT_ASSERT(s_effect_postprocess_generic_effect_instance, 0x74);

		struct s_effect_postprocess_generic_exposed_parameter {
			tag_string           exposed_name;
			tag_string           parameter_name;
			int32                shader_index;
			shader_variable_type value_type;

			struct {
				void *parameter_instance; //void* to avoid a problematic include
			} runtime;

			PAD32;
		};

		STAT_ASSERT(s_effect_postprocess_generic_exposed_parameter, 0x50);

		struct s_effect_postprocess_generic : s_effect_postprocess_definition {
			enum { k_group_tag = 'efpg' };

			Yelo::TagBlock<tag_reference>                                  shaders;
			Yelo::TagBlock<int32>                                          shader_indices;
			Yelo::TagBlock<s_effect_postprocess_generic_exposed_parameter> exposed_parameters;

		};
		STAT_ASSERT(s_effect_postprocess_generic, 0x24 + sizeof(s_effect_postprocess_definition));
	};
};
