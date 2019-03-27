#pragma once

#include "../math/real_math.h"
#include "../math/animations.h"
#include "../math/colors.h"
#include "../tags/tags.h"
#include "../scenario/structures.h"
#include "controls.h"

#include <enums/objects_enums.h>
#include <enums/model_enums.h>
#include <enums/generic_enums.h>

#pragma pack(push)
#pragma pack(1)

struct bone {
	float unknown[10];
	vect3 World;
};

struct s_garbage_data {
	short ticks_until_gc;
	unsigned short:16;
	__int32 _unused[5];
};

STAT_ASSERT(s_garbage_data, (k_object_size_garbage - k_object_size_item));

struct s_item_data {
	unsigned long       flags;                    // 0x1F4
	short               detonation_countdown;    // 0x1F8
	struct {
		short surface_index;            // 0x1FA
		short bsp_reference_index;        // 0x1FC
	}                   bsp_collision;
	short               __pad0;                                // 0x1FE
	Yelo::datum_index   dropped_by_unit_index;    // 0x200 set when the unit who had this item drops it
	long                last_update_time;        // 0x204
	struct {
		Yelo::datum_index object_index;        // 0x208
		real_point3d      object_position;    // 0x20C
	}                   object_collision;
	real_vector3d       __unk_type0;        // 0x218
	real_euler_angles2d __unk_type1;    // 0x224
};
STAT_ASSERT(s_item_data, (k_object_size_item - k_object_size_object));

INTELLISENSE_HACK(s_item_data)

// Object datums in the memory pool can have dynamically allocated nested-blocks.
// Due to the way the game implements this, OS can allocate a nested-block of its own in an object,
// and when the stock game uses a gamesave with an object like this, it will silently but safely
// operate on it, even when it comes time to delete the object from the pool.
// An object gets reallocated when a nested-block is requested (and the additional size is added to
// the object's overall pool block size)
struct s_object_header_block_reference {
	unsigned short size;
	unsigned short offset;
};
STAT_ASSERT(s_object_header_block_reference, 0x4);

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
};
STAT_ASSERT(s_object_datum_network_delta_data, 0x44);

struct s_object_datum_damage_data {
	float             maximum_health;                    // 0xD8
	float             maximum_shield;                    // 0xDC
	float             health;                            // 0xE0, health = body
	float             shield;                            // 0xE4
	float             shield_damage_current;            // 0xE8
	float             body_damage_current;                // 0xEC
	// when this object is damaged, the 'entangled' object will also get damaged.
	// this is an immediate link, the entangled object's parent chain or 'entangled' reference isn't walked
	Yelo::datum_index entangled_object_index;        // 0xF0
	float             shield_damage_recent;                // 0xF4
	float             body_damage_recent;                // 0xF8
	long              shield_damage_update_tick;            // 0xFC, these update ticks are set to NONE when not active
	long              body_damage_update_tick;            // 0x100
	short             stun_ticks;                        // 0x104, based on ftol(s_shield_damage_resistance->stun_time * 30f)
	unsigned short    flags;                    // 0x106
};

STAT_ASSERT(s_object_datum_damage_data, 0x30);

struct s_object_datum_attachments_data {
	attachment_type   attached_types[MAX_ATTACHMENTS_PER_OBJECT];    // 0x144
	// game state datum_index
	// ie, if Attachments[x]'s definition (object_attachment_block[x]) says it is a 'cont'
	// then the datum_index is a contrail_data handle
	Yelo::datum_index attachment_indices[MAX_ATTACHMENTS_PER_OBJECT];            // 0x14C
	Yelo::datum_index first_widget_index;                                                                // 0x16C
};

STAT_ASSERT(s_object_datum_attachments_data, 0x2C);

struct s_halo_pc_network {
	networked_datum Role;                                //0x0, or 0x4 in context of the object_data structure
	bool : 8;                                            //0x4 or 0x8
	bool should_force_baseline_update;                   //0x5  or 0x9
	unsigned short : 16;                              //0x6	or 0xA //TODO: verify if this is padding or not
	signed long network_time;                           // 0x8 or 0xC
};

STAT_ASSERT(s_halo_pc_network, 0xC);

#include "../scenario/scenario_location.h"


struct s_object_placement_data {
	datum_index   definition_index;
	unsigned long flags;
	datum_index   player_index;
	datum_index   owner_object_index;
	int : 32;
	game_team      owner_team;
	int16          region_permutation; // variant id
	real_point3d   position;
	float          unkAngleMaybe; // angle?
	real_vector3d  transitional_velocity;
	real_vector3d  forward;
	real_vector3d  up;
	real_vector3d  angular_velocity;
	real_rgb_color change_colors[k_number_of_object_change_colors];
};

STAT_ASSERT(s_object_placement_data, 0x88);

//s_object_data is also in forge's definitions. \\TODO: Rework and look at again.
struct s_object_data {
	Yelo::datum_index                   definition_index;                                            // 0x0
	s_halo_pc_network                   network_data;                                                // 0x4
	unsigned long                       flags;                                                        // 0x10 TODO: Document.
	signed long                         object_marker_id;                                            // 0x14
	s_object_datum_network_delta_data   network_delta;                                    // 0x18 // Added in HaloPC
	real_point3d                        position;                                                    // 0x5C
	real_vector3d                       transitional_velocity;                                        // 0x68
	real_vector3d                       forward;                                                    // 0x74
	real_vector3d                       up;                                                            // 0x80
	real_vector3d                       angular_velocity;                                            // 0x8C
	Yelo::Scenario::s_scenario_location location;                                                    // 0x98
	real_point3d                        center;                                                        // 0xA0
	float                               radius;                                                        // 0xBC
	float                               scale;                                                        // 0xB0
	short                               type;                                                        // 0xB4
	unsigned short:16;
	game_team         owner_team;                                                    // 0xB8
	short             name_list_index;                                                            // 0xBA
	// ticks spent not at_rest. only biped updates this
	short             moving_time;                                                                // 0xBC
	short             region_permutation;                                                        // 0xBE, variant id
	Yelo::datum_index player_index;                                                        // 0xC0
	// If this were a projectile, this might be the handle to the weapon which spawned it
	Yelo::datum_index owner_object_index;                                                    // 0xC4
	unsigned long:32;                                                                          // 0xC8 unused
	s_object_datum_animation_data animation;                                        // 0xCC
	s_object_datum_damage_data    damage;                                            // 0xD8
	int : 32;                                               // 0x108 unused
	Yelo::datum_index cluster_partition_index;                                // 0x10C
	Yelo::datum_index garbage_collection_object_index_rltd;                    // 0x110, object_index, garbage collection related
	Yelo::datum_index next_object_index;                                        // 0x114
	Yelo::datum_index first_object_index;                                        // 0x118
	Yelo::datum_index parent_object_index;                                    // 0x11C
	signed char       parent_node_index;                                                        // 0x120
	char              unused_byte_unk;                                                                // 0x121 idk if this is an sbyte or bool here
	bool              force_shield_update;                                                        // 0x122
	signed char       valid_outgoing_functions;                                            // 0x123, 1<<function_index
	float             incoming_function_values[k_number_of_incoming_object_functions];    // 0x124
	float             outgoing_function_values[k_number_of_outgoing_object_functions];    // 0x134

	s_object_datum_attachments_data attachments;                                    // 0x144
	Yelo::datum_index               cached_render_state_index;                                            // 0x170
	unsigned short                  regions_destroyed_flags;                                                // 0x174 once a region is destroyed, its bit index is set
	signed short                    shader_permutation;                                                        // 0x176, shader's bitmap block index
	unsigned char                   region_vitality[k_maximum_regions_per_model];                        // 0x178
	signed char                     region_permutation_indices[k_maximum_regions_per_model];            // 0x180

	real_rgb_color change_colors[k_number_of_object_change_colors];            // 0x188
	real_rgb_color change_colors2[k_number_of_object_change_colors];            // 0x1B8

	// one of these are for interpolating
	s_object_header_block_reference node_orientations;                                // 0x1E8 real_orientation3d[node_count]
	s_object_header_block_reference node_orientations2;                                // 0x1EC real_orientation3d[node_count]
	s_object_header_block_reference node_matrix_block;                                // 0x1F0 real_matrix4x3[node_count]

	void CopyToPlacementData(s_object_placement_data& data);
	bool VerifyType(long_flags type_mask);
};
STAT_ASSERT(s_object_data, object_sizes::k_object_size_object);


struct s_object_datum {
	enum {
		k_object_types_mask = object_type::_object_type_object
	};
	s_object_data object;
};

STAT_ASSERT(s_object_datum, object_sizes::k_object_size_object);

struct s_damage_result {
	short             category;                                    // 0x404 Enums::damage_category
	short             ai_handle_delay_ticks;                        // 0x406 ticks remaining until the engine tells the AI code to handle the damage result
	real              amount;                                    // 0x408
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
};
STAT_ASSERT(s_unit_datum_animation_data, 0x48);

#include "units/unit_speech.h"
#include "enums/objects_enums.h"

struct s_unit_data {
	Yelo::datum_index           actor_index;                              // 0x1F4 //0x0
	Yelo::datum_index           swarm_actor_index;                           // 0x1F8 //0x4
	Yelo::datum_index           swarm_next_unit_index;                        // 0x1FC //0x8
	Yelo::datum_index           swarm_prev_unit_index;                        // 0x200 //0x10
	long_flags                  flags;                                    // 0x204  //first 4 bits unknown,  5th bit == invisible 14 bits unknown
	unit_control_flags          control_flags;                     //0x208
	short                       pad00;
	short                       pad0;                                 // 0x20C related to the first two short's in s_unit_globals_data
	sbyte                       shield_sapping;                                 // 0x20E
	sbyte                       base_seat_index;                                 // 0x20F
	s_persistent_control        persistent_control;
	Yelo::datum_index           controlling_player_index;                     // 0x218
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
	byte                        aiming_speed;                                 // 0x288
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
	Yelo::datum_index           throwing_grenade_projectile_index;               // 0x294
	s_unit_datum_animation_data animation;                                       // 0x298
	real                        ambient;                                       // 0x2E0
	real                        illumination;                                    // 0x2E4
	real                        mouth_aperture;                                 // 0x2E8
	unsigned long               pad7s_unkown;                                             // 0x2EC unused
	short                       vehicle_seat_index;                              // 0x2F0
	short                       current_weapon_index;                              // 0x2F2
	short                       next_weapon_index;                              // 0x2F4
	unsigned short              pad8s_unknown;                                             // 0x2F6 need to verify this is padding
	Yelo::datum_index           weapon_object_indices[4];             // 0x2F8
	long                        weapon_ready_times[4];                     // 0x308
	Yelo::datum_index           equipment_index;                           // 0x218
	sbyte                       current_grenade_index;                           // 0x31C
	sbyte                       next_grenade_index;                              // 0x31D
	byte                        grenade_counts[2];         // 0x31E
	byte                        zoom_level;                              // 0x320
	byte                        desired_zoom_level;                        // 0x321

	sbyte              last_vehicle_speech_timer;                        // 0x322
	byte               aiming_change;                                    // 0x323
	Yelo::datum_index  powered_seats_riders[Yelo::Enums::k_number_of_powered_seats];   // 0x324
	//////////////////////////////////////////////////////////////////////////
	// these fields are all related
	Yelo::datum_index  pad9s_obj_index_rel;                              // 0x32C object index
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
	Yelo::datum_index  dialogue_definition_index;                     // 0x384
	s_unit_speech_data speech;

	s_damage_result               damage_result;
	Yelo::datum_index             responsible_flamer_object_index;               // 0x410 object which caused us to start flaming to death
	real                          padF_unkown;                                    // 0x414
	unsigned long                 pad10_unknown;
	long                          death_time;                                    // 0x41C // game time when this unit died
	short                         feign_death_timer;                              // 0x420
	Yelo::Enums::unit_camo_regrowth camo_regrowth;                  // 0x422
	real                          stun;                                          // 0x424
	short                         stun_timer;                                    // 0x428
	short                         killing_spree_count;                              // 0x42A
	long                          killing_spree_start_time;                           // 0x42C
	s_recent_damage               recent_damage[4];                        // 0x430
	unsigned long                 pad11_unknown;                                             // 0x470 unused
	//////////////////////////////////////////////////////////////////////////
	// Added in HaloPC
	bool                          pad11_networkpcOnly;                                    // 0x474 networking related. engine only writes to this, never reads. consider it 'unused'
	bool                          pad12_networkingpcOnly;                                    // 0x475 networking related. engine only writes to this, never reads. consider it 'unused'

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
STAT_ASSERT(s_unit_data, 0x2D8); //(Enums::k_object_size_unit - Enums::k_object_size_object));

struct s_unit_datum {
	enum {
		k_object_types_mask = object_type::_object_type_mask_unit
	};

	s_object_data unit_object;
	s_unit_data   unit;
};
STAT_ASSERT(s_unit_datum, sizeof(s_object_data) + sizeof(s_unit_data));

struct s_item_datum {
	enum {
		k_object_types_mask = object_type::_object_type_mask_item
	};

	s_object_data object;
	s_item_data   item;
};
STAT_ASSERT(s_item_datum, object_sizes::k_object_size_item);

struct s_garbage_datum : s_item_datum {
	enum {
		k_object_types_mask = FLAG(_object_type_garbage)
	};

	s_garbage_data garbage;
};

STAT_ASSERT(s_garbage_datum, k_object_size_garbage);

struct s_object_header_datum {
	short               header_salt;    //0x0
	object_header_flags flags;            //0x2
	signed char         object_type;            //0x4
	unsigned short      cluster_index;        //0x6
	unsigned short      data_size;            //0x8

	union {
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
	};
	//STAT_ASSERT(magic, 0x4)

};
STAT_ASSERT(s_object_header_datum, 0xC);

// Objects include bipeds, scenery, weapons, vehicles, powerups, projectiles, etc

struct object_data {
	Yelo::datum_index                   Meta;                                                            // 0x0
	s_halo_pc_network                   PcNetworkData;                                                   // 0x4
	unsigned long                       Flags;                                                         // 0x10 TODO: Document.
	signed long                         ObjectMarkerId;                                                // 0x14
	s_object_datum_network_delta_data   NetworkDeltaData;                                                // 0x18 // Added in HaloPC
	vect3                               World;                                                            // 0x5C
	vect3                               Velocity;                                                         // 0x68
	vect3                               Forward;                                                         // 0x74
	vect3                               Up;                                                            // 0x80
	vect3                               AngularVelocity;                                                // 0x8C
	Yelo::Scenario::s_scenario_location Location;                                                         // 0x98
	vect3                               Center;                                                         // 0xA0
	float                               Radius;                                                         // 0xBC
	float                               Scale;                                                         // 0xB0
	short                               type;                                                            // 0xB4
	short                               unknown0;
	game_team                           OwnerTeam;                                                      // 0xB8
	short                               NameListIdx;                                                     // 0xBA
	// ticks spent not at_rest. only biped updates this
	short                               MovingTime;                                                      // 0xBC
	short                               RegionPermutation;                                               // 0xBE, variant id
	Yelo::datum_index                   Player;                                                         // 0xC0
	// If this were a projectile, this might be the handle to the weapon which spawned it
	Yelo::datum_index                   owner_object_index;                                              // 0xC4
	long                                unknown1;                                                         // 0xC8 unused
	s_object_datum_animation_data       Animation;                                                      // 0xCC
	s_object_datum_damage_data          Damage;                                                         // 0xD8
	long                                unknown2;                                                         // 0x108 unused
	Yelo::datum_index                   cluster_partition_index;                                       // 0x10C
	Yelo::datum_index                   garbage_collection_object_index_rltd;                           // 0x110, object_index, garbage collection related
	Yelo::datum_index                   next_object_index;                                                // 0x114
	Yelo::datum_index                   first_object_index;                                             // 0x118
	Yelo::datum_index                   parent_object_index;                                             // 0x11C
	signed char                         parent_node_index;                                               // 0x120
	char                                unused_byte_unk;                                                 // 0x121 idk if this is an sbyte or bool here
	bool                                force_shield_update;                                             // 0x122
	signed char                         valid_outgoing_functions;                                        // 0x123, 1<<function_index
	float                               incoming_function_values[k_number_of_incoming_object_functions]; // 0x124
	float                               outgoing_function_values[k_number_of_outgoing_object_functions]; // 0x134
	s_object_datum_attachments_data     attachments;                                                      // 0x144
	Yelo::datum_index                   CachedRenderStateIdx;                                            // 0x170
	unsigned short                      RegionsDestroyedFlags;                                           // 0x174 once a region is destroyed, its bit index is set
	signed short                        ShaderPermutation;                                               // 0x176, shader's bitmap block index
	unsigned char                       RegionBvitality[k_maximum_regions_per_model];                    // 0x178
	signed char                         RegionPermutationsIndices[k_maximum_regions_per_model];          // 0x180

	real_rgb_color change_colors[k_number_of_object_change_colors];                                    // 0x188
	real_rgb_color change_colors2[k_number_of_object_change_colors];                                    // 0x1B8

	// one of these are for interpolating
	s_object_header_block_reference node_orientations;                                // 0x1E8 real_orientation3d[node_count]
	s_object_header_block_reference node_orientations2;                                // 0x1EC real_orientation3d[node_count]
	s_object_header_block_reference node_matrix_block;                                // 0x1F0 real_matrix4x3[node_count]

	bool IsPlayer() { return (Player.index >= 0); }
};

STAT_ASSERT(object_data, object_sizes::k_object_size_object);

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

struct s_object_iterator {
	enum { k_signature = 0x86868686 };

	long_flags          type_mask;                  // object types to iterate
	object_header_flags ignore_flags;   // When any of these bits are set, the object is skipped
	unsigned char : 8;
	int16       next_index;
	datum_index object_index;
	tag         signature;

	void SetEndHack() { signature = 'hack'; }

	bool IsEndHack() const { return signature == 'hack'; }
};

struct _core_1 {
	data_header<void>          *Widget;
	data_header<void>          *Unknown;
	data_header<void>          *Glow;
	data_header<void>          *GlowParticles;
	data_header<void>          *Flag;
	data_header<void>          *Antenna;
	data_header<object_header> *Object;
	data_header<void>          *Unknown1[3];
	data_header<void>          *Unknown2;
	data_header<void>          *ClusterNoncollideableObject;
	data_header<void>          *NoncollideableObjectCluster;
	unsigned long              UnknownCounter;
	data_header<void>          *Unknown3;
	data_header<void>          *ClusterCollideableObjectRefe;
	data_header<void>          *CollideableObjectClusterRefe;
	data_header<void>          *Unknown4;
};

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
 	// Yelo::datum_index                             meta;                                // 0x0000
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
	// short : 16;                                    //PAD
	// game_team OwnerTeam;                           // 0x00B8
	// short     NameListIdx;                           // 0x00BA
	// short     MovingTime;                            // 0x00BC
	// short     RegionPermutation;                    // 0x00BD
	// Yelo::datum_index     Player;                              // 0x00C0
	// // If this were a projectile, this might be the handle to the weapon which spawned it
	// Yelo::datum_index     OwnerId;                            // 0x00C4 - Parent object ID of this object (DOES NOT APPLY TO BIPEDS/PLAYER OBJECTS)
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
	// Yelo::datum_index unknown6;                              // 0x00F0  // (???) Always 0xFFFFFFFF?
	// float LastShieldDamagedAmt;                  // 0x00F4
	// float LastBodyDamagedAmount;               // 0x00F8
	// long  LastShieldDamagedTime;               // 0x00FC
	// long  LastBodyDamagedTime;                  // 0x00100
	// short ShieldsRechargeTime;                  // 0x00104
	// short DamageFlags;                           // 0x00106 //Some kind of hp related bitmask.
	// long  UnknownFlags;                        // 0x0108
	// long  ClusterPartition;                     // 0x010C // Continually counts up, and resumes even after object is destroyed and recreated (killed)
	// Yelo::datum_index some_obj_id2;                        // 0x0110 // garbage collection related?
	// Yelo::datum_index NextObjectId;                        // 0x0114
	// Yelo::datum_index CurrentWeapon;                        // 0x0118 // currently held weapon (if this is a player)
	// Yelo::datum_index VehicleWeapon;                        // 0x011C // or the thing that <this> is in (i.e. a weapon 'inside' a player, but only if it's wielded)
	// short SeatType;                              // 0x0120
	// short unknown16;                           // 0x0122
	// float ShieldsHit;                           // 0x0124 //Counts down from 1 after shields are hit (0 to 1)
	// float Shield2;                              // 0x0128 //With overshield it stays at 1 may be reason  over-shield drains. (0 to 1) [2nd function]
	// float FlashlightScale;                     // 0x012C //Intensity of flashlight as it turns on and off. (0 to 1) (On > 0) (Off = 0) [3rd function]
	// float ARFunction;                           // 0x0130
	// float Flashlight2;                           // 0x0134
	// long  unknown17[5];                        // 0x0138
	// Yelo::datum_index ObjectAttachmentId1;                  // 0x014C
	// Yelo::datum_index ObjectAttachmentId2;                  // 0x0150
	// long  zero[6];                              // 0x0154
	// Yelo::datum_index UnknownIdent3;                        // 0x016C
	// Yelo::datum_index CashedRndrIdx;                        // 0x0170
	// short
	// long  unknown18;                           // 0x0174
	// long  unknown19[4];                        // 0x0178
	// float unknown20[24];                        // 0x0188  // mostly 1.0, occasionally 0.

*/
#pragma pack(pop)
