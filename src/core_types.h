#pragma once

////////////////////////////////////////
// Generic Halo Structures
////////////////////////////////////////

#include <macros_generic.h>
#include "memory/datum_index.h"
#include "gamestate/player_types.h"

struct lruv_cache_block {
	long              unk0;
	long              page_count;
	Yelo::datum_index first_page_index;
	Yelo::datum_index next_block_index;
	Yelo::datum_index previous_block_index;
	long              unk14;
	long              unk18;
};

struct lruv_cache {
	char                          name[32];
	void                          *Functions[2];
	long                          page_count;
	long                          page_size_bits;
	long                          unknown;
	Yelo::datum_index             first_block_index;
	Yelo::datum_index             last_block_index;
	Yelo::data_header<lruv_cache_block> *data;
	unsigned long                 signature; // weee
};

struct memory_pool_block {
	// 'head'
	unsigned long     head;
	long              size;
	// the pointer to the beginning of this block
	void              *block_address;
	// the pool block that follows this one
	memory_pool_block *next;
	// the pool block the comes before this one
	memory_pool_block *prev;
	// 'tail'
	unsigned long     tail;
};

struct memory_pool {
	// 'head'
	unsigned long     signature;
	// 32 character string
	char              name[32];
	// next address to allocate a block at
	void              *base_address;
	// total size of the pool
	long              size;
	// total size left in the pool thats not own'd by anything
	long              free_size;
	// pointer to the first pool item
	memory_pool_block *first;
	// pointer to the last pool item
	memory_pool_block *last;
};

////////////////////////////////////////
// Core Halo Structures
////////////////////////////////////////

struct _core_0 {
	data_header<void>      *Unknown;
	data_header<void>      *ObjectListHeader;
	data_header<void>      *ListObjectReference;
	data_header<void>      *HSGlobals;
	data_header<void>      *HSThread;
	data_header<void>      *ScriptNode;
	s_players_globals_data *PlayersGlobals;
	data_header<void>      *Teams;
	data_header<player>    *Players;
};

constexpr uint core0_size = 0x815924-0x815900;
static_assert(sizeof(_core_0) == core0_size);

struct _core_2 {
	Yelo::data_header<void> *Unknown;
	Yelo::data_header<void> *Lights;
	Yelo::data_header<void> *Unknown1[2];
	Yelo::data_header<void> *Unknown2;
	Yelo::data_header<void> *ClusterLightReference;
	Yelo::data_header<void> *LightClusterReference;
};

struct _core_3 {
	Yelo::data_header<void> *WeatherParticles;
	Yelo::data_header<void> *Particle;
	Yelo::data_header<void> *ParticleSystems;
	Yelo::data_header<void> *ParticleSystemParticles;
	Yelo::data_header<void> *Effect;
	Yelo::data_header<void> *EffectLocation;
	Yelo::data_header<void> *Decals;
	Yelo::data_header<void> *ContrailPoint;
	Yelo::data_header<void> *Contrail;
	Yelo::data_header<void> *DeviceGroups;
};

struct _core_4 {
	Yelo::data_header<void> *Prop;
	Yelo::data_header<void> *Unknown;
	Yelo::data_header<void> *Encounter;
	Yelo::data_header<void> *Unknown2;
	Yelo::data_header<void> *AIPursuit;
	Yelo::data_header<void> *AIConversation;
	Yelo::data_header<void> *Unknown3[2];
};

struct _core_5 {
	Yelo::data_header<void> *Unknown;
	Yelo::data_header<void> *SwarmComponent;
	Yelo::data_header<void> *Swarm;
	Yelo::data_header<void> *Actor;
};

struct _core_6 {
	Yelo::data_header<void> *LoopObjects;
	Yelo::data_header<void> *Unknown[2];
	unsigned long     Unknown1[44];
	Yelo::data_header<void> *LightVolumes;
	Yelo::data_header<void> *Lightnings;
	unsigned long     Unknown2[18];
	Yelo::datum_index unkid[5];
};

struct _core_7 {
	lruv_cache *DecalVertexCache;
};

// struct biped_data {
// 	object_data _object;               // 0x0000
// 	long        ActorIndex;            // 0x01F4
// 	long        SwarmActorIndex;        // 0x01F8
// 	long        SwarmNextActorIndex;    // 0x01FC
// 	long        SwarmPrevObjectId;      // 0x0200
// 	long        IsInvisible;            // 0x0204	normal = 0x41 invis = 0x51 (bitfield?)
// 	char        KeyStateActionFlags;    // 0x0208	bit: crouch = 1, jump = 2, (3, unk), 4 - flashlight,
// 	char        Unknown11[3];         // 0x0209
// 	char        Unknown09[230];        // 0x020C // originally: 276
// 	short       selected_weap_index;    // 0x02F2
// 	short       selected_weap_index1;    // 0x02F4
// 	short       Unknown14;            // 0x02F6
// 	Yelo::datum_index       held_weapons_Yelo::datum_index[3];    // 0x02F8 // -1 if slot doesnt have a weap. (You can stick in any Yelo::datum_index into here, and <TAB> weapon switch and it will go into your hand. Even the biped_data object for the player. But that "weapon"'s first person is wierd.. but works!?.. but don't swap back. lol)
// 	Yelo::datum_index       weap_slot_invalid;    //0x02FC // I can paste into the other three to swap weapons, but this fourth one is reset to -1 always. (But the original weapon disappears... Wierd?)
// 	char        Unknown10[24];
// 	char        Zoom00;                // 0x0320
// 	char        Zoom01;                // 0x0321
// 	char        Unknown12[610];        // 0x0322
// 	bone        LeftThigh;            // 0x0584
// 	bone        RightThigh;            // 0x05B8
// 	bone        Pelvis;                // 0x05EC
// 	bone        LeftCalf;            // 0x0620
// 	bone        RightCalf;            // 0x0654
// 	bone        Spine;                // 0x0688
// 	bone        LeftClavicle;        // 0x06BC
// 	bone        LeftFoot;            // 0x06F0
// 	bone        Neck;                // 0x0724
// 	bone        RightClavicle;        // 0x0758
// 	bone        RightFoot;            // 0x078C
// 	bone        Head;                // 0x07C0
// 	bone        LeftUpperArm;        // 0x07F4
// 	bone        RightUpperArm;        // 0x0828
// 	bone        LeftLowerArm;        // 0x085C
// 	bone        RightLowerArm;        // 0x0890
// 	bone        LeftHand;            // 0x08C4
// 	bone        RightHand;            // 0x08F8
// 	BYTE        Unknown20[1216];    // 0x092C
// };// Size - 3564(0x0DEC) bytes
