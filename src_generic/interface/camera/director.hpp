#pragma once
#include <precompile.h>
namespace Yelo
{
	namespace Enums
	{
		enum game_perspective : short;

		enum {
			_director_game_mode_following,
			_director_game_mode_orbiting,
			_director_game_mode_flying,
			_director_game_mode_editor,
			_director_game_mode_first_person,

			k_number_of_director_game_modes
		};
	};

	namespace blam
	{
		// Saves the current camera location to "camera.txt"
		void __cdecl director_save_camera();

		// Loads the camera location from "camera.txt" and places the camera there
		void __cdecl director_load_camera();

		short __cdecl director_desired_perspective(const datum_index unit_index, Enums::game_perspective& desired_perspective);
	};
};
