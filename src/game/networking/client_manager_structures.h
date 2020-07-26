#pragma once

#include <macros_generic.h>

namespace Yelo::Netorking {
	// I'm pretty sure this is deprecated on the PC (gamespy and all)
	struct s_network_advertised_game : TStructImpl(0x130) {
	};

	struct s_network_game_client {
		struct s_join_parameters {
			UNKNOWN_TYPE(int16);                  // 0xAEE
			wchar_t password[Enums::k_network_server_password_length + 1];// 0xAF0
			char    token[Enums::k_network_game_join_token_size];         // 0xB02
		};
		STAT_ASSERT(s_join_parameters, 0x24);

		struct s_join_data {
			UNKNOWN_TYPE(int32);                  // 0xAE0
			int32 join_process_start_time;            // 0xAE4
			UNKNOWN_TYPE      (int32);                  // 0xAE8
			UNKNOWN_TYPE(bool);

			PAD8;               // 0xAEC
			s_join_parameters parameters;            // 0xAEE
									PAD16;
		};

		STAT_ASSERT(s_join_data, 0x34);

		struct s_ping_target {
			int32 last_ping_time;                  // 0xACC
			int16 samples_sent;                     // 0xAD0
			int16 samples_received;                  // 0xAD2
			int16 ping_time;                     // 0xAD4
			UNKNOWN_TYPE(bool);

			PAD8;               // 0xAD6
			s_network_client_time *tc;               // 0xAD8
		};

		STAT_ASSERT(s_ping_target, 0x10);

		int16                            machine_index;
													PAD16;
		s_network_advertised_game        discovered_games[9];
		s_transport_address              server_address;            // 0xAB4
		s_ping_target                    current_ping_target;            // 0xACC
		s_network_connection             *connection;            // 0xADC
		s_join_data                      join_data;                     // 0xAE0
		s_network_game                   game;                     // 0xB14
		UNKNOWN_TYPE(int32);                     // 0xF04
		int32                            time_of_last_game_search_packet;         // 0xF08
		int32                            next_update_number;                  // 0xF0C
		int32                            time_of_last_update;                  // 0xF10
		UNKNOWN_TYPE(int32);                     // 0xF14, time for something
		int16                            seconds_to_game_start;               // 0xF18
		Enums::network_game_client_state state;         // 0xF1A
		_enum                            error;                           // 0xF1C
		word_flags                       flags;                        // 0xF1E
		bool                             is_out_of_sync;                     // 0xF20
		bool                             is_lagging_out;                     // 0xF21, "network client connection has been silent for a dangerously long 'amount of time"
													PAD16; // guessing there is alignment padding here
		//////////////////////////////////////////////////////////////////////////
		// 0x30 byte data structure
		PAD64;                              // 0xF24, unknown
		UNKNOWN_TYPE(bool);

		PAD24;               // 0xF2C
		PAD32;                              // 0xF30, unknown
		UNKNOWN_TYPE(int32);                  // 0xF34
		//////////////////////////////////////////////////////////////////////////
		// 0x18 byte data structure
		PAD128;
		PAD64;                     // 0xF38
		int32 team_index;                     // 0xF50
		PAD128;
		PAD128;
		PAD128;
		PAD32;               // 0xF54, 0x34 byte data structure
		s_player_update_history *update_history;      // 0xF88
		byte_enum               connection_class;                  // 0xF8C Enums::network_connection_class
										PAD24;
	};

	STAT_ASSERT(s_network_game_client, 0xF90);

	// For increased player counts game states
	struct s_network_game_client_yelo : s_network_game_client {
		s_network_game_yelo game_yelo;
	};
};
