#pragma once

#include <macros_generic.h>
#include <enums/generic_enums.h>
#include "../../tags/group/markup.h"
#include "../../tags/group/tagreference.h"
#include "../../math/real_math.h"
#include "../../models/bsp_definitions.h"

namespace Yelo::TagGroups {
	struct object_attachment_definition {
		tag_reference type;
		tag_string    marker;
		struct {
			short primary_scale;
			short secondary_scale;
			short change_color;
					short : 16;
		}             function_references; // Enums::object_function_reference
		int padA[4];
	};

	struct object_definition_widget {
		tag_reference reference;
		int padA [4];
	};

	struct s_object_function_definition {
		struct _object_function_flags {
			unsigned long invert_bit:1;
			unsigned long additive_bit:1;
			unsigned long always_active_bit:1;
		} flags;
		STAT_ASSERT(_object_function_flags, sizeof(unsigned int));

		real period;
		short period_scale;

		short function;
		short function_scale;

		short wobble_function;
		real wobble_period;
		real wobble_magnitude;

		real square_wave_threshold;
		int16 step_count;
		short map_to;
		int16 sawtooth_count;

		short add;
		short result_scale;

		short bounds_mode;
		real_fraction_bounds bounds;
		PAD32;
		short : 16;
		int16 turn_off_with;
		real scale_by;
		byte padB[252]; // useless padding

		// postprocessed fields; labeled as PAD(16) in group definitions
		real_fraction bounds_normal;      // 1f / (max - min)
		real_fraction step_count_normal;   // 1f / (--step_count)
		real_fraction sawtooth_count_normal;// 1f / (--sawtooth_count)
		real_fraction period_normal;      // 1f / period

		tag_string usage;
	};
	STAT_ASSERT(s_object_function_definition, 0x168);

	struct _object_definition {
		short runtime_object_type;
		unsigned short flags;

		real         bounding_radius;
		real_point3d bounding_offset, origin_offset;
		real         acceleration_scale;
		struct {
			TAG_FLAG(change_colors_scaled_by_function); // toggled when there is 1+ change colors that have their scale-by set to something other than NONE
		} runtime_flags;

		struct {
			tag_reference render_model;
			tag_reference animations;
			int padA [10];
			tag_reference collision;
			tag_reference physics;
			tag_reference modifier_shader;
			tag_reference creation_effect;
		} references;

		int int32Pad[21];

		real render_bounding_radius;

		short function_exports[k_number_of_incoming_object_functions];
		int padB[11];
		int16 hud_text_message_index;
		int16 forced_shader_permutation;

		TagBlock<object_attachment_definition> attachments;

		TagBlock<object_definition_widget> widgets;

		tag_block padC; // object_function_definition
		tag_block padD; // object_change_color_definition
		TagBlock<predicted_resource> predicted_resources; // predicted resources
	};
	STAT_ASSERT(_object_definition, 0x17C);


	struct s_object_definition {
		enum { k_group_tag = 'obje' };

		_object_definition object;
	};
};
