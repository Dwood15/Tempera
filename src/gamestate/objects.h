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

#include "../math/real_math.h"
#include "../ce_base_types.h"
#include "../math/animations.h"
#include "../math/colors.h"
#include "../core.h"
#include "../tags/tags.h"
#include "controls.h"
#include <sal.h>
#include <enums/objects_enums.h>

#pragma pack(push)
#pragma pack(1)

struct bone {
	float unknown[10];
	vect3 World;
};

struct s_garbage_data {
	__int16 ticks_until_gc;
	PAD16;
	__int32 _unused[5];
}; static_assert(sizeof(s_garbage_data) == (k_object_size_garbage - k_object_size_item));

struct s_item_data {
	unsigned long       flags;                    // 0x1F4
	__int16             detonation_countdown;    // 0x1F8
	struct {
		__int16 surface_index;            // 0x1FA
		__int16 bsp_reference_index;        // 0x1FC
	}                   bsp_collision;
	__int16             __pad0;                                // 0x1FE
	Yelo::datum_index         dropped_by_unit_index;    // 0x200 set when the unit who had this item drops it
	long                last_update_time;        // 0x204
	struct {
		Yelo::datum_index  object_index;        // 0x208
		real_point3d object_position;    // 0x20C
	}                   object_collision;
	real_vector3d       __unk_type0;        // 0x218
	real_euler_angles2d __unk_type1;    // 0x224
}; static_assert(sizeof(s_item_data) == (k_object_size_item - k_object_size_object));
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
}; static_assert(sizeof(s_object_datum_network_delta_data) == 0x44);

struct s_object_datum_animation_data {
	Yelo::datum_index       definition_index;    // 0xCC
	s_animation_state state;        // 0xD0
	__int16           interpolation_frame_index;// 0xD4
	__int16           interpolation_frame_count;// 0xD6
}; static_assert(sizeof(s_object_datum_animation_data) == 0xC);

struct s_object_datum_damage_data {
	float            maximum_health;                    // 0xD8
	float            maximum_shield;                    // 0xDC
	float            health;                            // 0xE0, health = body
	float            shield;                            // 0xE4
	float            shield_damage_current;            // 0xE8
	float            body_damage_current;                // 0xEC
	// when this object is damaged, the 'entangled' object will also get damaged.
	// this is an immediate link, the entangled object's parent chain or 'entangled' reference isn't walked
	Yelo::datum_index      entangled_object_index;        // 0xF0
	float            shield_damage_recent;                // 0xF4
	float            body_damage_recent;                // 0xF8
	long             shield_damage_update_tick;            // 0xFC, these update ticks are set to NONE when not active
	long             body_damage_update_tick;            // 0x100
	__int16          stun_ticks;                        // 0x104, based on ftol(s_shield_damage_resistance->stun_time * 30f)
	unsigned __int16 flags;                    // 0x106
}; static_assert(sizeof(s_object_datum_damage_data) == 0x30);

struct s_object_datum_attachments_data {
	attachment_type attached_types[MAX_ATTACHMENTS_PER_OBJECT];    // 0x144
	// game state datum_index
	// ie, if Attachments[x]'s definition (object_attachment_block[x]) says it is a 'cont'
	// then the datum_index is a contrail_data handle
	Yelo::datum_index     attachment_indices[MAX_ATTACHMENTS_PER_OBJECT];            // 0x14C
	Yelo::datum_index     first_widget_index;                                                                // 0x16C
}; static_assert(sizeof(s_object_datum_attachments_data) == 0x2C);

struct s_halo_pc_network {
	networked_datum Role;                                //0x0, or 0x4 in context of the object_data structure
	bool : 8;                                            //0x4 or 0x8
	bool should_force_baseline_update;                   //0x5  or 0x9
	unsigned __int16 : 16;                              //0x6	or 0xA //TODO: verify if this is padding or not
	signed long network_time;                           // 0x8 or 0xC
}; static_assert(sizeof(s_halo_pc_network) == 0xC);

struct s_scenario_location {
	long             leaf_index;
	__int16          cluster_index;
	unsigned __int16 __unk_type0;
}; static_assert(sizeof(s_scenario_location) == 0x8);

//s_object_data is also in forge's definitions. \\TODO: Rework and look at again.
struct s_object_data {
	Yelo::datum_index                       definition_index;                                            // 0x0
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
	PAD16;
	game_team   owner_team;                                                    // 0xB8
	__int16     name_list_index;                                                            // 0xBA
	// ticks spent not at_rest. only biped updates this
	__int16     moving_time;                                                                // 0xBC
	__int16     region_permutation;                                                        // 0xBE, variant id
	Yelo::datum_index player_index;                                                        // 0xC0
	// If this were a projectile, this might be the handle to the weapon which spawned it
	Yelo::datum_index owner_object_index;                                                    // 0xC4
	PAD32;                                                                          // 0xC8 unused
	s_object_datum_animation_data animation;                                        // 0xCC
	s_object_datum_damage_data    damage;                                            // 0xD8
	PAD32;                                                 // 0x108 unused
	Yelo::datum_index cluster_partition_index;                                // 0x10C
	Yelo::datum_index garbage_collection_object_index_rltd;                    // 0x110, object_index, garbage collection related
	Yelo::datum_index next_object_index;                                        // 0x114
	Yelo::datum_index first_object_index;                                        // 0x118
	Yelo::datum_index parent_object_index;                                    // 0x11C
	signed char parent_node_index;                                                        // 0x120
	char        unused_byte_unk;                                                                // 0x121 idk if this is an sbyte or bool here
	bool        force_shield_update;                                                        // 0x122
	signed char valid_outgoing_functions;                                            // 0x123, 1<<function_index
	float       incoming_function_values[k_number_of_incoming_object_functions];    // 0x124
	float       outgoing_function_values[k_number_of_outgoing_object_functions];    // 0x134

	s_object_datum_attachments_data attachments;                                    // 0x144
	Yelo::datum_index                     cached_render_state_index;                                            // 0x170
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
static_assert(sizeof(s_object_data) == object_sizes::k_object_size_object);


struct s_object_datum {
	enum {
		k_object_types_mask = object_type::_object_type_object
	};
	s_object_data object;
};
static_assert(sizeof(s_object_datum) == object_sizes::k_object_size_object);

struct s_persistent_control {
	long          ticks_remaining;                              // 0x210
	unsigned long flags;                                 // 0x214
};

struct s_damage_result {
	short       category;                                    // 0x404 Enums::damage_category
	short       ai_handle_delay_ticks;                        // 0x406 ticks remaining until the engine tells the AI code to handle the damage result
	real        amount;                                    // 0x408
	Yelo::datum_index responsible_unit_index;                     // 0x40C
};

struct s_unit_datum_animation_data {
	unsigned short    flags;                     // 0x298
	short             pad0_anim_weap;                  // 0x29A animation index, weapon type
	short             pad1_anim;                  // 0x29C animation index
	short             pad2_unused;                  // 0x29E, appears unused except for getting initialized in unit_new
	//////////////////////////////////////////////////////////////////////////
	// animation graph unit indexes
	sbyte             seat_index;                     // 0x2A0
	sbyte             seat_weapon_index;               // 0x2A1
	sbyte             weapon_type_index;               // 0x2A2
	//////////////////////////////////////////////////////////////////////////
	byte_enum         state;                     // 0x2A3 [Enums::unit_animation_state]
	byte_enum         replacement_animation_state;      // 0x2A4 [Enums::unit_replacement_animation_state]
	byte_enum         overlay_animation_state;         // 0x2A5 [Enums::unit_overlay_animation_state]
	byte_enum         desired_animation_state;         // 0x2A6, set from s_unit_control_data's animation_state
	byte_enum         base_seat;                  // 0x2A7 [Enums::unit_base_seat]
	sbyte             emotion;                        // 0x2A8
	unsigned char     pad3_unknown;
	s_animation_state replacement_animation;   // 0x2AA
	s_animation_state overlay_animation;      // 0x2AE
	s_animation_state weapon_ik;            // 0x2B2
	bool              update_look_euler;                  // 0x2B6 these are set to true when the bounds below change
	bool              update_aim_euler;                  // 0x2B7
	real_rectangle2d  looking_bounds;         // 0x2B8
	real_rectangle2d  aiming_bounds;            // 0x2C8
	long              pad3;                              // 0x2D8
	long              pad4;
}; static_assert(sizeof(s_unit_datum_animation_data) == 0x48);



struct s_unit_speech {
	Yelo::Enums::unit_speech_priority priority;
	Yelo::Enums::unit_scream_type     scream;
	Yelo::datum_index                       sound_definition_index;
	short                             time_rel_pad0; // time related
	unsigned short                    pad_unknown;
	long                              pad_unverified; // haven't verified what is here yet
	byte                              ai_information[0x20];
}; static_assert(sizeof(s_unit_speech) == 0x30);

struct s_unit_speech_data {
	s_unit_speech current;                              // 0x388
	s_unit_speech next;                                 // 0x3B8 not *positive* of this field
	short         pad0;                              // 0x3E8
	short         pad1;                              // 0x3EA
	short         pad2;                              // 0x3EC
	short         pad3;                              // 0x3EE
	long          pad4;                              // 0x3F0 time related
	bool          pad5;                                 // 0x3F4
	bool          pad6;                                 // 0x3F5
	bool          pad7;                                 // 0x3F6
	unsigned char pad8;                                          // 0x3F7
	short         pad9;                              // 0x3F8
	short         padA;                              // 0x3FA
	short         padB;                              // 0x3FC
	short         padC;                              // 0x3FE
	long          padD;                              // 0x400
};

enum unit_camo_regrowth : short {
	_unit_camo_regrowth_off,
	_unit_camo_regrowth_on, // they fired their weapon, requiring active_camo_regrowth_rate to be applied
};

struct s_recent_damage {
	long        game_tick;            // the last game tick damage was dealt
	real        damage;               // total (read: additive) damage the responsible object has done
	Yelo::datum_index responsible_unit;
	Yelo::datum_index responsible_player;   // would be NONE if killed by AI
};

struct control_state {
	/** Unit is pressing the crouch button. */
	ushort crouch_button : 1;

	/** Unit is pressing the jump button. */
	ushort jump_button : 1;

	ushort unkA : 2;

	/** Unit is pressing the flashlight button. */
	ushort flashlight_button : 1;

	ushort unkB : 1;

	/** Unit is pressing the action button. */
	ushort action_button : 1;

	/** Unit is pressing the melee button. */
	ushort melee_button : 1;

	ushort unkC :  2;

	/** Unit is pressing the reload button. */
	ushort reload_button : 1;

	/** Unit is pressing the primary fire button. */
	ushort primary_fire_button : 1;

	/** Unit is pressing the primary fire button. */
	ushort secondary_fire_button : 1;

	/** Unit is pressing the grenade button. */
	ushort grenade_button : 1;

	/** Unit is holding the exchange weapon button or is holding down the action button. */
	ushort exchange_weapon_button : 1;

	ushort unkD : 1;
};
static_assert(sizeof(control_state) == sizeof(short));

union unit_control_flags {
	short         control_flags_a;
	control_state control_flags;
};
static_assert(sizeof(unit_control_flags) == sizeof(short));

struct s_unit_control_data {
	byte               animation_state;
	byte               aiming_speed;
	unit_control_flags control_flags;
	int16              weapon_index;
	int16              grenade_index;
	int16              zoom_index;
	PAD16;
	real_vector3d       throttle;
	real                primary_trigger;
	real_vector3d       facing_vector;
	real_vector3d       aiming_vector;
	real_euler_angles3d looking_vector;
};

static_assert(sizeof(s_unit_control_data) == 0x40);

struct s_player_action {
	unit_control_flags control_flagsA;
	unit_control_flags control_flagsB;
	float              desired_facing_yaw;
	float              desired_facing_pitch;
	float              throttle_forwardback;
	float              throttle_leftright;
	float              primary_trigger;
	__int16            desired_weapon_index;
	__int16            desired_grenade_index;
	__int16            desired_zoom_index;
	__int16            field_1E;
};

//If it's not here, it won't ever update...
static s_player_action ActionOverrides[MAX_PLAYER_COUNT_LOCAL];

struct s_unit_data {
	Yelo::datum_index                 actor_index;                              // 0x1F4 //0x0
	Yelo::datum_index                 swarm_actor_index;                           // 0x1F8 //0x4
	Yelo::datum_index                 swarm_next_unit_index;                        // 0x1FC //0x8
	Yelo::datum_index                 swarm_prev_unit_index;                        // 0x200 //0x10
	long_flags                  flags;                                    // 0x204  //first 4 bits unknown,  5th bit == invisible 14 bits unknown
	unit_control_flags          control_flags;                     //0x208
	short                       pad00;
	short                       pad0;                                 // 0x20C related to the first two short's in s_unit_globals_data
	sbyte                       shield_sapping;                                 // 0x20E
	sbyte                       base_seat_index;                                 // 0x20F
	s_persistent_control        persistent_control;
	Yelo::datum_index                 controlling_player_index;                     // 0x218
	short                       ai_effect_type;                                 // 0x21C ai_unit_effect
	short                       emotion_animation_index;                           // 0x21E
	long                        pad1;                                 // 0x220 time (game ticks) of next update for ai_effect_type related code
	real_vector3d               desired_facing_vector;                     // 0x224
	real_vector3d               desired_aiming_vector;                     // 0x230
	real_vector3d               aiming_vector;                           // 0x23C
	real_vector3d               aiming_velocity;                           // 0x248
	real_euler_angles3d         looking_angles;                        // 0x254
	real_vector3d               looking_vector;                           // 0x260
	real_vector3d               looking_velocity;                           // 0x26C
	real_vector3d               throttle;                                 // 0x278
	real                        primary_trigger;                                 // 0x284
	byte_enum                   aiming_speed;                                 // 0x288
	byte                        pad2_melee_related;                                    // 0x289 melee related (state enum?)
	byte                        pad3_melee_related;                                    // 0x28A melee related (some kind of counter)
	sbyte                       ticks_until_flame_to_death;                        // 0x28B
	// looks like the amount of frames left for the ping animation
	// also set to the same PersistentControlTicks value when an actor dies and they fire-wildely
	byte                        pad4_unknown;                                    // 0x28C sbyte
	byte_enum                   throwing_grenade_state;                        // 0x28D
	short                       pad4s_unknown;                                 // 0x28E
	short                       pad5s_unknown;                                 // 0x290
	unsigned short              pad6s_unkown;                                             // 0x292
	Yelo::datum_index                 throwing_grenade_projectile_index;               // 0x294
	s_unit_datum_animation_data animation;                                       // 0x298
	real                        ambient;                                       // 0x2E0
	real                        illumination;                                    // 0x2E4
	real                        mouth_aperture;                                 // 0x2E8
	unsigned long               pad7s_unkown;                                             // 0x2EC unused
	short                       vehicle_seat_index;                              // 0x2F0
	short                       current_weapon_index;                              // 0x2F2
	short                       next_weapon_index;                              // 0x2F4
	unsigned short              pad8s_unknown;                                             // 0x2F6 need to verify this is padding
	Yelo::datum_index                 weapon_object_indices[4];             // 0x2F8
	long                        weapon_ready_times[4];                     // 0x308
	Yelo::datum_index                 equipment_index;                           // 0x218
	sbyte                       current_grenade_index;                           // 0x31C
	sbyte                       next_grenade_index;                              // 0x31D
	byte                        grenade_counts[2];         // 0x31E
	byte                        zoom_level;                              // 0x320
	byte                        desired_zoom_level;                        // 0x321

	sbyte              last_vehicle_speech_timer;                        // 0x322
	byte               aiming_change;                                    // 0x323
	Yelo::datum_index        powered_seats_riders[Yelo::Enums::k_number_of_powered_seats];   // 0x324
	//////////////////////////////////////////////////////////////////////////
	// these fields are all related
	Yelo::datum_index        pad9s_obj_index_rel;                              // 0x32C object index
	long               padAs_gametime_rel;                                 // 0x330 game time
	//////////////////////////////////////////////////////////////////////////
	short              encounter_index;                                 // 0x334
	short              squad_index;                                    // 0x336
	real               powered_seats_power[Yelo::Enums::k_number_of_powered_seats];      // 0x338
	real               integrated_light_power;                           // 0x340
	real               integrated_light_toggle_power;                        // 0x344
	real               integrated_night_vision_toggle_power;                  // 0x348
	//////////////////////////////////////////////////////////////////////////
	// seat related
	real_vector3d      padB_seat_rel;                           // 0x34C
	real_vector3d      padC_seat_rel;                           // 0x358
	real_vector3d      padD_seat_rel;                           // 0x364
	real_vector3d      padE_seat_rel;                           // 0x370
	//////////////////////////////////////////////////////////////////////////
	real               camo_power;                                    // 0x37C
	real               full_spectrum_vision_power;                        // 0x380 gets updated in unit_update, but nothing actually seems to *use* it...full spectrum vision power?
	Yelo::datum_index        dialogue_definition_index;                     // 0x384
	s_unit_speech_data speech;

	s_damage_result    damage_result;
	Yelo::datum_index        responsible_flamer_object_index;               // 0x410 object which caused us to start flaming to death
	real               padF_unkown;                                    // 0x414
	unsigned long      pad10_unknown;
	long               death_time;                                    // 0x41C // game time when this unit died
	short              feign_death_timer;                              // 0x420
	unit_camo_regrowth camo_regrowth;                  // 0x422
	real               stun;                                          // 0x424
	short              stun_timer;                                    // 0x428
	short              killing_spree_count;                              // 0x42A
	long               killing_spree_start_time;                           // 0x42C
	s_recent_damage    recent_damage[4];                        // 0x430
	unsigned long      pad11_unknown;                                             // 0x470 unused
	//////////////////////////////////////////////////////////////////////////
	// Added in HaloPC
	bool               pad11_networkpcOnly;                                    // 0x474 networking related. engine only writes to this, never reads. consider it 'unused'
	bool               pad12_networkingpcOnly;                                    // 0x475 networking related. engine only writes to this, never reads. consider it 'unused'

	unsigned short pad13_yelo_uses_these; // 0x476

	s_unit_control_data control_data;                        // 0x478
	bool                last_completed_client_update_id_valid;                  // 0x4B8
	unsigned char       pad13;
	unsigned short      pad14;
	long                last_completed_client_update_id;                     // 0x4BC
	unsigned long       pad15;                                             // 0x4C0 unused
	unsigned long       pad16;                                             // 0x4C4 unused
	unsigned long       pad17;                                             // 0x4C8 unused
	//////////////////////////////////////////////////////////////////////////
};
static_assert(sizeof(s_unit_data) == 0x2D8); //(Enums::k_object_size_unit - Enums::k_object_size_object));

struct s_unit_datum {
	s_object_data unit_object;
	s_unit_data   unit;
};

static_assert(sizeof(s_unit_datum) == object_sizes::k_object_size_unit);

struct s_item_datum {
	enum {
		k_object_types_mask = object_type::_object_type_mask_item
	};

	s_object_data object;
	s_item_data   item;
}; static_assert(sizeof(s_item_datum) == object_sizes::k_object_size_item);

struct s_garbage_datum : s_item_datum {
	enum {
		k_object_types_mask = FLAG(_object_type_garbage)
	};

	s_garbage_data garbage;
}; static_assert(sizeof(s_garbage_datum) == k_object_size_garbage);

struct s_object_header_datum {
	short               header_salt;    //0x0
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

		struct s_biped_datum   *_biped;
		struct s_vehicle_datum *_vehicle;
	}; STAT_ASSRT(magic, 0x4);

}; //static_assert(sizeof(s_object_header_datum) == 0xC);


// Objects include bipeds, scenery, weapons, vehicles, powerups, projectiles, etc


struct object_data {
	ident                             Meta;                                                            // 0x0
	s_halo_pc_network                 PcNetworkData;                                                   // 0x4
	unsigned long                     Flags;                                                         // 0x10 TODO: Document.
	signed long                       ObjectMarkerId;                                                // 0x14
	s_object_datum_network_delta_data NetworkDeltaData;                                                // 0x18 // Added in HaloPC
	vect3                             World;                                                            // 0x5C
	vect3                             Velocity;                                                         // 0x68
	vect3                             Forward;                                                         // 0x74
	vect3                             Up;                                                            // 0x80
	vect3                             AngularVelocity;                                                // 0x8C
	s_scenario_location               Location;                                                         // 0x98
	vect3                             Center;                                                         // 0xA0
	float                             Radius;                                                         // 0xBC
	float                             Scale;                                                         // 0xB0
	__int16                           type;                                                            // 0xB4
	__int16                           unknown0;
	game_team                         OwnerTeam;                                                      // 0xB8
	__int16                           NameListIdx;                                                     // 0xBA
	// ticks spent not at_rest. only biped updates this
	__int16                           MovingTime;                                                      // 0xBC
	__int16                           RegionPermutation;                                               // 0xBE, variant id
	ident                             Player;                                                         // 0xC0
	// If this were a projectile, this might be the handle to the weapon which spawned it
	ident                             owner_object_index;                                              // 0xC4
	long                              unknown1;                                                         // 0xC8 unused
	s_object_datum_animation_data     Animation;                                                      // 0xCC
	s_object_datum_damage_data        Damage;                                                         // 0xD8
	long                              unknown2;                                                         // 0x108 unused
	ident                             cluster_partition_index;                                       // 0x10C
	ident                             garbage_collection_object_index_rltd;                           // 0x110, object_index, garbage collection related
	Yelo::datum_index                       next_object_index;                                                // 0x114
	ident                             first_object_index;                                             // 0x118
	ident                             parent_object_index;                                             // 0x11C
	signed char                       parent_node_index;                                               // 0x120
	char                              unused_byte_unk;                                                 // 0x121 idk if this is an sbyte or bool here
	bool                              force_shield_update;                                             // 0x122
	signed char                       valid_outgoing_functions;                                        // 0x123, 1<<function_index
	float                             incoming_function_values[k_number_of_incoming_object_functions]; // 0x124
	float                             outgoing_function_values[k_number_of_outgoing_object_functions]; // 0x134
	s_object_datum_attachments_data   attachments;                                                      // 0x144
	Yelo::datum_index                       CachedRenderStateIdx;                                            // 0x170
	unsigned __int16                  RegionsDestroyedFlags;                                           // 0x174 once a region is destroyed, its bit index is set
	signed __int16                    ShaderPermutation;                                               // 0x176, shader's bitmap block index
	unsigned char                     RegionBvitality[k_maximum_regions_per_model];                    // 0x178
	signed char                       RegionPermutationsIndices[k_maximum_regions_per_model];          // 0x180

	real_rgb_color change_colors[k_number_of_object_change_colors];                                    // 0x188
	real_rgb_color change_colors2[k_number_of_object_change_colors];                                    // 0x1B8

	// one of these are for interpolating
	s_object_header_block_reference node_orientations;                                // 0x1E8 real_orientation3d[node_count]
	s_object_header_block_reference node_orientations2;                                // 0x1EC real_orientation3d[node_count]
	s_object_header_block_reference node_matrix_block;                                // 0x1F0 real_matrix4x3[node_count]

	bool IsPlayer() { return (Player.id >= 0 || Player.index >= 0); }
};

static_assert(sizeof(object_data) == object_sizes::k_object_size_object);


struct object_header {
	short         object_id;         //0x0
	// x41: held by player (on back),
	// xE2: wielded by a player (held)
	// x63: on it's own (like on the ground - even counts for player bipeds)
	unsigned char flags;               //0x2
	unsigned char object_type;         //0x3
	short         sector;  // 0x4 // portal rendering ( cluster index )
	short         size;    // 0x6 // Structure size
	object_data   *address; // 0x8

};

struct biped_data {
	object_data _object;               // 0x0000
	long        ActorIndex;            // 0x01F4
	long        SwarmActorIndex;        // 0x01F8
	long        SwarmNextActorIndex;    // 0x01FC
	long        SwarmPrevObjectId;      // 0x0200
	long        IsInvisible;            // 0x0204	normal = 0x41 invis = 0x51 (bitfield?)
	char        KeyStateActionFlags;    // 0x0208	bit: crouch = 1, jump = 2, (3, unk), 4 - flashlight,
	char        Unknown11[3];         // 0x0209
	char        Unknown09[230];        // 0x020C // originally: 276
	short       selected_weap_index;    // 0x02F2
	short       selected_weap_index1;    // 0x02F4
	short       Unknown14;            // 0x02F6
	ident       held_weapons_ident[3];    // 0x02F8 // -1 if slot doesnt have a weap. (You can stick in any ident into here, and <TAB> weapon switch and it will go into your hand. Even the biped_data object for the player. But that "weapon"'s first person is wierd.. but works!?.. but don't swap back. lol)
	ident       weap_slot_invalid;    //0x02FC // I can paste into the other three to swap weapons, but this fourth one is reset to -1 always. (But the original weapon disappears... Wierd?)
	char        Unknown10[24];
	char        Zoom00;                // 0x0320
	char        Zoom01;                // 0x0321
	char        Unknown12[610];        // 0x0322
	bone        LeftThigh;            // 0x0584
	bone        RightThigh;            // 0x05B8
	bone        Pelvis;                // 0x05EC
	bone        LeftCalf;            // 0x0620
	bone        RightCalf;            // 0x0654
	bone        Spine;                // 0x0688
	bone        LeftClavicle;        // 0x06BC
	bone        LeftFoot;            // 0x06F0
	bone        Neck;                // 0x0724
	bone        RightClavicle;        // 0x0758
	bone        RightFoot;            // 0x078C
	bone        Head;                // 0x07C0
	bone        LeftUpperArm;        // 0x07F4
	bone        RightUpperArm;        // 0x0828
	bone        LeftLowerArm;        // 0x085C
	bone        RightLowerArm;        // 0x0890
	bone        LeftHand;            // 0x08C4
	bone        RightHand;            // 0x08F8
	BYTE        Unknown20[1216];    // 0x092C
}; // Size - 3564(0x0DEC) bytes

/*
////////// Unused ///////////////

// Main header to the entire array of object classes
class CLoopObjectsHeader
{
public:

	char name[32]; // 'loop objects'
	long zero;
	CLoopObjectsSubHeader* start;
	short unknown[4];
	CLoopObjectsSubHeader* first;
	CLoopObjectsSubHeader* last;

};

// Each object class has one of these before it
class CLoopObjectsSubHeader
{
public:

	char head[4]; // 'daeh'
	unsigned long size; // of entire object class, including this header
	void* address; // pointer to the CObjectArray->Offset
	CLoopObjectsSubHeader* next; // pointer to the next subheader
	long unknown;
	char foot[4]; // 'toof'

};
 //From old HaloForge.
 	// ident                             meta;                                // 0x0000
	// long                              object_role;                         // 0x0004
	// char                              flags1[4];                           // 0x0008
	// long                              NetworkTime;                              // 0x000C
	// char                              flags2[4];                           // 0x0010 TODO: Document.
	// long                              timer2;                              // 0x0014
	// s_object_datum_network_delta_data NetworkData;                           // 0x0018
	// vect3                             World;                               // 0x005C
	// vect3                             Velocity;                            // 0x0068
	// vect3                             Forward;                        // 0x0074
	// vect3                             Up;                               // 0x0080
	// vect3                             AngularVelocity;                         // 0x008C
	// s_scenario_location               Location;                          // 0x0098
	// vect3                             CenterCoord;                         // 0x00A0 Apparently these are coordinates, used for the game code's trigger volume point testing
	// float                             Radius;                              // 0x00AC - Radius of object. In Radians. (-1 to 1)
	// float                             Scale;                               // 0x00B0 - Seems to be some random float for all objects (all same objects have same value)
	// short                             Type;                                // 0x00B4 - (0 = Biped) (1 = Vehicle) (2 = Weapon) (3 = Equipment) (4 = Garbage) (5 = Projectile) (6 = Scenery) (7 = Machine) (8 = Control) (9 = Light Fixture) (10 = Placeholder) (11 = Sound Scenery)
	// PAD16;                                    //PAD
	// game_team OwnerTeam;                           // 0x00B8
	// short     NameListIdx;                           // 0x00BA
	// short     MovingTime;                            // 0x00BC
	// short     RegionPermutation;                    // 0x00BD
	// ident     Player;                              // 0x00C0
	// // If this were a projectile, this might be the handle to the weapon which spawned it
	// ident     OwnerId;                            // 0x00C4 - Parent object ID of this object (DOES NOT APPLY TO BIPEDS/PLAYER OBJECTS)
	// PAD32;
	// s_object_datum_animation_data AntrMeta;                            // 0x00CC - Animation Related?
	//
	// short AnimState;                           // 0x00D0 -
	// short TimeSinceAnimChange;                  // 0x00D2 // (0 to 60) Time since last animation_state change. Restarts at 0 when animation_state changes
	// long  unknown5;                              // 0x00D4
	// float MaxHp;                                 // 0x00D8
	// float MaxShields;                           // 0x00DC
	// float Health;                              // 0x00E0 (0 to 1) percent?
	// float Shields;                              // 0x00E4  (0 to 3) (Normal = 1) (Full overshield = 3) percent?
	// long  CurrentShieldDamage;                  // 0x00E8  // INSTANTANEOUS amount of shield being damaged.
	// long  CurrentHealthDamage;                  // 0x00EC // INSTANTANEOUS amount of health being damaged.
	// ident unknown6;                              // 0x00F0  // (???) Always 0xFFFFFFFF?
	// float LastShieldDamagedAmt;                  // 0x00F4
	// float LastBodyDamagedAmount;               // 0x00F8
	// long  LastShieldDamagedTime;               // 0x00FC
	// long  LastBodyDamagedTime;                  // 0x00100
	// short ShieldsRechargeTime;                  // 0x00104
	// short DamageFlags;                           // 0x00106 //Some kind of hp related bitmask.
	// long  UnknownFlags;                        // 0x0108
	// long  ClusterPartition;                     // 0x010C // Continually counts up, and resumes even after object is destroyed and recreated (killed)
	// ident some_obj_id2;                        // 0x0110 // garbage collection related?
	// ident NextObjectId;                        // 0x0114
	// ident CurrentWeapon;                        // 0x0118 // currently held weapon (if this is a player)
	// ident VehicleWeapon;                        // 0x011C // or the thing that <this> is in (i.e. a weapon 'inside' a player, but only if it's wielded)
	// short SeatType;                              // 0x0120
	// short unknown16;                           // 0x0122
	// float ShieldsHit;                           // 0x0124 //Counts down from 1 after shields are hit (0 to 1)
	// float Shield2;                              // 0x0128 //With overshield it stays at 1 may be reason  over-shield drains. (0 to 1) [2nd function]
	// float FlashlightScale;                     // 0x012C //Intensity of flashlight as it turns on and off. (0 to 1) (On > 0) (Off = 0) [3rd function]
	// float ARFunction;                           // 0x0130
	// float Flashlight2;                           // 0x0134
	// long  unknown17[5];                        // 0x0138
	// ident ObjectAttachmentId1;                  // 0x014C
	// ident ObjectAttachmentId2;                  // 0x0150
	// long  zero[6];                              // 0x0154
	// ident UnknownIdent3;                        // 0x016C
	// ident CashedRndrIdx;                        // 0x0170
	// short
	// long  unknown18;                           // 0x0174
	// long  unknown19[4];                        // 0x0178
	// float unknown20[24];                        // 0x0188  // mostly 1.0, occasionally 0.

*/

#pragma pack(pop)
