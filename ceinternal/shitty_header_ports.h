#pragma once
/**
 * <GPLv3 License>
 */

#include "shitty_enums.h"
#include "ce_base_types.h"
#include "gamestate_headers/real_math.h"
#include "gamestate_headers/objects.h"

#pragma pack(push)
#pragma pack(1)

//struct s_hud_messaging { char unk[1160]; }; //1160 bytes  0x64E970
/*
struct s_network_game_player {
wchar_t name[12];			// 0x0
int16 primary_color_index;	// 0x18
int16 icon_index;			// 0x1A
sbyte machine_index;		// 0x1C
sbyte controller_index;		// 0x1D
// These values will be the same on the client as they are on the server, so
// we can use them safely for player operations in multiplayer code
sbyte team_index;			// 0x1E
sbyte player_list_index;	// 0x1F
}; static_assert( sizeof(s_network_game_player) == 0x20 );
*/

#pragma region player_specific

//s_players_globals_data
struct s_players_globals_data {
	long          unused_after_initialize_unk;                                                                // 0x0, initialized to NONE but that's all I can tell
	// the player_index for each local player
	datum_index   local_player_players[MAX_PLAYER_COUNT_LOCAL];        // 0x4
	// the object_index of each local player's dead unit (their old body)
	datum_index   local_player_dead_units[MAX_PLAYER_COUNT_LOCAL];    // 0x8
	__int16       local_player_count;                                                        // 0xC
	__int16       double_speed_ticks_remaining;                                                // 0xE
	bool          are_all_dead;                                                                // 0x10
	bool          input_disabled;                                                            // 0x11
	__int16       _bsp_switch_trigger_idx;                                                // 0x12
	__int16       respawn_failure;                                                            // 0x14
	bool          was_teleported;                                                            // 0x16, or respawned
	char          _unk_pad_char;
	//There's 16 players in a game.
	unsigned long combined_pvs[MAX_PLAYERS_IN_GAME];                        // 0x18 combined pvs of all players in the game
	//TODO: verify if combined_pvs_local is actually the same as regular combined_pvs - Maybe they're separate and synched via network?
	unsigned long combined_pvs_local[MAX_PLAYERS_IN_GAME];                    // 0x58 combined pvs of all local players
}; static_assert (sizeof (s_players_globals_data) == 0x10 + ( 0x4 * 2 * MAX_PLAYER_COUNT_LOCAL ) + ( 0x4 * 2 * MAX_PLAYERS_IN_GAME ), STATIC_ASSERT_FAIL);
INTELLISENSE_HACK(s_players_globals_data);

//s_player_control
struct s_player_control {
	datum_index         unit_index;                                                                      // 0x0
	unsigned long       control_flags;                                                                   // 0x4
	__int16             __pad_unk0; // unknown field                                                              // 0x8
	__int16             __pad_unk1; // unknown field                                                              // 0xA
	real_euler_angles2d desired_angles;                                                    // 0xC
	real_vector2d       throttle;                                                                  // 0x14
	float               primary_trigger;                                                                         // 0x1C
	__int16             weapon_index;                                                                          // 0x20
	__int16             grenade_index;                                                                         // 0x22
	__int16             zoom_level;                                                                            // 0x24
	signed char         weapon_swap_ticks;                        // unknown field                         // 0x26
	unsigned char       __cpad_unk;                             // unknown field                         // 0x27
	datum_index         target_object_index;                                                             // 0x28
	float               autoaim_level;                                                                           // 0x2C
	unsigned long       _unk_fld0_32; // unknown field                                                   // 0x30
	unsigned long       _unk_fld1_32;                                                                    // 0x34
	unsigned long       _unk_fld2_32; // unknown field                                                   // 0x38
	unsigned long       _unk_fld3_32; // unknown field                                                   // 0x3C
};  static_assert (sizeof (s_player_control) == 0x40, STATIC_ASSERT_FAIL);

struct s_player_control_globals_data {
	unsigned long action_flags[2]; // see "action_test" script functions
	unsigned long __pad_unk; // TODO: document the fields in the first 12 bytes of this struct
	unsigned long flags; // FLAG(0) = camera control

	s_player_control local_players[MAX_PLAYER_COUNT_LOCAL]; //0x10
}; static_assert (sizeof (s_player_control_globals_data) == ( 0x10 + sizeof (s_player_control) * MAX_PLAYER_COUNT_LOCAL ), STATIC_ASSERT_FAIL);
#pragma endregion

#pragma region weapon specific
struct s_weapon_datum_network_data {
	real_point3d  position;
	real_vector3d transitional_velocity;
	int           a;
	int           b;
	int           c; // not used in the update...probably a real_vector3d (angular_velocity?)
	short         magazine_rounds_totals[MAX_MAGAZINES_PER_WEAPON];
	real          age;
}; static_assert (sizeof (s_weapon_datum_network_data) == 0x2C, STATIC_ASSERT_FAIL);

INTELLISENSE_HACK(s_weapon_datum_network_data)
#pragma endregion

//Doesn't get synched.
struct s_animation_state {
	//TODO: Double check if these are signed..
	__int16 animation_index;
	__int16 frame_index;
}; static_assert (sizeof (s_animation_state) == 0x4, STATIC_ASSERT_FAIL);

struct s_scenario_location {
	long             leaf_index;
	__int16          cluster_index;
	unsigned __int16 __unk_type0;
}; static_assert (sizeof (s_scenario_location) == 0x8, STATIC_ASSERT_FAIL);

#pragma region structure data shit
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
}; static_assert (sizeof (s_item_data) == ( k_object_size_item - k_object_size_object ), STATIC_ASSERT_FAIL);
INTELLISENSE_HACK(s_item_data)

struct s_garbage_data {
	__int16 ticks_until_gc;
	__int16 PAD16;
	__int32 _unused[5];
}; static_assert (sizeof (s_garbage_data) == ( k_object_size_garbage - k_object_size_item ), STATIC_ASSERT_FAIL);
#pragma endregion

#pragma region generic object shit
// Object datums in the memory pool can have dynamically allocated nested-blocks.
// Due to the way the game implements this, OS can allocate a nested-block of its own in an object,
// and when the stock game uses a gamesave with an object like this, it will silently but safely
// operate on it, even when it comes time to delete the object from the pool.
// An object gets reallocated when a nested-block is requested (and the additional size is added to
// the object's overall pool block size)
struct s_object_header_block_reference {
	unsigned __int16 size;
	unsigned __int16 offset;
}; static_assert (sizeof (s_object_header_block_reference) == 0x4);

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
}; static_assert (sizeof (s_object_datum_network_delta_data) == 0x44, STATIC_ASSERT_FAIL);

struct s_object_datum_animation_data {
	datum_index       definition_index;    // 0xCC
	s_animation_state state;        // 0xD0
	__int16           interpolation_frame_index;// 0xD4
	__int16           interpolation_frame_count;// 0xD6
}; static_assert (sizeof (s_object_datum_animation_data) == 0xC, STATIC_ASSERT_FAIL);

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
}; static_assert (sizeof (s_object_datum_damage_data) == 0x30, STATIC_ASSERT_FAIL);

struct s_object_datum_attachments_data {
	attachment_type attached_types[MAX_ATTACHMENTS_PER_OBJECT];    // 0x144
	// game state datum_index
	// ie, if Attachments[x]'s definition (object_attachment_block[x]) says it is a 'cont'
	// then the datum_index is a contrail_data handle
	datum_index     attachment_indices[MAX_ATTACHMENTS_PER_OBJECT];            // 0x14C
	datum_index     first_widget_index;                                                                // 0x16C
}; static_assert (sizeof (s_object_datum_attachments_data) == 0x2C, STATIC_ASSERT_FAIL);

struct s_halo_pc_network {
	networked_datum  datum_role;                                                //0x0
	bool             unknown_type0;                                            //0x4
	bool             should_force_baseline_update;                            //0x5
	unsigned __int16 unknown_type1;                                            //0x6	//TODO: verify if this is padding or not
	signed long      network_time;                                            // 0x8
}; static_assert (sizeof (s_halo_pc_network) == 0xC, STATIC_ASSERT_FAIL);

struct s_weapon_data {
	struct s_trigger_state {
		sbyte                idle_time;                    // 0x0 used for determining when to fire next projectile (rounds per second)
		weapon_trigger_state state;
		short                time;
		long                 flags0;            // 0x4
		__int16              __unk_type_i16_0;                // 0x8 firing effect related
		__int16              __unk_type_i16_1;                // 0xA firing effect related
		__int16              __unk_type_i16_2;                // 0xC firing effect related
		short                rounds_since_last_tracer;
		float                rate_of_fire;                    // 0x10
		float                ejection_port_recovery_time;    // 0x14
		float                illumination_recovery_time;    // 0x18
		float                __unk_type_flt_0;    // 0x1C used in the calculation of projectile error angle
		datum_index          charging_effect_index;    // 0x20
		sbyte                network_delay_time;            // 0x24 hardedcoded to delay fire/reload by 10 frames in networked game
		byte                 __pad_byte_0;
		__int16              __pad_i16_0;
	}; static_assert (sizeof (s_trigger_state) == 0x28, STATIC_ASSERT_FAIL);

	// __, unk, pad, '?'
	//means IDK if its actually padding or there are values there. If there are, IDK their types (could be a boolean!)
	struct s_magazine_state {
		weapon_magazine_state state;
		__int16               reload_time_remaining;        // 0x2 in ticks
		__int16               reload_time;                    // 0x4 in ticks
		__int16               rounds_unloaded;                // 0x6
		__int16               rounds_loaded;                // 0x8
		__int16               rounds_left_to_recharge;        // 0xA number of rounds left to apply to rounds_loaded (based on tag's rounds_recharged)
		__int16               __UNKNOWN_TYPE0;                // 0xC I just know a WORD is here, may be an _enum
		__int16               __pad0; // ?
	}; static_assert (sizeof (s_magazine_state) == 0x10, STATIC_ASSERT_FAIL);

	struct s_start_reload_data {
		__int16 starting_total_rounds[MAX_MAGAZINES_PER_WEAPON];
		__int16 starting_loaded_rounds[MAX_MAGAZINES_PER_WEAPON];
	}; static_assert (sizeof (s_start_reload_data) == 0x8, STATIC_ASSERT_FAIL);

	struct s_first_16_bytes {
		unsigned long  flags;                            // 0x0
		unsigned short owner_unit_flags;                // 0x4
		__int16        __pad_i16_1;                    // 0x6
		real           primary_trigger;                // 0x8
		weapon_state   weapon_state;                    // 0xC	//weap state is supposed to be the size of a byte
		unsigned char  __pad_byte_1;                    // 0xD
		short          ready_time;                        // 0xE
	} first_16_bytes;
	static_assert (sizeof (s_first_16_bytes) == 0x10, STATIC_ASSERT_FAIL);
	struct s_2nd_16_bytes {
		real heat;                            // 0x0
		real age;                            // 0x4
		real illumination_fraction;            // 0x8
		real integrated_light_power;            // 0xC
	} second_16_bytes;
	static_assert (sizeof (s_first_16_bytes) == 0x10, STATIC_ASSERT_FAIL);

	struct s_3rd_16_bytes {
		int         __unused_pad0;                    // 0x0
		datum_index tracked_object_index;            // 0x4
		__int64     __pad64_0;                        // 0x8
	} third_16_bytes;
	STAT_ASSRT(s_3rd_16_bytes, 0x10);
	struct next_set {
		short   alt_shots_loaded;                // 0x0
		__int16 __pad_i16_2;                    // 0x2
	} first_4_bytes;
	STAT_ASSRT(next_set, 0x4);

	s_trigger_state triggers[2];                    // 0x34, size == 0x50

	//STAT_ASSRT(triggers, sizeof(s_trigger_state) * 2);
	//INTELLISENSE_HACK(triggers);


	s_magazine_state magazines[2];                    //0x84
	INTELLISENSE_HACK(magazines);

	long                        last_trigger_fire_time;                        // 0xA0
	s_start_reload_data         start_reload_update;            // 0xA4
	long                        __pad32_1;                        // 0xAC need to verify this is unused
	bool                        baseline_valid;                                // 0xB0
	sbyte                       baseline_index;                                // 0xB1
	sbyte                       message_index;                                // 0xB2
	byte                        __pad8_4;                                        // 0xB3
	s_weapon_datum_network_data update_baseline;        // 0xB4
	bool                        __unk_char_type;                                // 0xDC probably delta_valid
	char                        _pad_c_byte[3];                                    // 0xDD
	s_weapon_datum_network_data update_delta;            // 0xE0
}; ///This stupid structure EFFFFFFFFFFFFFF
//s_weapon_data)

//STAT_ASSRT(s_weapon_data, (k_object_size_weapon - k_object_size_item));
//static_assert(sizeof(s_weapon_data) == (), STATIC_ASSERT_FAIL);
/* OG struct:
 struct s_weapon_data {
			// FLAG(3) - _weapon_must_be_readied_bit
			long_flags flags;						// 0x22C
			word_flags owner_unit_flags;			// 0x230
			PAD16;
			real primary_trigger;					// 0x234
			Enums::weapon_state weapon_state;		// 0x238
			PAD8;
			int16 ready_time;						// 0x23A in ticks
			real heat;								// 0x23C
			real age;								// 0x240
			real illumination_fraction;				// 0x244
			real integrated_light_power;			// 0x248
			PAD32;									// 0x24C unused
			datum_index tracked_object_index;		// 0x250
			PAD64;									// 0x254 unused
			int16 alt_shots_loaded;					// 0x25C
			PAD16;									// 0x25E
			s_trigger_state triggers[Enums::k_maximum_number_of_triggers_per_weapon];	// 0x260
			s_magazine_state magazines[Enums::k_maximum_number_of_magazines_per_weapon];// 0x2B0
			int32 last_trigger_fire_time;			// 0x2D0
			s_start_reload_data start_reload_update;// 0x2D4
			PAD32; // 0x2DC need to verify this is unused
			bool baseline_valid;					// 0x2E0
			sbyte baseline_index;					// 0x2E1
			sbyte message_index;					// 0x2E2
			PAD8;									// 0x2E3
			s_weapon_datum_network_data update_baseline;	// 0x2E4
			UNKNOWN_TYPE(bool);						// 0x310 probably delta_valid
			PAD24;									// 0x311
			s_weapon_datum_network_data update_delta;		// 0x314
		}; static_assert(sizeof(s_weapon_data) == (Enums::k_object_size_weapon - Enums::k_object_size_item), STATIC_ASSERT_FAIL);
 */


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

static_assert (sizeof (s_object_data) == object_sizes::k_object_size_object, STATIC_ASSERT_FAIL);
#pragma endregion

#pragma region item_datums
struct s_object_datum {
	enum {
		k_object_types_mask = object_type::_object_type_object
	};
	s_object_data object;
};

static_assert (sizeof (s_object_datum) == object_sizes::k_object_size_object, STATIC_ASSERT_FAIL);

struct s_item_datum {
	enum {
		k_object_types_mask = object_type::_object_type_mask_item
	};

	s_object_data object;
	s_item_data   item;
}; static_assert (sizeof (s_item_datum) == object_sizes::k_object_size_item, STATIC_ASSERT_FAIL);

struct s_garbage_datum : s_item_datum {
	enum {
		k_object_types_mask = FLAG(_object_type_garbage)
	};

	s_garbage_data garbage;
}; static_assert (sizeof (s_garbage_datum) == k_object_size_garbage, STATIC_ASSERT_FAIL);

struct s_weapon_datum : s_item_datum {
	enum {
		k_object_types_mask = FLAG(_object_type_weapon)
	};
	s_weapon_data weapon;
}; //static_assert(sizeof(s_weapon_datum) == k_object_size_weapon, STATIC_ASSERT_FAIL);
//HOLY MOLEY
INTELLISENSE_HACK(s_weapon_datum);
INTELLISENSE_HACK(k_object_size_weapon);
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
	}; static_assert (sizeof (magic) == 0x4, STATIC_ASSERT_FAIL);

}; //static_assert(sizeof(s_object_header_datum) == 0xC, STATIC_ASSERT_FAIL);
#pragma endregion


/*
struct s_render_camera
{
	real_point3d point;
	real_vector3d forward;
	real_vector3d up;

	UNKNOWN_TYPE(bool); // controls the rendering of stuff related to certain objects
	PAD24;
	real vertical_field_of_view;
	rectangle2d viewport_bounds;
	point2d window_bounds;
	real z_near, z_far;
	int __PAD32[4]; // just looks like 16 bytes of unused poop
}; BOOST_STATIC_ASSERT(sizeof(s_render_camera) == 0x54)

struct s_render_window
{
	int16 local_player_index;
	UNKNOWN_TYPE(bool);
	PAD8;
	s_render_camera render_camera, rasterizer_camera;
}; static_assert(sizeof(s_render_window) == 0xAC);
s_render_window* RenderWindow(); // defined in the implementing extension's code

struct s_render_fog
{
	word_flags flags; // same flags as in the tag definition
	PAD16;
	real_rgb_color atmospheric_color;
	real atmospheric_maximum_density,
		atmospheric_minimum_distance,
		atmospheric_maximum_distance;
	UNKNOWN_TYPE(int16);
	PAD16;
	real_plane3d plane; // copied from the bsp's fog planes
	real_rgb_color color; // copied from tag definition
	real max_density; // copied from tag definition
	real planar_maximum_distance;
	real opaque_depth; // copied from tag definition
	const void* screen_layers_definition; // pointer to the tag definition data
	UNKNOWN_TYPE(real);
}; static_assert(sizeof(s_render_fog) == 0x50);

struct s_render_globals
{
	int		__iUnk0;
	int		__iUnk1;
	short local_player_index;
	short __sUnk0;
	long __lUnk0;
	real __rUnk0;
	byte camera[];
	s_render_frustum frustum;
	s_render_fog fog;
	int leaf_index;
	int cluster_index;
	char pad; // probably an unused bool
	bool visible_sky_model;
	short visible_sky_index;

	struct {
		long cluster_pvs[512];
		byte clusters[0x1A0][k_maximum_rendered_clusters];
		short count;
		short pad;
	}rendered_clusters;

	struct {
		unsigned int visibility_bitvector[0x20000];
		int count;
		unsigned int triangles[k_maximum_rendered_triangles];
	}rendered_triangles;
}; static_assert(sizeof(s_render_globals) == 0x9D298);
//s_render_globals* RenderGlobals(); // defined in the implementing extension's code
*/


struct s_structure_render_globals {
	bool          render_bsp;
	char          pad[3];                        //0x4
	int           dynamic_triangle_buffer_index;  //0x1C
	char          __UNKNOWN_TYPE;                //0x20
	char          pad1[3];
	real_vector3d __guessed_type;
}; //static_assert(sizeof(s_structure_render_globals) == 0x18);
//s_structure_render_globals* StructureRenderGlobals(); // defined in the implementing extension's code
#pragma region game state related
namespace game_options {
	struct s_game_options {
		int                   unk_type; // never see this referenced or explicitly set. due to memset, this will always be set to zero
		short                 unk_2; // never see this referenced besides in game_options_new code where it's set to 0, even after a memset call. highly likely to be an enum field, albeit unused
		game_difficulty_level difficulty_level;
		int                   game_random_seed;
		// not always the actual path, sometimes just the name, go figure
		char                  map_name[255 + 1];
	}; STAT_ASSRT(s_game_options, 0x10C);

	struct s_game_globals {
		bool           map_loaded;
		bool           active;
		bool           players_are_double_speed;
		bool           map_loading_in_progress;
		real           map_loading_precentage;
		s_game_options options;
	}; STAT_ASSRT(s_game_globals, 0x114);
};



/*
struct s_main_globals
{
	UNKNOWN_TYPE(uint32); // time related
	PAD32;
	LARGE_INTEGER performance_counter;
	UNKNOWN_TYPE(bool);
	bool is_taking_screenshot;
	PAD16;
	real delta_time;
	Enums::game_connection game_connection;

	struct s_screenshot {
		int16 counter;
		void* movie; // screenshot bitmap
		PAD32; PAD32;
		int32 movie_frame_index;
		UNKNOWN_TYPE(real);
	}screenshot;

	struct s_map {
		bool reset_map;
		bool switch_to_campaign;
		bool revert_map;
		bool skip_cinematic;
		bool save_map;
		bool save_map_nonsafe;
		bool save_map_with_timeout;
		bool is_saving_map;
		int32 saving_map_timeout_countdown;
		int32 saving_map_timeout_timer;
		UNKNOWN_TYPE(int32);
		UNKNOWN_TYPE(int16);
		bool won_map;
		bool lost_map;
		bool respawn_coop_players;

		struct s_core {
			bool save;
			bool load;
			bool load_at_startup;
		}core;

		int16 switch_to_structure_bsp; // if not NONE, switches to the scenario's bsp by index
		bool main_menu_scenario_loaded;
		bool main_menu_scenario_load;
	}map;

	UNUSED_TYPE(bool);
	UNKNOWN_TYPE(bool);
	UNKNOWN_TYPE(bool);
	bool quit;
	UNKNOWN_TYPE(int32);
	UNKNOWN_TYPE(int32);
	UNKNOWN_TYPE(int32);
	bool set_game_connection_to_film_playback;
	bool time_is_stopped;
	bool start_time;
	UNUSED_TYPE(bool);

	bool skip_frames;
	PAD8;
	int16 skip_frames_count;

	int16 lost_map_count;
	int16 respawn_count;

	UNKNOWN_TYPE(bool);
	PAD24;
	UNKNOWN_TYPE(bool);
	char scenario_tag_path[256];
	char multiplayer_map_name[256];
	char queued_map[256];

	UNKNOWN_TYPE(bool);
	UNKNOWN_TYPE(tag_string);
	PAD(0, 8 + 1); // char[8+1]
	PAD8;
	PAD32;


};
*/
//struct s_game_globals {
//	enum {
//		k_group_tag = 'matg'
//	};
//
//	long language;
//
//	byte bytePad[244];
//
//	tag_reference sounds;
//	tag_reference camera;
//
//	tag_block<s_player_control_globals_data> game_globals_player_control; //
//
//	tag_block<> difficulty_info; //, s_game_globals_difficulty_information);
//	tag_block grenades;  //s_game_globals_grenade);
//
//	tag_block game_globals_rasterizer_data; // s_game_globals_rasterizer_data
//	tag_block game_globals_interface_tag_references; // s_game_globals_interface_tag_references
//
//	tag_block weapons_list; // s_game_globals_tag_reference;
//	tag_block cheat_powerups; // s_game_globals_tag_reference);
//	tag_block multiplayer_info; // s_game_globals_multiplayer_information);
//	tag_block player_info; // s_game_globals_player_information);
//	tag_block player_representation; // s_game_globals_player_representation);
//	tag_block falling_damage; // s_game_globals_falling_damage);
//	tag_block materials; //material_definition
//	tag_block playlist_members;
//	tag_block playlist_autogenerate_choice;
//};

#pragma endregion
#pragma pack(pop)
