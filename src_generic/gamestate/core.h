#include <memory_map.h>
#include "../Direct3D/d3d9hook.h"
#include "halo_types.h"
#include "players.h"
#include "objects.h"
#include "../tags/map.h"
#include "camera.h"
#include "objectcontroller.h"

/*
	Project: tempera
	File: forge.cpp
	Copyright � 2009 SilentK, Abyll
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
class Core;

#pragma once


//using namespace std;
////////////////////////////////////////
// Defines
////////////////////////////////////////
#define CONSOLE_HOOK_ADDRESS 0x004C9DC0 //CE + 1
#define DEVMODE_HOOK_ADDRESS 0x004836DB
#define CONSOLE_TEXT_HOOK_ADDRESS 0x00499AB0 //CE + 1

static Core *core;
static CD3D cd3d;

////////////////////////////////////////
// Core Halo Structures
////////////////////////////////////////
struct _core_0 {
	data_header<void>   *Unknown;
	data_header<void>   *ObjectListHeader;
	data_header<void>   *ListObjectReference;
	data_header<void>   *HSGlobals;
	data_header<void>   *HSThread;
	data_header<void>   *ScriptNode;
	local_player        *LocalPlayer;
	data_header<void>   *Teams;
	data_header<player> *Players;
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

struct _core_2 {
	data_header<void> *Unknown;
	data_header<void> *Lights;
	data_header<void> *Unknown1[2];
	data_header<void> *Unknown2;
	data_header<void> *ClusterLightReference;
	data_header<void> *LightClusterReference;
};

struct _core_3 {
	data_header<void> *WeatherParticles;
	data_header<void> *Particle;
	data_header<void> *ParticleSystems;
	data_header<void> *ParticleSystemParticles;
	data_header<void> *Effect;
	data_header<void> *EffectLocation;
	data_header<void> *Decals;
	data_header<void> *ContrailPoint;
	data_header<void> *Contrail;
	data_header<void> *DeviceGroups;
};

struct _core_4 {
	data_header<void> *Prop;
	data_header<void> *Unknown;
	data_header<void> *Encounter;
	data_header<void> *Unknown2;
	data_header<void> *AIPursuit;
	data_header<void> *AIConversation;
	data_header<void> *Unknown3[2];
};

struct _core_5 {
	data_header<void> *Unknown;
	data_header<void> *SwarmComponent;
	data_header<void> *Swarm;
	data_header<void> *Actor;
};

struct _core_6 {
	data_header<void> *LoopObjects;
	data_header<void> *Unknown[2];
	unsigned long     Unknown1[44];
	data_header<void> *LightVolumes;
	data_header<void> *Lightnings;
	unsigned long     Unknown2[18];
	ident             unkid[5];
};

struct _core_7 {
	lruv_cache *DecalVertexCache;
};

class ObjectController;

////////////////////////////////////////
// The Core Class of HaloForge
////////////////////////////////////////
class Core {
public:
	_core_0 *core_0;
	_core_1 *core_1;
	_core_2 *core_2;
	_core_3 *core_3;
	_core_4 *core_4;
	_core_5 *core_5;
	_core_6 *core_6;
	_core_7 *core_7;

	map_header       *MapHeader;
	tag_index_header *TagIndexHeader;

	CMyCamera *MyCamera;
	_camera   *camera;

	ObjectController *ObjectControl;

	Core(LPCoreAddressList list);

	~Core();

	player *GetPlayer(short index);

	bool IsPlayerSpawned(short index);

	bool IsPlayerValid(short index);

	void TryLogPlayer(short index, bool toConsole = false);

	wchar_t *GetPlayerName(short player_index);

	long GetPlayerTeam(short player_index);

	ident GetPlayerObjectIdent(short player_index);

	biped_data *GetBiped(short player_index);

	object_header *GetObjectHeader(short object_index);

	object_data *GetGenericObject(short object_index);

	vect3 &GetObjectCoord(short object_index);

	const char *GetObjectName(short object_index);

	const char *GetObjectName(object_data *obj);

	char *GetMapName();

	// Console Methods
	void ToggleConsole(bool bSwitch); // Toggles Console on / off
	void ToggleDevmode(bool bSwitch); // Toggles Devmode on / off
	void ConsoleCMD(char *command);   // Calls a console / devcommand
	void ConsoleText(HaloColor fColor, const char *cFmt, ...);

	void ToggleFlycam(char = -1);
};
