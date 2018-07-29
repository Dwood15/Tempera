#pragma once

#include "../runtime/memory_map.h"
#include "gamestate/camera.h"
#include "gamestate/players.h"
#include "scenario/structures.h"
#include "tags/map.h"
#include "tags/tags.h"
#include "math.h"
#include "Direct3D/d3d9hook.h"
#include "math/colors.h"
#include "gamestate/player_types.h"
#include "game/time/time_structures.hpp"
#include "core_types.h"

struct object_data;
class ObjectController;
struct s_player_control_globals_data;
struct s_motion_sensor;
struct object_header;
struct _core_1;

namespace Yelo::Scenario {
	struct s_scenario_globals;
}
////////////////////////////////////////
// The Core Class of HaloForge
////////////////////////////////////////

class Core {
	_core_0 *core_0;
	_core_1 *core_1;
	_core_2 *core_2;
	_core_3 *core_3;
	_core_4 *core_4;
	_core_5 *core_5;
	_core_6 *core_6;
	_core_7 *core_7;
public:

	map_header       *MapHeader;
	tag_index_header *TagIndexHeader;

	short *to_respawn_count;
	short *spawn_count;
	short *render_window_count;
	bool  *at_main_menu;

	void **hud_scripted_globals;
	void **hud_messaging_state;

	ushort                        *main_globals_game_connection_type;
	s_player_control_globals_data *player_control_globals_data;
	s_players_globals_data        *players_globals;
	Yelo::Scenario::s_scenario_globals * scenario_globals;
	s_motion_sensor               *motion_sensor;
	Yelo::GameState::s_game_time_globals	  *game_time_globals;
	uintptr                       game_state_globals_location_ptr;
	uintptr                       game_state_globals_ptr;
	void                          **crc_checksum_buffer;

	////////////////////////////////////////
	// Defines
	////////////////////////////////////////
	uintptr_t CONSOLE_HOOK_ADDRESS; //CE + 1
	uintptr_t DEVMODE_HOOK_ADDRESS;
	uintptr_t CONSOLE_TEXT_HOOK_ADDRESS; //CE + 1

	CMyCamera *MyCamera;
	_camera   *camera;

	ObjectController *ObjectControl;

	Core() = default;

	Core(LPCoreAddressList list);

	~Core();

	player *GetPlayer(short index);

	bool AreWeInMainMenu();

	bool IsPlayerSpawned(short index);

	bool IsPlayerValid(short index);

	void TryLogPlayer(short index, bool toConsole = false);

	wchar_t *GetPlayerName(short player_index);

	long GetPlayerTeam(short player_index);

	ident GetPlayerObjectIdent(short player_index);

	// biped_data *GetBiped(short player_index);

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

static CD3D cd3d;
static Core *eCore = reinterpret_cast<Core *> (-1);
