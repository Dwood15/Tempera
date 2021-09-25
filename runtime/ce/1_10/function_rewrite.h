/*
	Copyright � 2018 Dwood15

	This file is part of tempera.

    You should have received a copy of the GNU General Public License
    along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once



//yolo coding at its finest.
namespace spcore {
	namespace initializations {
		void __inline patch_game_state_allocat_func();

		void __cdecl motion_sensor_initialize_for_new_map();

		int __fastcall interface_get_tag_index(short x);

		void interface_initialize_for_new_map();
		void players_initialize_for_new_map();
		void scripted_hud_messages_clear();
	};
	namespace rendering {
		//This function is good for debugging- it gets called every frame, if not more.
		//__declspec(naked) int gen_render_window_count();
	};

	namespace player_updates {
		int calculatePlayerCount();
		void compute_combined_pvs_local();
	};

	namespace player_control {
		// 			signature: "8B 35 20 59 81 00 57 8B FA B9 .26 00 00 00 F3 AB 83 CF FF"
		//		extern uintptr_t players_init_for_new_map_overwrite; // overwrite the .26 with the size of the 4 player structure.
		void __cdecl  player_control_initialize_for_new_map();

		const void player_control_initialize();
	};
};
