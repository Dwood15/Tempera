#pragma once

#include <precompile.h>
#include <rpcndr.h>
#include <cstdint>
#include "../../../../../../../BlamSauce/effects/particle/particle_systems.hpp"
#include "../../../../../../../BlamSauce/effects/contrail/contrails.hpp"
#include "../../../../../../../BlamSauce/memory/memory_interface_base.hpp"
#include "../../../../../../../BlamSauce/ai/actors/actor_structures.hpp"
#include "../../../../../../../BlamSauce/effects/effects.h"
#include "../../../../../../../BlamSauce/ai/ai_structures.h"
#include "../../../../../../../BlamSauce/ai/prop_structures.h"
#include "../../../../../../../BlamSauce/ai/encounters.h"
#include "../../../../../../../BlamSauce/game/engines/engine_ctf.hpp"
#include "../../../../../../../BlamSauce/game/engines/engine_race.hpp"
#include "../../../../../../../BlamSauce/game/engines/engine_king.hpp"
#include "../../../../../../../BlamSauce/game/engines/engine_oddball.hpp"
#include "../../../../../../../BlamSauce/game/engines/engine_slayer.hpp"
#include "../../../../../../../BlamSauce/game/engines/variants.h"
#include "../../../../../../../BlamSauce/game/engines/engine.hpp"
#include "../../../../../../../BlamSauce/cseries/base.h"
#include "../../../../../../../BlamSauce/interface/camera/camera.h"

#ifdef ENGINE_DPTR
#undef ENGINE_DPTR
#endif

#ifdef ENGINE_PTR
#undef ENGINE_PTR
#endif

#ifdef FUNC_PTR
#undef FUNC_PTR
#endif

#define ENGINE_PTR(type, name, ...) static auto* const name = CAST_PTR(type*, __VA_ARGS__);
#define DUO_PTR(...) __VA_ARGS__
#define CAST_PTR(type, ptr)      reinterpret_cast<type>(ptr)


#define K_HUD_RENDER_UNIT_INTERFACE_JMP_PTR                             0x4B466C
#define K_INIT_TXT_FILENAME                                              0x60AA9C
//////////////////////////////////////////////////////////////////////////
// AI.cpp
using namespace Yelo::AI;
static auto **const ai_globals = reinterpret_cast<Yelo::AI::s_ai_globals_data **>(0x81B894);
static auto **const actors = reinterpret_cast<actor_data_t **>(0x81B8A0);
static auto **const swarms = reinterpret_cast<swarm_data_t **>(0x81B89C);
static auto **const swarm_components = reinterpret_cast<swarm_component_data_t **>(0x81B898);
static auto **const props = reinterpret_cast<prop_data_t **>(0x81B800);
static auto **const encounters = reinterpret_cast<encounter_data_t **>(0x81B808);
static auto **const squads = reinterpret_cast<squads_data_t **>(0x81B80C);
static auto **const platoons = reinterpret_cast<platoons_data_t **>(0x81B804);
static auto **const ai_pursuits = reinterpret_cast<ai_pursuit_data_t **>(0x81B810);
	//ai_communication_dialogue_events_t
	static auto **const ai_communication_replies = reinterpret_cast<ai_communication_reply_events_t **>(0x68BC5C);;
static auto **const ai_conversations = reinterpret_cast<ai_conversation_data_t **>(0x81B814);;

static auto const AI_UPDATE_HOOK = 								 0x42A949;
static auto const ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK = 0x40B105;
static auto const ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN = 0x40B10A;
static auto const PROP_STATUS_REFRESH_HOOK =					 0x41C9CE;
static auto const PROP_STATUS_REFRESH_RETN =					 0x41C9D4;
static auto const ACTOR_INPUT_UPDATE_HOOK =					 0x429CA7;
static auto const ACTOR_INPUT_UPDATE_RETN =					 0x429CAE;


//////////////////////////////////////////////////////////////////////////
// Camera.cpp
namespace Camera
{
	static auto *const observers = 0x64758C;
	static auto *const camera_script_globals = 0x621F90;
	static auto **const director_scripting = reinterpret_cast<Yelo::Camera::s_director_scripting_data **>(0x81713C);
	static auto **const cinematic_globals = reinterpret_cast<Yelo::Camera::s_cinematic_globals_data **>(0x68C83C);
	static auto *const global_director = 0x647490;

	static auto const DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK = 0x446585;

	static auto const OBSERVER_UPDATE_COMMAND_HOOK = 0x4484BA;

	static auto const DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET = 0x44644F;
	static auto const HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD = 0x4AC8EC;
	static auto const HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG = 0x4B184F;
};

namespace Fov
{
	static auto const OBSERVER_UPDATE_COMMAND_HOOK = 0x4484BA;

	static auto const OBSERVER_UPDATE_POSITIONS = 0x448B80;
	static auto *const OBSERVER_UPDATE_POSITIONS_no_scope_blur = 0x49737F;
	// Address of the call opcodes to [OBSERVER_UPDATE_POSITIONS] inside the function
	// known as [OBSERVER_TICK]
	static auto const OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS = 0x448509;

	static auto *const MAX_FOV_CHECK_JMP = 0x44930D;
};

//////////////////////////////////////////////////////////////////////////
// Campaign.cpp. CLIENT BUILDS ONLY
	#pragma region Scenario Paths
	static const char *** ScenarioPathsReferences[] = {
		reinterpret_cast<const char * **>(0x49F9E3),
		reinterpret_cast<const char * **>(0x49FB43),
		reinterpret_cast<const char * **>(0x49FEEB),
		reinterpret_cast<const char * **>(0x49FFC0),
		reinterpret_cast<const char * **>(0x4A4A3B),
		//CAST_PTR(const char ***, K_SCENARIO_PATHS_REFERENCE_5),
	};

	static const char *** ScenarioPathsReferences2[] = {
		reinterpret_cast<const char * **>(0x4A7205),
		reinterpret_cast<const char * **>(0x4CCFCC),
		reinterpret_cast<const char * **>(0x53C4CB),
		reinterpret_cast<const char * **>(0x53C594),
	};
	#pragma endregion

	#pragma region Scenario Help
static auto const DISPLAY_SCENARIO_HELP_HOOK = 0x49C1EA;
static auto const DISPLAY_SCENARIO_HELP_SUCCESS = 0x49C316;
static auto const DISPLAY_SCENARIO_HELP_FAIL = 0x49C369;
	#pragma endregion

	#pragma region Campaign Level Data
	static void** CampaignLevelDataReferences[] = {
		reinterpret_cast<void **>(0x49F8EB),
		reinterpret_cast<void **>(0x49F9EA),
		reinterpret_cast<void **>(0x49FB1E),
		reinterpret_cast<void **>(0x49FD69),
		//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_4),
		//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_5),
		//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_6),
	};

	static size_t* CampaignLevelDataSizeReferences[] = {
		reinterpret_cast<size_t *>(0x49F8E6),
		reinterpret_cast<size_t *>(0x49FC26),
		reinterpret_cast<size_t *>(0x49FD64),
	};
	#pragma endregion

	#pragma region Campaign Level Count
	static byte* CampaignLevelCountReferences_8bit[] = {
		reinterpret_cast<byte *>(0x4CCFB9),
		//CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_1),
	};

	static short* CampaignLevelCountReferences_16bit[] = {
		reinterpret_cast<short *>(0x49F9B1),
	};

	static long* CampaignLevelCountReferences_32bit[] = {
		reinterpret_cast<long *>(0x49F9B9),
		//CAST_PTR(long*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_1),
	};

	//////////////////////////////////////////////////////////////////////////

	static byte* CampaignLevelCountReferences2_8bit[] = {
		reinterpret_cast<byte *>(0x49FB04),
		reinterpret_cast<byte *>(0x49FB5C),
		reinterpret_cast<byte *>(0x49FB94),
		reinterpret_cast<byte *>(0x4CCF87),
		reinterpret_cast<byte *>(0x4CCFC6),
		reinterpret_cast<byte *>(0x53C4C2),
		//CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_6),
	};

	static short* CampaignLevelCountReferences2_16bit[] = {
		reinterpret_cast<short *>( 0x49FB26),
		reinterpret_cast<short *>( 0x53C58A),
		//CAST_PTR(short*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_2),
	};
	#pragma endregion


//////////////////////////////////////////////////////////////////////////
// Console.cpp
static auto **const terminal_globals = reinterpret_cast<s_terminal_globals **>(0x64DE2C);;
static auto *const console_globals = reinterpret_cast<s_console_globals *>(0x651F50);;

static auto const CONSOLE_UPDATE_HOOK = 0x4C9A63;


//////////////////////////////////////////////////////////////////////////
// Effects.cpp
static auto **const contrails = reinterpret_cast<contrail_data_t **>(0x81610C);;
static auto **const contrail_points = reinterpret_cast<contrail_point_data_t **>(0x816108);;
static auto **const particles = reinterpret_cast<particle_data_t **>(0x8160F0);;
static auto **const effects = reinterpret_cast<Yelo::Effects::effect_data_t **>(0x8160FC);;
static auto **const effect_locations = reinterpret_cast<Yelo::Effects::effect_location_data_t **>(0x816100);;
static auto **const particle_systems = reinterpret_cast<particle_systems_data_t **>(0x8160F8);;
static auto **const particle_system_particles = reinterpret_cast<particle_system_particles_data_t **>(0x8160F4);;
static auto **const structure_detail_objects = reinterpret_cast<Yelo::Effects::s_structure_detail_objects_data **>(0x6BDA4C);;
static auto **const structure_decals = reinterpret_cast<Yelo::Effects::s_structure_decals_data **>(0x6BDA5C);;
static auto **const breakable_surface_globals = reinterpret_cast<Yelo::Effects::s_breakable_surface_globals_data **>(0x653CA8);;
static auto **const decals = reinterpret_cast<Yelo::Effects::decals_data_t **>(0x816104);;
static auto **const decal_globals = reinterpret_cast<Yelo::Effects::s_decal_globals_data **>(0x64BA08);;
static auto **const decal_vertex_cache = reinterpret_cast<Yelo::Effects::s_decal_vertex_cache_data **>(0x6B8458);;

static auto const EFFECTS_UPDATE_HOOK = 0x451487;

		static void* K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[] = {
			reinterpret_cast<void *>(0x5136BA),
			reinterpret_cast<void *>(0x51374B),
			reinterpret_cast<void *>(0x513752),
			reinterpret_cast<void *>(0x513779),
			reinterpret_cast<void *>(0x5137CB)
		};

static auto const GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES = 0x45B4B7;
static auto const GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS = 0x45B4CB;
static auto const GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS = 0x45B4E4;
static auto const GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES = 0x45B52C;


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp


//////////////////////////////////////////////////////////////////////////
// GameBuildNumber.cpp
static auto *const game_build_version = reinterpret_cast<char *>(0x60A370);;
static auto *const game_build_version_gamespy = reinterpret_cast<char *>(K_GAME_BUILD_VERSION_GAMESPY);;

static auto *const network_version_id1 = reinterpret_cast<Yelo::Enums::network_game_protocol_id *>(0x4DC196);;
static auto *const network_version_id2 = reinterpret_cast<Enums::network_game_protocol_id *>(0x4DC373);;
static auto *const network_version_id3 = reinterpret_cast<Enums::network_game_protocol_id *>(0x4E3CF3);;

static auto const GAME_STATE_HEADER_IS_VALID_HOOK = 0x53B7E7;
static auto const GAME_STATE_HEADER_IS_VALID_HOOK_RET = 0x53B83B;
static auto const GAME_STATE_HEADER_TRY_AND_LOAD_HOOK = 0x53B59E;
static auto const GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE = 0x53B5FE;
static auto const GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE = 0x53B6B9;


//////////////////////////////////////////////////////////////////////////
// GameEngine.cpp
static auto *const ctf_globals = reinterpret_cast<Yelo::GameEngine::s_ctf_globals *>(0x64BDB8);;
static auto *const slayer_globals = reinterpret_cast<Yelo::GameEngine::s_slayer_globals *>(0x64C308);;
static auto *const oddball_globals = reinterpret_cast<Yelo::GameEngine::s_oddball_globals *>(0x64C078);;
static auto *const king_globals = reinterpret_cast<Yelo::GameEngine::s_king_globals *>(0x64BDF0);;
static auto *const race_globals = reinterpret_cast<Yelo::GameEngine::s_race_globals *>(0x64C1C0);;

static auto *const global_variant = reinterpret_cast<Yelo::GameEngine::s_game_variant *>(0x68CC48);;

static auto **const game_engines = reinterpret_cast<Yelo::GameEngine::game_engine_definition **>(0x6238C8);;
static auto **const game_engine = reinterpret_cast<Yelo::GameEngine::game_engine_definition **>(0x68CD24);;

	#pragma region New Engines functions
	//////////////////////////////////////////////////////////////////////////
	// FixHudRenderUnitInterface()
	static auto const HUD_RENDER_UNIT_INTERFACE_JMP_TABLE = 0x424953;
static auto *const hud_render_unit_interface_jmp_ptr = CAST_PTR;;

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7440()
	static auto const Func4F7440JmpTable = K_FUNC4F7440JMPTABLE;
static auto *const Func4F7440JmpPtr = reinterpret_cast<void ***>(0x4F71E5);;

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7580()
	static auto const Func4F7580JmpTable = K_FUNC4F7580JMPTABLE;
static auto *const Func4F7580JmpPtr = reinterpret_cast<void ***>(0x4F7343);;

	//////////////////////////////////////////////////////////////////////////
	// Initialize()
	static auto *const game_engine_init_mod = reinterpret_cast<Yelo::GameEngine::game_engine_definition ***>(0x45CC94);
	#pragma endregion

static auto const GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK = 0x45CDC8;
static auto const GAME_ENGINE_UPDATE_HOOK = 0x460D51;


//////////////////////////////////////////////////////////////////////////
// GameState.cpp
	static uintptr_t K_GAME_STATE_BEFORE_SAVE_PROCS_CALLS[] = {
	0x4CABEA,
	0x53B4A0,
	};
	static uintptr_t K_GAME_STATE_BEFORE_LOAD_PROCS_CALLS[] = {
		0x53B53D,
		0x53B683,
		0x53B775,
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_REFS[] = {
		0x53B253,
		0x53B549,
		0x53B79F,
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_COUNTS[] = {
		0x53B258,
		0x53B54E,
		0x53B7A4,
	};

static auto *const main_globals = reinterpret_cast<Yelo::GameEngine::s_main_globals *>(0x6B4798);

static auto *const physical_memory_globals = reinterpret_cast<s_physical_memory_map_globals *>(0x647458);
static auto const PHYSICAL_MEMORY_INITIALIZE = 0x445230;
enum DATA_PTR_PHYSICAL_MEMORY_ALLOCATION_SIZE { PTR_PHYSICAL_MEMORY_ALLOCATION_SIZE = 0x445258 };
static_assert(PTR_PHYSICAL_MEMORY_ALLOCATION_SIZE != 0);

static auto *const game_state_globals = reinterpret_cast<s_game_state_globals *>(0x67DD88);
static auto **const game_globals = reinterpret_cast<s_game_globals **>(0x64BAB0);
static auto **const game_time_globals = reinterpret_cast<s_game_time_globals **>(0x68CD70);
static auto **const game_allegiance_globals = reinterpret_cast<s_game_allegiance_globals **>(0x64BAB4);

static auto *const global_game_globals = reinterpret_cast<TagGroups::s_game_globals **>(0x6E2260);
static auto *const global_physics = reinterpret_cast<s_physics_globals *>(0x637BE4);
static auto *const global_point_physics = reinterpret_cast<s_point_physics_globals *>(0x653CAC);

static auto *const developer_mode = reinterpret_cast<byte *>(0x816126);
static auto *const transport_dumping = reinterpret_cast<bool *>(0x68C474);
static auto *const init_txt_filename = CAST_PTR;
static auto *const devmode_enabled = reinterpret_cast<bool *>(0x6BD15E);

static auto const GAME_INITIALIZE_HOOK = 0x45B63B;
static auto const GAME_INITIALIZE_FOR_NEW_MAP_HOOK = 0x45BCDB;
static auto const GAME_DISPOSE_FROM_OLD_MAP_HOOK = 0x45BE50;
	static void** K_GAME_SCENARIO_SWITCH_BSP_CALLS[] =
	{
		reinterpret_cast<void **>(0x4811B0),
		reinterpret_cast<void **>(0x4CCEF7),
		reinterpret_cast<void **>(0x4CD021),
		reinterpret_cast<void **>(0x541A68),
		reinterpret_cast<void **>(0x542379),
	};
static auto const GAME_DISPOSE_FROM_OLD_BSP_HOOK = 0x442986;

static auto const GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL = 0x4BA5FB;

static auto const GAME_CREATE_NETWORK_SERVER = 0x4E2090;
static auto const GAME_CREATE_NETWORK_CLIENT = 0x4DBE80;
static auto const GAME_CREATE_NETWORK_SERVER_CALL = 0x4E1190;

	static void** K_GAME_CREATE_NETWORK_CLIENT_CALL[] =
	{
		reinterpret_cast<void **>(0x498AC4),
		reinterpret_cast<void **>(0x4A0173),
		reinterpret_cast<void **>(0x4A0215),
		reinterpret_cast<void **>(0x4A045E),
		reinterpret_cast<void **>(0x53F4F2),
		reinterpret_cast<void **>(0x4E12A0),
		reinterpret_cast<void **>(0x4E77BE),
	};

//////////////////////////////////////////////////////////////////////////
// Lights.cpp
static auto **const lights = reinterpret_cast<lights_data_t **>(0x7FBE74);
static auto **const light_game_globals = reinterpret_cast<s_lights_globals_data **>(0x6B8250);
static auto **const light_cluster = reinterpret_cast<s_light_cluster_data **>(0x7FBE80);
static auto **const cluster_light_reference = reinterpret_cast<cluster_light_reference_data_t **>(0x7FBE84);
static auto **const light_cluster_reference = reinterpret_cast<light_cluster_reference_data_t **>(0x7FBE88);


//////////////////////////////////////////////////////////////////////////
// Players.cpp
static auto **const players = reinterpret_cast<players_data_t **>(0x815920);
static auto **const teams = reinterpret_cast<teams_data_t **>(0x81591C);
static auto **const players_globals = reinterpret_cast<s_players_globals_data **>(0x815918);
static auto **const player_control_globals = reinterpret_cast<s_player_control_globals_data **>(0x64C38C);
static auto **const player_effects = reinterpret_cast<s_player_effects_data **>(0x68C844);
static auto *const machine_to_player_table = reinterpret_cast<datum_index *>(0x64C390);

static auto const PLAYER_SPAWN__CREATE_UNIT_MP_HOOK = 0x47AAC1;
static auto const PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN = 0x47ABE0;

static auto const PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK = 0x477127;
static auto const PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK = 0x47755B;
static auto const PLAYERS_UPDATE_AFTER_GAME_HOOK = 0x4778EB;
	
//////////////////////////////////////////////////////////////////////////
// Players.NearbyObjects

static auto const PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN = 0x47B008;
static auto *const player_examine_nearby_objects_server_jmp_ptr = reinterpret_cast<void ***>(0x47AFE8);
static auto *const PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE = reinterpret_cast<byte *>(0x47AFE2);
static auto *const PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE = reinterpret_cast<void ***>(0x466272);

static auto const PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN = 0x47B108;
static auto *const player_examine_nearby_objects_client_jmp_ptr = reinterpret_cast<void ***>(0x47B0E8);
static auto *const PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE = reinterpret_cast<byte *>(0x47B0E2);
static auto *const PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE = reinterpret_cast<void ***>(0x4A7DFB);
	
//////////////////////////////////////////////////////////////////////////
// Players.ActionResults
static auto const PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE = 0x47BF23;
static auto const PLAYER_HANDLE_ACTION_JMP_TABLE_RETN = 0x47C21B;
static auto *const PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE = reinterpret_cast<byte *>(0x47BF28);
static auto *const player_handle_action_jmp_ptr = reinterpret_cast<void ***>(0x47BF39);
static auto *const PLAYER_HANDLE_ACTION_JMP_TABLE = reinterpret_cast<void ***>(0x5C36A3);

static auto const FOLLOWING_CAMERA_GET_UNIT_CAMERA_INFO_CALL = 0x447CE2;
