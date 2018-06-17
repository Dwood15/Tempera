#pragma once

#include <precompile.h>
#include "../../../../../../BlamSauce/interface/input/player_control.h"
#include "../../../../../../BlamSauce/interface/hud/hud.hpp"
#include "../../../../../../BlamSauce/interface/hud/hud_messaging.hpp"
#include "../../../../../../BlamSauce/interface/hud/hud_unit.hpp"
#include "../../../../../../BlamSauce/interface/hud/hud_weapon.hpp"
#include "../../../../../../BlamSauce/interface/hud/hud_nav_points.hpp"
#include "../../../../../../BlamSauce/interface/hud/motion_sensor.hpp"
#include "../../../../../../BlamSauce/interface/hud/first_person_weapons.hpp"
#include "../../../../../../BlamSauce/interface/strings/draw_string.hpp"
#include "../../../../../../BlamSauce/interface/hud/hud_chat.hpp"
#include "../../../../../../BlamSauce/models/animations/recorded_animations.hpp"
#include "../../../../../../BlamSauce/hs/object_lists.hpp"
#include "../../../../../../BlamSauce/hs/structures.hpp"
#include "../../../../../../BlamSauce/cseries/base.h"

// TODO: Make this whole thing _not_ suck as bad as it does. :)
//	FUNC_PTR(INPUT_UPDATE_HOOK,						K_INPUT_UPDATE_HOOK, FUNC_PTR_NULL);
static auto const INPUT_UPDATE_HOOK = (uintptr_t) 0x493856;
static auto *const InputStateFlags    = (reinterpret_cast<byte *>(0x6AD572));
static auto *const ControlState       = (reinterpret_cast<Yelo::Input::ControlStates *>(0x64C550));
static auto *const MousePositionState = (reinterpret_cast<Yelo::Input::PositionState *>(0x6B4008));
static auto *const Settings           = (reinterpret_cast<Yelo::Input::ControlSettings *>(0x6AD802));

namespace Yelo::GameUI {
	//#define FUNC_PTR(name, ...)  enum FUNC_PTR_##name  { PTR_##name = PLATFORM_VALUE_HACK_((__VA_ARGS__)) }; static_assert( GET_FUNC_PTR(name) != NULL );


	//FUNC_PTR(HUD_UPDATE,										K_HUD_UPDATE, FUNC_PTR_NULL);
	//FUNC_PTR(HUD_UPDATE_CALL,									K_HUD_UPDATE_CALL, FUNC_PTR_NULL);
	//FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE,						K_FIRST_PERSON_WEAPONS_UPDATE, FUNC_PTR_NULL);
	//FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE_CALL,					K_FIRST_PERSON_WEAPONS_UPDATE_CALL, FUNC_PTR_NULL);
	//FUNC_PTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK,				K_FIRST_PERSON_WEAPON_SET_STATE_HOOK, FUNC_PTR_NULL);

	enum HUD_UPDATE {
		PTR_HUD_UPDATE  = 0x512360,
		HUD_UPDATE_CALL = 0x45C1A2
	};

	enum FIRST_PERSON_WEAPONS {
		FIRST_PERSON_WEAPONS_UPDATE        = 0x45AFA0,
		FIRST_PERSON_WEAPONS_UPDATE_CALL   = 0x45C17E,
		FIRST_PERSON_WEAPON_SET_STATE_HOOK = 0x495E19
	};

	static auto *const text_bounds_globals  = (reinterpret_cast<s_text_bounds_globals_data *>(0x67F6D4));
	static auto *const font_drawing_globals = (reinterpret_cast<s_font_drawing_globals_data *>(0x67F6E8));
	static auto *const hud_chat_globals     = (reinterpret_cast<s_hud_chat_globals *>(0x64E788));
	static auto *const hud_chat_line_count  = (reinterpret_cast<long *>(0x6B44B4));

	//ENGINE_//DPTR(type, name, ...) static auto** const pp##name = CAST_PTR(type**, PLATFORM_VALUE(__VA_ARGS__)); static_assert( PLATFORM_VALUE(__VA_ARGS__) != NULL );
	static auto **const hud_scripted_globals = (reinterpret_cast<Yelo::GameUI::s_hud_scripted_globals **>(0x6B44A8));
	static auto **const hud_messaging        = (reinterpret_cast<s_hud_messaging **>(0x64E970));
	static auto **const hud_unit_interface   = (reinterpret_cast<s_hud_unit_interface **>(0x6B44BC));
	static auto **const hud_weapon_interface = (reinterpret_cast<s_hud_weapon_interface **>(0x6B44C0));
	static auto **const hud_nav_points       = (reinterpret_cast<s_hud_nav_points **>(0x64E974));
	static auto **const motion_sensor        = (reinterpret_cast<s_motion_sensor **>(0x6B44C8));
	static auto **const first_person_weapons = (reinterpret_cast<s_first_person_weapons **>(0x64DCC8));
}

namespace Yelo::Hud {

#pragma region ShowCrosshair
	static auto *const RENDER_CROSSHAIRS_DISABLE_MOD = (reinterpret_cast<unsigned short *>(0x4B60F1));
#pragma endregion

#pragma region Scale
	// FUNC_PTR(RENDER_WIDGET_RECURSIVE,							K_RENDER_WIDGET_RECURSIVE, PTR_NULL);
	enum RENDER_WIDGET_RECURSIVE { PTR_RENDER_WIDGET_RECURSIVE = 0x49D850 };
	static void **K_RENDER_WIDGET_RECURSIVE_CALLS[]  = {
		(reinterpret_cast<void **>(0x49B42A)),
		(reinterpret_cast<void **>(0x49B506)),
		(reinterpret_cast<void **>(0x49DB4E)),
		(reinterpret_cast<void **>(0x49E589)),
		(reinterpret_cast<void **>(0x49EA81)),
		(reinterpret_cast<void **>(0x49EABF)),
	};

	static void *K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS[] = {
		(reinterpret_cast<void *>(0x520485)),
		(reinterpret_cast<void *>(0x534C57)),
		(reinterpret_cast<void *>(0x52F429)),
	};
#pragma endregion
	//#define FUNC_PTR(name, ...)
	//enum FUNC_PTR_##name  { PTR_##name = PLATFORM_VALUE_HACK_((__VA_ARGS__)) }; static_assert( GET_FUNC_PTR(name) != NULL );

#pragma region render nav points
	// FUNC_PTR(HUD_RENDER_NAV_POINTS,								K_HUD_RENDER_NAV_POINTS, PTR_NULL);
	enum FUNC_PTR_HUD_RENDER_NAV_POINTS { PTR_HUD_RENDER_NAV_POINTS = 0x4B2F80 };

	enum FUNC_PTR_HUD_RENDER_NAV_POINTS_CALL { PTR_HUD_RENDER_NAV_POINTS_CALL = 0x4ACAED };

	static auto *const HUD_RENDER_NAV_POINT_ANCHOR_HALF_X = (reinterpret_cast<uint *>(0x4B2B01));
	static auto *const HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y = (reinterpret_cast<uint *>(0x4B2B0B));

	static auto *const HUD_ANCHOR_SCREEN_X = (reinterpret_cast<real *>(0x612268));
	static auto *const HUD_ANCHOR_SCREEN_Y = (reinterpret_cast<real *>(0x000124));

	static auto *const HUD_ANCHOR_SCREEN_HALF_X = (reinterpret_cast<real *>(0x000028));
	static auto *const HUD_ANCHOR_SCREEN_HALF_Y = (reinterpret_cast<real *>(0x000020));
#pragma endregion

#pragma region render team indicator
	//enum FUNC_PTR_HUD_RENDER_NAV_POINTS_CALL { PTR_HUD_RENDER_NAV_POINTS_CALL = 0x4ACAED};
	enum FUNC_PTR_HUD_RENDER_PLAYERS { PTR_HUD_RENDER_PLAYERS = 0x4AD830 };
	enum FUNC_PTR_HUD_RENDER_PLAYERS_CALL { PTR_HUD_RENDER_PLAYERS_CALL = 0x4ACA6E };
#pragma endregion

#pragma region render scoreboard
	//enum FUNC_PTR_HUD_RENDER_NAV_POINTS_CALL { PTR_HUD_RENDER_NAV_POINTS_CALL = 0x4ACAED};
	enum FUNC_PTR_HUD_RENDER_SCOREBOARD_INGAME { PTR_HUD_RENDER_SCOREBOARD_INGAME = 0x45FC50 };
	enum FUNC_PTR_HUD_RENDER_SCOREBOARD_INGAME_CALL { PTR_HUD_RENDER_SCOREBOARD_INGAME_CALL = 0x497892 };
	enum FUNC_PTR_HUD_RENDER_SCOREBOARD_POSTGAME { PTR_HUD_RENDER_SCOREBOARD_POSTGAME = 0x45E090 };
	enum FUNC_PTR_HUD_RENDER_SCOREBOARD_POSTGAME_CALL { PTR_HUD_RENDER_SCOREBOARD_POSTGAME_CALL = 0x497889 };
#pragma endregion

#pragma region hud
	static auto *const HUD_POINT_ANCHOR_WIDTH       = (reinterpret_cast<uint *>(0x4AEAF0));
	static auto *const HUD_POINT_ANCHOR_HEIGHT      = (reinterpret_cast<uint *>(0x4AEB22));
	static auto *const HUD_POINT_ANCHOR_HALF_WIDTH  = (reinterpret_cast<uint *>(0x4AEB42));
	static auto *const HUD_POINT_ANCHOR_HALF_HEIGHT = (reinterpret_cast<uint *>(0x4AEB66));

	static auto *const HUD_POINT_DAMAGE_ANCHOR_WIDTH       = (reinterpret_cast<uint *>(0x4B49A5));
	static auto *const HUD_POINT_DAMAGE_ANCHOR_HEIGHT      = (reinterpret_cast<uint *>(0x4B497E));
	static auto *const HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT = (reinterpret_cast<real *>(0x4B49BF));

	static void **K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTRS[] = {(reinterpret_cast<void **>(0x4B499A)), (reinterpret_cast<void **>(0x4B495A))};
#pragma endregion

#pragma region multiplayer icons
	static auto *const HUD_MULTIPLAYER_ICON_CTF_x = (reinterpret_cast<unsigned short *>(0x4B4684));
	static auto *const HUD_MULTIPLAYER_ICON_CTF_Y = (reinterpret_cast<unsigned short *>(0x4B468B));

	static auto *const HUD_MULTIPLAYER_ICON_KING_X = (reinterpret_cast<unsigned short *>(0x4B46B6));

	static auto *const HUD_MULTIPLAYER_ICON_ODDBALL_x = (reinterpret_cast<unsigned short *>(0x4B46E1));
	static auto *const HUD_MULTIPLAYER_ICON_ODDBALL_Y = (reinterpret_cast<unsigned short *>(0x4B46E8));

	static auto *const HUD_MULTIPLAYER_ICON_RACE_X = (reinterpret_cast<unsigned short *>(0x4B4710));
	static auto *const HUD_MULTIPLAYER_ICON_RACE_Y = (reinterpret_cast<unsigned short *>(0x4B4717));

	static auto *const HUD_MULTIPLAYER_ICON_SLAYER_X = (reinterpret_cast<unsigned short *>(0x4B473F));

	static auto *const HUD_MULTIPLAYER_ICON_BACKGROUND_x = (reinterpret_cast<unsigned short *>(0x4B47EA));
	static auto *const HUD_MULTIPLAYER_ICON_BACKGROUND_Y = (reinterpret_cast<unsigned short *>(0x4B47F1));
#pragma endregion

	static auto **const hud_globals = (reinterpret_cast<void **>(0x6B44A4));

};

namespace Yelo::Keystone {
	enum FUNC_PTR_KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE { PTR_KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE = 0x4AE8A0 };

	static auto const ChatLogName = (reinterpret_cast<wchar_t *>(0x4B47EA));

	//#define ENGINE_//PTR(type, name, ...) static auto* const p##name = CAST_PTR(type*, PLATFORM_VALUE(__VA_ARGS__));  static_assert( PLATFORM_VALUE(__VA_ARGS__) != NULL );
	//The Triple pointer here is _on purpose_.
	static auto *const MainWindow = (reinterpret_cast<void ***>(0x4B47EA));

	enum FUNC_PTR_KS_TRANSLATEACCELERATOR_CALL { PTR_KS_TRANSLATEACCELERATOR_CALL = 0x4AE8A0 };
	enum FUNC_PTR_KS_TRANSLATEACCELERATOR { PTR_KS_TRANSLATEACCELERATOR = 0x4AE8A0 };
	enum FUNC_PTR_KS_GETWINDOW { PTR_KS_GETWINDOW = 0x4AE8A0 };
	enum FUNC_PTR_KW_RELEASE { PTR_KW_RELEASE = 0x4AE8A0 };
	enum FUNC_PTR_KW_GETCONTROLBYID { PTR_KW_GETCONTROLBYID = 0x4AE8A0 };
	enum FUNC_PTR_KC_SENDMESSAGE { PTR_KC_SENDMESSAGE = 0x4AE8A0 };

}

namespace Yelo::GameUI::Widget {
	enum FUNC_PTR_UI_WIDGET_LOAD_BY_NAME_OR_TAG { PTR_UI_WIDGET_LOAD_BY_NAME_OR_TAG = 0x49AA00 };
}

#ifdef ENGINE_DPTR
#undef ENGINE_DPTR
#endif

#ifdef ENGINE_PTR
#undef ENGINE_PTR
#endif

#ifdef FUNC_PTR
#undef FUNC_PTR
#endif

#define FUNC_PTR(name, value) static auto const name  =  value

static auto const gather_exception_data = (reinterpret_cast<void *>(0x6BDE8C));

//ENGINE_PTR(const void*, gather_exception_data, 0x6BDE8C);
static auto const GATHER_EXCEPTION = 0x582060;
static auto const RASTERIZER_SET_TEXTURE_BITMAP_DATA = 0x51BF30;

static auto const RESOLUTION_LIST_ADD_RESOLUTION = 0x4BE1B0;

static auto const ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL = 0x4D9680;

static auto const GSMD5DIGEST = 0x5BF440;

static auto const SHELL_GET_COMMAND_LINE_ARGUMENT = 0x545B70;

//////////////////////////////////////////////////////////////////////////
// AI
static auto const ACTOR_DELETE = 0x427E60;
static auto const ACTOR_UPDATE = 0x429160;
static auto const ACTOR_CUSTOMIZE_UNIT = 0x426CF0;
static auto const ACTOR_SET_ACTIVE = 0x4276E0;
static auto const ACTOR_SET_DORMANT = 0x427860;
static auto const ACTOR_DELETE_PROPS = 0x427E00;
static auto const ACTOR_FREEZE = 0x429000;
static auto const ACTOR_CREATE_FOR_UNIT = 0x426AC0;
static auto const ACTOR_ACTION_CHANGE = 0x40D8D0;
static auto const AI_SCRIPTING_ATTACH_FREE = 0x435420;

//////////////////////////////////////////////////////////////////////////
// Cache
static auto const CACHE_FILE_READ_REQUEST = 0x443410;
static auto const SOUND_CACHE_SOUND_REQUEST = 0x444770;
static auto const TEXTURE_CACHE_BITMAP_GET_HARDWARE_FORMAT = 0x444EB0;
static auto const PHYSICAL_MEMORY_MAP_PREDICT_RESOURCES = 0x445350;

//////////////////////////////////////////////////////////////////////////
// Camera
static auto const DIRECTOR_DESIRED_PERSPECTIVE = 0x446480;

//////////////////////////////////////////////////////////////////////////
// Console
static auto const CONSOLE_PROCESS_COMMAND = 0x4C9DC0;
static auto const CONSOLE_PRINTF = 0x4C9BC0;
static auto const CONSOLE_RESPONSE_PRINTF = 0x4C9C60;
static auto const CONSOLE_WARNING = 0x4C9D20;

//////////////////////////////////////////////////////////////////////////
// Cheats
static auto const CHEAT_ALL_WEAPONS = 0x45AEA0;
static auto const CHEAT_SPAWN_WARTHOG = 0x47FA50;
static auto const CHEAT_TELEPORT_TO_CAMERA = 0x45AFA0;
static auto const CHEAT_ACTIVE_CAMOFLAGE = 0x45B030;
static auto const CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER = 0x45B090;
static auto const CHEAT_PLAYER_INDEX = 0x45B110;
static auto const DIRECTOR_SAVE_CAMERA = 0x4461E0;
static auto const DIRECTOR_LOAD_CAMERA = 0x4462A0;

//////////////////////////////////////////////////////////////////////////
// Effects
static auto const EFFECT_NEW_ON_OBJECT_MARKER = 0x48B520;

//////////////////////////////////////////////////////////////////////////
// Game
static auto const SCENARIO_SWITCH_STRUCTURE_BSP = 0x542210;
static auto const BINK_PLAYBACK_START = 0x43ED20;
static auto const GAME_ENGINE_RASTERIZE_MESSAGE = 0x462A80;
static auto const GAME_TEAM_IS_ENEMY = 0x45C6C0;
static auto const GAME_ENGINE_PLAY_MULTIPLAYER_SOUND = 0x46EA60;

//////////////////////////////////////////////////////////////////////////
// HS
static auto const OBJECT_LIST_ADD = 0x48DEE0;

//////////////////////////////////////////////////////////////////////////
// Input
static auto const INPUT_KEY_IS_DOWN = 0x493910;

//////////////////////////////////////////////////////////////////////////
// Interface
static auto const KEYSTONE_CHAT_LOG_ADD_STRING = 0x4AE8A0; // I forget if the dedicated has a chat log equivalent...
static auto const HUD_PRINT_MESSAGE = 0x4B1570;
static auto const HUD_GET_ITEM_MESSAGE = 0x4AD400;
static auto const HUD_DRAW_METER = 0x4AEFB0;
static auto const HUD_DRAW_NUMBERS = 0x4AF4A0;
// FUNC_PTR(HUD_DRAW_STATIC_ELEMENT,				K_HUD_DRAW_STATIC_ELEMENT);
static auto const HUD_DRAW_BITMAP = 0x4AFEC0;
static auto const MAINMENU_LOAD = 0x4CBDC0;

//////////////////////////////////////////////////////////////////////////
// Items
static auto const WEAPON_PREVENTS_MELEE_ATTACK = 0x4C62A0;
static auto const WEAPON_PREVENTS_GRENADE_THROWING = 0x4C62F0;
static auto const WEAPON_STOP_RELOAD = 0x4C7F10;
static auto const FIRST_PERSON_WEAPON_MESSAGE_FROM_UNIT = 0x4954F0;
static auto const WEAPON_GET_FIRST_PERSON_ANIMATION_TIME = 0x4C6340;

//////////////////////////////////////////////////////////////////////////
// Math
static auto const PERIODIC_FUNCTION_EVALUATE = 0x4CFDB0;
static auto const TRANSITION_FUNCTION_EVALUATE = 0x4CFEC0;

//////////////////////////////////////////////////////////////////////////
// Memory
static auto const DATA_NEW = 0x4D3770;
static auto const DATUM_NEW_AT_INDEX = 0x4D37D0;
static auto const DATUM_NEW = 0x4D3880;
static auto const DATUM_DELETE = 0x4D3910;
static auto const DATA_DELETE_ALL = 0x4D3980;
static auto const DATA_ITERATOR_NEXT = 0x4D39D0;
static auto const DATA_NEXT_INDEX = 0x4D3A30;
static auto const DATUM_TRY_AND_GET = 0x4D3A80;
static auto const DATUM_INITIALIZE = 0x4D3AC0;

//////////////////////////////////////////////////////////////////////////
// Models
static auto const MODEL_FIND_MARKER = 0x4DABC0;

//////////////////////////////////////////////////////////////////////////
// Networking
static auto const HUD_CHAT_TO_NETWORK = 0x4ADD40;
static auto const INDEX_RESOLUTION_TABLE_TRANSLATE = 0x4F3EF0;
static auto const MAIN_CONNECT = 0x4CB800;

//////////////////////////////////////////////////////////////////////////
// Objects
static auto const HS_OBJECT_ORIENT = 0x48AB80;

static auto const OBJECT_HEADER_BLOCK_ALLOCATE = 0x4FB760;
static auto const OBJECT_ITERATOR_NEXT = 0x4FA830;

static auto const OBJECT_PLACEMENT_DATA_NEW = 0x4F8CB0;
static auto const OBJECT_NEW = 0x4F8D70;
static auto const OBJECT_NEW_WITH_ROLE = 0x4F8DC0;
static auto const OBJECT_DELETE_TO_NETWORK = 0x4F93B0;
static auto const OBJECT_DELETE = 0x4F94E0;
static auto const OBJECT_ATTACH_TO_MARKER = 0x4F9A90;
static auto const OBJECT_DETACH = 0x4F9F20;
static auto const OBJECTS_FIND_IN_SPHERE = 0x4FA8F0;
static auto const OBJECT_START_INTERPOLATION = 0x4FA480;
static auto const OBJECT_RESET = 0x4F8A70;
static auto const OBJECT_RECONNECT_TO_MAP = 0x4F9540;
static auto const OBJECT_DISCONNECT_FROM_MAP = 0x4F96F0;
static auto const OBJECT_GET_ORIGIN = 0x4FA210;
static auto const OBJECT_GET_ORIENTATION = 0x4FA280;
static auto const OBJECT_GET_LOCATION = 0x4FA420;
static auto const OBJECT_SET_POSITION = 0x4F8AD0;
static auto const OBJECT_SET_POSITION_NETWORK = 0x4F8BD0;
static auto const OBJECT_RESTORE_BODY = 0x4F12E0;
static auto const OBJECT_DEPLETE_BODY = 0x4F1330;
static auto const OBJECT_DEPLETE_SHIELD = 0x4F1420;
static auto const OBJECT_DOUBLE_CHARGE_SHIELD = 0x4F14B0;
static auto const OBJECT_CAUSE_DAMAGE = 0x4F1EF0;
static auto const OBJECT_DEFINITION_PREDICT = 0x4FB3E0;
static auto const OBJECT_SET_SCALE = 0x4FCFB0;
static auto const OBJECT_GET_LEVEL_OF_DETAIL_PIXELS = 0x512FB0;
static auto const OBJECT_RENDER_STATE_REFRESH = 0x512B80;
static auto const OBJECTS_UPDATE = 0x4F87A0;
static auto const OBJECT_UPDATE = 0x4FB800;
static auto const OBJECT_GET_MARKER_BY_NAME = 0x4F9990;
static auto const OBJECT_DESTROY = 0x4F1590;

//////////////////////////////////////////////////////////////////////////
// Units
static auto const UNIT_UPDATE = 0x565A70;
static auto const UNIT_GET_CAMERA_POSITION = 0x56C440;
static auto const UNIT_SET_ANIMATION = 0x5722D0;
static auto const UNIT_ANIMATION_START_ACTION = 0x5692C0;
static auto const UNIT_GET_CUSTOM_ANIMATION_TIME = 0x5738B0;
static auto const UNIT_CAN_ENTER_SEAT = 0x569D00;
static auto const UNIT_ENTER_SEAT = 0x569E30;
static auto const UNIT_EXIT_VEHICLE = 0x56ECF0;
static auto const UNIT_OPEN = 0x56E210;
static auto const UNIT_CLOSE = 0x56E230;
static auto const UNIT_FIND_NEARBY_SEAT = 0x569A20;
static auto const UNIT_EXIT_SEAT_END = 0x56FD40;
static auto const UNIT_CAN_SEE_POINT = 0x56F800;
static auto const UNIT_TRY_AND_EXIT_SEAT = 0x56FB70;
static auto const UNIT_DETACH_FROM_PARENT = 0x570140;
static auto const UNIT_START_USER_ANIMATION = 0x5739A0;
static auto const UNIT_DROP_CURRENT_WEAPON = 0x5715C0;
static auto const UNIT_DAMAGE_AFTERMATH = 0x56A960;
static auto const UNIT_SCRIPTING_SET_CURRENT_VITALITY = 0x565040;
static auto const UNIT_ANIMATION_SET_STATE = 0x569450;
static auto const UNIT_INVENTORY_GET_WEAPON = 0x56D070;
static auto const UNIT_THROW_GRENADE_RELEASE = 0x571B40;
static auto const UNIT_CAUSE_PLAYER_MELEE_DAMAGE = 0x572C50;
static auto const UNIT_SET_ACTIVELY_CONTROLLED = 0x56D2F0;
static auto const UNIT_READY_DESIRED_WEAPON = 0x570DE0;

//////////////////////////////////////////////////////////////////////////
// Physics
static auto const COLLISION_TEST_VECTOR = 0x509190;

//////////////////////////////////////////////////////////////////////////
// Players
static auto const PLAYER_INDEX_FROM_UNIT_INDEX = 0x477970;
static auto const PLAYER_TELEPORT = 0x478820;
static auto const PLAYER_OVER_SHIELD_SCREEN_EFFECT = 0x47C250;
static auto const PLAYER_CAMO_SCREEN_EFFECT = 0x47C310;
static auto const PLAYER_HEALTH_PACK_SCREEN_EFFECT = 0x47C3D0;
static auto const FIND_CLOSEST_PLAYER_INDEX = 0x45ECD0;
static auto const PLAYER_EXAMINE_NEARBY_VEHICLE = 0x47B140;
static auto const PLAYER_SET_ACTION_RESULT = 0x47B940;
static auto const PLAYER_SET_ACTION_RESULT_TO_NETWORK = 0x47BB30;
static auto const PLAYER_CONTROL_GET_UNIT_CAMERA_INFO = 0x474C00;



//////////////////////////////////////////////////////////////////////////
// Render
static auto const RENDER_FRAME = 0x50F7B0;
static auto const RENDER_WINDOW = 0x50F8C0;

//////////////////////////////////////////////////////////////////////////
// Scenario
static auto const SCENARIO_TRIGGER_VOLUME_TEST_POINT = 0x542380;

//////////////////////////////////////////////////////////////////////////
// TagGroups
static auto const TAG_LOADED = 0x442990;
static auto const TAG_ITERATOR_NEXT = 0x442A10;
static auto const UNICODE_STRING_LIST_GET_STRING = 0x55AD90;

//////////////////////////////////////////////////////////////////////////
// Scripting.cpp
static auto **const recorded_animations = (reinterpret_cast<Yelo::Scripting::recorded_animations_data_t **>(0x64B940));
static auto **const object_list_header = (reinterpret_cast<Yelo::Scripting::object_list_header_data_t **>(0x815904));

static auto **const list_object_reference = (reinterpret_cast<Yelo::Scripting::list_object_reference_data_t **>(0x815908));
static auto **const hs_syntax = (reinterpret_cast<Yelo::Scripting::hs_syntax_data_t **>(0x815914));
static auto **const hs_globals = (reinterpret_cast<Yelo::Scripting::hs_globals_data_t **>(0x81590C));
static auto **const hs_threads = (reinterpret_cast<Yelo::Scripting::hs_thread_data_t **>(0x815910));

static_assert(0x64B940 != NULL);
static_assert(0x815914 != NULL);
static_assert(0x815904 != NULL);
static_assert(0x815908 != NULL);
static_assert(0x81590C != NULL);
static_assert(0x815910 != NULL);

static auto const HS_UPDATE_HOOK = 0x48CE78;

static uint *K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST[] = {
	(reinterpret_cast<uint *>(0x485D7B)),
};

static uint *K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST[] = {
	//CAST_PTR(uint*, PLATFORM_VALUE(0x485E93, 0x47D783)), // don't modify this one, modify the size check using the address below
	(reinterpret_cast<uint *>(0x485DCA)),
};

static byte *ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK = (reinterpret_cast<byte *>(0x485D77));


//////////////////////////////////////////////////////////////////////////
// ScriptLibrary.cpp
static auto const HS_MACRO_FUNCTION_PARSE = 0x48A070;
static auto const HS_COMPILE_AND_EVALUATE = 0x487030; // currently unused
static auto const HS_NULL_EVALUATE = 0x483F40;
static auto const HS_NULL_WITH_PARAMS_EVALUATE = 0x483430;

//////////////////////////////////////////////////////////////////////////
// script functions related
static auto* const hs_function_table_count = reinterpret_cast<long *>(0x5F9C10);
static auto** const hs_function_table = reinterpret_cast<Yelo::Scripting::hs_function_definition**>(0x624118);

#pragma region hs function table references
static short *K_HS_FUNCTION_TABLE_COUNT_REFERENCES_16bit[] = {
	(reinterpret_cast<short *>(0x4861E1)),
	(reinterpret_cast<short *>(0x486F14)),
};
static long *K_HS_FUNCTION_TABLE_COUNT_REFERENCES_32bit[] = {
	(reinterpret_cast<long *>(0x4864FA)),
};

// really hs_function_definition***
static void *K_HS_FUNCTION_TABLE_REFERENCES[] = { // RIVERSIDE MOTHER FUCKER! *techno beat*
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_0),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_1),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_2),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_3),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_4),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_5),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_6),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_7),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_8),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_9),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_10),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_11),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_12),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_13),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_14),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_15),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_16),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_17),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_18),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_19),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_20),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_21),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_22),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_23),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_24),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_25),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_26),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_27),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_28),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_29),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_30),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_31),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_32),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_33),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_34),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_35),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_36),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_37),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_38),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_39),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_40),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_41),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_42),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_43),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_44),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_45),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_46),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_47),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_48),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_49),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_50),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_51),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_52),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_53),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_54),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_55),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_56),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_57),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_58),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_59),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_60),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_61),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_62),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_63),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_64),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_65),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_66),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_67),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_68),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_69),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_70),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_71),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_72),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_73),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_74),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_75),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_76),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_77),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_78),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_79),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_80),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_81),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_82),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_83),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_84),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_85),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_86),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_87),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_88),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_89),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_90),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_91),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_92),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_93),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_94),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_95),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_96),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_97),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_98),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_99),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_100),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_101),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_102),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_103),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_104),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_105),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_106),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_107),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_108),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_109),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_110),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_111),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_112),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_113),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_114),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_115),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_116),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_117),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_118),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_119),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_120),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_121),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_122),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_123),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_124),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_125),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_126),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_127),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_128),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_129),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_130),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_131),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_132),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_133),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_134),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_135),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_136),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_137),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_138),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_139),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_140),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_141),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_142),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_143),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_144),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_145),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_146),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_147),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_148),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_149),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_150),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_151),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_152),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_153),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_154),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_155),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_156),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_157),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_158),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_159),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_160),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_161),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_162),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_163),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_164),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_165),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_166),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_167),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_168),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_169),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_170),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_171),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_172),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_173),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_174),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_175),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_176),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_177),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_178),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_179),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_180),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_181),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_182),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_183),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_184),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_185),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_186),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_187),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_188),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_189),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_190),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_191),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_192),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_193),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_194),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_195),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_196),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_197),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_198),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_199),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_200),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_201),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_202),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_203),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_204),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_205),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_206),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_207),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_208),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_209),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_210),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_211),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_212),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_213),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_214),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_215),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_216),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_217),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_218),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_219),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_220),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_221),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_222),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_223),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_224),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_225),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_226),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_227),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_228),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_229),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_230),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_231),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_232),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_233),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_234),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_235),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_236),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_237),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_238),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_239),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_240),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_241),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_242),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_243),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_244),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_245),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_246),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_247),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_248),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_249),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_250),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_251),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_252),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_253),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_254),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_255),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_256),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_257),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_258),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_259),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_260),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_261),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_262),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_263),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_264),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_265),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_266),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_267),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_268),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_269),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_270),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_271),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_272),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_273),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_274),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_275),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_276),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_277),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_278),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_279),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_280),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_281),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_282),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_283),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_284),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_285),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_286),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_287),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_288),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_289),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_290),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_291),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_292),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_293),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_294),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_295),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_296),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_297),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_298),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_299),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_300),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_301),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_302),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_303),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_304),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_305),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_306),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_307),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_308),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_309),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_310),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_311),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_312),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_313),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_314),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_315),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_316),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_317),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_318),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_319),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_320),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_321),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_322),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_323),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_324),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_325),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_326),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_327),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_328),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_329),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_330),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_331),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_332),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_333),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_334),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_335),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_336),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_337),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_338),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_339),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_340),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_341),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_342),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_343),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_344),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_345),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_346),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_347),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_348),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_349),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_350),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_351),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_352),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_353),
	reinterpret_cast<void*>(K_HS_FUNCTION_TABLE_REFERENCE_354),
};
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// script globals related
static auto * const hs_external_globals_count = reinterpret_cast<long *>(0x5F9D0C);

static auto** const hs_external_globals = reinterpret_cast<Yelo::Scripting::hs_global_definition**>(0x626988);

static short *K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_16bit[] = {
	(reinterpret_cast<short *>(0x4860F1)),
};
static long *K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_32bit[] = {
	reinterpret_cast<long*>(0x4865AA),
	reinterpret_cast<long*>(0x48BCDA),
	reinterpret_cast<long*>(0x48CAFB),
	reinterpret_cast<long*>(0x48CC0F),
	reinterpret_cast<long*>(0x48CC6D),
	reinterpret_cast<long*>(0x48CD70),
	reinterpret_cast<long*>(0x48D38A),
};
static void  *K_HS_EXTERNAL_GLOBALS_REFERENCES[]             = {
	reinterpret_cast<void*>(0x48607C),
	reinterpret_cast<void*>(0x4860AC),
	reinterpret_cast<void*>(0x4860D9),
	reinterpret_cast<void*>(0x486410),
	reinterpret_cast<void*>(0x4865A5),
	reinterpret_cast<void*>(0x4891E2),
	reinterpret_cast<void*>(0x48BC6E),
	reinterpret_cast<void*>(0x48D1E6),
	reinterpret_cast<void*>(0x48DB1B),
	reinterpret_cast<void*>(0x48DC87),
};

//////////////////////////////////////////////////////////////////////////

static auto const HS_VALID_ACCESS_FLAGS = 0x486220;

//////////////////////////////////////////////////////////////////////////
// InitializeCreateScriptFunction()
static auto const HS_ARGUMENTS_EVALUATE = 0x48D480;
static auto const HS_RETURN = 0x48D270;




//////////////////////////////////////////////////////////////////////////
// Scenario.cpp
static auto** const scenario_globals = reinterpret_cast<Yelo::Scenario::s_scenario_globals**>(0x6E2254);



static auto* const global_scenario = reinterpret_cast<Yelo::TagGroups::scenario**>(0x6E224C);
static auto* const global_bsp3d = reinterpret_cast<Yelo::TagGroups::collision_bsp**>(0x6E2250);
#include "../../../../../../BlamSauce/models/collision/bsp_definitions.hpp"
#include "../../../../../../BlamSauce/models/structure/bsp_definitions.hpp"

static auto* const global_collision_bsp = reinterpret_cast<Yelo::TagGroups::collision_bsp**>(0x6E2258);
static auto* const global_structure_bsp = reinterpret_cast<Yelo::TagGroups::structure_bsp**>(0x6E225C);
static auto* const global_scenario_index = reinterpret_cast<datum_index*>(0x6397CC);
static auto* const structure_bsp_index = reinterpret_cast<short*>(0x6397D0);

static_assert( 0x626988 != NULL );
static_assert( 0x6E2254 != NULL );
static_assert( 0x6E224C != NULL );
static_assert( 0x6E2250 != NULL );
static_assert( 0x6E2258 != NULL );
static_assert( 0x6E225C != NULL );
static_assert( 0x6397CC != NULL );
static_assert( 0x6397D0 != NULL );

static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ = 0x4F8207;
static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE = 0x4F83CE;
static auto const OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS = 0x4F84E2;
//////////////////////////////////////////////////////////////////////////

#define ENGINE_DPTR(type, name, value) static auto** const name = CAST//_PTR(type**, value); static_assert( value != NULL )


#undef ENGINE_DPTR
#undef ENGINE_PTR
#undef FUNC_PTR
