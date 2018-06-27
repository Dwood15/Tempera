#include <memory_locations.h>
#include "../../../../../../../BlamSauce/memory/memory_interface_base.hpp"
#include "../../../../../../../BlamSauce/network/player_update/client.hpp"
#include "../../../../../../../BlamSauce/network/player_update/server.hpp"
#include "../../../../../../../BlamSauce/network/client/manager_structures.hpp"
#include "../../../../../../../BlamSauce/network/server/network_server_manager_structures.hpp"
#include "../../../../../../../BlamSauce/network/messages/delta_definitions.hpp"
#include "../../../../../../../BlamSauce/network/messages/delta_parameters_protocol.hpp"

#ifdef ENGINE_DPTR
#undef ENGINE_DPTR
#endif

#ifdef ENGINE_PTR
#undef ENGINE_PTR
#endif

#ifdef FUNC_PTR
#undef FUNC_PTR
#endif

#define ENGINE_DPTR(type, name, ...) static auto** const name = reinterpret_cast<type**>(__VA_ARGS__);
#define ENGINE_PTR(type, name, ...) static auto* const name = CAST_PTR(type*, __VA_ARGS__);

#define FUNC_PTR(name, value) static auto const name  =  value


//////////////////////////////////////////////////////////////////////////
// MDP.hpp
static auto const MDP_FIELD_ENCODE_ARRAY = (0x4EC9A0);
static auto const MDP_FIELD_DECODE_ARRAY = (0x4EC9E0);

using field_properties_definition = Yelo::MessageDeltas::field_properties_definition;

static auto *const pmessage_delta_field_integer_small = reinterpret_cast<field_properties_definition *>(0x6229B8);;
static auto *const pmessage_delta_field_integer_medium = reinterpret_cast<field_properties_definition *>(0x622A28);
static auto *const pmessage_delta_field_integer_large = reinterpret_cast<field_properties_definition *>(0x622600);

static auto *const pmessage_delta_field_real = reinterpret_cast<field_properties_definition *>(0x622770);
static auto *const pmessage_delta_field_boolean = reinterpret_cast<field_properties_definition *>(0x622950);
static auto *const pmessage_delta_field_ascii_character = reinterpret_cast<field_properties_definition *>(0x635E10);
static auto *const pmessage_delta_field_time = reinterpret_cast<field_properties_definition *>(0x635F58);
static auto *const pmessage_delta_field_grenade_counts = reinterpret_cast<field_properties_definition *>(0x635C48);

static auto *const pmessage_delta_field_locality_reference_position = reinterpret_cast<field_properties_definition *>(0x6314B0);
static auto *const pmessage_delta_field_digital_throttle = reinterpret_cast<field_properties_definition *>(0x633650);
static auto *const pmessage_delta_field_fixed_width_weapon_index = reinterpret_cast<field_properties_definition *>(0x633728);
static auto *const pmessage_delta_field_fixed_width_grenade_index = reinterpret_cast<field_properties_definition *>(0x633790);
static auto *const pmessage_delta_field_item_placement_position = reinterpret_cast<field_properties_definition *>(0x635FC0);

static auto *const pmessage_delta_field_local_player_update_sequence_number = reinterpret_cast<field_properties_definition *>(0x634EB8);
static auto *const pmessage_delta_field_remote_player_update_sequence_number = reinterpret_cast<field_properties_definition *>(0x634F98);
static auto *const pmessage_delta_field_remote_player_action_update_baseline_id = reinterpret_cast<field_properties_definition *>(0x634F28);
static auto *const pmessage_delta_field_update_tick_count = reinterpret_cast<field_properties_definition *>(0x633558);
static auto *const pmessage_delta_field_update_id = reinterpret_cast<field_properties_definition *>(0x6334E8);
static auto *const pmessage_delta_field_fixed_width_3bits = reinterpret_cast<field_properties_definition *>(0x635CB8);
static auto *const pmessage_delta_field_fixed_width_6bits = reinterpret_cast<field_properties_definition *>(0x631740);
static auto *const pmessage_delta_field_fixed_width_1bit = reinterpret_cast<field_properties_definition *>(0x6336C0);
static auto *const pmessage_delta_field_fixed_width_normal_4bit = reinterpret_cast<field_properties_definition *>(0x635D30);
static auto *const pmessage_delta_field_fixed_width_normal_8bit = reinterpret_cast<field_properties_definition *>(0x630A68);
static auto *const pmessage_delta_field_fixed_width_normal_16bit = reinterpret_cast<field_properties_definition *>(0x635DA8);

static auto *const pmessage_delta_field_object_index = reinterpret_cast<field_properties_definition *>(0x622698);
static auto *const pmessage_delta_field_player_index = reinterpret_cast<field_properties_definition *>(0x622AC0);

static auto *const pmessage_delta_field_definition_index = reinterpret_cast<field_properties_definition *>(0x62E4D0);

static auto *const pmessage_delta_field_point2d = reinterpret_cast<field_properties_definition *>(0x635E80);
static auto *const pmessage_delta_field_point3d = reinterpret_cast<field_properties_definition *>(0x630AD8);
static auto *const pmessage_delta_field_vector2d = reinterpret_cast<field_properties_definition *>(0x635EF0);
static auto *const pmessage_delta_field_vector3d = reinterpret_cast<field_properties_definition *>(0x622708);

static auto *const pmessage_delta_field_control_flags = reinterpret_cast<field_properties_definition *>(0x6335E8);
static auto *const pmessage_delta_field_angular_velocity = reinterpret_cast<field_properties_definition *>(0x6311E8);
static auto *const pmessage_delta_field_translational_velocity = reinterpret_cast<field_properties_definition *>(0x630E60);
static auto *const pmessage_delta_field_map_name = reinterpret_cast<field_properties_definition *>(0x6339D8);
static auto *const pmessage_delta_field_network_map = reinterpret_cast<field_properties_definition *>(0x633A68);
static auto *const pmessage_delta_field_network_player_name = reinterpret_cast<field_properties_definition *>(0x633AD8);


static auto *const pmessage_delta_field_network_player = reinterpret_cast<field_properties_definition *>(0x633BB8);
static auto *const pmessage_delta_field_network_game_name = reinterpret_cast<field_properties_definition *>(0x633C28);
static auto *const pmessage_delta_field_network_game_players = reinterpret_cast<field_properties_definition *>(0x633CA0);
static auto *const pmessage_delta_field_game_variant = reinterpret_cast<field_properties_definition *>(0x622F78);
static auto *const pmessage_delta_field_game_variant_human_readable_description = reinterpret_cast<field_properties_definition *>(0x622E28);
static auto *const pmessage_delta_field_universal_variant = reinterpret_cast<field_properties_definition *>(0x622D40);
static auto *const pmessage_delta_field_universal_variant_flags = reinterpret_cast<field_properties_definition *>(0x622B50);
static auto *const pmessage_delta_field_game_engine_variant = reinterpret_cast<field_properties_definition *>(0x622DB8);
static auto *const pmessage_delta_field_game_variant_flags = reinterpret_cast<field_properties_definition *>(0x622EB8);
static auto *const pmessage_delta_field_parameters_protocol_array = reinterpret_cast<field_properties_definition *>(0x6367F8);
static auto *const pmessage_delta_field_hud_chat_message = reinterpret_cast<field_properties_definition *>(0x62E388);
static auto *const pmessage_delta_field_hud_chat_message_ptr = reinterpret_cast<field_properties_definition *>(0x62E3F8);
static auto *const pmessage_delta_field_object_change_colors = reinterpret_cast<field_properties_definition *>(0x636520);
static auto *const pmessage_delta_field_ctf_score_array = reinterpret_cast<field_properties_definition *>(0x6233A0);
static auto *const pmessage_delta_field_race_score_array = reinterpret_cast<field_properties_definition *>(0x623AC8);
static auto *const pmessage_delta_field_king_score_array = reinterpret_cast<field_properties_definition *>(0x623548);
static auto *const pmessage_delta_field_oddball_score_array = reinterpret_cast<field_properties_definition *>(0x623970);
static auto *const pmessage_delta_field_oddball_owner_array = reinterpret_cast<field_properties_definition *>(0x6239E8);
static auto *const pmessage_delta_field_slayer_score_array = reinterpret_cast<field_properties_definition *>(0x623BC8);
static auto *const pmessage_delta_field_damage_data_flags = reinterpret_cast<field_properties_definition *>(0x622810);
static auto *const pmessage_delta_field_motd_text = reinterpret_cast<field_properties_definition *>(0x634E00);
static auto *const pmessage_delta_field_rcon_response = reinterpret_cast<field_properties_definition *>(0x634D48);
static auto *const pmessage_delta_field_rcon_password = reinterpret_cast<field_properties_definition *>(0x634C10);
static auto *const pmessage_delta_field_rcon_command = reinterpret_cast<field_properties_definition *>(0x634C80);

using field_reference_set = Yelo::MessageDeltas::field_reference_set;

static auto *const pmessage_delta_empty_field_set = reinterpret_cast<field_reference_set *>(0x6225E0);

	//ENGINE_PTR(field_reference_set, message_delta_guaranteed_object_header_field_set,					0x0, 0x0);
	static auto *const pmessage_delta_game_object_header_field_set = reinterpret_cast<field_reference_set *>(0x631250);
static auto *const pmessage_delta_game_object_timestamped_header_field_set = reinterpret_cast<field_reference_set *>(0x636050);
static auto *const pmessage_delta_biped_update_header_field_set = reinterpret_cast<field_reference_set *>(0x636430);
static auto *const pmessage_delta_ctf_update_header_field_set = reinterpret_cast<field_reference_set *>(0x623408);
static auto *const pmessage_delta_client_game_update_header_field_set = reinterpret_cast<field_reference_set *>(0x6338DC);
static auto *const pmessage_delta_player_score_update_header_field_set = reinterpret_cast<field_reference_set *>(0x6230DC);
static auto *const pmessage_delta_remote_player_action_update_header_field_set = reinterpret_cast<field_reference_set *>(0x635120);
static auto *const pmessage_delta_remote_player_position_update_header_field_set = reinterpret_cast<field_reference_set *>(0x6352B8);
static auto *const pmessage_delta_remote_player_total_update_header_field_set = reinterpret_cast<field_reference_set *>(0x6353E0);
	//ENGINE_PTR(field_reference_set, message_delta__header_field_set, 0, PTR_UNKNOWN);


//////////////////////////////////////////////////////////////////////////
// MessageDefinitions.cpp

	// Currently there is nothing to add here


//////////////////////////////////////////////////////////////////////////
// MessageDeltas.cpp
using s_message_delta_parameters = Yelo::MessageDeltas::s_message_delta_parameters;
using field_type_definition = Yelo::MessageDeltas::field_type_definition;
using s_parameters_protocol_globals = Yelo::MessageDeltas::s_parameters_protocol_globals;
typedef Yelo::MessageDeltas::message_delta_parameter exposed_parameters_t[Enums::k_mdp_max_exposed_parameters];

static auto *const pmessage_delta_parameters = reinterpret_cast<s_message_delta_parameters *>(0x635984);
static auto *const pmessage_delta_global_field_type_list = reinterpret_cast<field_type_definition *>(0x6359A8);
static auto *const pg_message_delta_encoding_class = reinterpret_cast<Enums::message_delta_encoding_class *>(0x636A08);
static auto *const pg_exposed_params = reinterpret_cast<exposed_parameters_t *>(0x6535F0);
static auto *const pmdp_parameters_protocol_globals = reinterpret_cast<s_parameters_protocol_globals *>(0x6B8240);

static auto *const pmdp_packet_buffer_received_data = reinterpret_cast<byte *>(0x7FD180);
static auto *const pmdp_packet_buffer_sent_data = reinterpret_cast<byte *>(0x80D180);

using message_delta_definition = Yelo::MessageDeltas::message_delta_definition;

static auto **const ppmessage_delta_packets = reinterpret_cast<message_delta_definition **>(0x5FBA40);
static_assert(0x5FBA40 != 0);

		#pragma region MessageDeltaPointerReferences
		static void** MessageDeltaPointerReferences[] = {
			(reinterpret_cast<void **>(0x4EFC23)),
			(reinterpret_cast<void **>(0x4EFCA4)),
			(reinterpret_cast<void **>(0x4EFE3D)),
			(reinterpret_cast<void **>(0x4F0063)),
			(reinterpret_cast<void **>(0x4F00F0)),
			(reinterpret_cast<void **>(0x4F0280)),
			(reinterpret_cast<void **>(0x4F041C)),
			(reinterpret_cast<void **>(0x4F0489)),
			(reinterpret_cast<void **>(0x4F051E)),
			(reinterpret_cast<void **>(0x4F0548)),
			(reinterpret_cast<void **>(0x4F06FF)),
			(reinterpret_cast<void **>(0x4F08D9)),
			(reinterpret_cast<void **>(0x4F0991)),
			(reinterpret_cast<void **>(0x4F0A59)),
			(reinterpret_cast<void **>(0x4F0AE9)),
			(reinterpret_cast<void **>(0x4F0B37)),
			(reinterpret_cast<void **>(0x4F0BA5)),
			(reinterpret_cast<void **>(0x4F0BC3))
		};
		#pragma endregion

		#pragma region MessageDeltaPointerEndChecks
		static void** MessageDeltaPointerEndChecks[] = {
			(reinterpret_cast<void **>(0x4EFC33)),
			(reinterpret_cast<void **>(0x4EFCC7)),
			(reinterpret_cast<void **>(0x4F0087)),
		};

		static byte* MessageDeltaTypeCountChecks8bit[] = {
			(reinterpret_cast<byte *>(0x4F084D)),
		};
		#pragma endregion

static auto const NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED = (0x4DCC40);
static auto const NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1 = (0x4DF360);
static auto const NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2 = (0x4E1E01);

#define K_GS_SOCKET                                                   0x68C484
#define K_GS_LOOPBACKSOCKET                                           0x68C488
#define K_GAMESPY_CONFIG                                              0x63ACF4
#define K_GAMESPY_GLOBALS                                             0x6BDA68
#define K_GAMESPY_SERVER_BROWSER_GLOBALS                              0x6B44E0
#define K_GAMESPY_QR2                                                 0x6BDCF0
#define K_G_GAMESPY_PATCH_CHECK_FOR_UPDATES                           0x62E101
#define K_GAMESPY_PRODUCTS_LIST                                       0x6BE4C8
#define K_COMPILER_NULL_STRING                                        0x5F363C


static auto const NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL = (0x4DED6A);
enum DATA_PTR_DONT_SEND_OBJECT_NEW_MSG { PTR_DONT_SEND_OBJECT_NEW_MSG = 0x6B7350 };
static_assert(PTR_DONT_SEND_OBJECT_NEW_MSG != 0);
static auto const NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY = 0x4DD760;

static auto *const pglobal_network_game_server_data = reinterpret_cast<Yelo::Networking::s_network_game_server *>(0x7FC6A0);
static auto *const pglobal_network_game_client_data = reinterpret_cast<Yelo::Networking::s_network_game_client *>(0x80E180);

static auto const MDPI_ENCODE = (0x4F0250);
static auto const MDP_DECODE_STATELESS_ITERATED = (0x4EFEA0);
static auto const MDP_DECODE_INCREMENTAL_ITERATED = (0x4EFF10);
static auto const MDP_DISCARD_ITERATION_BODY = (0x4EFF80);



//////////////////////////////////////////////////////////////////////////
// Networking.cpp
static auto *const pupdate_client_globals = reinterpret_cast<Yelo::Networking::s_update_client_globals *>(0x692E98);
static auto *const pupdate_server_globals = reinterpret_cast<Yelo::Networking::s_update_server_globals *>(0x68CD88);
static auto *const pnetwork_update_globals = reinterpret_cast<Yelo::Networking::network_update_globals *>(0x624A68);
static auto **const ppglobal_network_game_server = reinterpret_cast<Yelo::Networking::s_network_game_server **>(0x6B7364);
static_assert(0x6B7364 != 0);
static auto **const ppglobal_network_game_client = reinterpret_cast<Yelo::Networking::s_network_game_client **>(0x6B7368);
static_assert(0x6B7368 != 0);

static auto const NETWORK_GAME_ADD_PLAYER = (0x4E1930);
static auto const NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER = (0x4E2D4A);
static auto const NETWORK_GAME_REMOVE_PLAYER = (0x4DC590);
	// just a few bytes after this gets called is another call to ngrp, but its for the s_network_client's game member
	static auto const CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER = (0x5C4533);

static auto const NETWORK_CONNECTION_WRITE = (0x4E0290);
static auto const NETWORK_GAME_SERVER_SEND_REJECTION_MESSAGE = (0x4E3F80);
static auto const NETWORK_GAME_SERVER_HOLDUP_NEW_CLIENT = (0x4E4060);
static auto const NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE = (0x4E4DA0);
static auto const NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES = (0x4E4E30);
static auto const NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES_INGAME = (0x4E4EF0);


//////////////////////////////////////////////////////////////////////////
// Server.cpp
	namespace Networking
	{
		static auto *const psv_password = reinterpret_cast<wchar_t *>(0x6B7384);
		static auto *const psv_rcon_password = reinterpret_cast<char *>(0x6B74A0);
		static auto *const psv_timelimit = reinterpret_cast<long *>(0x634C08);
		static auto *const phalo_argv_port = reinterpret_cast<long *>(0x6337F8);
		static auto *const phalo_argv_client_port = reinterpret_cast<long *>(0x6337FC);
	};

	//////////////////////////////////////////////////////////////////////////
	// Server
	namespace Server
	{
		static auto *const pnetwork_sv_file_globals = reinterpret_cast<Yelo::Networking::s_network_sv_file_globals *>(0x6534CC);
		static auto *const pnetwork_sv_mapcycle_globals = reinterpret_cast<Yelo::Networking::s_network_sv_mapcycle_globals *>(0x6230CC);
		static auto *const pnetwork_sv_globals = reinterpret_cast<s_network_sv_globals *>(0x6B7294);

		// ENGINE_PTR(long, server_instance,			PTR_NULL, K_SERVER_INSTANCE);
		//
		// FUNC_PTR(SV_EVENT_LOG_WITH_TIME,			FUNC_PTR_NULL, K_SV_EVENT_LOG_WITH_TIME);
		// FUNC_PTR(SV_EVENT_ECHO,						FUNC_PTR_NULL, K_SV_EVENT_ECHO);
		//
		// ENGINE_PTR(bool, event_type_logging,		PTR_NULL, K_EVENT_TYPE_LOGGING);
		// ENGINE_PTR(bool, event_type_echoing,		PTR_NULL, K_EVENT_TYPE_ECHOING);
		//
		// ENGINE_PTR(wconst char **, log_entry_titles,		PTR_NULL, K_LOG_ENTRY_TITLES);
		// ENGINE_PTR(wconst char **, log_entry_titles_ref,	PTR_NULL, K_LOG_ENTRY_TITLES_REF);
		//
		// static byte** K_EVENT_ENABLE_LOGGING_0_REFS[] =
		// {
		// 	CAST_PTR(byte**, K_EVENT_ENABLE_LOGGING_0_REF_0),
		// 	CAST_PTR(byte**, K_EVENT_ENABLE_LOGGING_0_REF_1)
		// };
		//
		// ENGINE_PTR(byte**, event_enable_logging_4_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_4_REF);
		// ENGINE_PTR(byte**, event_enable_logging_8_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_8_REF);
		// ENGINE_PTR(byte**, event_enable_logging_12_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_12_REF);
		//
		// static byte** K_EVENT_ENABLE_ECHOING_0_REFS[] =
		// {
		// 	CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_0),
		// 	CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_1),
		// 	CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_2)
		// };
		//
		// ENGINE_PTR(byte**, event_enable_echoing_2_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_2_REF);
		// ENGINE_PTR(byte**, event_enable_echoing_3_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_3_REF);
		// ENGINE_PTR(byte**, event_enable_echoing_4_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_4_REF);
		// ENGINE_PTR(byte**, event_enable_echoing_8_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_8_REF);
		// ENGINE_PTR(byte**, event_enable_echoing_12_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_12_REF);
	};


//////////////////////////////////////////////////////////////////////////
// GameSpyApi.cpp
	namespace Networking
	{
		static auto **const ppgs_Socket = reinterpret_cast<s_gamespy_socket **>(K_GS_SOCKET);
		static_assert((K_GS_SOCKET) != 0);
		static auto **const ppgs_LoopbackSocket = reinterpret_cast<s_gamespy_socket **>(K_GS_LOOPBACKSOCKET);
		static_assert((K_GS_LOOPBACKSOCKET) != 0);
		static auto *const pgamespy_config = reinterpret_cast<s_gamespy_config *>(K_GAMESPY_CONFIG);
		static auto *const pgamespy_globals = reinterpret_cast<s_gamespy_globals *>(K_GAMESPY_GLOBALS);
		static auto *const pgamespy_server_browser_globals = reinterpret_cast<s_gamespy_server_browser_globals *>(0x6B44E0);
		static auto **const ppgamespy_qr2 = reinterpret_cast<s_gamespy_qr2 **>(0x6BDCF0);
		static auto *const pg_gamespy_patch_check_for_updates = reinterpret_cast<bool *>(K_G_GAMESPY_PATCH_CHECK_FOR_UPDATES, 0x1337BEEF);

		static auto *const pgamespy_products_list = reinterpret_cast<s_gamespy_product *>(K_GAMESPY_PRODUCTS_LIST); // [4]

		enum DATA_PTR_compiler_null_string { PTR_compiler_null_string = PLATFORM_VALUE((K_COMPILER_NULL_STRING)) };

		static_assert(PTR_compiler_null_string != 0); // TODO: not the best place for this...

		static auto const GAMESPY_GET_CLIENT_KEY_HASH = (0x579690);
		enum DATA_PTR_GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL { PTR_GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL = 0x4A7870 };
		static_assert(PTR_GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL != 0);

		namespace GameSpy
		{
			static auto *const pMatchup1Hostname = reinterpret_cast<const char * *>(0x61EDF0);
			static auto *const pMatchup2Hostname = reinterpret_cast<const char * *>(0x61EDF4);
			static auto *const pMASTER_ADDR_REFERENCE = reinterpret_cast<const char * *>(0x5BB24C);
			static auto *const pMASTER_ADDR_SB_REFERENCE = reinterpret_cast<const char * *>(0x5C3752);
			static auto *const pPTA_DEFAULT_VERCHECK_URL_REFERENCE = reinterpret_cast<const char * *>(0x5C0FB5);

			static auto const CREATE_GAMESPY_QR2_HOOK = (0x57AFDA);
			static auto const QR2_REGISTER_KEY = (0x5C0750);
		};
	};


