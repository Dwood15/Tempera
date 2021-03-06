/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#define K_GATHER_EXCEPTION_DATA 0x6BDE8C
#define K_GATHER_EXCEPTION 0x582060
#define K_RASTERIZER_SET_TEXTURE_BITMAP_DATA 0x51BF30
#define K_RESOLUTION_LIST_ADD_RESOLUTION 0x4BE1B0
#define K_ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL 0x4D9680
#define K_GSMD5DIGEST 0x5BF440
#define K_SHELL_GET_COMMAND_LINE_ARGUMENT 0x545B70
#define K_ACTOR_DELETE 0x427E60
#define K_ACTOR_UPDATE 0x429160
#define K_ACTOR_CUSTOMIZE_UNIT 0x426CF0
#define K_ACTOR_SET_ACTIVE 0x4277C0
#define K_ACTOR_SET_DORMANT 0x427860
#define K_ACTOR_DELETE_PROPS 0x427E00
#define K_ACTOR_FREEZE 0x429000
#define K_ACTOR_CREATE_FOR_UNIT 0x426AC0
#define K_ACTOR_ACTION_CHANGE 0x40D8D0
#define K_AI_SCRIPTING_ATTACH_FREE 0x435420
#define K_CACHE_FILE_READ_REQUEST 0x443410
#define K_SOUND_CACHE_SOUND_REQUEST 0x444770
#define K_TEXTURE_CACHE_BITMAP_GET_HARDWARE_FORMAT 0x444EB0
#define K_PHYSICAL_MEMORY_MAP_PREDICT_RESOURCES 0x445350
#define K_DIRECTOR_DESIRED_PERSPECTIVE 0x446480
#define K_CONSOLE_PROCESS_COMMAND 0x4C9DC0
#define K_CONSOLE_PRINTF 0x4C9BC0
#define K_CONSOLE_RESPONSE_PRINTF 0x4C9C60
#define K_CONSOLE_WARNING 0x4C9D20
#define K_CHEAT_ALL_WEAPONS 0x45AEA0
#define K_CHEAT_SPAWN_WARTHOG 0x45AF30
#define K_CHEAT_TELEPORT_TO_CAMERA 0x45AFA0
#define K_CHEAT_ACTIVE_CAMOFLAGE 0x45B030
#define K_CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER 0x45B090
#define K_CHEAT_PLAYER_INDEX 0x45B110
#define K_DIRECTOR_SAVE_CAMERA 0x4461E0
#define K_DIRECTOR_LOAD_CAMERA 0x4462A0
#define K_EFFECT_NEW_ON_OBJECT_MARKER 0x48B520
#define K_SCENARIO_SWITCH_STRUCTURE_BSP 0x542210
#define K_BINK_PLAYBACK_START 0x43ED20
#define K_GAME_ENGINE_RASTERIZE_MESSAGE 0x462A80
#define K_GAME_TEAM_IS_ENEMY 0x45C6C0
#define K_GAME_ENGINE_PLAY_MULTIPLAYER_SOUND 0x46EA60
#define K_OBJECT_LIST_ADD 0x48DEE0
#define K_INPUT_KEY_IS_DOWN 0x493910
#define K_KEYSTONE_CHAT_LOG_ADD_STRING 0x4AE8A0
#define K_HUD_PRINT_MESSAGE 0x4B1570
#define K_HUD_GET_ITEM_MESSAGE 0x4AD400
#define K_HUD_DRAW_METER 0x4AEFB0
#define K_HUD_DRAW_NUMBERS 0x4AF4A0
#define K_HUD_DRAW_STATIC_ELEMENT 0x4AFAE0
#define K_HUD_DRAW_BITMAP 0x4AFEC0
#define K_MAINMENU_LOAD 0x4CBDC0
#define K_WEAPON_PREVENTS_MELEE_ATTACK 0x4C62A0
#define K_WEAPON_PREVENTS_GRENADE_THROWING 0x4C62F0
#define K_WEAPON_STOP_RELOAD 0x4C7F10
#define K_FIRST_PERSON_WEAPON_MESSAGE_FROM_UNIT 0x4954F0
#define K_WEAPON_GET_FIRST_PERSON_ANIMATION_TIME 0x4C6340
#define K_PERIODIC_FUNCTION_EVALUATE 0x4CFDB0
#define K_TRANSITION_FUNCTION_EVALUATE 0x4CFEC0
#define K_DATA_NEW 0x4D3770
#define K_DATUM_NEW_AT_INDEX 0x4D37D0
#define K_DATUM_NEW 0x4D3880
#define K_DATUM_DELETE 0x4D3910
#define K_DATA_DELETE_ALL 0x4D3980
#define K_DATA_ITERATOR_NEXT 0x4D39D0
#define K_DATA_NEXT_INDEX 0x4D3A30
#define K_DATUM_TRY_AND_GET 0x4D3A80
#define K_DATUM_INITIALIZE 0x4D3AC0
#define K_MODEL_FIND_MARKER 0x4DABC0
#define K_HUD_CHAT_TO_NETWORK 0x4ADD40
#define K_INDEX_RESOLUTION_TABLE_TRANSLATE 0x4F3EF0
#define K_MAIN_CONNECT 0x4CB800
#define K_HS_OBJECT_ORIENT 0x48AB80
#define K_OBJECT_HEADER_BLOCK_ALLOCATE 0x4FB760
#define K_OBJECT_ITERATOR_NEXT 0x4FA830
#define K_OBJECT_PLACEMENT_DATA_NEW 0x4F8CB0
#define K_OBJECT_NEW 0x4F8D70
#define K_OBJECT_NEW_WITH_ROLE 0x4F8DC0
#define K_OBJECT_DELETE_TO_NETWORK 0x4F93B0
#define K_OBJECT_DELETE 0x4F94E0
#define K_OBJECT_ATTACH_TO_MARKER 0x4F9A90
#define K_OBJECT_DETACH 0x4F9F20
#define K_OBJECTS_FIND_IN_SPHERE 0x4FA8F0
#define K_OBJECT_START_INTERPOLATION 0x4FA480
#define K_OBJECT_RESET 0x4F8A70
#define K_OBJECT_RECONNECT_TO_MAP 0x4F9540
#define K_OBJECT_DISCONNECT_FROM_MAP 0x4F96F0
#define K_OBJECT_GET_ORIGIN 0x4FA210
#define K_OBJECT_GET_ORIENTATION 0x4FA280
#define K_OBJECT_GET_LOCATION 0x4FA420
#define K_OBJECT_SET_POSITION 0x4F8AD0
#define K_OBJECT_SET_POSITION_NETWORK 0x4F8BD0
#define K_OBJECT_RESTORE_BODY 0x4F12E0
#define K_OBJECT_DEPLETE_BODY 0x4F1330
#define K_OBJECT_DEPLETE_SHIELD 0x4F1420
#define K_OBJECT_DOUBLE_CHARGE_SHIELD 0x4F14B0
#define K_OBJECT_CAUSE_DAMAGE 0x4F1EF0
#define K_OBJECT_DEFINITION_PREDICT 0x4FB3E0
#define K_OBJECT_SET_SCALE 0x4FCFB0
#define K_OBJECT_GET_LEVEL_OF_DETAIL_PIXELS 0x512FB0
#define K_OBJECT_RENDER_STATE_REFRESH 0x512B80
#define K_OBJECTS_UPDATE 0x4F87A0
#define K_OBJECT_UPDATE 0x4FB800
#define K_OBJECT_GET_MARKER_BY_NAME 0x4F9990
#define K_OBJECT_DESTROY 0x4F1590
#define K_UNIT_UPDATE 0x565A70
#define K_UNIT_GET_CAMERA_POSITION 0x56C440
#define K_UNIT_SET_ANIMATION 0x5722D0
#define K_UNIT_ANIMATION_START_ACTION 0x5692C0
#define K_UNIT_GET_CUSTOM_ANIMATION_TIME 0x5738B0
#define K_UNIT_CAN_ENTER_SEAT 0x569D00
#define K_UNIT_ENTER_SEAT 0x569E30
#define K_UNIT_EXIT_VEHICLE 0x56ECF0
#define K_UNIT_TRY_AND_EXIT_SEAT 0x56FB70
#define K_UNIT_OPEN 0x56E210
#define K_UNIT_CLOSE 0x56E230
#define K_UNIT_FIND_NEARBY_SEAT 0x569A20
#define K_UNIT_EXIT_SEAT_END 0x56FD40
#define K_UNIT_CAN_SEE_POINT 0x56F800
#define K_UNIT_DETACH_FROM_PARENT 0x570140
#define K_UNIT_START_USER_ANIMATION 0x5739A0
#define K_UNIT_DROP_CURRENT_WEAPON 0x5715C0
#define K_UNIT_DAMAGE_AFTERMATH 0x56A960
#define K_UNIT_SCRIPTING_SET_CURRENT_VITALITY 0x565040
#define K_UNIT_ANIMATION_SET_STATE 0x569450
#define K_UNIT_INVENTORY_GET_WEAPON 0x56D070
#define K_UNIT_THROW_GRENADE_RELEASE 0x571B40
#define K_UNIT_CAUSE_PLAYER_MELEE_DAMAGE 0x572C50
#define K_UNIT_SET_ACTIVELY_CONTROLLED 0x56D2F0
#define K_UNIT_READY_DESIRED_WEAPON 0x570DE0
#define K_COLLISION_TEST_VECTOR 0x509190
#define K_PLAYER_INDEX_FROM_UNIT_INDEX 0x477970
#define K_PLAYER_TELEPORT 0x478820
#define K_PLAYER_HEALTH_PACK_SCREEN_EFFECT 0x47C3D0
#define K_FIND_CLOSEST_PLAYER_INDEX 0x45ECD0
#define K_PLAYER_EXAMINE_NEARBY_VEHICLE 0x47B140
#define K_PLAYER_SET_ACTION_RESULT 0x47B940
#define K_PLAYER_SET_ACTION_RESULT_TO_NETWORK 0x47BB30
#define K_PLAYER_CONTROL_GET_UNIT_CAMERA_INFO 0x474C00
#define K_RENDER_FRAME 0x50F7B0
#define K_RENDER_WINDOW 0x50F8C0
#define K_SCENARIO_TRIGGER_VOLUME_TEST_POINT 0x542380
#define K_TAG_LOADED 0x442990
#define K_TAG_ITERATOR_NEXT 0x442A10
#define K_UNICODE_STRING_LIST_GET_STRING 0x55AD90
