
#pragma once

#include "macros_generic.h"
#include "../../cseries/MacrosCpp.h"
#include "time_structures.hpp"

namespace Yelo {
	static const long K_TICKS_PER_SECOND = 30;
	static const real  K_SECONDS_PER_TICK = 0.033333335f; // 0x3D088889

	namespace GameState {
		static s_game_time_globals *GameTimeGlobals() { return nullptr; }
	};

	namespace blam {
		using namespace Yelo;
		using namespace Yelo::GameState;

		static real game_time_get_seconds_per_tick() {
			return K_SECONDS_PER_TICK;
		}

		static real game_ticks_to_seconds(long ticks) {
			return CAST(real, ticks) * K_SECONDS_PER_TICK;
		}

		static long game_seconds_to_ticks(long seconds) {
			return seconds * K_TICKS_PER_SECOND;
		}

		static real game_seconds_to_ticks(real seconds) {
			return seconds * CAST(real, K_TICKS_PER_SECOND);
		}

		static long game_seconds_to_ticks_round(real seconds) {
			return CAST(long, game_seconds_to_ticks(seconds));
		}

		static game_ticks_t game_time_get() {
			auto game_time_globals = GameState::GameTimeGlobals();

			return game_time_globals->game_time;
		}

		static game_time_t game_time_get_elapsed() {
			auto game_time_globals = GameState::GameTimeGlobals();

			return game_time_globals->elapsed_time;
		}

		static game_ticks_t local_time_get() {
			auto game_time_globals = GameState::GameTimeGlobals();

			return game_time_globals->game_time;
		}

		static game_time_t local_time_get_elapsed() {
			auto game_time_globals = GameState::GameTimeGlobals();

			return game_time_globals->elapsed_time;
		}

		static bool game_in_progress() {
			auto game_time_globals = GameState::GameTimeGlobals();

			if (game_time_globals->initialized) {
				return game_time_globals->active || game_time_globals->paused;
			}

			return false;
		}

		static bool game_time_get_paused() {
			auto game_time_globals = GameState::GameTimeGlobals();

			return game_time_globals->paused;
		}

		static void game_time_set_paused(bool paused) {
			auto game_time_globals = GameState::GameTimeGlobals();

			if (game_time_globals->initialized) {
				game_time_globals->active = !paused;
			}

			game_time_globals->paused = paused;
		}

		static float game_time_get_speed() {
			auto game_time_globals = GameState::GameTimeGlobals();

			return game_time_globals->game_speed;
		}

		static void game_time_set_speed(float speed) {
			auto game_time_globals = GameState::GameTimeGlobals();

			game_time_globals->game_speed = speed;
		}
	};
};
