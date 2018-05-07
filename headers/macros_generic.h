/**
 * GPLv3
 */
#pragma once
#ifndef shitty_macros
#define shitty_macros
//WARNING: Please avoid nesting macros/typedefs, that breaks intellisense 9/10 times.
#pragma region sanity checks
#define STATIC_ASSERT_FAIL         "Static Assert Failure!"
#define STAT_ASSRT(object, size)  static_assert(sizeof(object) == size, STATIC_ASSERT_FAIL)

typedef float         real;
typedef int           int32;
typedef signed char   sbyte;
typedef long          byte_swap_code_t;
typedef unsigned char byte;
typedef unsigned int  *uintptr;
typedef unsigned int  uint32;

STAT_ASSRT(bool, 0x1);
STAT_ASSRT(char, 0x1);
STAT_ASSRT(short, 0x2);
STAT_ASSRT(__int16, 0x2);
STAT_ASSRT(__int32, 0x4);
STAT_ASSRT(long, 0x4);
STAT_ASSRT(float, 0x4);
STAT_ASSRT(long long, 0x8);
STAT_ASSRT(double, 0x8);
#pragma endregion
#define INTELLISENSE_HACK(item) //int a[sizeof(item)] = 0;

#define INVALID -1

#pragma region functional
#define FLAG(bit)               ( 1<<(bit) )
#define UNK_TYPE(type) pad_##type
#define PAD8(name) char name;
#define PAD24(name) unsigned char name[24];

#pragma region pad/unknown/unused macros
// Pad a structure using a byte[] field named pad[num] by [count] bytes
#define PAD(num, count) byte pad##num##[ count ]
// Pad a structure using a byte[] field named pad_[name] by [count] bytes
#define PAD_(name, count) byte pad_##name##[ count ]
// Markup a field as unknown
#define UNKNOWN(num) Unknown##num

/// Add an anonymous 8-bit (1 byte) field to a structure.
#define PAD8 unsigned char : 8;
/// Add an anonymous 16-bit (2 byte) field to a structure.
#define PAD16 unsigned short : 16;
/// Add an anonymous 24-bit field to a structure.
//#define PAD24 unsigned char : 8; unsigned short : 16;
/// Add an anonymous 32-bit (4 byte) field to a structure.
#define PAD32 unsigned long : 32;
/// Add an anonymous 48-bit field to a structure.
#define PAD48 unsigned short : 16; unsigned long : 32;
/// Add an anonymous 64-bit (8 byte) field to a structure.
#define PAD64 unsigned __int64 : 64;
/// Add an anonymous 128-bit (16 byte) field to a structure.
#define PAD128 unsigned __int64 : 64; unsigned __int64 : 64;

// Add a field to a structure that pads as if it were of type [type]
#define PAD_TYPE(type) pad_##type
// Add a field to a structure that markups what is thought to be a field of type [type]
#define UNKNOWN_TYPE(type) pad_##type
// Add a field to a structure that markups a unused field of type [type]
#define UNUSED_32(type) unsigned long : 32;

#pragma endregion

#pragma endregion

#pragma region maximums
#ifndef MAX_PLAYER_COUNT_LOCAL
#define MAX_PLAYER_COUNT_LOCAL      1
#endif
#ifndef MAX_PLAYERS_IN_GAME
#define MAX_PLAYERS_IN_GAME         16
#endif
#ifndef MAX_ATTACHMENTS_PER_OBJECT
#define MAX_ATTACHMENTS_PER_OBJECT  8
#endif
#define MAX_MAGAZINES_PER_WEAPON   2
#define MAX_CUSTOM_BLIPS         16
#pragma region models
#define   k_maximum_regions_per_model            8
#define   k_maximum_nodes_per_model            64
#define   k_maximum_nodes_per_model_halo_stock   44 // 'Cause Xbox1 has limits
#define   k_maximum_nodes_per_model_geometry_part 22
#define   k_number_of_rows_per_node_matrix      4

#define k_maximum_rendered_objects            256

#define k_maximum_rendered_clusters            0x80

#define k_maximum_rendered_triangles         0x4000
#define k_maximum_object_names_per_scenario      512
#define k_maximum_rendered_distant_lights      2
#define k_maximum_rendered_point_lights         2
#pragma endregion
#pragma endregion

#define NONE                  -1

//TODO: BEGONE WITH THIS SHIT. :)
#pragma region addresses
#define K_INPUT_UPDATE_HOOK            0x493856
#define K_INPUT_STATE_FLAGS         0x6AD572
#define K_HUD_SCRIPTED_GLOBALS         0x6B44A8
#define K_HUD_MESSAGING            0x64E970
#define K_HUD_UNIT_INTERFACE         0x6B44BC
#define K_HUD_WEAPON_INTERFACE         0x6B44C0
#define K_HUD_NAV_POINTS            0x64E974
#define K_MOTION_SENSOR            0x6B44C8
#define K_FIRST_PERSON_WEAPONS         0x64DCC8
#define K_HUD_UPDATE               0x512360
#define K_HUD_UPDATE_CALL            0x45C1A2
#define K_FIRST_PERSON_WEAPONS_UPDATE   0x45AFA0
#define K_HUD_RENDER_NAV_POINTS      0x4B2F80
#define K_HUD_RENDER_PLAYERS         0x4AD830
#define K_HUD_RENDER_PLAYERS_CALL      0x4ACA6E
#define K_HUD_GLOBALS               0x6B44A4

#define PLAYERS_INITIALIZE_FOR_NEW_MAP  0x476230
#pragma endregion
#endif
