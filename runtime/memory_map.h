#pragma once

#include <windows.h>
#include <macros_generic.h>

struct defined_functionrange {
	const char * funcName;
	const uintptr_t                               begin;
	const uintptr_t                               end;

	bool operator <(const defined_functionrange  &other) { return begin < other.end; }

	bool operator >(const defined_functionrange  &other) { return end > other.end; }

	bool operator ==(const defined_functionrange  &other) { return end == other.end && begin == other.begin; }

	bool contains(uintptr_t other) { return (other <= end && other >= begin); }

	constexpr bool operator <(defined_functionrange &other) { return begin < other.end; }

	constexpr bool operator >(defined_functionrange &other) { return end > other.end; }

	constexpr bool operator ==(defined_functionrange &other) { return end == other.end && begin == other.begin; }

	constexpr bool contains(const uintptr_t other) const { return (other <= end && other >= begin); }
};

struct LPCoreAddressList {
	uint core_0;
	uint core_1;
	uint core_2;
	uint core_3;
	uint core_4;
	uint core_5;
	uint core_6;
	uint core_7;
	uint CAMERA;
	uint MAP_HEADER_ADDRESS;
	uint TAG_INDEX_HEADER_ADDRESS;

	uintptr_t CONSOLE_HOOK_ADDRESS;
	uintptr_t DEVMODE_HOOK_ADDRESS;
	uintptr_t CONSOLE_TEXT_HOOK_ADDRESS;

	uint to_respawn_count;
	uint spawn_count;
	uint render_window_count;
	uint at_main_menu;

	uint hud_scripted_globals;
	uint hud_messaging_state;

	uint game_state_globals_crc;
	uint game_state_cpu_allocation;
	uint game_state_globals_ptr;
	uint crc_checksum_buffer;

	uint player_control_globals_data;
	uint game_time_globals;
	uint players_global_data;
	uint game_globals_conn_type;
	uint cinematic_globals;
};


