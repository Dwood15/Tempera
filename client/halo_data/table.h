#pragma once

#include <stdint.h>
#include "../../math/data_types.h"
#include "tag_data.h"

#define HALO_NAME_LENGTH 12
#define NULL_ID 0xFFFFFFFF

/// This is an array of data from Halo for movement
struct MovementInfo {
    uint8_t jump; //0x0
    uint8_t change_grenades;
    uint8_t action;
    uint8_t change_weapons;
    uint8_t melee; //0x4
    uint8_t flashlight;
    uint8_t secondary_fire;
    uint8_t primary_fire;
    uint8_t unknown_1; //0x8
    uint8_t unknown_2;
    uint8_t crouch;
    uint8_t zoom;
    uint8_t score_screen; //0xC
    uint8_t reload;
    uint8_t exchange_weapons;
    uint8_t chat;
    uint8_t unused_1[4]; //0x10
    uint8_t unused_2[4]; //0x14
    uint8_t unused_3[3]; //0x18
    uint8_t game_info; //0x1B
    uint8_t show_player_names; //0x1C
    uint8_t unused_4[3]; //0x1D
    float move_up; //0x20
    float move_side; //0x24
    float aim_horizontal; //0x28
    float aim_vertical; //0x2C
    float custom_move_up; // 0x30 - custom by chimera
    float custom_move_side; // 0x34 - custom by chimera
    float custom_look_horizontal; // 0x38 - custom by chimera
    float custom_look_vertical; // 0x3C - custom by chimera
};

/// Retrieve the movement info.
MovementInfo &get_movement_info() noexcept;

/// Halo's tables are mapped out like this.
struct GenericTable {
    char name[0x20];
    uint16_t max_count;
    uint16_t index_size;
    uint32_t one;
    uint32_t data_v;
    uint16_t zero;
    uint16_t size;
    uint16_t count;
    uint16_t next_id;
    void *first;
};

/// Return a reference to the object table.
GenericTable &get_object_table() noexcept;

/// Return a reference to the player table.
GenericTable &get_player_table() noexcept;

/// Return a reference to the particle table.
GenericTable &get_particle_table() noexcept;

/// Return a reference to the antenna table.
GenericTable &get_antenna_table() noexcept;

/// Return a reference to the flag cloth table.
GenericTable &get_flag_table() noexcept;

/// Return a reference to the light table.
GenericTable &get_light_table() noexcept;

/// Return a reference to the effect table.
GenericTable &get_effect_table() noexcept;

/// Return a reference to the decal table.
GenericTable &get_decal_table() noexcept;

/// Get the client player index.
uint32_t client_player_index() noexcept;

/// Check if two wide strings are equal, optionally up to a maximum length.
bool unichar_equal(const short *a, const short *b, size_t size = SIZE_MAX) noexcept;

/// Check if a char string is equal to a wide string, optionally up to a maximum length.
bool unichar_equal(const char *a, const short *b, size_t size = SIZE_MAX) noexcept;

class HaloPlayer {
private:
    uint32_t player_index;
public:
    uint32_t index() noexcept;
    uint32_t object_id() noexcept;
    char *player_data() noexcept;
    HaloPlayer(uint32_t player_index=client_player_index()) noexcept;
    HaloPlayer(const short *name) noexcept;
    HaloPlayer(const char *name) noexcept;
};

struct BaseHaloObject {
    /// This is the tag ID of the object.
    HaloTagID tag_id;
    PAD_CHARS(8);
    /// This is the length of time the object has existed. (ticks)
    uint32_t existence_time;
    /// If 1, the object is invisible and has no collision.
    uint32_t phased_out : 1;
    /// If 1, the object is on the ground and is not affected by gravity.
    uint32_t on_ground : 1;
    /// If 1, the object ignores gravity. (untested)
    uint32_t ignore_gravity : 1;
    uint32_t in_water : 1;
    PAD_BITS(uint32_t, 1);
    /// If 1, the object is stationary.
    uint32_t stationary : 1;
    PAD_BITS(uint32_t, 1);
    uint32_t no_collision2 : 1;
    PAD_BITS(uint32_t, 10);
    /// If 1, the object does not cast any shadow. Shadow count is subject to a limit.
    uint32_t does_not_cast_shadow : 1;
    PAD_BITS(uint32_t, 2);
    /// If 1, the object is outside of the map.
    uint32_t outside_of_map : 1;
    PAD_BITS(uint32_t, 2);
    /// If 1, objects cannot phase through this object. (untested)
    uint32_t object_is_collideable : 1;
    PAD_BITS(uint32_t, 7);
    uint32_t marker_id;
    PAD_CHARS(0x44);
    Vector3D position;
    Vector3D velocity;
    Vector3D orientation[2];
    Euler3DPYR rotation_velocity;
    uint32_t location_id;
    PAD_CHARS(4);
    Vector3D position_script;
    float radius;
    float scale;
    uint16_t object_type;
    PAD_CHARS(2);
    uint32_t game_objective;
    uint32_t namelist_index;
    PAD_CHARS(4);
    uint32_t player_id;
    uint32_t owner_object_id;
    PAD_CHARS(4);
    uint16_t animation_state;
    PAD_CHARS(6);
    /// This is the maximum health hitpoints. This is typically equal to the value in the collision model.
    float max_health;
    /// This is the maximum shield hitpoints. This is typically equal to the value in the collision model.
    float max_shields;
    float health;
    float shield;
    float current_shield_damage;
    float current_body_damage;
    PAD_CHARS(4);
    float last_shield_damage_amount;
    float last_body_damge_amount;
    uint32_t last_shield_damage_time;
    uint32_t last_body_damage_time;
    uint16_t shields_recharge_time;
    PAD_CHARS(6);
    uint32_t cluster_partition_index;
    PAD_CHARS(8);
    uint32_t weapon_object_id;
    uint32_t parent_object_id;
    uint16_t vehicle_seat;
    PAD_CHARS(2);
    float shields_hit;
    float shields_target;
    float flashlight_scale;
    float assault_rifle_function;
    float export_function_1;
    float flashlight_scale2;
    float shields_hit2;
    float export_function4;
    char attachment_type[8];
    uint32_t attachment_id[8];
    uint32_t first_widget_id;
    uint32_t cached_render_state_index;
    PAD_CHARS(2);
    uint16_t shader_permutation;
    char health_region[8];
    char region_permutation_index[8];
    ColorRGB color1[4];
    ColorRGB color2[4];
    struct s_header_block_ref_node_orientation {
        uint16_t size;
        uint16_t offset;
    } header_block_ref_node_orientation[3];
};

class HaloObject {
private:
    uint32_t object_index;
public:
    uint32_t index() noexcept;
    char *object_data() noexcept;
    HaloObject(uint32_t object_index) noexcept;
};

enum CameraType {
    CAMERA_FIRST_PERSON = 0x0,
    CAMERA_VEHICLE = 0x1,
    CAMERA_CINEMATIC = 0x2,
    CAMERA_DEBUG = 0x3
};
CameraType get_camera_type() noexcept;
