/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_configuration.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			_waypoint_type_none = NONE,

			_waypoint_type_cutscene_flag = 0,
			_waypoint_type_object,
			_waypoint_type_game_goal,

			k_number_of_waypoint_types,
		};
	};

	namespace GameUI
	{
		struct s_hud_nav_points
		{
			struct s_nav_point
			{
				short navpoint_index;
				struct {
					// if the bitfield size was calculated in their code, and not by hand then it
					// probably factored in k_number_of_waypoint_types's value (not value-1).
					// Then there's +1 more bit for the sign
					short type : 4;

					real vertical_offset;

					union {
						long cutscene_flag_index;
						datum_index object_index;
						short game_goal_index;
					};
				}waypoint;
			}; static_assert( sizeof(s_nav_point) == 0xC );

			struct s_local_player
			{
				s_nav_point nav_points[4];
			};

			s_local_player local_players[Enums::k_maximum_number_of_local_players];
		}; static_assert( sizeof(s_hud_nav_points) == 0x30 );
		s_hud_nav_points*			HudNavPoints();
	};

	namespace blam
	{
		void __cdecl render_nav_point(short local_player_index, const real_point3d& point,
			long waypoint_arrow_index, signed long waypoint_type);
	};
};
