#pragma once

#include <macros_generic.h>
#include <enums/animations.h>
#include "../../math/real_math.h"
#include "../../tags/group/markup.h"
#include "../../tags/group/tagdata.h"
#include "../../tags/group/tagblock.h"

namespace Yelo::TagGroups {
	struct animation_graph_weapon_type {
		tag_string label;
		int padA[4];
		TagBlock<int16> animations; // block index to model_animation
	};
	STAT_ASSERT(animation_graph_weapon_type, 0x3C); // max count: 16
	struct animation_graph_weapon
	{
		tag_string name;
		tag_string grip_marker;
		tag_string hand_marker;

		////////////////////////////////////////////////////////////////
		// aiming screen bounds
		angle right_yaw_per_frame;
		angle left_yaw_per_frame;
		int16 right_frame_count;
		int16 left_frame_count;
		angle down_pitch_per_frame;
		angle up_pitch_per_frame;
		int16 down_pitch_frame_count;
		int16 up_pitch_frame_count;
		int pad[8];
		TagBlock<int16> animations; // block index to model_animation
		// TagBlock<animation_graph_unit_seat_ik_point> ik_points;
		TagBlock<void*> ik_points;
		TagBlock<animation_graph_weapon_type> weapon_types;
	};
	STAT_ASSERT(animation_graph_weapon, 0xBC); // max count: 16
	struct animation_graph_unit_seat
	{
		tag_string label;

		////////////////////////////////////////////////////////////////
		// looking screen bounds
		angle right_yaw_per_frame;
		angle left_yaw_per_frame;
		int16 right_frame_count;
		int16 left_frame_count;
		angle down_pitch_per_frame;
		angle up_pitch_per_frame;
		int16 down_pitch_frame_count;
		int16 up_pitch_frame_count;
		int32 padA[2];
		TagBlock<int16> animations; // block index to model_animation
		// TagBlock<animation_graph_unit_seat_ik_point> ik_points;
		TagBlock< void*> ik_points;
		TagBlock<animation_graph_weapon> weapons;
	};
	STAT_ASSERT(animation_graph_unit_seat, 0x64); // max count: 32

	struct model_animation
	{
		tag_string name;
		short type;
		int16 frame_count;
		int16 frame_size;
		short frame_info_type;
		int32 node_list_checksum;
		int16 node_count;
		int16 loop_frame_index;
		real_fraction weight;
		int16 key_frame_index;
		int16 second_key_frame_index;
		int16 next_animation;
		struct _flags {
			ushort compressed_data_bit:1;
			ushort world_relative_bit:1;
			ushort _25Hz_PAL_bit:1;
		}flags;

		STAT_ASSERT(_flags, sizeof(ushort));

		int16 sound;
		int16 sound_frame_index;
		sbyte left_foot_frame_index;
		sbyte right_foot_frame_index;
		//////////////////////////////////////////////////////////////////////////
		// postprocessed fields (thus, not exposed to the editor)
		int16 first_animation; // if this is a permutated animation, this represents the first animation (first animation will also have this set)
		real_fraction random_fraction;
		//////////////////////////////////////////////////////////////////////////
		tag_data frame_info;
		int32 node_trans_flag_data1;
		int32 node_trans_flag_data2;
		byte padbyte8[8];
		int32 node_rotation_flag_data1;
		int32 node_rotation_flag_data2;
		char padchar8[8];
		int32 node_scale_flag_data1;
		int32 node_scale_flag_data2;
		byte padbyte4[4];
		int32 offset_to_compressed_data;
		tag_data default_data;
		tag_data frame_data;
	};
	STAT_ASSERT(model_animation, 0xB4);

	struct model_animation_graph
	{
		enum { k_group_tag = 'antr' };

		// TagBlock<s_object_overlay> objects;
		TagBlock<void *> objects;
		TagBlock<animation_graph_unit_seat> units;
		// TagBlock<animation_graph_weapon_animation> weapons;
		// TagBlock<animation_graph_vehicle_animation> vehicles;

		TagBlock<void *> weapons;
		TagBlock<void *> vehicles;
		TagBlock<Enums::device_animation> devices;
		// TagBlock<unit_damage_animation> unit_damage;
		// TagBlock<animation_graph_first_person_weapon_animation> first_person_weapons;
		// TagBlock<animation_graph_sound_reference> sound_references;
		TagBlock<void *> unit_damage;
		TagBlock<void *> first_person_weapons;
		TagBlock<void *> sound_references;
		real limp_body_node_radius;
		struct _flags {
			ushort compress_all_animations_bit:1;
			ushort force_idle_compression_bit:1;
		}flags;
		STAT_ASSERT(_flags, sizeof(ushort));
		short : 16;
		// TagBlock<animation_graph_node> nodes;
		TagBlock<void *> nodes;
		TagBlock<model_animation> animations;
	};
	STAT_ASSERT(model_animation_graph, 0x80 );
};
