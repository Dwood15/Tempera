/*
	Project: tempera
	File: core.cpp
	Copyright � 2009 SilentK, Abyll
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "headers/core.h"
#include "../gamestate/halo_types.h"
#include "math.h"

typedef std::string string;

typedef void (*pConsoleCMD)(char *command);

pConsoleCMD oConsoleCMD;

typedef void (*pConsoleText)(const char *pString, const HaloColor *fColor);

pConsoleText oConsoleText;

////////////////////////////////////////
// Global Core Class Def
////////////////////////////////////////
Core *gCore;

// Gets the global Core class address
Core *GetCore() {
	return gCore;
}

// Sets the global Core class address
void SetCore(Core *core) {
	gCore = core;
}

////////////////////////////////////////
// Core Constructor / Deconstructor
////////////////////////////////////////
Core::Core() {
	//110 HCE:
	core_0 = (_core_0 *) 0x815900;//0x008154C0; //00 00 00 00 18 7C 3D 40 90 7E 3D 40 00 9D 3F 40 add [eax], al

	//110 HCE: 00
	core_1 = (_core_1 *) 0x7FB6F8;//0x007FB2B8; //18 2E 01 40 00 00 00 00 08 80 01 40 20 93 01 40 (PC) add [ebp-7b37bfc1], bl -CE sbb [esi], ch

	//110 HCE
	core_2 = (_core_2 *) 0x7FBE70; //0x007FBA30; //00 00 00 00 08 8C 02 40 00 00 00 00 00 00 00 00 add [eax], al -PC is the same

	//PC ONLY ???
	core_3 = (_core_3 *) 0x87A76C; //08 00 69 08 5C F9 2B 40 04 2E 2F 40 3C 84 2F 40 or [eax], al -PC address

	//110 HCE 0x81B800
	core_4 = (_core_4 *) 0x81B800; //0x0081B3C0; //C8 D8 38 40 38 37 3D 40 00 81 3C 40 38 B7 3C 40 enter 38d8, 40 -PC is same

	//110 HCE:
	core_5 = (_core_5 *) 0x81B894; //0x0081B454; //44 58 31 40 90 98 38 40 58 85 38 40 20 61 31 40 inc esp -PC same

	//110 HCE:
	core_6 = (_core_6 *) 0x653BE4; //0x00653B1C; //EC 66 05 40 BC 67 25 40 24 67 25 40 00 00 00 00 in al, dx -PC same

	//heyo:
	core_7 = (_core_7 *) 0x71D0E8; //18 CF 29 40 00 00 00 00 00 5F 3F 07 00 00 00 00 sbb bh, cl -'Nothing Found' is still PC address

	// Initialize all the core structures
	// Currently only Halo PC 1.08 compatible addresses
	//	core_0 = (_core_0*)0x0087A000;
	//	core_1 = (_core_1*)0x0085FF38;
	//	core_2 = (_core_2*)0x008606B0;
	//	core_3 = (_core_3*)0x0087A76C;
	//	core_4 = (_core_4*)0x0087FE60;
	//	core_5 = (_core_5*)0x0087FEF4;
	//	core_6 = (_core_6*)0x006B8BDC;
	//	core_7 = (_core_7*)0x0071D0E8;
	// Map
	MapHeader      = (map_header *) MAP_HEADER_ADDRESS;
	TagIndexHeader = (tag_index_header *) TAG_INDEX_HEADER_ADDRESS;

	// Camera
	MyCamera = new CMyCamera;
	camera   = (_camera *) MyCamera->Camera;

	// Object Control
	ObjectControl = new ObjectController(this);
}

Core::~Core() {
	delete MyCamera;
	delete ObjectControl;
}
////////////////////////////////////////
// Core Methods
////////////////////////////////////////

////////////////////////////////////////
// Player Methods

// Returns a player structure address, by player index
player *Core::GetPlayer(short index) {
	player *newplayer = (player *) ((unsigned long) core_0->Players->first + (index * core_0->Players->size));
	return newplayer;
}

// Check to see if a player is spawned && biped object is valid?
bool Core::IsPlayerSpawned(short index) {
	player *newplayer = GetPlayer(index);
	return (newplayer->object.id != 0 && newplayer->object.id != INVALID);
}

// Check to see if a player is valid
bool Core::IsPlayerValid(short index) {
	player *newplayer = GetPlayer(index);
	return (newplayer->playerid != 0 && newplayer->playerid != INVALID);
}

// Returns a player's name by player index
wchar_t *Core::GetPlayerName(short player_index) {
	player *newplayer = GetPlayer(player_index);
	if (!newplayer)
		return NULL;
	else
		return newplayer->PlayerName0;
}

// Returns a player team by player index
long Core::GetPlayerTeam(short player_index) {
	player *newplayer = GetPlayer(player_index);
	return newplayer->Team;
}

void Core::TryLogPlayer(short index, bool toConsole) {
	if (!IsPlayerSpawned(index)) {
		return;
	}

	GetPlayer(index)->DumpData(toConsole);
}

// Returns a player object ident by player index
ident Core::GetPlayerObjectIdent(short player_index) {
	player *newplayer = GetPlayer(player_index);
	return newplayer->object;
}
////////////////////////////////////////
// Object Methods

/**
 * @param player_index 0-15 index of player to try to get.
 * @returns the address of a players biped object structure
 */
biped_data *Core::GetBiped(short player_index) {
	short object_index = GetPlayerObjectIdent(player_index).index;

	if (object_index == INVALID)
		return NULL;

	return (biped_data *) GetObjectHeader(object_index)->address;
}

// Returns an object_header structure by object index
object_header *Core::GetObjectHeader(short object_index) {
	object_header *objectheader = (object_header *) ((unsigned long) core_1->Object->first + (object_index * core_1->Object->size));
	return objectheader;
}

// Returns a generic object_data structure by object index
object_data *Core::GetGenericObject(short object_index) {
	return GetObjectHeader(object_index)->address;
}

// Returns the x,y,z coordinates of an object by object index
vect3 &Core::GetObjectCoord(short object_index) {
	return GetGenericObject(object_index)->World;
}

// Returns an object name by object index
const char *Core::GetObjectName(short object_index) {
	return GetObjectName(GetGenericObject(object_index));
}

//Returns an object name by object structure
const char *Core::GetObjectName(object_data *obj) {
	short  metaind = obj->Meta.index;
	char   *name   = TagIndexHeader->FirstTag[metaind].TName->Name;
	string str     = name;

	return name + str.rfind("\\") + 1;
}

////////////////////////////////////////
// Map Methods
// Returns the loaded map name
char *Core::GetMapName() {
	return MapHeader->MapName;
}

////////////////////////////////////////
// Console Methods
// Toggles console on / off
void Core::ToggleConsole(bool bSwitch) {
	DWORD dwOldProtect = NULL;
	if (bSwitch) {

	} else {

	}
}

// Toggles devmode on / off
void Core::ToggleDevmode(bool bSwitch) {
	Core::ConsoleText(hRed, "Dev-mode toggling isn't enabled.");
	//	DWORD dwOldProtect = NULL;
	//	BYTE bEnable[1] = {0xEB};
	//	BYTE bDisable[1] = {0x74};
	//	VirtualProtect((void *) DEVMODE_HOOK_ADDRESS, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	//
	//	if (bSwitch)
	//		memcpy((void *) DEVMODE_HOOK_ADDRESS, (void *) bEnable, 1);
	//	else
	//		memcpy((void *) DEVMODE_HOOK_ADDRESS, (void *) bDisable, 1);
	//
	//	VirtualProtect((void *) DEVMODE_HOOK_ADDRESS, 1, dwOldProtect, &dwOldProtect);
}

// Toggles devmode flycam on / off
void Core::ToggleFlycam(char onoff) {
	static bool on = true; // first run means turn on.

	// if -1, toggle. Else, follow what was given.
	if (onoff == -1)
		on = !on;
	else
		on = (bool) onoff;

	if (on) {
		ConsoleCMD((char *) "debug_camera_load");
		//ConsoleCMD("camera_control 1"); // not necessary
	} else {
		ConsoleCMD((char *) "debug_camera_save");
		ConsoleCMD((char *) "camera_control 0");
	}
}

// Hooked console function
void hkConsoleCMD(char *command) {
	DEBUG("Executing console command: %s", command);
#ifdef __GNUC__

#elif _MSC_VER
	__asm {
	PUSH 0
	MOV EDI, command
	CALL DWORD PTR DS:[oConsoleCMD]
	ADD ESP, 0x4
	}
#endif
}

// Calls the requested console command
void Core::ConsoleCMD(char *command) {
	DWORD dwOldProtect    = NULL;
	BYTE  bConsoleOrig[8] = {0x8A, 0x07, 0x81, 0xEC, 0x00, 0x05, 0x00, 0x00};

	VirtualProtect((void *) CONSOLE_HOOK_ADDRESS, 8, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	oConsoleCMD = (pConsoleCMD) DetourFunction((PBYTE) CONSOLE_HOOK_ADDRESS, (PBYTE) hkConsoleCMD);

	ToggleDevmode(1);

	hkConsoleCMD(command);

	//ToggleDevmode( 0 );

	memcpy((void *) CONSOLE_HOOK_ADDRESS, (void *) bConsoleOrig, 8);
	VirtualProtect((void *) CONSOLE_HOOK_ADDRESS, 8, dwOldProtect, &dwOldProtect);
}

// Hooked console output function
void hkConsoleText(const char *cFmt, HaloColor *fColor) {
#ifdef __GNUC__
#elif _MSC_VER
	__asm {
	MOV EAX, fColor
	PUSH cFmt
	CALL DWORD PTR DS:[oConsoleText]
	ADD ESP, 04h
	}
#endif
}

// Outputs console text, with custom colors and formatting
void Core::ConsoleText(HaloColor fColor, const char *cFmt, ...) {
	va_list mvalist;
	char    cBuffer[256] = {0};

	DWORD dwOldProtect = 0;
	BYTE  bOrig[6]     = {0x83, 0xEC, 0x10, 0x57, 0x8B, 0xF8};

	VirtualProtect((void *) CONSOLE_TEXT_HOOK_ADDRESS, 10, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	oConsoleText = (pConsoleText) DetourFunction((PBYTE) CONSOLE_TEXT_HOOK_ADDRESS, (PBYTE) &hkConsoleText);

		va_start(mvalist, cFmt);
	_vsnprintf(cBuffer, sizeof(cBuffer), cFmt, mvalist);
	DEBUG(cFmt, mvalist);
		va_end(mvalist);

	hkConsoleText(cBuffer, &fColor);

	memcpy((void *) CONSOLE_TEXT_HOOK_ADDRESS, (void *) bOrig, 6);
	VirtualProtect((void *) CONSOLE_TEXT_HOOK_ADDRESS, 10, dwOldProtect, &dwOldProtect);
}
