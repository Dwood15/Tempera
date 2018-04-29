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

struct object_header;
struct object_data;
struct biped_data;

struct object_header {
	short object_id;
	unsigned char flags;        // some flags: x41: held by player (on back), xE2: wielded by a player (held), x63: on it's own (like on the ground - even counts for player bipeds)
	unsigned char object_type;
	short sector;   // portal rendering ( cluster index )
	short size;     // Structure size
	object_data *address;
};

struct object_data {
	ident meta;            // 0x0000
	long zero1;            // 0x0004
	char flags1[4];        // 0x0008
	long timer1;            // 0x000C
	char flags2[4];        // 0x0010
	long timer2;            // 0x0014
	long zero2[17];        // 0x0018
	vect3 World;            // 0x005C
	vect3 Velocity;        // 0x0068
	vect3 LowerRot;        // 0x0074
	vect3 Scale;            // 0x0080
	vect3 UnknownVect1;    // 0x008C
	long LocationID;        // 0x0098
	long unknown1;        // 0x009C
	vect3 UnknownVect2;    // 0x00A0
	float unknown2[2];    // 0x00AC
	long unknown3[3];    // 0x00B4
	ident Player;            // 0x00C0
	long unknown4[2];    // 0x00C4
	ident AntrMeta;        // 0x00CC
	long unknown5[4];    // 0x00D0
	float Health;            // 0x00E0
	float Shield1;        // 0x00E4
	long unknown6[11];    // 0x00E8
	ident VehicleWeapon;    // 0x0114
	ident Weapon;            // 0x0118	// currently held weapon (if this is a player)
	ident Vehicle;        // 0x011C	// or the thing that <this> is in (i.e. a weapon 'inside' a player, but only if it's wielded)
	short SeatType;        // 0x0120
	short unknown7;        // 0x0122
	long unknown8;        // 0x0124
	float Shield2;        // 0x0128
	float Flashlight1;    // 0x012C
	float unknown9;        // 0x0130
	float Flashlight2;    // 0x0134
	long unknown10[5];    // 0x0138
	ident UnknownIdent1;    // 0x014C
	ident UnknownIdent2;    // 0x0150
	long zero[6];        // 0x0154
	ident UnknownIdent3;    // 0x016C
	ident UnknownIdent4;    // 0x0170
	long unknown11[5];    // 0x0174
	float unknown12[24];    // 0x0188  // mostly 1.0, occasionally 0.
};

struct biped_data {
	object_data _object;        // 0x0000
	long unknown13[7];        // 0x01E8
	long IsInvisible;        // 0x0204	normal = 0x41 invis = 0x51 (bitfield?)
	char IsCrouching;        // 0x0208	crouch = 1, jump = 2
	char Unknown11[3];        // 0x0209
	char Unknown09[230];        // 0x020C // originally: 276
	short selected_weap_index;    // 0x02F2
	short selected_weap_index1;    // 0x02F4
	short Unknown14;            // 0x02F6
	ident held_weapons_ident[3];    // 0x02F8 // -1 if slot doesnt have a weap. (You can stick in any ident into here, and <TAB> weapon switch and it will go into your hand. Even the biped_data object for the player. But that "weapon"'s first person is wierd.. but works!?.. but don't swap back. lol)
	ident weap_slot_invalid;    //0x02FC // I can paste into the other three to swap weapons, but this fourth one is reset to -1 always. (But the original weapon disappears... Wierd?)
	char Unknown10[24];
	char Zoom00;                // 0x0320
	char Zoom01;                // 0x0321
	char Unknown12[610];        // 0x0322
	bone LeftThigh;            // 0x0584
	bone RightThigh;            // 0x05B8
	bone Pelvis;                // 0x05EC
	bone LeftCalf;            // 0x0620
	bone RightCalf;            // 0x0654
	bone Spine;                // 0x0688
	bone LeftClavicle;        // 0x06BC
	bone LeftFoot;            // 0x06F0
	bone Neck;                // 0x0724
	bone RightClavicle;        // 0x0758
	bone RightFoot;            // 0x078C
	bone Head;                // 0x07C0
	bone LeftUpperArm;        // 0x07F4
	bone RightUpperArm;        // 0x0828
	bone LeftLowerArm;        // 0x085C
	bone RightLowerArm;        // 0x0890
	bone LeftHand;            // 0x08C4
	bone RightHand;            // 0x08F8
	BYTE Unknown20[1216];    // 0x092C
}; // Size - 3564(0x0DEC) bytes

struct weapon_data {
	object_data _object;
	char Unknown5[72];
	long attacking;            // some sort of bitfield: 0x10 when used in a melee, 0x02 when shooting (at least for plasma, i think the 2 is for automatic.)
	char Unknown1[8];
	float heat;
	float age_of_battery;    // (1.0 - battery left) not sure why it's like that.
	char Unknown6[36];
	short toggle1;
	short toggle2;            // these 2 seem to toggle something on the weapon. From 3,0 it switches to 2,1 and then back. (At least for plasma rifle, and I noticed that the fire effect alternates to each tip of the 'claw' of the rifle)
	char Unknown7[70];
	short Reloading;        // 1=reloading
	short reload_time;        // countdown until reload is complete.
	short reserve_ammo;
	short current_ammo;
	char Unknown8[78];
	short reload_ammo;        // matches reserve_ammo after a reload or when you switch weapons, but not after collecting ammo.
	char Unknown9[518];
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
*/
