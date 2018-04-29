#pragma once
/**
 *  Project: Tempera
 *  File: camera.h
 *  Copyright � 2018 Dwood
 *
 *  This file is part of Tempera.
 *
 *  haloforge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  haloforge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with haloforge. If not, see <http://www.gnu.org/licenses/>.
 **/
#include "../headers/colors.h"
#include "../headers/enums_generic.h"
#include "../ce_base_types.h"
#include "../headers/int_math.h"
#include "../headers/real_math.h"
#include "../headers/animations.h"
#include "scenario.h"
#include "networking.h"
#include <sal.h>

typedef bool reBoolGivenShort(short);

#pragma pack(push)
#pragma pack(1)

struct s_garbage_data {
	__int16 ticks_until_gc;
	PAD16;
	__int32 _unused[5];
}; static_assert(sizeof(s_garbage_data) == (k_object_size_garbage - k_object_size_item), STATIC_ASSERT_FAIL);

struct s_item_data {
	unsigned long       flags;                    // 0x1F4
	__int16             detonation_countdown;    // 0x1F8
	struct {
		__int16 surface_index;            // 0x1FA
		__int16 bsp_reference_index;        // 0x1FC
	}                   bsp_collision;
	__int16             __pad0;                                // 0x1FE
	datum_index         dropped_by_unit_index;    // 0x200 set when the unit who had this item drops it
	long                last_update_time;        // 0x204
	struct {
		datum_index  object_index;        // 0x208
		real_point3d object_position;    // 0x20C
	}                   object_collision;
	real_vector3d       __unk_type0;        // 0x218
	real_euler_angles2d __unk_type1;    // 0x224
}; static_assert(sizeof(s_item_data) == (k_object_size_item - k_object_size_object), STATIC_ASSERT_FAIL);
INTELLISENSE_HACK(s_item_data)

namespace obj {
	namespace Memory {
		struct s_header_data {
			uint32     allocation_crc;
			char       level[256];
			tag_string version;
			short      player_spawn_count;
			short      game_difficulty;
			uint32     cache_crc;

			//Also known as YELO//Open Saucey territory.
			sbyte _unused[32];

		}; STAT_ASSRT(s_header_data, 0x14C);

		struct s_memory_pool_block {
			typedef void **reference_t;

			long                head;         // 'head'
			long                size;         // size of the block, exclusive of this header, within the pool
			reference_t         reference;      // the pointer referencing the block allocation
			s_memory_pool_block *next;      // linked list: the pool block that follows this one
			s_memory_pool_block *prev;      // linked list: the pool block the comes before this one
			long                tail;         // 'tail'
		};

		struct s_memory_pool {
			long                signature;      // 'head'
			::tag_string        name;         // debug code name for the allocation
			void                *base_address;   // next address to allocate a block at
			long                size;         // total size of the pool
			long                free_size;      // total size left in the pool thats not owned by anything
			s_memory_pool_block *first;      // pointer to the first pool item
			s_memory_pool_block *last;      // pointer to the last pool item
		};
	};

	struct s_objects_pool_data {
		Memory::s_memory_pool header;
		byte                  data[k_object_memory_pool_allocation_size];
	};

};

// Object datums in the memory pool can have dynamically allocated nested-blocks.
// Due to the way the game implements this, OS can allocate a nested-block of its own in an object,
// and when the stock game uses a gamesave with an object like this, it will silently but safely
// operate on it, even when it comes time to delete the object from the pool.
// An object gets reallocated when a nested-block is requested (and the additional size is added to
// the object's overall pool block size)
struct s_object_header_block_reference {
	unsigned __int16 size;
	unsigned __int16 offset;
}; static_assert(sizeof(s_object_header_block_reference) == 0x4);

//should be populated during the object type's process_update_delta
struct s_object_datum_network_delta_data {
	bool          valid_position;                    // 0x18
	unsigned char __pad__unk0[3];
	real_point3d  position;                    // 0x1C

	bool          valid_forward_and_up;                // 0x28
	unsigned char __pad__unk1[3];
	real_vector3d forward;                    // 0x2C
	real_vector3d up;                        // 0x38

	bool          valid_transitional_velocity;        // 0x44
	unsigned char __pad__unk2[3];
	real_vector3d transitional_velocity;    // 0x48

	bool          valid_timestamp;                    // 0x54
	unsigned char __pad__unk3[3];
	signed long   timestamp;                        // 0x58
}; static_assert(sizeof(s_object_datum_network_delta_data) == 0x44, STATIC_ASSERT_FAIL);

struct s_object_datum_animation_data {
	datum_index       definition_index;    // 0xCC
	s_animation_state state;        // 0xD0
	__int16           interpolation_frame_index;// 0xD4
	__int16           interpolation_frame_count;// 0xD6
}; static_assert(sizeof(s_object_datum_animation_data) == 0xC, STATIC_ASSERT_FAIL);

struct s_object_datum_damage_data {
	float            maximum_health;                    // 0xD8
	float            maximum_shield;                    // 0xDC
	float            health;                            // 0xE0, health = body
	float            shield;                            // 0xE4
	float            shield_damage_current;            // 0xE8
	float            body_damage_current;                // 0xEC
	// when this object is damaged, the 'entangled' object will also get damaged.
	// this is an immediate link, the entangled object's parent chain or 'entangled' reference isn't walked
	datum_index      entangled_object_index;        // 0xF0
	float            shield_damage_recent;                // 0xF4
	float            body_damage_recent;                // 0xF8
	long             shield_damage_update_tick;            // 0xFC, these update ticks are set to NONE when not active
	long             body_damage_update_tick;            // 0x100
	__int16          stun_ticks;                        // 0x104, based on ftol(s_shield_damage_resistance->stun_time * 30f)
	unsigned __int16 flags;                    // 0x106
}; static_assert(sizeof(s_object_datum_damage_data) == 0x30, STATIC_ASSERT_FAIL);

struct s_object_datum_attachments_data {
	attachment_type attached_types[MAX_ATTACHMENTS_PER_OBJECT];    // 0x144
	// game state datum_index
	// ie, if Attachments[x]'s definition (object_attachment_block[x]) says it is a 'cont'
	// then the datum_index is a contrail_data handle
	datum_index     attachment_indices[MAX_ATTACHMENTS_PER_OBJECT];            // 0x14C
	datum_index     first_widget_index;                                                                // 0x16C
}; static_assert(sizeof(s_object_datum_attachments_data) == 0x2C, STATIC_ASSERT_FAIL);

//s_object_data is also in forge's definitions. \\TODO: Rework and look at again.
struct s_object_data {
	datum_index                       definition_index;                                            // 0x0
	s_halo_pc_network                 network_data;                                                // 0x4
	unsigned long                     flags;                                                        // 0x10 TODO: Document.
	signed long                       object_marker_id;                                            // 0x14
	s_object_datum_network_delta_data network_delta;                                    // 0x18 // Added in HaloPC
	real_point3d                      position;                                                    // 0x5C
	real_vector3d                     transitional_velocity;                                        // 0x68
	real_vector3d                     forward;                                                    // 0x74
	real_vector3d                     up;                                                            // 0x80
	real_vector3d                     angular_velocity;                                            // 0x8C
	s_scenario_location               location;                                                    // 0x98
	real_point3d                      center;                                                        // 0xA0
	float                             radius;                                                        // 0xBC
	float                             scale;                                                        // 0xB0
	__int16                           type;                                                        // 0xB4
	__int16                           __pad0;
	game_team                         owner_team;                                                    // 0xB8
	__int16                           name_list_index;                                                            // 0xBA
	// ticks spent not at_rest. only biped updates this
	__int16                           moving_time;                                                                // 0xBC
	__int16                           region_permutation;                                                        // 0xBE, variant id
	datum_index                       player_index;                                                        // 0xC0
	// If this were a projectile, this might be the handle to the weapon which spawned it
	datum_index                       owner_object_index;                                                    // 0xC4
	unsigned long                     __pad1;                                                                            // 0xC8 unused
	s_object_datum_animation_data     animation;                                        // 0xCC
	s_object_datum_damage_data        damage;                                            // 0xD8
	unsigned long                     __pad2;                                                    // 0x108 unused
	datum_index                       cluster_partition_index;                                // 0x10C
	datum_index                       garbage_collection_object_index_rltd;                    // 0x110, object_index, garbage collection related
	datum_index                       next_object_index;                                        // 0x114
	datum_index                       first_object_index;                                        // 0x118
	datum_index                       parent_object_index;                                    // 0x11C
	signed char                       parent_node_index;                                                        // 0x120
	char                              unused_byte_unk;                                                                // 0x121 idk if this is an sbyte or bool here
	bool                              force_shield_update;                                                        // 0x122
	signed char                       valid_outgoing_functions;                                            // 0x123, 1<<function_index
	float                             incoming_function_values[k_number_of_incoming_object_functions];    // 0x124
	float                             outgoing_function_values[k_number_of_outgoing_object_functions];    // 0x134

	s_object_datum_attachments_data attachments;                                    // 0x144
	datum_index                     cached_render_state_index;                                            // 0x170
	unsigned __int16                regions_destroyed_flags;                                                // 0x174 once a region is destroyed, its bit index is set
	signed __int16                  shader_permutation;                                                        // 0x176, shader's bitmap block index
	unsigned char                   region_vitality[k_maximum_regions_per_model];                        // 0x178
	signed char                     region_permutation_indices[k_maximum_regions_per_model];            // 0x180

	real_rgb_color change_colors[k_number_of_object_change_colors];            // 0x188
	real_rgb_color change_colors2[k_number_of_object_change_colors];            // 0x1B8

	// one of these are for interpolating
	s_object_header_block_reference node_orientations;                                // 0x1E8 real_orientation3d[node_count]
	s_object_header_block_reference node_orientations2;                                // 0x1EC real_orientation3d[node_count]
	s_object_header_block_reference node_matrix_block;                                // 0x1F0 real_matrix4x3[node_count]
};
static_assert(sizeof(s_object_data) == object_sizes::k_object_size_object, STATIC_ASSERT_FAIL);


struct s_object_datum {
	enum {
		k_object_types_mask = object_type::_object_type_object
	};
	s_object_data object;
};
static_assert(sizeof(s_object_datum) == object_sizes::k_object_size_object, STATIC_ASSERT_FAIL);


struct s_item_datum {
	enum {
		k_object_types_mask = object_type::_object_type_mask_item
	};

	s_object_data object;
	s_item_data   item;
}; static_assert(sizeof(s_item_datum) == object_sizes::k_object_size_item, STATIC_ASSERT_FAIL);

struct s_garbage_datum : s_item_datum {
	enum {
		k_object_types_mask = FLAG(_object_type_garbage)
	};

	s_garbage_data garbage;
}; static_assert(sizeof(s_garbage_datum) == k_object_size_garbage, STATIC_ASSERT_FAIL);

struct s_object_header_datum {
	datum_index::t_salt header_salt;    //0x0
	object_header_flags flags;            //0x2
	signed char         object_type;            //0x4
	unsigned __int16    cluster_index;        //0x6
	unsigned __int16    data_size;            //0x8

	union magic {
		void *address;

		s_object_data *_object;
		// Note: since 's_object_data' is the base of all object datums, we can do this:
		// Implicitly treat 'address' as an specific object type ptr
		// in situations where we're programming for something more specific (eg, unit types only)

		struct s_item_datum      *_item;
		struct s_weapon_datum    *_weapon;
		struct s_equipment_datum *_equipment;

		struct s_unit_datum    *_unit;
		struct s_biped_datum   *_biped;
		struct s_vehicle_datum *_vehicle;
	}; STAT_ASSRT(magic, 0x4);

}; //static_assert(sizeof(s_object_header_datum) == 0xC, STATIC_ASSERT_FAIL);

#pragma pack(pop)
