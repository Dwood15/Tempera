#pragma once

#include "map.h"
#include "../math/colors.h"
#include "../memory/datum_index.h"

using namespace Yelo;
struct reflexive {
	unsigned long Count;
	void          * Offset;
	unsigned long Zeros;
};

struct tag_name {
	char Name[1];
};

struct tag_header {
	unsigned long TagClass[3];
	datum_index         h_tag_idx;
	tag_name* TName;
	void * Offset;
	long Zeros[2];
};

struct tag_index_header {
	tag_header    * FirstTag;
	datum_index         firsttag;
	long          Unknown;
	unsigned long TagCount;
	unsigned long VertexObjectCount;
	unsigned long VertexOffset;
	unsigned long IndicesObjectCount;
	unsigned long IndicesOffest;
	unsigned long ModelRawDataSize;
	char          Tags[4]; // 'sgat' translates to 'tags'
};

// Reference to a tag. Found all over the map file
struct tagref {
	unsigned long Tag;
	tag_name* TName;
	long  Unknown;
	datum_index tag_ref;
};

// ************************
//      Tag Structures
// ************************
// unknown = cont, foot, str#

struct actor_variant // 'actv'
{
	unsigned long flags;
	tagref        actor_definition;
	tagref        unit;
	tagref        major_variant;

	// movement switching
	long zero1[12];

	// ranged combat
	tagref weapon;
	float  maximum_firing_distance;
	float  rate_of_fire;
	float  projectile_error;
	float  first_burst_delay_time_low;
	float  first_burst_delay_time_high;
	float  new_target_firing_pattern_time;
	float  suprise_delay_time;
	float  suprise_fire_wildly_time;
	float  death_fire_wildly_chance;
	float  death_fire_wildly_time;
	float  desired_combat_range_low;
	float  desired_combat_range_high;
	vect3  custom_stand_gun_offset;
	vect3  custom_crouch_gun_offset;
	float  target_tracking;
	float  target_leading;
	float  weapon_damage_modifier;
	float  damage_per_second;

	// burst geometry
	float burst_origin_radius;
	float burst_origin_angle;
	float burst_return_length_low;
	float burst_return_length_high;
	float burst_return_angle;
	float burst_duration_low;
	float burst_duration_high;
	float burst_separation_low;
	float burst_separation_high;
	float burst_angular_velocity;
	float special_damage_modifier;
	float special_projectile_error;

	// firing patterns
	float     new_target_burst_duration;
	long      zero6[31];
	tagref    ojpr;
	long      zero7[12];
	tagref    equip;
	long      zero8[26];
	tagref    wphi;
	long      zero9[29];
	reflexive unkref;
	long      zero10[3];
	long      unknown5;
	long      zero11[55];
};

struct animation_trigger // 'antr'
{
	reflexive objects;
	long      zero[26];
	reflexive animations;
};

struct antenna // 'ant!'
{
	char      attachment_marker_name[32]; // antenna
	tagref    bitmaps;
	tagref    physics;
	long      zero[20];
	float     sprint_strength_coefficient;
	float     falloff_pixels;
	float     cutoff_pixels;
	float     unknown;
	long      zero1[9];
	reflexive antenna_vertices;
};

struct biped // 'bipd'
{
	unsigned long flags; // bitfield
	float         boundingRadius;
	vect3         boundingOffset;
	vect3         originOffset;
	float         accelerationScale;
	long          unknown;
	tagref        model;
	tagref        animation_trigger;
	long          zero2[10];
	tagref        collision_model;
	tagref        physics;
	tagref        shader_plasma;
	tagref        creation_effect;
	long          zero3[21];
	float         unknown3;
	short         unknown4[2];
	long          zero4[13];
	reflexive     unkref;
	long          zero5[3];
	reflexive     unkref1;
	reflexive     unkref2;
	reflexive     unkref3;
	short         unknown5[2];
	long          unknown6;
	long          zero6;
	tagref        effe1;
	long          zero7;
	long          unknown7;
	float         unknown8;
	long          zero8[20];
	reflexive     unkref4;
	long          zero9[7];
	float         unknown9[2];
	long          zero10[10];
	tagref        actv;
	long          zero11[11];
	tagref        jpt;
	long          zero12[4];
	reflexive     unkref5;
	reflexive     unkref6;
	float         unknown10;
	short         unknown11[4];
	//long zero


	// TODO: Finish this struct.
};

struct bitmap // 'bitm'
{
	short     unknown[4];
	long      zero[4];
	short     unknown1[2];
	long      offset;
	long      zero1;
	long      unknown2;
	long      zero2[4];
	long      offset2;
	long      zero3[6];
	reflexive UnkRef;
	reflexive UnkRef1;
};

struct collision_model // 'coll'
{
	long      zero[28];
	tagref    effe;
	long      zero1;
	tagref    effe1;
	long      zero2;
	tagref    effe2;
	tagref    effe3;
	long      zero3;
	tagref    effe4;
	long      zero4[47];
	tagref    effe5;
	tagref    effe6;
	tagref    effe7;
	long      zero5[2];
	float     unknown;
	long      zero6[28];
	reflexive unkref;
	reflexive unkref1;
	long      zero7[13];
	reflexive unkref2;
	reflexive unkref3;
};

struct damage // 'jpt!'
{
};

struct decal // 'deca'
{
};

struct dialog // 'udlg'
{
};

struct effect // 'effe'
{
};

struct equipment // 'eqip'
{
};

struct font // 'font'
{
};

struct game_globals // 'matg'
{
};

struct grenade_hud_interface // 'grhi'
{
};

struct hud_globals // 'hudg'
{
};

struct hud_message_text // 'hmt '
{
};

struct hud_number // 'hud#'
{
};

struct item_collection // 'itmc'
{
};

struct lens // 'lens'
{
	float         falloff_angle;
	float         cutoff_angle;
	float         unknown[2];
	float         occlusion_radius;
	float         occlusion_offset_direction;
	float         near_fade_distance;
	float         far_fade_distance;
	tagref        bitm;
	long          zero2[20];
	unsigned long rotation_function;
	float         rotation_function_scale;
	long          zero3[6];
	float         unknown3[2];
	long          zero4[7];
	reflexive     UnkRef;
	long          unknown4[8];
};

struct light // 'ligh'
{
};

struct light_volume // 'mgs2'
{
};

struct looping_sound // 'lsnd'
{
};

struct meter // 'metr'
{
};

struct model // 'mod2'
{
	unsigned long flags;
	unsigned long node_list_checksum;
	unsigned long superhigh_detail_cutoff;
	unsigned long high_detail_cutoff;
	unsigned long medium_detail_cutoff;
	unsigned long low_detail_cutoff;
	unsigned long superlow_detail_cutoff;
	unsigned long superhigh_detail_node_count;
	unsigned long high_detail_node_count;
	unsigned long medium_detail_node_count;
	unsigned long low_detail_node_count;
	unsigned long superlow_detail_node_count;
	float         base_map_u_scale;
	float         base_map_v_scale;
	unsigned long zero[32];
	reflexive     nodes;
	reflexive     regions;
	reflexive     geometries;
	reflexive     shaders;
};

struct particle // 'part'
{
};

struct particle_system // 'pctl'
{
};

struct physics // 'phys'
{
};

struct point_physics // 'pphy'
{
};

struct projectile // 'proj'
{
};

struct scenery // 'scen'
{
	long      unknown;
	float     unknown1;
	long      zero[8];
	tagref    mod2;
	tagref    antr;
	long      zero1[10];
	tagref    coll;
	tagref    phys;
	tagref    shdr;
	tagref    effe;
	long      zero2[21];
	float     unknown2;
	long      zero3[26];
	reflexive UnkRef;
	long      unknown3[32];
};

struct shader_environment // 'senv'
{
};

struct shader_glass // 'sgla'
{
};

struct shader_metal // 'smet'
{
};

struct shader_model // 'soso'
{
};

struct shader_plasma // 'spla'
{
};

struct shader_transparancy_variant // 'schi'
{
	long      zero[9];
	long      unknown;
	long      zero2[4];
	tagref    lens;
	long      zero3[3];
	reflexive bitm;
	long      zero4[3];
};

struct sky // 'sky '
{
	tagref    mod2;
	tagref    antr;
	long      zero[6];
	float     unknown[11];
	long      zero2[2];
	float     unknown2[6];
	long      zero3[2];
	float     unknown3[3];
	tagref    fog;
	long      zero5[7];
	reflexive UnkRef;
};

struct sound // 'snd!'
{
};

struct sound_environment // 'snde'
{
};

struct sound_scenery // 'ssce'
{
};

struct structure_bsp // 'sbsp'
{
};

struct track // 'trak'
{
};

struct unicode_string // 'ustr'
{
};

struct unit_hud_interface // 'unhi'
{
};

struct unknown_pc_shader // 'scex'
{
	long      zero[9];
	long      unknown;
	long      zero1[4];
	tagref    lens;
	long      zero2[3];
	reflexive UnkRef;
	reflexive UnkRef1;
	long      zero3[3];
};

struct vehicle // 'vehi'
{
};

struct weapon // 'weap'
{
};

struct weapon_hud_interface // 'wphi'
{
};

struct wind // 'wind'
{
};

// To be continued...
