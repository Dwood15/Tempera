#pragma once

#include <macros_generic.h>
#include <enums/generic_enums.h>
#include <enums/ai_enums.h>
#include <enums/unit_enums.h>
#include "../../math/real_math.h"
#include "../../tags/group/tagblock.h"
#include "../../tags/group/tagreference.h"

namespace Yelo::TagGroups {
	struct actor_variant_transform_keyframe_action {
		unit_animation_keyframe keyframe;
		Enums::actor_variant_transform_keyframe_rider_handling rider_handling;
		Enums::actor_variant_transform_keyframe_effect_target target;
		unsigned short padA;
		tag_reference damage_effect;
		tag_reference effect;
		tag_string effect_marker;
	};

#pragma region actor_variant_transform_in_definition
	struct actor_variant_transform_in_target {
		tag_string target_name;
		unsigned short flags;
		signed short:16;
		real_fraction selection_chance[game_difficulty_level::k_number_of_game_difficulty_levels];

		tag_reference actor_variant;
		Enums::actor_variant_transform_in_encounter_squad_handling encounter_squad_handling;
		signed short:16;
		Enums::actor_variant_transform_team_handling team_handling;
		::game_team team_override;
		Enums::actor_variant_transform_in_actor_state_handling initial_state_handling;
		Enums::actor_default_state initial_state_override;
		Enums::actor_variant_transform_in_actor_state_handling return_state_handling;
		Enums::actor_default_state return_state_override;
		signed short:16; // TAG_FIELD(Enums::actor_variant_transform_in_actor_action_handling, actor_action_handling);
		signed short:16; // TAG_FIELD(Enums::actor_action, actor_action_override);

		tag_string transform_in_anim;
		TagBlock<actor_variant_transform_keyframe_action> keyframe_actions;

		unsigned long:32;;
		Enums::actor_variant_transform_in_vitality_handling vitality_inheritance;
		Enums::actor_variant_transform_in_vitality_handling vitality_override;
		real shield_override;
		real health_override;
		tag_block padC[2];
	};
	STAT_ASSERT(actor_variant_transform_in_target, 0xAC);

	struct actor_variant_transform_in_definition {
		enum { k_group_tag = 'avti' };

		Yelo::TagBlock<actor_variant_transform_in_target> targets;
		tag_block padC[2];
	};
	STAT_ASSERT(actor_variant_transform_in_definition, 0x24);
#pragma endregion

#pragma region actor_variant_transform_out_definition
	struct actor_variant_transform_out_instigator {
		tag_reference unit;
		Enums::actor_variant_transform_out_damage_type damage_type;
		signed short:16;
		tag_block padC;
	};
	STAT_ASSERT(actor_variant_transform_out_instigator, 0x20);

	struct actor_variant_transform_out_attachment {
		tag_reference object_type;
		tag_string object_marker;
		tag_string destination_marker;
		int16 destination_marker_count;
		signed short:16;
		Enums::actor_variant_transform_team_handling team_handling;
		::game_team team_override;
		real_bounds attachment_scale;
		tag_block padC[2];
	};
	STAT_ASSERT(actor_variant_transform_out_attachment, 0x78);

	struct actor_variant_transform_out_definition {
		enum { k_group_tag = 'avto' };

		unsigned short flags;
		signed short:16;

		unsigned short criteria_flags;
		signed short:16;
		unsigned short actor_action;
		unsigned short actor_state;
		real_bounds shield_range;
		real_bounds health_range;
		tag_block padB[2];

		Yelo::TagBlock<actor_variant_transform_out_instigator> instigators;

		tag_string transform_out_anim;
		Yelo::TagBlock<actor_variant_transform_keyframe_action> keyframe_actions;

		unsigned short attachment_flags;
		signed short:16;
		Yelo::TagBlock<actor_variant_transform_out_attachment> attachments;
		tag_block padC[2];
	};
	STAT_ASSERT(actor_variant_transform_out_definition, 0x94);
#pragma endregion

#pragma region actor_variant_transform_collection_definition

	struct actor_variant_transform_collection_transform {
		unsigned short flags;
		signed short:16;
		tag_string transform_name;
		real_fraction selection_chance[k_number_of_game_difficulty_levels];

		tag_reference transform_out;
		actor_variant_transform_out_definition *transform_out_ptr;
		tag_reference transform_in;
		actor_variant_transform_in_definition *transform_in_ptr;
		tag_block padC[2];
	};
	STAT_ASSERT(actor_variant_transform_collection_transform, 0x74);

	struct actor_variant_transform_collection_entry {
		tag_reference actor_variant;
		Yelo::TagBlock<actor_variant_transform_collection_transform> transforms;
		tag_block padC[2];
	};
	STAT_ASSERT(actor_variant_transform_collection_entry, 0x34);

	struct actor_variant_transform_collection_definition {
		enum { k_group_tag = 'avtc' };

		Yelo::TagBlock<actor_variant_transform_collection_entry> actor_variant_transforms;
		tag_block padA[2];
	};
	STAT_ASSERT(actor_variant_transform_collection_definition, 0x24);
#pragma endregion
};

