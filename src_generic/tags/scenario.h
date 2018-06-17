#pragma once
/*
	Project: haloforge
	File: scenario.h
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

#include "map.h"
#include "bsp.h"

class ScnrFiller;

class CScnrHeader;

class CSkyBox;

class CScenerySpawn;

class CSceneryRef;

class CVehicleSpawn;

class CVehicleRef;

class CPlayerSpawn;

class CMpEquip;

template<class T>
	class CScnrReflexive {
	 public:
		DWORD Count;
		T     * Offset;
		DWORD Zeros;
	};

// Place holder for unwritten classes
class ScnrScnrFiller {
 public:
};

class CScnrHeader {
 public:
	BYTE                       Unknown1[16];                     // 0x0000
	BYTE                       Unknown2[16];                     // 0x0010
	BYTE                       Unknown3[16];                     // 0x0020
	CScnrReflexive<CSkyBox>    RSkyBox;            // 0x0030
	DWORD                      Unknown4;                        // 0x003C
	CScnrReflexive<ScnrFiller> RChildScenarios;         // 0x0040

	DWORD         UnNeeded1[46];                  // 0x004C
	unsigned long EditorScenarioSize;         // 0x0104
	DWORD         Unknown5;                        // 0x0108
	DWORD         Unknown6;                        // 0x010C
	void          * PointerToIndex;                  // 0x0110
	DWORD         UnNeeded2[2];                     // 0x0114
	void          * PointerToEndOfIndex;               // 0x011C
	DWORD         Zeros[57];                     // 0x0120

	CScnrReflexive<ScnrFiller>      RObjectNames;         // 0x0204
	CScnrReflexive<CScenerySpawn>   RScenery;         // 0x0210
	CScnrReflexive<CSceneryRef>     RSceneryRef;      // 0x021C
	CScnrReflexive<ScnrFiller>      RBiped;               // 0x0228
	CScnrReflexive<ScnrFiller>      RBipedRef;            // 0x0234
	CScnrReflexive<CVehicleSpawn>   RVehicle;         // 0x0240
	CScnrReflexive<CVehicleRef>     RVehicleRef;      // 0x024C
	CScnrReflexive<ScnrFiller>      REquip;               // 0x0258
	CScnrReflexive<ScnrFiller>      REquipRef;            // 0x0264
	CScnrReflexive<ScnrFiller>      RWeap;               // 0x0270
	CScnrReflexive<ScnrFiller>      RWeapRef;            // 0x027C
	CScnrReflexive<ScnrFiller>      RDeviceGroups;         // 0x0288
	CScnrReflexive<ScnrFiller>      RMachine;            // 0x0294
	CScnrReflexive<ScnrFiller>      RMachineRef;            // 0x02A0
	CScnrReflexive<ScnrFiller>      RControl;            // 0x02AC
	CScnrReflexive<ScnrFiller>      RControlRef;            // 0x02B8
	CScnrReflexive<ScnrFiller>      RLightFixture;         // 0x02C4
	CScnrReflexive<ScnrFiller>      RLightFixtureRef;      // 0x02D0
	CScnrReflexive<ScnrFiller>      RSoundScenery;         // 0x02DC
	CScnrReflexive<ScnrFiller>      RSoundSceneryRef;      // 0x02E8
	CScnrReflexive<ScnrFiller>      RUnknown7[7];         // 0x02F4
	CScnrReflexive<ScnrFiller>      RPlayerStartingProfile;   // 0x0348
	CScnrReflexive<CPlayerSpawn>    RPlayerSpawn;      // 0x0354
	CScnrReflexive<ScnrFiller>      RTriggerVolumes;         // 0x0360
	CScnrReflexive<ScnrFiller>      RAnimations;            // 0x036C
	CScnrReflexive<ScnrFiller>      RMultiplayerFlags;      // 0x0378
	CScnrReflexive<CMpEquip>        RMpEquip;            // 0x0384
	CScnrReflexive<ScnrFiller>      RStartingEquip;         // 0x0390
	CScnrReflexive<ScnrFiller>      RBspSwitchTrigger;      // 0x039C
	CScnrReflexive<ScnrFiller>      RDecals;               // 0x03A8
	CScnrReflexive<ScnrFiller>      RDecalsRef;            // 0x03B4
	CScnrReflexive<ScnrFiller>      RDetailObjCollRef;      // 0x03C0
	CScnrReflexive<ScnrFiller>      RUnknown8[7];         // 0x03CC
	CScnrReflexive<ScnrFiller>      RActorVariantRef;      // 0x0420
	CScnrReflexive<ScnrFiller>      REncounters;            // 0x042C
	//below this,
	//structs still not confirmed
	CScnrReflexive<ScnrFiller>      RCommandLists;         // 0x0438
	CScnrReflexive<ScnrFiller>      RUnknown9;            // 0x0444
	CScnrReflexive<ScnrFiller>      RStartingLocations;      // 0x0450
	CScnrReflexive<ScnrFiller>      RPlatoons;            // 0x045C
	CScnrReflexive<ScnrFiller>      RAiConversations;      // 0x0468
	unsigned long                   ScriptSyntaxDataSize;         // 0x0474
	unsigned long                   Unknown10;               // 0x0478
	CScnrReflexive<ScnrFiller>      RScriptCrap;            // 0x047C
	CScnrReflexive<ScnrFiller>      RCommands;            // 0x0488
	CScnrReflexive<ScnrFiller>      RPoints;               // 0x0494
	CScnrReflexive<ScnrFiller>      RAiAnimationRefs;      // 0x04A0
	CScnrReflexive<ScnrFiller>      RGlobalsVerified;      // 0x04AC
	CScnrReflexive<ScnrFiller>      RAiRecordingRefs;      // 0x04B8
	CScnrReflexive<ScnrFiller>      RUnknown11;            // 0x04C4
	CScnrReflexive<ScnrFiller>      RParticipants;         // 0x04D0
	CScnrReflexive<ScnrFiller>      RLines;               // 0x04DC
	CScnrReflexive<ScnrFiller>      RScriptTriggers;         // 0x04E8
	CScnrReflexive<ScnrFiller>      RVerifyCutscenes;      // 0x04F4
	CScnrReflexive<ScnrFiller>      RVerifyCutsceneTitle;   // 0x0500
	CScnrReflexive<ScnrFiller>      RSourceFiles;         // 0x050C
	CScnrReflexive<ScnrFiller>      RCutsceneFlags;         // 0x0518
	CScnrReflexive<ScnrFiller>      RCutsceneCameraPoi;      // 0x0524
	CScnrReflexive<ScnrFiller>      RCutsceneTitles;         // 0x0530
	CScnrReflexive<ScnrFiller>      RUnknown12[8];         // 0x053C
	DWORD                           Unknown13[2];                     // 0x059C
	CScnrReflexive<CBSPInfoPrimary> RStructBsp;         // 0x05A4
	// Binary Space Partitioning (BSP Tree) - The actual map layout ( excluding objects! )
};   /* CScnrHeaderSize = 0x05B0 ( 1456 bytes ) */

class CSkyBox {
 public:
	tagref SkyBox;
};

class CScenerySpawn {
 public:
	short         numid;
	short         flag;
	unsigned long Unknown1;
	vect3         World;
	vect3         Rotation;
	long          unknown[10];
};

class CSceneryRef {
 public:
	tagref        SceneryRef;
	unsigned long zero[8];
};

class CVehicleSpawn {
 public:
	short         numid;
	short         flag;
	unsigned long Unknown1;
	vect3         World;
	vect3         Rotation;
	unsigned long Unknown2[22];
};

class CVehicleRef {
 public:
	tagref        VehicleRef;
	unsigned long zero[8];
};

class CPlayerSpawn {
 public:
	vect3 World;
	float fYaw;
	long  Team;
	DWORD Unknown[8];
};

class CMpEquip {
 public:
	unsigned long Unknown[16];
	vect3         World;
	float         yaw;
	tagref        MpEquipRef;
	unsigned long Unknown1[12];
};
