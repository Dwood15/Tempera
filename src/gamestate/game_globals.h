#pragma once

#include "macros_generic.h"
#include "../ce_base_types.h"
#include "player_types.h"

namespace game_options {
	struct s_game_options {
		int                   unk_type; // never see this referenced or explicitly set. due to memset, this will always be set to zero
		short                 unk_2; // never see this referenced besides in game_options_new code where it's set to 0, even after a memset call. highly likely to be an enum field, albeit unused
		game_difficulty_level difficulty_level;
		int                   game_random_seed;
		// not always the actual path, sometimes just the name, go figure
		char                  map_name[255 + 1];
	}; STAT_ASSRT(s_game_options, 0x10C);

	struct s_game_globals {
		bool           map_loaded;
		bool           active;
		bool           players_are_double_speed;
		bool           map_loading_in_progress;
		real           map_loading_precentage;
		s_game_options options;
	}; STAT_ASSRT(s_game_globals, 0x114);

	/*
struct s_main_globals
{
	UNKNOWN_TYPE(uint32); // time related
	PAD32;
	LARGE_INTEGER performance_counter;
	UNKNOWN_TYPE(bool);
	bool is_taking_screenshot;
	PAD16;
	real delta_time;
	Enums::game_connection game_connection;

	struct s_screenshot {
		int16 counter;
		void* movie; // screenshot bitmap
		PAD32; PAD32;
		int32 movie_frame_index;
		UNKNOWN_TYPE(real);
	}screenshot;

	struct s_map {
		bool reset_map;
		bool switch_to_campaign;
		bool revert_map;
		bool skip_cinematic;
		bool save_map;
		bool save_map_nonsafe;
		bool save_map_with_timeout;
		bool is_saving_map;
		int32 saving_map_timeout_countdown;
		int32 saving_map_timeout_timer;
		UNKNOWN_TYPE(int32);
		UNKNOWN_TYPE(int16);
		bool won_map;
		bool lost_map;
		bool respawn_coop_players;

		struct s_core {
			bool save;
			bool load;
			bool load_at_startup;
		}core;

		int16 switch_to_structure_bsp; // if not NONE, switches to the scenario's bsp by index
		bool main_menu_scenario_loaded;
		bool main_menu_scenario_load;
	}map;

	UNUSED_TYPE(bool);
	UNKNOWN_TYPE(bool);
	UNKNOWN_TYPE(bool);
	bool quit;
	UNKNOWN_TYPE(int32);
	UNKNOWN_TYPE(int32);
	UNKNOWN_TYPE(int32);
	bool set_game_connection_to_film_playback;
	bool time_is_stopped;
	bool start_time;
	UNUSED_TYPE(bool);

	bool skip_frames;
	PAD8;
	int16 skip_frames_count;

	int16 lost_map_count;
	int16 respawn_count;

	UNKNOWN_TYPE(bool);
	PAD24;
	UNKNOWN_TYPE(bool);
	char scenario_tag_path[256];
	char multiplayer_map_name[256];
	char queued_map[256];

	UNKNOWN_TYPE(bool);
	UNKNOWN_TYPE(tag_string);
	PAD(0, 8 + 1); // char[8+1]
	PAD8;
	PAD32;
};

*/
	// struct s_game_globals_tag {
	// enum {
	// 	k_group_tag = 'matg'
	// };
	//
	// long language;
	//
	// byte bytePad[244];
	//
	// tag_reference sounds;
	// tag_reference camera;
	//
	// tag_block<s_player_control_globals_data> game_globals_player_control; //
	//
	// tag_block<s_game_globals_difficulty_information> difficulty_info; //, );
	// tag_block<s_game_globals_grenade> grenades;  //);
	//
	// tag_block<s_game_globals_rasterizer_data> game_globals_rasterizer_data; //
	// tag_block<s_game_globals_interface_tag_references> game_globals_interface_tag_references; //
	//
	// tag_block<s_game_globals_tag_reference> weapons_list; // ;
	// tag_block<s_game_globals_tag_reference> cheat_powerups;
	// tag_block<s_game_globals_multiplayer_information> multiplayer_info; //
	// tag_block<s_game_globals_player_information> player_info; // );
	// tag_block<s_game_globals_player_representation> player_representation; // );
	// tag_block<s_game_globals_falling_damage> falling_damage; // );
	// tag_block materials; //material_definition
	// tag_block playlist_members;
	// tag_block playlist_autogenerate_choice;
	// }; */
};
