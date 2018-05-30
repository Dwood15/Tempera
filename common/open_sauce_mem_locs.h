#pragma once

#include "target_version.h"
#include <precompile.h>

//I recognize this isn't an ideal constexpr setup. Will look into different solutions.
// constexpr engines::major ENGINE_TARGET = engines::major::CE;
// constexpr engines::with_minor GAME_MINOR = engines::with_minor::halo_1_10;

ADDR game_engine_definition   = 0x68CD24;
ADDR game_engine_globals_mode = 0x815EB0; // some enum like >= 2 && <= 3
ADDR cinematic_globals        = 0x68C83C; // and cinematic globals in progress

//HaloForge
ADDR CORE0 = 0x815900;
ADDR CORE1 = 0x7FB6F8;
ADDR CORE2 = 0x7FBE70;
ADDR CORE4 = 0x81B800;
ADDR CORE5 = 0x81B894;
ADDR CORE6 = 0x653BE4;


//TODO: Verify These addresses are accurate to 110...
ADDR CORE3 = 0x87A76C;
ADDR CORE7 = 0x71D0E8;




//From Open Sauce.
ADDR K_RECORDED_ANIMATIONS                                     = 0x64B940;
ADDR K_HS_SYNTAX                                               = 0x815914;
ADDR K_OBJECT_LIST_HEADER                                      = 0x815904;
ADDR K_LIST_OBJECT_REFERENCE                                   = 0x815908;
ADDR K_HS_GLOBALS                                              = 0x81590C;
ADDR K_HS_THREADS                                              = 0x815910;
ADDR K_HS_UPDATE_HOOK                                          = 0x48CE78;
ADDR K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST_0 = 0x485D7B;
ADDR K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST_0    = 0x485DCA;
ADDR K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK           = 0x485D77;
ADDR K_HS_MACRO_FUNCTION_PARSE                                 = 0x48A070;
ADDR K_HS_COMPILE_AND_EVALUATE                                 = 0x487030;
ADDR K_HS_NULL_EVALUATE                                        = 0x483F40;
ADDR K_HS_NULL_WITH_PARAMS_EVALUATE                            = 0x483430;
ADDR K_HS_FUNCTION_TABLE                                       = 0x624118;
ADDR K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_0               = 0x4861E1;
ADDR K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_1               = 0x486F14;
ADDR K_HS_FUNCTION_TABLE_COUNT_REFERENCE_32BIT_0               = 0x4864FA;
ADDR K_HS_EXTERNAL_GLOBALS                                     = 0x626988;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_16BIT_0             = 0x4860F1;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_0             = 0x4865AA;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_1             = 0x48BCDA;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_2             = 0x48CAFB;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_3             = 0x48CC0F;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_4             = 0x48CC6D;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_5             = 0x48CD70;
ADDR K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_6             = 0x48D38A;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_0                         = 0x48607C;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_1                         = 0x4860AC;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_2                         = 0x4860D9;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_3                         = 0x486410;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_4                         = 0x4865A5;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_5                         = 0x4891E2;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_6                         = 0x48BC6E;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_7                         = 0x48D1E6;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_8                         = 0x48DB1B;
ADDR K_HS_EXTERNAL_GLOBALS_REFERENCE_9                         = 0x48DC87;
ADDR K_HS_VALID_ACCESS_FLAGS                                   = 0x486220;
ADDR K_HS_ARGUMENTS_EVALUATE                                   = 0x48D480;
ADDR K_HS_RETURN                                               = 0x48D270;

ADDR K_AI_GLOBALS                                                    = 0x81B894;
ADDR K_ACTORS                                                        = 0x81B8A0;
ADDR K_SWARMS                                                        = 0x81B89C;
ADDR K_SWARM_COMPONENTS                                              = 0x81B898;
ADDR K_PROPS                                                         = 0x81B800;
ADDR K_ENCOUNTERS                                                    = 0x81B808;
ADDR K_SQUADS                                                        = 0x81B80C;
ADDR K_PLATOONS                                                      = 0x81B804;
ADDR K_AI_PURSUITS                                                   = 0x81B810;
ADDR K_AI_COMMUNICATION_REPLIES                                      = 0x68BC5C;
ADDR K_AI_CONVERSATIONS                                              = 0x81B814;

ADDR K_AI_UPDATE_HOOK                                                = 0x42A949;
ADDR K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK                         = 0x40B105;
ADDR K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN                         = 0x40B10A;
ADDR K_PROP_STATUS_REFRESH_HOOK                                      = 0x41C9CE;
ADDR K_PROP_STATUS_REFRESH_RETN                                      = 0x41C9D4;
ADDR K_ACTOR_INPUT_UPDATE_HOOK                                       = 0x429CA7;
ADDR K_ACTOR_INPUT_UPDATE_RETN                                       = 0x429CAE;

ADDR K_OBSERVERS                                                     = 0x64758C;

ADDR K_CAMERA_SCRIPT_GLOBALS                                         = 0x621F90;
ADDR K_DIRECTOR_SCRIPTING                                            = 0x81713C;
ADDR K_CINEMATIC_GLOBALS                                             = 0x68C83C;
ADDR K_GLOBAL_DIRECTOR                                               = 0x647490;

ADDR K_DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET                          = 0x44644F;
ADDR K_HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD               = 0x4AC8EC;
ADDR K_HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG                     = 0x4B184F;

ADDR K_OBSERVER_UPDATE_COMMAND_HOOK                                  = 0x4484BA;
ADDR K_OBSERVER_UPDATE_POSITIONS                                     = 0x448B80;
ADDR K_OBSERVER_UPDATE_POSITIONS_NO_SCOPE_BLUR                       = 0x49737F;
ADDR K_OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS             = 0x448509;

ADDR K_MAX_FOV_CHECK_JMP                                             = 0x44930D;

ADDR K_DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK                         = 0x446585;

ADDR K_SCENARIO_PATHS_REFERENCE_0                                    = 0x49F9E3;
ADDR K_SCENARIO_PATHS_REFERENCE_1                                    = 0x49FB43;
ADDR K_SCENARIO_PATHS_REFERENCE_2                                    = 0x49FEEB;
ADDR K_SCENARIO_PATHS_REFERENCE_3                                    = 0x49FFC0;
ADDR K_SCENARIO_PATHS_REFERENCE_4                                    = 0x4A4A3B;

ADDR K_SCENARIO_PATHS_REFERENCE2_0                                   = 0x4A7205;
ADDR K_SCENARIO_PATHS_REFERENCE2_1                                   = 0x4CCFCC;
ADDR K_SCENARIO_PATHS_REFERENCE2_2                                   = 0x53C4CB;
ADDR K_SCENARIO_PATHS_REFERENCE2_3                                   = 0x53C594;

ADDR K_DISPLAY_SCENARIO_HELP_HOOK                                    = 0x49C1EA;
ADDR K_DISPLAY_SCENARIO_HELP_SUCCESS                                 = 0x49C316;
ADDR K_DISPLAY_SCENARIO_HELP_FAIL                                    = 0x49C369;

ADDR K_CAMPAIGN_LEVEL_DATA_REFERENCE_0                               = 0x49F8EB;
ADDR K_CAMPAIGN_LEVEL_DATA_REFERENCE_1                               = 0x49F9EA;
ADDR K_CAMPAIGN_LEVEL_DATA_REFERENCE_2                               = 0x49FB1E;
ADDR K_CAMPAIGN_LEVEL_DATA_REFERENCE_3                               = 0x49FD69;

ADDR K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_0                          = 0x49F8E6;
ADDR K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_1                          = 0x49FC26;
ADDR K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_2                          = 0x49FD64;

ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_0                         = 0x4CCFB9;

ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE_16BIT_0                        = 0x49F9B1;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_0                        = 0x49F9B9;

ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_0                        = 0x49FB04;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_1                        = 0x49FB5C;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_2                        = 0x49FB94;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_3                        = 0x4CCF87;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_4                        = 0x4CCFC6;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_5                        = 0x53C4C2;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_0                       = 0x49FB26;
ADDR K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_1                       = 0x53C58A;

ADDR K_TERMINAL_GLOBALS                                              = 0x64DE2C;
ADDR K_CONSOLE_GLOBALS                                               = 0x651F50;
ADDR K_CONSOLE_UPDATE_HOOK                                           = 0x4C9A63;

ADDR K_CONTRAILS                                                     = 0x81610C;
ADDR K_CONTRAIL_POINTS                                               = 0x816108;
ADDR K_PARTICLES                                                     = 0x8160F0;
ADDR K_EFFECTS                                                       = 0x8160FC;
ADDR K_EFFECT_LOCATIONS                                              = 0x816100;
ADDR K_PARTICLE_SYSTEMS                                              = 0x8160F8;
ADDR K_PARTICLE_SYSTEM_PARTICLES                                     = 0x8160F4;
ADDR K_STRUCTURE_DETAIL_OBJECTS                                      = 0x6BDA4C;
ADDR K_STRUCTURE_DECALS                                              = 0x6BDA5C;
ADDR K_BREAKABLE_SURFACE_GLOBALS                                     = 0x653CA8;
ADDR K_DECALS                                                        = 0x816104;
ADDR K_DECAL_GLOBALS                                                 = 0x64BA08;
ADDR K_DECAL_VERTEX_CACHE                                            = 0x6B8458;
ADDR K_EFFECTS_UPDATE_HOOK                                           = 0x451487;
ADDR K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_0           = 0x5136BA;
ADDR K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_1           = 0x51374B;
ADDR K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_2           = 0x513752;
ADDR K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_3           = 0x513779;
ADDR K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_4           = 0x5137CB;
ADDR K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES                 = 0x45B4B7;
ADDR K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS                   = 0x45B4CB;
ADDR K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS          = 0x45B4E4;
ADDR K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES = 0x45B52C;
ADDR K_GAME_BUILD_VERSION                                            = 0x60A370;
ADDR K_NETWORK_VERSION_ID1                                           = 0x4DC196;
ADDR K_NETWORK_VERSION_ID2                                           = 0x4DC373;
ADDR K_NETWORK_VERSION_ID3                                           = 0x4E3CF3;
ADDR K_GAME_STATE_HEADER_IS_VALID_HOOK                               = 0x53B7E7;
ADDR K_GAME_STATE_HEADER_IS_VALID_HOOK_RET                           = 0x53B83B;
ADDR K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK                           = 0x53B59E;
ADDR K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE                  = 0x53B5FE;
ADDR K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE                 = 0x53B6B9;
ADDR K_CTF_GLOBALS                                                   = 0x64BDB8;
ADDR K_SLAYER_GLOBALS                                                = 0x64C308;
ADDR K_ODDBALL_GLOBALS                                               = 0x64C078;
ADDR K_KING_GLOBALS                                                  = 0x64BDF0;
ADDR K_RACE_GLOBALS                                                  = 0x64C1C0;
ADDR K_GLOBAL_VARIANT                                                = 0x68CC48;
ADDR K_GAME_ENGINES                                                  = 0x6238C8;
ADDR K_GAME_ENGINE                                                   = 0x68CD24;
ADDR K_HUD_RENDER_UNIT_INTERFACE_JMP_TABLE                           = 0x424953;
ADDR K_HUD_RENDER_UNIT_INTERFACE_JMP_PTR                             = 0x4B466C;
ADDR K_FUNC4F7440JMPPTR                                              = 0x4F71E5;
ADDR K_FUNC4F7580JMPPTR                                              = 0x4F7343;
ADDR K_GAME_ENGINE_INIT_MOD                                          = 0x45CC94;
ADDR K_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK                       = 0x45CDC8;
ADDR K_GAME_ENGINE_UPDATE_HOOK                                       = 0x460D51;
ADDR K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_0                           = 0x4CABEA;
ADDR K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_1                           = 0x53B4A0;
ADDR K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_0                           = 0x53B53D;
ADDR K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_1                           = 0x53B683;
ADDR K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_2                           = 0x53B775;
ADDR K_GAME_STATE_AFTER_LOAD_PROCS_REF_0                             = 0x53B253;
ADDR K_GAME_STATE_AFTER_LOAD_PROCS_REF_1                             = 0x53B549;
ADDR K_GAME_STATE_AFTER_LOAD_PROCS_REF_2                             = 0x53B79F;
ADDR K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_0                           = 0x53B258;
ADDR K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_1                           = 0x53B54E;
ADDR K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_2                           = 0x53B7A4;
ADDR K_MAIN_GLOBALS                                                  = 0x6B4798;
ADDR K_PHYSICAL_MEMORY_GLOBALS                                       = 0x647458;
ADDR K_PHYSICAL_MEMORY_INITIALIZE                                    = 0x445230;
ADDR K_PHYSICAL_MEMORY_ALLOCATION_SIZE                               = 0x445258;
ADDR K_GAME_STATE_GLOBALS                                            = 0x67DD88;
ADDR K_GAME_GLOBALS                                                  = 0x64BAB0;
ADDR K_GAME_TIME_GLOBALS                                             = 0x68CD70;
ADDR K_GAME_ALLEGIANCE_GLOBALS                                       = 0x64BAB4;
ADDR K_GLOBAL_GAME_GLOBALS                                           = 0x6E2260;
ADDR K_GLOBAL_PHYSICS                                                = 0x637BE4;
ADDR K_GLOBAL_POINT_PHYSICS                                          = 0x653CAC;
ADDR K_DEVELOPER_MODE                                                = 0x816126;
ADDR K_TRANSPORT_DUMPING                                             = 0x68C474;
ADDR K_INIT_TXT_FILENAME                                             = 0x60AA9C;
ADDR K_DEVMODE_ENABLED                                               = 0x6BD15E;
ADDR K_GAME_INITIALIZE_HOOK                                          = 0x45B63B;
ADDR K_GAME_INITIALIZE_FOR_NEW_MAP_HOOK                              = 0x45BCDB;
ADDR K_GAME_DISPOSE_FROM_OLD_MAP_HOOK                                = 0x45BE50;
ADDR K_GAME_SCENARIO_SWITCH_BSP_CALL_0                               = 0x4811B0;
ADDR K_GAME_SCENARIO_SWITCH_BSP_CALL_1                               = 0x4CCEF7;
ADDR K_GAME_SCENARIO_SWITCH_BSP_CALL_2                               = 0x4CD021;
ADDR K_GAME_SCENARIO_SWITCH_BSP_CALL_3                               = 0x541A68;
ADDR K_GAME_SCENARIO_SWITCH_BSP_CALL_4                               = 0x542379;
ADDR K_GAME_DISPOSE_FROM_OLD_BSP_HOOK                                = 0x442986;
ADDR K_GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL                   = 0x4BA5FB;
ADDR K_GAME_CREATE_NETWORK_SERVER                                    = 0x4E2090;
ADDR K_GAME_CREATE_NETWORK_CLIENT                                    = 0x4DBE80;
ADDR K_GAME_CREATE_NETWORK_SERVER_CALL                               = 0x4E1190;
ADDR K_GAME_CREATE_NETWORK_CLIENT_CALL_0                             = 0x498AC4;
ADDR K_GAME_CREATE_NETWORK_CLIENT_CALL_1                             = 0x4A0173;
ADDR K_GAME_CREATE_NETWORK_CLIENT_CALL_2                             = 0x4A0215;
ADDR K_GAME_CREATE_NETWORK_CLIENT_CALL_3                             = 0x4A045E;
ADDR K_GAME_CREATE_NETWORK_CLIENT_CALL_4                             = 0x53F4F2;
ADDR K_GAME_CREATE_NETWORK_CLIENT_CALL_5                             = 0x4E12A0;
ADDR K_GAME_CREATE_NETWORK_CLIENT_CALL_6                             = 0x4E77BE;
ADDR K_LIGHTS                                                        = 0x7FBE74;
ADDR K_LIGHT_GAME_GLOBALS                                            = 0x6B8250;
ADDR K_LIGHT_CLUSTER                                                 = 0x7FBE80;
ADDR K_CLUSTER_LIGHT_REFERENCE                                       = 0x7FBE84;
ADDR K_LIGHT_CLUSTER_REFERENCE                                       = 0x7FBE88;
ADDR K_PLAYERS                                                       = 0x815920;
ADDR K_TEAMS                                                         = 0x81591C;
ADDR K_PLAYERS_GLOBALS                                               = 0x815918;
ADDR K_PLAYER_CONTROL_GLOBALS                                        = 0x64C38C;
ADDR K_PLAYER_EFFECTS                                                = 0x68C844;
ADDR K_MACHINE_TO_PLAYER_TABLE                                       = 0x64C390;
ADDR K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK                             = 0x47AAC1;
ADDR K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN                        = 0x47ABE0;
ADDR K_PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK                        = 0x477127;
ADDR K_PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK                        = 0x47755B;
ADDR K_PLAYERS_UPDATE_AFTER_GAME_HOOK                                = 0x4778EB;
ADDR K_FOLLOWING_CAMERA_GET_UNIT_CAMERA_INFO_CALL                    = 0x447CE2;
ADDR K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE                = 0x466272;
ADDR K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE                = 0x4A7DFB;
ADDR K_PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE                    = 0x47BF23;
ADDR K_PLAYER_HANDLE_ACTION_JMP_TABLE_RETN                           = 0x47C21B;
ADDR K_PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE                          = 0x47BF28;
ADDR K_PLAYER_HANDLE_ACTION_JMP_PTR                                  = 0x47BF39;
ADDR K_PLAYER_HANDLE_ACTION_JMP_TABLE                                = 0x5C36A3;

/*
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
 */


// ADDR K_INPUT_UPDATE_HOOK = 0x493856;
// ADDR K_SETTINGS = 0x6AD802;
// ADDR K_CONTROL_STATE = 0x64C550;
// ADDR K_MOUSE_POSITION_STATE = 0x6B4008;

ADDR K_INPUT_STATE_FLAGS = 0x6AD572;

// ADDR K_HUD_SCRIPTED_GLOBALS = 0x6B44A8;
// ADDR K_HUD_MESSAGING = 0x64E970;
// ADDR K_HUD_UNIT_INTERFACE = 0x6B44BC;
// ADDR K_HUD_WEAPON_INTERFACE = 0x6B44C0;
// ADDR K_HUD_NAV_POINTS = 0x64E974;
// ADDR K_MOTION_SENSOR = 0x6B44C8;
// ADDR K_FIRST_PERSON_WEAPONS = 0x64DCC8;
// ADDR K_TEXT_BOUNDS_GLOBALS = 0x67F6D4;
// ADDR K_FONT_DRAWING_GLOBALS = 0x67F6E8;
// ADDR K_HUD_CHAT_GLOBALS = 0x64E788;
// ADDR K_HUD_CHAT_LINE_COUNT = 0x6B44B4;
ADDR K_HUD_UPDATE      = 0x512360;
ADDR K_HUD_UPDATE_CALL = 0x45C1A2;

ADDR K_FIRST_PERSON_WEAPONS_UPDATE        = 0x45AFA0;
ADDR K_FIRST_PERSON_WEAPONS_UPDATE_CALL   = 0x45C17E;
ADDR K_FIRST_PERSON_WEAPON_SET_STATE_HOOK = 0x495E19;

// ADDR K_RENDER_CROSSHAIRS_DISABLE_MOD = 0x4B60F1;

ADDR K_RENDER_WIDGET_RECURSIVE      = 0x49D850;
// ADDR K_RENDER_WIDGET_RECURSIVE_CALL_0 = 0x49B42A;
// ADDR K_RENDER_WIDGET_RECURSIVE_CALL_1 = 0x49B506;
// ADDR K_RENDER_WIDGET_RECURSIVE_CALL_2 = 0x49DB4E;
// ADDR K_RENDER_WIDGET_RECURSIVE_CALL_3 = 0x49E589;
// ADDR K_RENDER_WIDGET_RECURSIVE_CALL_4 = 0x49EA81;
// ADDR K_RENDER_WIDGET_RECURSIVE_CALL_5 = 0x49EABF;
// ADDR K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALL_0 = 0x520485;
// ADDR K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALL_1 = 0x534C57;
// ADDR K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALL_2 = 0x52F429;
// ADDR K_HUD_RENDER_NAV_POINTS = 0x4B2F80;
// ADDR K_HUD_RENDER_NAV_POINTS_CALL = 0x4ACAED;
// ADDR K_HUD_RENDER_NAV_POINT_ANCHOR_HALF_X = 0x4B2B01;
// ADDR K_HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y = 0x4B2B0B;
// ADDR K_HUD_ANCHOR_SCREEN_X = 0x612268;
// ADDR K_HUD_ANCHOR_SCREEN_Y = 0x000124;
// ADDR K_HUD_ANCHOR_SCREEN_HALF_X = 0x000028;
// ADDR K_HUD_ANCHOR_SCREEN_HALF_Y = 0x000020;
// ADDR K_HUD_RENDER_PLAYERS = 0x4AD830;
// ADDR K_HUD_RENDER_SCOREBOARD_INGAME = 0x45FC50;
// ADDR K_HUD_RENDER_SCOREBOARD_INGAME_CALL = 0x497892;
// ADDR K_HUD_RENDER_SCOREBOARD_POSTGAME = 0x45E090;
// ADDR K_HUD_RENDER_SCOREBOARD_POSTGAME_CALL = 0x497889;
// ADDR K_HUD_POINT_ANCHOR_WIDTH       = 0x4AEAF0;
// ADDR K_HUD_POINT_ANCHOR_HEIGHT      = 0x4AEB22;
// ADDR K_HUD_POINT_ANCHOR_HALF_WIDTH  = 0x4AEB42;
// ADDR K_HUD_POINT_ANCHOR_HALF_HEIGHT = 0x4AEB66;

// ADDR K_HUD_POINT_DAMAGE_ANCHOR_WIDTH            = 0x4B49A5;
// ADDR K_HUD_POINT_DAMAGE_ANCHOR_HEIGHT           = 0x4B497E;
// ADDR K_HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT      = 0x4B49BF;
// ADDR K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTR_0 = 0x4B499A;
	// K_ACTOR_DELETE
// ADDR K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTR_1 = 0x4B495A;

ADDR K_HUD_MULTIPLAYER_ICON_CTF_X             = 0x4B4684;
ADDR K_HUD_MULTIPLAYER_ICON_CTF_Y             = 0x4B468B;

ADDR K_HUD_MULTIPLAYER_ICON_KING_X            = 0x4B46B6;

ADDR K_HUD_MULTIPLAYER_ICON_ODDBALL_X         = 0x4B46E1;
ADDR K_HUD_MULTIPLAYER_ICON_ODDBALL_Y         = 0x4B46E8;

ADDR K_HUD_MULTIPLAYER_ICON_RACE_X            = 0x4B4710;
ADDR K_HUD_MULTIPLAYER_ICON_RACE_Y            = 0x4B4717;

ADDR K_HUD_MULTIPLAYER_ICON_SLAYER_X          = 0x4B473F;

ADDR K_HUD_MULTIPLAYER_ICON_BACKGROUND_X      = 0x4B47EA;
ADDR K_HUD_MULTIPLAYER_ICON_BACKGROUND_Y      = 0x4B47F1;




ADDR K_HUD_GLOBALS                            = 0x6B44A4;
ADDR K_KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE = 0x4AE8A0;

ADDR K_CHAT_LOG_NAME                          = 0x637D54;
ADDR K_MAIN_WINDOW                            = 0x6BD174;
ADDR K_KS_TRANSLATEACCELERATOR_CALL           = 0x544E82;
ADDR K_KS_TRANSLATEACCELERATOR                = 0x6BD180;
ADDR K_KS_GETWINDOW                           = 0x6BD188;
ADDR K_KW_RELEASE                             = 0x6BD198;
ADDR K_KW_GETCONTROLBYID                      = 0x6BD19C;
ADDR K_KC_SENDMESSAGE                         = 0x6BD1B8;

ADDR K_UI_WIDGET_LOAD_BY_NAME_OR_TAG          = 0x49AA00;

ADDR K_MDP_FIELD_ENCODE_ARRAY                                     = 0x4EC9A0;
ADDR K_MDP_FIELD_DECODE_ARRAY                                     = 0x4EC9E0;


ADDR K_MESSAGE_DELTA_FIELD_OBJECT_INDEX                           = 0x622698;
ADDR K_MESSAGE_DELTA_FIELD_PLAYER_INDEX                           = 0x622AC0;
ADDR K_MESSAGE_DELTA_PARAMETERS                                   = 0x635984;
ADDR K_MESSAGE_DELTA_GLOBAL_FIELD_TYPE_LIST                       = 0x6359A8;
ADDR K_G_MESSAGE_DELTA_ENCODING_CLASS                             = 0x636A08;
ADDR K_G_EXPOSED_PARAMS                                           = 0x6535F0;
ADDR K_MDP_PARAMETERS_PROTOCOL_GLOBALS                            = 0x6B8240;
ADDR K_MDP_PACKET_BUFFER_RECEIVED_DATA                            = 0x7FD180;
ADDR K_MDP_PACKET_BUFFER_SENT_DATA                                = 0x80D180;
ADDR K_MESSAGE_DELTA_PACKETS                                      = 0x5FBA40;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_0                          = 0x4EFC23;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_1                          = 0x4EFCA4;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_2                          = 0x4EFE3D;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_3                          = 0x4F0063;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_4                          = 0x4F00F0;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_5                          = 0x4F0280;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_6                          = 0x4F041C;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_7                          = 0x4F0489;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_8                          = 0x4F051E;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_9                          = 0x4F0548;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_10                         = 0x4F06FF;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_11                         = 0x4F08D9;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_12                         = 0x4F0991;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_13                         = 0x4F0A59;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_14                         = 0x4F0AE9;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_15                         = 0x4F0B37;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_16                         = 0x4F0BA5;
ADDR K_MESSAGE_DELTA_POINTER_REFERENCE_17                         = 0x4F0BC3;
ADDR K_MESSAGE_DELTA_POINTER_END_CHECK_0                          = 0x4EFC33;
ADDR K_MESSAGE_DELTA_POINTER_END_CHECK_1                          = 0x4EFCC7;
ADDR K_MESSAGE_DELTA_POINTER_END_CHECK_2                          = 0x4F0087;
ADDR K_MESSAGE_DELTA_TYPE_COUNT_CHECK_8BIT_0                      = 0x4F084D;
ADDR K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED                  = 0x4DCC40;
ADDR K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1            = 0x4DF360;
ADDR K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2            = 0x4E1E01;

ADDR K_NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL = 0x4DED6A;
ADDR K_DONT_SEND_OBJECT_NEW_MSG                                   = 0x6B7350;
ADDR K_NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY      = 0x4DD760;
ADDR K_GLOBAL_NETWORK_GAME_SERVER_DATA                            = 0x7FC6A0;
ADDR K_GLOBAL_NETWORK_GAME_CLIENT_DATA                            = 0x80E180;
ADDR K_MDPI_ENCODE                                                = 0x4F0250;
ADDR K_MDP_DECODE_STATELESS_ITERATED                              = 0x4EFEA0;
ADDR K_MDP_DECODE_INCREMENTAL_ITERATED                            = 0x4EFF10;
ADDR K_MDP_DISCARD_ITERATION_BODY                                 = 0x4EFF80;
ADDR K_UPDATE_CLIENT_GLOBALS                                      = 0x692E98;
ADDR K_UPDATE_SERVER_GLOBALS                                      = 0x68CD88;
ADDR K_NETWORK_UPDATE_GLOBALS                                     = 0x624A68;
ADDR K_GLOBAL_NETWORK_GAME_SERVER                                 = 0x6B7364;
ADDR K_GLOBAL_NETWORK_GAME_CLIENT                                 = 0x6B7368;
ADDR K_NETWORK_GAME_ADD_PLAYER                                    = 0x4E1930;
ADDR K_NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER    = 0x4E2D4A;
ADDR K_NETWORK_GAME_REMOVE_PLAYER                                 = 0x4DC590;
ADDR K_CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER                 = 0x5C4533;
ADDR K_NETWORK_CONNECTION_WRITE                                   = 0x4E0290;
ADDR K_NETWORK_GAME_SERVER_SEND_REJECTION_MESSAGE                 = 0x4E3F80;
ADDR K_NETWORK_GAME_SERVER_HOLDUP_NEW_CLIENT                      = 0x4E4060;
ADDR K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE                = 0x4E4DA0;
ADDR K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES           = 0x4E4E30;
ADDR K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES_INGAME    = 0x4E4EF0;
ADDR K_SV_PASSWORD                                                = 0x6B7384;
ADDR K_SV_RCON_PASSWORD                                           = 0x6B74A0;
ADDR K_SV_TIMELIMIT                                               = 0x634C08;
ADDR K_HALO_ARGV_PORT                                             = 0x6337F8;
ADDR K_HALO_ARGV_CLIENT_PORT                                      = 0x6337FC;
ADDR K_NETWORK_SV_FILE_GLOBALS                                    = 0x6534CC;
ADDR K_NETWORK_SV_MAPCYCLE_GLOBALS                                = 0x6230CC;
ADDR K_GS_SOCKET                                                  = 0x68C484;
ADDR K_GS_LOOPBACKSOCKET                                          = 0x68C488;
ADDR K_GAMESPY_CONFIG                                             = 0x63ACF4;
ADDR K_GAMESPY_GLOBALS                                            = 0x6BDA68;
ADDR K_GAMESPY_SERVER_BROWSER_GLOBALS                             = 0x6B44E0;
ADDR K_GAMESPY_QR2                                                = 0x6BDCF0;
ADDR K_G_GAMESPY_PATCH_CHECK_FOR_UPDATES                          = 0x62E101;
ADDR K_GAMESPY_PRODUCTS_LIST                                      = 0x6BE4C8;
ADDR K_COMPILER_NULL_STRING                                       = 0x5F363C;

ADDR K_GAMESPY_GET_CLIENT_KEY_HASH                                = 0x579690;
ADDR K_GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL          = 0x4A7870;

ADDR K_MATCHUP1_HOSTNAME                                          = 0x61EDF0;
ADDR K_MATCHUP2_HOSTNAME                                          = 0x61EDF4;
ADDR K_MASTER_ADDR_REFERENCE                                      = 0x5BB24C;
ADDR K_MASTER_ADDR_SB_REFERENCE                                   = 0x597B87;
ADDR K_PTA_DEFAULT_VERCHECK_URL_REFERENCE                         = 0x5C0FB5;
ADDR K_CREATE_GAMESPY_QR2_HOOK                                    = 0x57AFDA;
ADDR K_QR2_REGISTER_KEY                                           = 0x5C0750;

ADDR K_SBSERVER_GET_BOOL_VALUE                                    = 0x5BC1F0;
ADDR K_SBSERVER_GET_INT_VALUE                                     = 0x5BC920;
ADDR K_SBSERVER_GET_STRING_VALUE                                  = 0x5BC1B0;

ADDR K_OBJECT_TYPE_DEFINITIONS                                               = 0x637694;
ADDR K_WIDGET_TYPE_DEFINITIONS                                               = 0x6376C8;
ADDR K_WIDGETS                                                               = 0x7FB6F8;
ADDR K_FLAGS                                                                 = 0x7FB708;
ADDR K_ANTENNAS                                                              = 0x7FB70C;
ADDR K_GLOW                                                                  = 0x7FB700;
ADDR K_GLOW_PARTICLES                                                        = 0x7FB704;
ADDR K_LIGHT_VOLUMES                                                         = 0x653CA0;
ADDR K_LIGHTNINGS                                                            = 0x653CA4;
ADDR K_CACHED_OBJECT_RENDER_STATES                                           = 0x75E44C;
ADDR K_UNIT_GLOBALS                                                          = 0x68A8D0;
ADDR K_DEVICE_GROUPS                                                         = 0x816110;
ADDR K_OBJECT_HEADER                                                         = 0x7FB710;
ADDR K_OBJECTS_POOL                                                          = 0x653BE4;
ADDR K_OBJECT_GLOBALS                                                        = 0x653BEC;
ADDR K_OBJECT_NAME_LIST                                                      = 0x653BE8;
ADDR K_COLLIDEABLE_OBJECT                                                    = 0x7FB730;
ADDR K_CLUSTER_COLLIDEABLE_OBJECT_REFERENCE                                  = 0x7FB734;
ADDR K_COLLIDEABLE_OBJECT_CLUSTER_REFERENCE                                  = 0x7FB738;
ADDR K_NONCOLLIDEABLE_OBJECT                                                 = 0x7FB720;
ADDR K_CLUSTER_NONCOLLIDEABLE_OBJECT_REFERENCE                               = 0x7FB724;
ADDR K_NONCOLLIDEABLE_OBJECT_CLUSTER_REFERENCE                               = 0x7FB728;
ADDR K_OBJECT_MEMORY_POOL_ALLOCATION_SIZE                                    = 0x4F840D;
ADDR K_OBJECT_TYPES_PLACE_ALL_MOD_VEHI_REMAP                                 = 0x4F74E3;
ADDR K_OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP1                             = 0x4F8834;
ADDR K_OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP2                             = 0x4F88CA;
ADDR K_OBJECTS_UPDATE_CALL                                                   = 0x45C198;
ADDR K_OBJECTS_UPDATE_HOOK                                                   = 0x4F89EE;
ADDR K_OBJECTS_GARBAGE_COLLECTION                                            = 0x4FD570;
ADDR K_OBJECT_DAMAGE_OBJECT_BODY_HOOK                                        = 0x4F312A;
ADDR K_BIPED_UPDATE_MOVING_HOOK                                              = 0x561556;
ADDR K_BIPED_JUMP_MOD_STUN_PENALTY_FIELD_REF                                 = 0x56222E;
ADDR K_OBJECT_DEPLETE_BODY_KILL_CHILD_HOOK                                   = 0x4F1403;
ADDR K_OBJECT_DEPLETE_BODY_KILL_CHILD_RETN                                   = 0x4F140A;
ADDR K_OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK                              = 0x4F369B;
ADDR K_OBJECT_DAMAGE_AFTERMATH__CALC_ACCEL_HOOK_RETURN                       = 0x4F36D2;
ADDR K_OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK                = 0x4F37B8;
ADDR K_OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN         = 0x4F37CB;
ADDR K_OBJECT_DAMAGE_AFTERMATH__UNIT_VALIDATE_INST_ACCEL_HOOK_RETURN_INVALID = 0x4F3838;
ADDR K_MAXIMUM_RENDERED_OBJECTS_REF_32BIT_0                                  = 0x45B40D;
ADDR K_MAXIMUM_RENDERED_OBJECTS_REF_32BIT_1                                  = 0x5123F6;
ADDR K_MAXIMUM_RENDERED_OBJECTS_REF_16BIT_0                                  = 0x512466;
ADDR K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_0                         = 0x5122B6;
ADDR K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_1                         = 0x5123FB;
ADDR K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_2                         = 0x512441;
ADDR K_RENDER_OBJECT_GLOBALS__RENDERED_OBJECTS_REF_3                         = 0x512496;
ADDR K_OBJECT_NEW_SET_SHADOW_FLAG_HOOK                                       = 0x4F8FCA;
ADDR K_OBJECT_NEW_SET_SHADOW_FLAG_RETN                                       = 0x4F8FD7;
ADDR K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDE_0                                 = 0x44C00A;
ADDR K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDE_1                                 = 0x4FE16A;
ADDR K_OBJECT_DISABLE_SHADOW_CAST_OVERRIDE_2                                 = 0x551C69;
ADDR K_FIRST_PERSON_WEAPON_RENDER_UPDATE                                     = 0x4951F0;
ADDR K_RENDER_WINDOW_CALL_HOOK_FIRST_PERSON_WEAPON_RENDER_UPDATE             = 0x50F9A7;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_0                                          = 0x459D71;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_1                                          = 0x55D1D9;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_2                                          = 0x5656C9;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_3                                          = 0x56619D;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_4                                          = 0x5661AB;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_5                                          = 0x566239;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_6                                          = 0x566742;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_7                                          = 0x568EDA;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_8                                          = 0x568F46;
ADDR K_UNIT_ZOOM_LEVEL_OFFSET_REF_9                                          = 0x56903E;
ADDR K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REF_0                                  = 0x473B21;
ADDR K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REF_1                                  = 0x55E7F4;
ADDR K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REF_2                                  = 0x5656CF;
ADDR K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REF_3                                  = 0x566197;
ADDR K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REF_4                                  = 0x566F52;
ADDR K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REF_5                                  = 0x568F4D;
ADDR K_NUMBER_OF_UNIT_GRENADE_TYPES_REF_0                                    = 0x4768CB;
ADDR K_NUMBER_OF_UNIT_GRENADE_TYPES_REF_1                                    = 0x47F0ED;
ADDR K_NUMBER_OF_UNIT_GRENADE_TYPES_REF_2                                    = 0x4B503A;
ADDR K_NUMBER_OF_UNIT_GRENADE_TYPES_REF_3                                    = 0x566165;
ADDR K_NUMBER_OF_UNIT_GRENADE_TYPES_REF_4                                    = 0x4B649D;
ADDR K_NUMBER_OF_UNIT_GRENADE_TYPES_REF_5                                    = 0x57269A;
ADDR K_MAXIMUM_UNIT_GRENADE_INDEX_REF_0                                      = 0x474936;
ADDR K_MAXIMUM_UNIT_GRENADE_INDEX_REF_1                                      = 0x56D0F4;
ADDR K_MAXIMUM_UNIT_GRENADE_INDEX_REF_2                                      = 0x56D102;
ADDR K_ACTOR_DIED_UNIT_GRENADE_COUNT_MOD                                     = 0x428D35;
ADDR K_PLAYER_ADD_EQUIPMENT_UNIT_GRENADE_COUNT_MOD                           = 0x476847;
ADDR K_BIPED_NEW_FROM_NETWORK_UNIT_GRENADE_COUNT_MOD                         = 0x55E83A;
ADDR K_BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD1                         = 0x55E8D6;
ADDR K_BIPED_UPDATE_BASELINE_UNIT_GRENADE_COUNT_MOD2                         = 0x55E8F4;
ADDR K_BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD1                      = 0x55E9FF;
ADDR K_BIPED_BUILD_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD2                      = 0x55EA15;
ADDR K_BIPED_PROCESS_UPDATE_DELTA_UNIT_GRENADE_COUNT_MOD                     = 0x55EBC6;
ADDR K_BIPED_UPDATE_CHECK_PARENT_UNIT_TYPE                                   = 0x55C5FA;
ADDR K_UNIT_DAMAGE_AFTERMATH_CALL                                            = 0x4F38D7;
ADDR K_UNIT_CAN_ENTER_SEAT_HOOK                                              = 0x569D00;
ADDR K_BIPED_SEATED_MELEE_HOOK                                               = 0x55D292;
ADDR K_BIPED_SEATED_MELEE_RETN                                               = 0x55D299;
ADDR K_UNIT_THROW_GRENADE_RELEASE_HOOK                                       = 0x568A73;
ADDR K_WEAPON_PREVENTS_GRENADE_THROWING_HOOK                                 = 0x571821;
ADDR K_WEAPON_PREVENTS_GRENADE_THROWING_RETN                                 = 0x571826;
ADDR K_ANIMATION_STATE_DEFINED_KEYFRAME_HOOK                                 = 0x568A4F;
ADDR K_ANIMATION_STATE_DEFINED_KEYFRAME_STOCK_RETN                           = 0x568A56;
ADDR K_ANIMATION_STATE_DEFINED_KEYFRAME_SKIP_RETN                            = 0x568C77;
ADDR K_ANIMATION_STATE_FINAL_KEYFRAME_HOOK                                   = 0x568AA4;
ADDR K_ANIMATION_STATE_FINAL_KEYFRAME_STOCK_RETN                             = 0x568AAB;
ADDR K_ANIMATION_STATE_FINAL_KEYFRAME_SKIP_RETN                              = 0x568C64;
ADDR K_UNIT_ANIMATION_STATE_INTERRUPTABLE_CALL_0                             = 0x5689D0;
ADDR K_UNIT_ANIMATION_STATE_INTERRUPTABLE_CALL_1                             = 0x568D2C;
ADDR K_UNIT_ANIMATION_STATE_INTERRUPTABLE_CALL_2                             = 0x56A5F8;
ADDR K_UNIT_ANIMATION_BUSY_CALL_0                                            = 0x40B5EA;
ADDR K_UNIT_ANIMATION_BUSY_CALL_1                                            = 0x4343A8;
ADDR K_UNIT_ANIMATION_BUSY_CALL_2                                            = 0x55CA45;
ADDR K_UNIT_ANIMATION_BUSY_CALL_3                                            = 0x5660BF;
ADDR K_UNIT_ANIMATION_BUSY_CALL_4                                            = 0x56611B;
ADDR K_UNIT_ANIMATION_BUSY_CALL_5                                            = 0x566716;
ADDR K_UNIT_ANIMATION_BUSY_CALL_6                                            = 0x56B148;
ADDR K_UNIT_ANIMATION_BUSY_CALL_7                                            = 0x56E82B;
ADDR K_UNIT_ANIMATION_BUSY_CALL_8                                            = 0x56F195;
ADDR K_UNIT_ANIMATION_BUSY_CALL_9                                            = 0x56FBF9;
ADDR K_UNIT_ANIMATION_STATE_LOOPS_CALL                                       = 0x568C6A;
ADDR K_UNIT_ANIMATION_WEAPON_IK_CALL                                         = 0x5679AE;
ADDR K_UNIT_ANIMATION_VEHICLE_IK_CALL                                        = 0x567946;
//END OBJEcTS.INL


ADDR K_RENDER_SKY                                         = 0x5144C0;
ADDR K_RENDER_WINDOW_CALL_HOOK_RENDER_SKY                 = 0x50F9A2;
ADDR K_RENDER_OBJECTS                                     = 0x512240;
// ADDR K_RENDER_WINDOW_CALL_HOOK_RENDER_OBJECTS = 0x50F9B1;
ADDR K_STRUCTURE_RENDER_LIGHTMAPS                         = 0x555D30;
ADDR K_RENDER_WINDOW_CALL_HOOK_STRUCTURE_RENDER_LIGHTMAPS = 0x50F9BB;
ADDR K_WEATHER_PARTICLE_SYSTEMS_RENDER                    = 0x459400;
// ADDR K_RENDER_WINDOW_CALL_HOOK_WEATHER_PARTICLE_SYSTEMS_RENDER = 0x50FD3C;
ADDR K_RENDER_UI_WIDGETS                                  = 0x49B450;
ADDR K_RENDER_WINDOW_CALL_HOOK_RENDER_UI_WIDGETS          = 0x50FE6D;
ADDR K_RENDER_UI_CURSOR                                   = 0x49A2E0;
ADDR K_CALL_HOOK_RENDER_UI_CURSOR                         = 0x49B520;
ADDR K_INTERFACE_DRAW_SCREEN                              = 0x4974F0;
// ADDR K_RENDER_WINDOW_CALL_HOOK_INTERFACE_DRAW_SCREEN = 0x50FE61;
ADDR K_UPDATE_UI_WIDGETS                                  = 0x49AE10;
ADDR K_CALL_HOOK_UPDATE_UI_WIDGETS                        = 0x4CAF57;
ADDR K_QUERY_EXITFLAG_REG_CALL                            = 0x544B41;
ADDR K_QUERY_EXITFLAG_REG                                 = 0x581E40;
ADDR K_RELEASE_RESOURCES_ON_EXIT_CALL                     = 0x544D60;
ADDR K_RELEASE_RESOURCES_ON_EXIT                          = 0x544410;
ADDR K_WINMAIN_EXCEPTION_FILTER                           = 0x5463B0;
ADDR K_WINMAIN_EXCEPTION_FILTER_CALL                      = 0x544DE1;
ADDR K_RASTERIZER_DX9_SAVE_GAMMA                          = 0x525A00;
ADDR K_RASTERIZER_WINDOWS_PRESENT_FRAME                   = 0x51B980;
ADDR K_SOUND_STOP_ALL                                     = 0x54E200;
ADDR K_VIRTUALPROTECT_LOCK                                = 0x6BD1D8;
ADDR K_VIRTUALPROTECT_OLD_PROTECT                         = 0x6BD1DC;

ADDR K_PARAMS                                                                          = 0x75B760;
ADDR K_CAPS                                                                            = 0x75C420;
ADDR K_D3D                                                                             = 0x6B8410;
ADDR K_DEVICE                                                                          = 0x6B840C;
ADDR K_DINPUT8                                                                         = 0x64C52C;
ADDR K_DINPUT8DEVICEKEYBOARD                                                           = 0x64C730;
ADDR K_DINPUT8DEVICEMOUSE                                                              = 0x64C734;
ADDR K_DINPUT8DEVICEJOYSTICKS                                                          = 0x64C778;
ADDR K_VSF_TABLE_START_REFERENCE                                                       = 0x533812;
ADDR K_RENDER_OBJECT_LIST_HOOK                                                         = 0x512752;
ADDR K_RENDER_OBJECT_LIST_HOOK_RETN                                                    = 0x51275A;
ADDR K_RENDER_OBJECT_LIST_END_HOOK                                                     = 0x512A51;
ADDR K_RENDER_OBJECT_OBJECT_LOD_HOOK                                                   = 0x4DA526;
ADDR K_RENDER_OBJECT_OBJECT_LOD_HOOK_RETN                                              = 0x4DA52C;
ADDR K_FIRST_PERSON_WEAPON_DRAW_HOOK                                                   = 0x4952F3;
ADDR K_FIRST_PERSON_WEAPON_DRAW_HOOK_RETN                                              = 0x4952F8;
ADDR K_COMMAND_CAMERA_SET_HOOK                                                         = 0x4EBB38;
ADDR K_COMMAND_CAMERA_SET_HOOK_RETN                                                    = 0x4EBB3D;
ADDR K_COMMAND_SWITCH_BSP_HOOK                                                         = 0x54221B;
ADDR K_COMMAND_SWITCH_BSP_HOOK_RETN                                                    = 0x542222;
ADDR K_COMMAND_GAME_SAVE_HOOK                                                          = 0x482697;
ADDR K_COMMAND_GAME_SAVE_HOOK_RETN                                                     = 0x48269D;
ADDR K_RENDER_OBJECTS_TRANSPARENT                                                      = 0x518D40;
ADDR K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_HOOK                              = 0x50FE04;
ADDR K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_RETN                              = 0x50FE09;
ADDR K_RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK   = 0x51FDD6;
ADDR K_RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK = 0x51FB58;
ADDR K_RASTERIZER_DX9_EFFECT_COLLECTION                                                = 0x75B720;
ADDR K_RASTERIZER_EFFECT_SHADERS                                                       = 0x638AC8;
ADDR K_VSF_TABLE                                                                       = 0x639248;
ADDR K_RASTERIZER_D3D_CREATE_DEVICE_HOOK                                               = 0x51A9A2;
ADDR K_RASTERIZER_D3D_CREATE_DEVICE_RETN                                               = 0x51A9A7;
ADDR K_RASTERIZER_D3D_RESET_DEVICE_HOOK                                                = 0x51974F;
ADDR K_RASTERIZER_D3D_BEGIN_SCENE_CALL                                                 = 0x51AE37;
ADDR K_RASTERIZER_D3D_END_SCENE_CALL                                                   = 0x51B960;
ADDR K_RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_CALL                = 0x51C893;
ADDR K_RASTERIZER_SET_MODEL_TEX_SCALE_VERTEX_CONSTANT_CALL                             = 0x52DED3;
ADDR K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_0                          = 0x52DEED;
ADDR K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_1                          = 0x52B74D;
ADDR K_RASTERIZER_SET_MODEL_VERTEX_LIGHT_VERTEX_CONSTANT_CALL                          = 0x51C62E;

ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_0                              = 0x51C2F3;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_1                              = 0x52A0BF;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_2                              = 0x52A116;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_3                              = 0x52A16E;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_4                              = 0x52A1CC;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCE_5                              = 0x52A26B;
ADDR K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8_0                      = 0x51C26A;
ADDR K_RASTERIZER_GLOBALS_MAXIMUM_NODES_PER_MODEL_WRITE_VALUE_0             = 0x51AAC1;
ADDR K_RASTERIZER_CONFIG                                                    = 0x6BDDF8;
ADDR K_RASTERIZER_GLOBALS                                                   = 0x637CE8;
ADDR K_RASTERIZER_DEBUG_DATA                                                = 0x6249C0;
ADDR K_RASTERIZER_FRAME_PARAMS                                              = 0x75C570;
ADDR K_RASTERIZER_DISPOSE                                                   = 0x51BD00;
ADDR K_RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER                              = 0x51ACE0;
ADDR K_RASTERIZER_DISPOSE_CALL_FROM_SHELL                                   = 0x544459;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_0                            = 0x50FAAE;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_1                            = 0x50FC0C;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_2                            = 0x50FC4E;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_3                            = 0x50FC87;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_4                            = 0x50FCB9;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_5                            = 0x50FCE3;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_6                            = 0x50FD1E;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_7                            = 0x50FE2F;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_8                            = 0x50FE50;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_9                            = 0x555C40;
ADDR K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_10                           = 0x555D7C;
ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_0                         = 0x556D87;
ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_1                         = 0x556DF7;
ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_2                         = 0x556E5A;
ADDR K_MAXIMUM_RENDERED_TRIANGLES_UPGRADE_ADDRESS_3                         = 0x556F77;
ADDR K_RASTERIZER_DYNAMIC_GEOMETRY_INITIALIZE__CREATEINDEXBUFFER_LENGTH_ARG = 0x51F2C2;
ADDR K_RASTERIZER_DEVICE_ADAPTER_INDEX                                      = 0x6B8418;
ADDR K_RESOLUTION_LIST_COUNT                                                = 0x4BE1EC;
ADDR K_RESOLUTION_LIST_STRING_NULL_REFERENCE                                = 0x4BE22B;
ADDR K_RESOLUTION_LIST_SETUP_RESOLUTIONS_CALL                               = 0x4BE14F;
ADDR K_RESOLUTION_LIST_X_REFERENCE_0                                        = 0x4BE13A;
ADDR K_RESOLUTION_LIST_X_REFERENCE_1                                        = 0x4BE162;
ADDR K_RESOLUTION_LIST_X_REFERENCE_2                                        = 0x4BE20C;
ADDR K_RESOLUTION_LIST_X_REFERENCE_3                                        = 0x4BE3DC;
ADDR K_RESOLUTION_LIST_X_REFERENCE_4                                        = 0x4BE7C7;
ADDR K_RESOLUTION_LIST_Y_REFERENCE_0                                        = 0x4BE1CA;
ADDR K_RESOLUTION_LIST_Y_REFERENCE_1                                        = 0x4BE212;
ADDR K_RESOLUTION_LIST_Y_REFERENCE_2                                        = 0x4BE32C;
ADDR K_RESOLUTION_LIST_Y_REFERENCE_3                                        = 0x4BE35E;
ADDR K_RESOLUTION_LIST_Y_REFERENCE_4                                        = 0x4BE7DB;
ADDR K_RESOLUTION_LIST_STRING_REFERENCE_0                                   = 0x4BE206;
ADDR K_RESOLUTION_LIST_STRING_REFERENCE_1                                   = 0x4BEA8C;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_0                            = 0x4BE21D;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_1                            = 0x4BE241;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_2                            = 0x4BE264;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_3                            = 0x4BE285;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_4                            = 0x4BE396;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_5                            = 0x4BE3EE;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_6                            = 0x4BE80B;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_7                            = 0x4BEAB8;
ADDR K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCE_8                            = 0x4BEABE;
ADDR K_RESOLUTION_LIST_REFRESH_RATE_REFERENCE_0                             = 0x4BE17B;
ADDR K_RESOLUTION_LIST_REFRESH_RATE_REFERENCE_1                             = 0x4BE233;
ADDR K_RESOLUTION_LIST_REFRESH_RATE_REFERENCE_2                             = 0x4BE25E;
ADDR K_RESOLUTION_LIST_REFRESH_RATE_REFERENCE_3                             = 0x4BE291;
ADDR K_RESOLUTION_LIST_REFRESH_RATE_REFERENCE_4                             = 0x4BE3A6;
ADDR K_RESOLUTION_LIST_REFRESH_RATE_REFERENCE_5                             = 0x4BE81C;
ADDR K_RESOLUTION_LIST_REFRESH_RATE_REFERENCE_6                             = 0x4BEAFA;
ADDR K_NVIDIA_USE_BASIC_CAMO_TOGGLE                                         = 0x51A788;
ADDR K_SCREENSHOT_FOLDER_REFERENCE_0                                        = 0x4CD77C;
ADDR K_SCREENSHOT_FOLDER_REFERENCE_1                                        = 0x4CD793;
ADDR K_RENDER_WINDOW                                                        = 0x6B4C00;
ADDR K_RENDER_GLOBALS                                                       = 0x75E460;
ADDR K_GLOBAL_RENDER_TARGETS                                                = 0x638A10;
ADDR K_STRUCTURE_RENDER_GLOBALS                                             = 0x67EA98;
ADDR K_RENDER_WINDOW_REFLECTION_CALL                                        = 0x50F680;
ADDR K_RENDER_WINDOW_REFLECTION_CALL_RETN                                   = 0x50F685;
ADDR K_RENDER_WINDOW_END_HOOK                                               = 0x51B958;
ADDR K_BSP_LIGHTMAP_INDEX_HOOK                                              = 0x5562DA;
ADDR K_BSP_LIGHTMAP_INDEX_RETN                                              = 0x5562E0;
ADDR K_SET_LIGHTMAP_SAMPLER_CALL                                            = 0x522B6C;
ADDR K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK                          = 0x4F5F84;
ADDR K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN                          = 0x4F5F8A;
ADDR K_FOG_ATMOSPHERIC_SKY_TAG_INDEX_HOOK                                   = 0x541C5F;
ADDR K_FOG_ATMOSPHERIC_SKY_TAG_INDEX_RETN                                   = 0x541C66;
ADDR K_FOG_PLANAR_SKY_TAG_INDEX_HOOK                                        = 0x5587BC;
ADDR K_FOG_PLANAR_SKY_TAG_INDEX_RETN                                        = 0x5587C2;
ADDR K_RENDER_SKY_SKY_TAG_INDEX_HOOK                                        = 0x5144FE;
ADDR K_RENDER_SKY_SKY_TAG_INDEX_RETN                                        = 0x514505;
ADDR K_VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_HOOK                                = 0x556960;
ADDR K_VISIBLE_SKY_ENABLE_SKY_TAG_INDEX_RETN                                = 0x55696A;

ADDR K_RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP_SHADER_DRAW__DRAW_SHADER_LIGHTMAP_PTR = 0x51A0E4;
ADDR K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_ENVIRONMENT_PTR = 0x52E735;
ADDR K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_MODEL_PTR       = 0x52E73F;
ADDR K_RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP                                       = 0x522970;
ADDR K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_ENVIRONMENT                       = 0x52B340;
ADDR K_RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_MODEL                             = 0x52D060;
ADDR K_RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK                   = 0x52DFC5;
ADDR K_RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN                   = 0x52DFCB;
ADDR K_RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL             = 0x532C94;
ADDR K_VSH_COLLECTION_PATH_REFERENCE_0                                            = 0x5336FE;
ADDR K_VSH_COLLECTION_PATH_REFERENCE_1                                            = 0x533799;
ADDR K_PS_VERSION_ITERATOR_START_0                                                = 0x532C28;
ADDR K_PS_VERSION_ITERATOR_START_1                                                = 0x533152;
ADDR K_SHADER_USAGE_ID_ARRAY_REFERENCE_0                                          = 0x529D7D;
ADDR K_SHADER_USAGE_ID_ARRAY_REFERENCE_1                                          = 0x529DF5;
ADDR K_SHADER_USAGE_ID_ARRAY_REFERENCE_2                                          = 0x529E74;
ADDR K_SHADER_USAGE_ID_ARRAY_REFERENCE_3                                          = 0x529EF4;
ADDR K_SHADER_USAGE_ID_ARRAY_REFERENCE_4                                          = 0x529F95;
ADDR K_SHADER_USAGE_ID_ARRAY_REFERENCE_5                                          = 0x529D27;
ADDR K_RASTERIZER_MODEL_SHADER_LOAD_COUNT                                         = 0x529C4E;
ADDR K_RASTERIZER_MODEL_SHADER_LOAD_NO_INVERSE_COUNT                              = 0x529C53;
ADDR K_RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_HOOK                       = 0x529D18;
ADDR K_RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_RETN                       = 0x529D25;
ADDR K_RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_HOOK                                   = 0x529F86;
ADDR K_RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_RETN                                   = 0x529F93;
ADDR K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO_0                                   = 0x529C97;
ADDR K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO_1                                   = 0x529D59;
ADDR K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR_0                                     = 0x529CB7;
ADDR K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR_1                                     = 0x529E36;
ADDR K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE_0                                     = 0x529CC7;
ADDR K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE_1                                     = 0x529EB6;
ADDR K_PIXEL_SHADER_REFERENCES_NO_0                                               = 0x529CED;
ADDR K_PIXEL_SHADER_REFERENCES_NO_1                                               = 0x529FC5;
ADDR K_PIXEL_SHADER_REFERENCES_REFLECTION_0                                       = 0x529CD7;
ADDR K_PIXEL_SHADER_REFERENCES_REFLECTION_1                                       = 0x529F36;
ADDR K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION_0                                = 0x529CA7;
ADDR K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION_1                                = 0x529DB7;
ADDR K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_HOOK                              = 0x52B52A;
ADDR K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_RETN                              = 0x52B531;
ADDR K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_HOOK                          = 0x52B51A;
ADDR K_RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_RETN                          = 0x52B521;
ADDR K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_HOOK                                = 0x52A951;
ADDR K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_RETN                                = 0x52A958;
ADDR K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_HOOK                            = 0x52A948;
ADDR K_RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_RETN                            = 0x52A94F;
ADDR K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_HOOK                                = 0x52A963;
ADDR K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_RETN                                = 0x52A96A;
ADDR K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_HOOK                            = 0x52A95A;
ADDR K_RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_RETN                            = 0x52A961;
ADDR K_RASTERIZER_MODEL_PS_INDEX_NO_HOOK                                          = 0x52A91B;
ADDR K_RASTERIZER_MODEL_PS_INDEX_NO_RETN                                          = 0x52A922;
ADDR K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_HOOK                                  = 0x52A93F;
ADDR K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_RETN                                  = 0x52A946;
ADDR K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_HOOK                              = 0x52A936;
ADDR K_RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_RETN                              = 0x52A93D;
ADDR K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_HOOK                           = 0x52A92D;
ADDR K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_RETN                           = 0x52A934;
ADDR K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_HOOK                       = 0x52A924;
ADDR K_RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_RETN                       = 0x52A92B;
ADDR K_RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_HOOK                  = 0x522BD1;
ADDR K_RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_RETN                  = 0x522BD7;
ADDR K_STRUCTURE_RENDER_DYNAMIC_TRIANGLES_COUNT_REF                               = 0x7EB6F4;
ADDR K_STRUCTURE_RENDER_DYNAMIC_TRIANGLES_BUFFER_REF                              = 0x555D7C;
ADDR K_STRUCTURE_RENDER_PASS_FUNC                                                 = 0x556220;
ADDR K_LIGHTS_RENDER_SPECULAR_CALL                                                = 0x50FBBA;
ADDR K_LIGHTS_RENDER_SPECULAR_FUNC                                                = 0x4F5260;
ADDR K_RASTERIZER_ENVIRONMENT_LIGHTMAP_BEGIN_FUNC                                 = 0x515790;
ADDR K_RASTERIZER_ENVIRONMENT_LIGHTMAP_DRAW_FUNC                                  = 0x522970;
ADDR K_RASTERIZER_EFFECT_PARTICLE_SYSTEM_HOOK                                     = 0x51512B;
ADDR K_RASTERIZER_EFFECT_PARTICLE_SYSTEM_RETN                                     = 0x515131;
ADDR K_RASTERIZER_EFFECT_PARTICLE_HOOK                                            = 0x536EF1;
ADDR K_RASTERIZER_EFFECT_PARTICLE_RETN                                            = 0x536EF6;
ADDR K_RASTERIZER_EFFECT_CONTRAIL_HOOK                                            = 0x51FFB3;
ADDR K_RASTERIZER_EFFECT_CONTRAIL_RETN                                            = 0x51FFB9;
ADDR K_RASTERIZER_EFFECT_PS_INDEX_NO_HOOK                                         = 0x5374CC;
ADDR K_RASTERIZER_EFFECT_PS_INDEX_NO_RETN                                         = 0x5374D2;
ADDR K_SCENARIO_GLOBALS                                                           = 0x6E2254;
ADDR K_GLOBAL_SCENARIO                                                            = 0x6E224C;
ADDR K_GLOBAL_BSP3D                                                               = 0x6E2250;
ADDR K_GLOBAL_COLLISION_BSP                                                       = 0x6E2258;
ADDR K_GLOBAL_STRUCTURE_BSP                                                       = 0x6E225C;
ADDR K_GLOBAL_SCENARIO_INDEX                                                      = 0x6397CC;
ADDR K_STRUCTURE_BSP_INDEX                                                        = 0x6397D0;
ADDR K_OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ                        = 0x4F8207;
ADDR K_OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE                       = 0x4F83CE;
ADDR K_OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS                          = 0x4F84E2;
ADDR K_CACHE_FILES_INITIALIZE                                                     = 0x442FF0;
ADDR K_CACHE_FILES_INITIALIZE_HOOK                                                = 0x442FFB;
ADDR K_CACHE_FILES_INITIALIZE_RETN                                                = 0x4432E3;
ADDR K_CACHE_FILES_DISPOSE                                                        = 0x56C650;
ADDR K_CACHE_FILES_DISPOSE_HOOK                                                   = 0x56C698;
ADDR K_CACHE_FILES_DISPOSE_RETN                                                   = 0x56C8B2;
ADDR K_DATA_FILE_READ                                                             = 0x444420;
ADDR K_SCENARIO_TAGS_LOAD_HOOK                                                    = 0x4423AD;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_0                                           = 0x442364;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_1                                           = 0x442B65;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_2                                           = 0x53BD62;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_3                                           = 0x443FAC;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_4                                           = 0x444114;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_5                                           = 0x444141;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_6                                           = 0x444164;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_7                                           = 0x44412A;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_8                                           = 0x444159;
ADDR K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_9                                           = 0x44417A;
ADDR K_CACHE_PATH_SPRINTF_CALL_0                                                  = 0x53BCEF;
ADDR K_CACHE_PATH_SPRINTF_CALL_1                                                  = 0x443CFB;
ADDR K_CACHE_PATH_SPRINTF_CALL_2                                                  = 0x4CCB6E;
ADDR K_CACHE_FILE_READ_HEADER_CALL_0                                              = 0x443CA8;
ADDR K_CACHE_FILE_READ_HEADER_CALL_1                                              = 0x49800D;
ADDR K_CACHE_FILE_READ_HEADER_CALL_2                                              = 0x4A07D3;
ADDR K_UI_SCENARIO_NAME_REFERENCE_0                                               = 0x4CBC9F;
ADDR K_UI_SCENARIO_NAME_REFERENCE_1                                               = 0x4CBCC4;
ADDR K_CACHE_FILE_GLOBALS                                                         = 0x643040;
ADDR K_MAPS_FOLDER_PARENT_DIR                                                     = 0x68C698;
ADDR K_MULTIPLAYER_MAPS                                                           = 0x6ADE14;
ADDR K_MULTIPLAYER_MAP_LIST_INITIALIZE                                            = 0x4980E0;
ADDR K_MULTIPLAYER_MAP_LIST_DISPOSE                                               = 0x498160;
ADDR K_TAG_INSTANCES                                                              = 0x817144;
ADDR K_GAME_BUILD_VERSION_GAMESPY                                                 = 0x6BDCF8;

// #undef K_HUD_RENDER_UNIT_INTERFACE_JMP_TABLE
// #define K_HUD_RENDER_UNIT_INTERFACE_JMP_TABLE 0x4B4894
//
// #define K_FUNC4F7440JMPTABLE 0x4F72CC
// #define K_FUNC4F7580JMPTABLE 0x4F73CC
//
// #undef K_GAME_SCENARIO_SWITCH_BSP_CALL_0
// #define K_GAME_SCENARIO_SWITCH_BSP_CALL_0 0x4821E0
//
// #undef K_GAME_CREATE_NETWORK_CLIENT_CALL_4
// #define K_GAME_CREATE_NETWORK_CLIENT_CALL_4 0x4DFD3F
//
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN 0x47B008
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_PTR 0x47AFE8
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE 0x47AFE2
//
// #undef K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE 0x47B014
//
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN 0x47B108
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_PTR 0x47B0E8
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE 0x47B0E2
//
// #undef K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE
// #define K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE 0x47B114
//
// #undef K_PLAYER_HANDLE_ACTION_JMP_TABLE
// #define K_PLAYER_HANDLE_ACTION_JMP_TABLE 0x47C228
//
// #undef K_HUD_UPDATE
// #define K_HUD_UPDATE 0x4AC9A0
//
// #undef K_FIRST_PERSON_WEAPONS_UPDATE
// #define K_FIRST_PERSON_WEAPONS_UPDATE 0x495190
//
// #undef K_HUD_RENDER_NAV_POINTS_CALL
// #define K_HUD_RENDER_NAV_POINTS_CALL 0x4ACAF3
//
// #undef K_HUD_RENDER_NAV_POINT_ANCHOR_HALF_X
// #define K_HUD_RENDER_NAV_POINT_ANCHOR_HALF_X 0x4B2B06
//
// #undef K_HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y
// #define K_HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y 0x4B2B10
//
// #undef K_HUD_ANCHOR_SCREEN_Y
// #define K_HUD_ANCHOR_SCREEN_Y 0x612264
//
// #undef K_HUD_ANCHOR_SCREEN_HALF_X
// #define K_HUD_ANCHOR_SCREEN_HALF_X 0x6128E4
//
// #undef K_HUD_ANCHOR_SCREEN_HALF_Y
// #define K_HUD_ANCHOR_SCREEN_HALF_Y 0x6128E8
//
// #undef K_HUD_RENDER_PLAYERS_CALL
// #define K_HUD_RENDER_PLAYERS_CALL 0x4ACA74
//
// #undef K_HUD_POINT_DAMAGE_ANCHOR_WIDTH
// #define K_HUD_POINT_DAMAGE_ANCHOR_WIDTH 0x4B49A9
//
// #undef K_HUD_POINT_DAMAGE_ANCHOR_HEIGHT
// #define K_HUD_POINT_DAMAGE_ANCHOR_HEIGHT 0x4B4982
//
// #undef K_HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT
// #define K_HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT 0x4B49C3
//
// #undef K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTR_0
// #define K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTR_0 0x4B499E
//
// #undef K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTR_1
// #define K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTR_1 0x4B495E


ADDR K_MESSAGE_DELTA_FIELD_INTEGER_SMALL                            = 0x6229B8;
ADDR K_MESSAGE_DELTA_FIELD_INTEGER_MEDIUM                           = 0x622A28;
ADDR K_MESSAGE_DELTA_FIELD_INTEGER_LARGE                            = 0x622600;


ADDR K_MESSAGE_DELTA_FIELD_REAL                                     = 0x622770;
ADDR K_MESSAGE_DELTA_FIELD_BOOLEAN                                  = 0x622950;
ADDR K_MESSAGE_DELTA_FIELD_ASCII_CHARACTER                          = 0x635E10;
ADDR K_MESSAGE_DELTA_FIELD_TIME                                     = 0x635F58;
ADDR K_MESSAGE_DELTA_FIELD_GRENADE_COUNTS                           = 0x635C48;

ADDR K_MESSAGE_DELTA_FIELD_LOCALITY_REFERENCE_POSITION              = 0x6314B0;
ADDR K_MESSAGE_DELTA_FIELD_DIGITAL_THROTTLE                         = 0x633650;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_WEAPON_INDEX                 = 0x633728;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_GRENADE_INDEX                = 0x633790;
ADDR K_MESSAGE_DELTA_FIELD_ITEM_PLACEMENT_POSITION                  = 0x635FC0;


ADDR K_MESSAGE_DELTA_FIELD_LOCAL_PLAYER_UPDATE_SEQUENCE_NUMBER      = 0x634EB8;
ADDR K_MESSAGE_DELTA_FIELD_REMOTE_PLAYER_UPDATE_SEQUENCE_NUMBER     = 0x634F98;
ADDR K_MESSAGE_DELTA_FIELD_REMOTE_PLAYER_ACTION_UPDATE_BASELINE_ID  = 0x634F28;
ADDR K_MESSAGE_DELTA_FIELD_UPDATE_TICK_COUNT                        = 0x633558;
ADDR K_MESSAGE_DELTA_FIELD_UPDATE_ID                                = 0x6334E8;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_3BITS                        = 0x635CB8;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_6BITS                        = 0x631740;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_1BIT                         = 0x6336C0;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_4BIT                  = 0x635D30;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_8BIT                  = 0x630A68;
ADDR K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_16BIT                 = 0x635DA8;

ADDR K_MESSAGE_DELTA_FIELD_DEFINITION_INDEX                         = 0x62E4D0;

ADDR K_MESSAGE_DELTA_FIELD_POINT2D                                  = 0x635E80;
ADDR K_MESSAGE_DELTA_FIELD_POINT3D                                  = 0x630AD8;

ADDR K_MESSAGE_DELTA_FIELD_VECTOR2D                                 = 0x635EF0;
ADDR K_MESSAGE_DELTA_FIELD_VECTOR3D                                 = 0x622708;

ADDR K_MESSAGE_DELTA_FIELD_CONTROL_FLAGS                            = 0x6335E8;
ADDR K_MESSAGE_DELTA_FIELD_ANGULAR_VELOCITY                         = 0x6311E8;
ADDR K_MESSAGE_DELTA_FIELD_TRANSLATIONAL_VELOCITY                   = 0x630E60;

ADDR K_MESSAGE_DELTA_FIELD_MAP_NAME                                 = 0x6339D8;

ADDR K_MESSAGE_DELTA_FIELD_NETWORK_MAP                              = 0x633A68;
ADDR K_MESSAGE_DELTA_FIELD_NETWORK_PLAYER_NAME                      = 0x633AD8;

ADDR K_MESSAGE_DELTA_FIELD_NETWORK_PLAYER                           = 0x633BB8;
ADDR K_MESSAGE_DELTA_FIELD_NETWORK_GAME_NAME                        = 0x633C28;
ADDR K_MESSAGE_DELTA_FIELD_NETWORK_GAME_PLAYERS                     = 0x633CA0;
ADDR K_MESSAGE_DELTA_FIELD_GAME_VARIANT                             = 0x622F78;
ADDR K_MESSAGE_DELTA_FIELD_GAME_VARIANT_HUMAN_READABLE_DESCRIPTION  = 0x622E28;
ADDR K_MESSAGE_DELTA_FIELD_UNIVERSAL_VARIANT                        = 0x622D40;
ADDR K_MESSAGE_DELTA_FIELD_UNIVERSAL_VARIANT_FLAGS                  = 0x622B50;
ADDR K_MESSAGE_DELTA_FIELD_GAME_ENGINE_VARIANT                      = 0x622DB8;
ADDR K_MESSAGE_DELTA_FIELD_GAME_VARIANT_FLAGS                       = 0x622EB8;

ADDR K_MESSAGE_DELTA_FIELD_PARAMETERS_PROTOCOL_ARRAY                = 0x6367F8;
ADDR K_MESSAGE_DELTA_FIELD_HUD_CHAT_MESSAGE                         = 0x62E388;
ADDR K_MESSAGE_DELTA_FIELD_HUD_CHAT_MESSAGE_PTR                     = 0x62E3F8;
ADDR K_MESSAGE_DELTA_FIELD_OBJECT_CHANGE_COLORS                     = 0x636520;

ADDR K_MESSAGE_DELTA_FIELD_CTF_SCORE_ARRAY                          = 0x6233A0;
ADDR K_MESSAGE_DELTA_FIELD_RACE_SCORE_ARRAY                         = 0x623AC8;
ADDR K_MESSAGE_DELTA_FIELD_KING_SCORE_ARRAY                         = 0x623548;

ADDR K_MESSAGE_DELTA_FIELD_ODDBALL_SCORE_ARRAY                      = 0x623970;
ADDR K_MESSAGE_DELTA_FIELD_ODDBALL_OWNER_ARRAY                      = 0x6239E8;
ADDR K_MESSAGE_DELTA_FIELD_SLAYER_SCORE_ARRAY                       = 0x623BC8;
ADDR K_MESSAGE_DELTA_FIELD_DAMAGE_DATA_FLAGS                        = 0x622810;

ADDR K_MESSAGE_DELTA_FIELD_MOTD_TEXT                                = 0x634E00;
ADDR K_MESSAGE_DELTA_FIELD_RCON_RESPONSE                            = 0x634D48;
ADDR K_MESSAGE_DELTA_FIELD_RCON_PASSWORD                            = 0x634C10;
ADDR K_MESSAGE_DELTA_FIELD_RCON_COMMAND                             = 0x634C80;
ADDR K_MESSAGE_DELTA_EMPTY_FIELD_SET                                = 0x6225E0;

ADDR K_MESSAGE_DELTA_GAME_OBJECT_HEADER_FIELD_SET                   = 0x631250;
ADDR K_MESSAGE_DELTA_GAME_OBJECT_TIMESTAMPED_HEADER_FIELD_SET       = 0x636050;
ADDR K_MESSAGE_DELTA_BIPED_UPDATE_HEADER_FIELD_SET                  = 0x636430;
ADDR K_MESSAGE_DELTA_CTF_UPDATE_HEADER_FIELD_SET                    = 0x623408;
ADDR K_MESSAGE_DELTA_CLIENT_GAME_UPDATE_HEADER_FIELD_SET            = 0x6338DC;
ADDR K_MESSAGE_DELTA_PLAYER_SCORE_UPDATE_HEADER_FIELD_SET           = 0x6230DC;
ADDR K_MESSAGE_DELTA_REMOTE_PLAYER_ACTION_UPDATE_HEADER_FIELD_SET   = 0x635120;
ADDR K_MESSAGE_DELTA_REMOTE_PLAYER_POSITION_UPDATE_HEADER_FIELD_SET = 0x6352B8;
ADDR K_MESSAGE_DELTA_REMOTE_PLAYER_TOTAL_UPDATE_HEADER_FIELD_SET    = 0x6353E0;
ADDR K_NETWORK_SV_GLOBALS                                           = 0x6B7294;

// #undef K_NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER
// #define K_NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER 0x4E2D4B
//
// #undef K_NETWORK_GAME_REMOVE_PLAYER
// #define K_NETWORK_GAME_REMOVE_PLAYER 0x4E1A90
//
// #undef K_CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER
// #define K_CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER 0x4E44E1
//
// #undef K_MASTER_ADDR_REFERENCE
// #define K_MASTER_ADDR_REFERENCE 0x5BB247
//
// #undef K_MASTER_ADDR_SB_REFERENCE
// #define K_MASTER_ADDR_SB_REFERENCE 0x5C3752
//
// #undef K_UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE
// #define K_UNIT_UPDATE_ANIMATION_PRIMARY_KEYFRAME_JMP_TABLE 0x568D6C
//
// #undef K_QUERY_EXITFLAG_REG_CALL
// #define K_QUERY_EXITFLAG_REG_CALL 0x544B3F
//
// #undef K_RELEASE_RESOURCES_ON_EXIT_CALL
// #define K_RELEASE_RESOURCES_ON_EXIT_CALL 0x544D5E
//
// #undef K_WINMAIN_EXCEPTION_FILTER_CALL
// #define K_WINMAIN_EXCEPTION_FILTER_CALL 0x544DDF
//
// #undef K_FIRST_PERSON_WEAPON_DRAW_HOOK
// #define K_FIRST_PERSON_WEAPON_DRAW_HOOK 0x4952FE
//
// #undef K_FIRST_PERSON_WEAPON_DRAW_HOOK_RETN
// #define K_FIRST_PERSON_WEAPON_DRAW_HOOK_RETN 0x495303
//
// #undef K_COMMAND_CAMERA_SET_HOOK
// #define K_COMMAND_CAMERA_SET_HOOK 0x4455AC
//
// #undef K_COMMAND_CAMERA_SET_HOOK_RETN
// #define K_COMMAND_CAMERA_SET_HOOK_RETN 0x4455B1
//
// #undef K_RASTERIZER_D3D_CREATE_DEVICE_HOOK
// #define K_RASTERIZER_D3D_CREATE_DEVICE_HOOK 0x51A992
//
// #undef K_RASTERIZER_D3D_CREATE_DEVICE_RETN
// #define K_RASTERIZER_D3D_CREATE_DEVICE_RETN 0x51A997
//
// #undef K_RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER
// #define K_RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER 0x51ACD0
//
// #define K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_11 0x555E66
// #define K_RENDERED_TRIANGLE_ARRAY_UPGRADE_ADDRESS_12 0x555F46
//
// #define K_RASTERIZER_ENVIRONMENT_LIGHTMAP_END_FUNC 0x44B900
//
//
// #undef K_CACHE_FILES_DISPOSE
// #define K_CACHE_FILES_DISPOSE 0x443310
//
// #undef K_CACHE_FILES_DISPOSE_HOOK
// #define K_CACHE_FILES_DISPOSE_HOOK 0x44334F
//
// #undef K_CACHE_FILES_DISPOSE_RETN
// #define K_CACHE_FILES_DISPOSE_RETN 0x44340A
//
// #undef K_SCENARIO_TAGS_LOAD_HOOK
// #define K_SCENARIO_TAGS_LOAD_HOOK 0x4423A7
//
// #undef K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_0
// #define K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_0 0x442373
//
// #undef K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_1
// #define K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_1 0x442B71
//
// #undef K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_2
// #define K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_2 0x443526
//
// #undef K_CACHE_PATH_SPRINTF_CALL_0
// #define K_CACHE_PATH_SPRINTF_CALL_0 0x4434BF
