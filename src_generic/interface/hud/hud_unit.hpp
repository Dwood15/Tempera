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
			k_maximum_number_of_hud_sounds = 12, // MAXIMUM_NUMBER_OF_HUD_SOUNDS
		};

		enum hud_auxilary_meter_type : short
		{
			_hud_auxilary_meter_type_integrated_light,

			k_number_of_hud_auxilary_meter_types,
		};
	};

	namespace Flags
	{
		enum unit_interface_flags : long_flags
		{
			_unit_interface_show_health_bit,
			_unit_interface_blink_health_bit,
			_unit_interface_show_shield_bit,
			_unit_interface_blink_shield_bit,
			_unit_interface_show_motion_sensor_bit,
			_unit_interface_blink_motion_sensor_bit,
		};
	};

	namespace GameUI
	{
		struct s_hud_unit_interface_unit
		{
			real shields, health;
			UNKNOWN_TYPE(real); // shield related (for gauging how much the last damage dealt, I believe)
			game_ticks_t last_update_time;
			struct {
				game_ticks_t hud_background_element;
				game_ticks_t health_panel_background_element;
				game_ticks_t motion_sensor_elements;
			}first_render_time;
			datum_index unit_index;
			unsigned short auxilary_meters_active[BIT_VECTOR_SIZE_IN_WORDS(Enums::k_number_of_hud_auxilary_meter_types)];
			game_time_t auxilary_meter_times[Enums::k_number_of_hud_auxilary_meter_types];
			unsigned short active_sound_elements;
			unsigned short : 16;
			datum_index sound_elements[Enums::k_maximum_number_of_hud_sounds]; // sound cache index
		}; static_assert( sizeof(s_hud_unit_interface_unit) == 0x58 );
		struct s_hud_unit_interface
		{
			s_hud_unit_interface_unit units[Enums::k_maximum_number_of_local_players]; // 0x0
			unsigned long flags; // 0x58, unit_interface_flags
		}; static_assert( sizeof(s_hud_unit_interface) == 0x5C );
		s_hud_unit_interface*		HudUnitInterface();
	};
};
