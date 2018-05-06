/*
	Project: haloforge
	File: objects.h
	Copyright � 2009 SilentK, Abyll

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.

*/

// Objects include bipeds, scenery, weapons, vehicles, powerups, projectiles, etc

#pragma once

#include <windows.h>
#include "../../gamestate/halo_types.h"
#include "core.h"
#include "math.h"
#include "addlog.h"
#include "../../gamestate/objects.h"

struct object_header;
struct object_data;
struct biped_data;

struct object_header {
	short         object_id;
	// x41: held by player (on back),
	// xE2: wielded by a player (held)
	// x63: on it's own (like on the ground - even counts for player bipeds)
	unsigned char flags;
	unsigned char object_type;
	short         sector;   // portal rendering ( cluster index )
	short         size;     // Structure size
	object_data   *address;

	void DumpData(bool toConsole = false) {
		Print(toConsole, "Object Header!\n");
		Print(toConsole, "ID: %d\n", object_id);
		Print(toConsole, "Flags: %x\n", flags);
		Print(toConsole, "Object Type: %x\n", object_type);
		Print(toConsole, "Cluster Index: %d\n", sector);
		Print(toConsole, "Object Size: %d\n", size);
		Print(toConsole, "Data's Address: %x\n", (unsigned int) address);
	}
};

struct object_data {
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


	ident                             Meta;                                            // 0x0
	s_halo_pc_network                 PcNetworkData;                                                // 0x4
	unsigned long                     Flags;                                                        // 0x10 TODO: Document.
	signed long                       ObjectMarkerId;                                            // 0x14
	s_object_datum_network_delta_data NetworkDeltaData;                                    // 0x18 // Added in HaloPC
	vect3                             World;                                                    // 0x5C
	vect3                             Velocity;                                        // 0x68
	vect3                             Forward;                                                    // 0x74
	vect3                             Up;                                                            // 0x80
	vect3                             AngularVelocity;                                            // 0x8C
	s_scenario_location               Location;                                                    // 0x98
	vect3                             Center;                                                        // 0xA0
	float                             Radius;                                                        // 0xBC
	float                             Scale;                                                        // 0xB0
	__int16                           type;                                                        // 0xB4
	PAD16;
	game_team OwnerTeam;                                                    // 0xB8
	__int16   NameListIdx;                                                            // 0xBA
	// ticks spent not at_rest. only biped updates this
	__int16   MovingTime;                                                                // 0xBC
	__int16   RegionPermutation;                                                        // 0xBE, variant id
	ident     Player;                                                        // 0xC0
	// If this were a projectile, this might be the handle to the weapon which spawned it
	ident     owner_object_index;                                                    // 0xC4
	PAD32;                                                                          // 0xC8 unused
	s_object_datum_animation_data Animation;                                        // 0xCC
	s_object_datum_damage_data    Damage;                                            // 0xD8
	PAD32;                                                 // 0x108 unused
	ident       cluster_partition_index;                                // 0x10C
	ident       garbage_collection_object_index_rltd;                    // 0x110, object_index, garbage collection related
	datum_index next_object_index;                                        // 0x114
	ident       first_object_index;                                        // 0x118
	ident       parent_object_index;                                    // 0x11C
	signed char parent_node_index;                                                        // 0x120
	char        unused_byte_unk;                                                                // 0x121 idk if this is an sbyte or bool here
	bool        force_shield_update;                                                        // 0x122
	signed char valid_outgoing_functions;                                            // 0x123, 1<<function_index
	float       incoming_function_values[k_number_of_incoming_object_functions];    // 0x124
	float       outgoing_function_values[k_number_of_outgoing_object_functions];    // 0x134

	s_object_datum_attachments_data attachments;                                    // 0x144
	datum_index                     CachedRenderStateIdx;                                            // 0x170
	unsigned __int16                RegionsDestroyedFlags;                                                // 0x174 once a region is destroyed, its bit index is set
	signed __int16                  ShaderPermutation;                                                        // 0x176, shader's bitmap block index
	unsigned char                   RegionBvitality[k_maximum_regions_per_model];                        // 0x178
	signed char                     RegionPermutationsIndices[k_maximum_regions_per_model];            // 0x180

	real_rgb_color change_colors[k_number_of_object_change_colors];            // 0x188
	real_rgb_color change_colors2[k_number_of_object_change_colors];            // 0x1B8

	// one of these are for interpolating
	s_object_header_block_reference node_orientations;                                // 0x1E8 real_orientation3d[node_count]
	s_object_header_block_reference node_orientations2;                                // 0x1EC real_orientation3d[node_count]
	s_object_header_block_reference node_matrix_block;                                // 0x1F0 real_matrix4x3[node_count]

	bool IsPlayer() { return (Player.id > 0 || Player.index > 0); }

	void DumpData(bool toConsole = false) {
		if (!IsPlayer()) {
			return;
		}

		Print(toConsole, "Object Data!\n");
		Print(toConsole, "Begin Address: %x, End address: 0x%x\n", this, (((unsigned int) this) + sizeof(object_data)));
		Print(toConsole, "Object Role: %x\n", PcNetworkData.Role);
		this->World.DumpData(toConsole, "World Position: ");
		this->Velocity.DumpData(toConsole, "Velocity: ");
		this->Forward.DumpData(toConsole, "Lower Rotation: ");
		this->Up.DumpData(toConsole, "Up: ");
		this->AngularVelocity.DumpData(toConsole, "Unk_Vector: ");
	}
};

static_assert(sizeof(object_data) == object_sizes::k_object_size_object, STATIC_ASSERT_FAIL);

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
*/
