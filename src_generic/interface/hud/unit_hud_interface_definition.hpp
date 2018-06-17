#pragma once

#include <precompile.h>
#include "hud_unit.hpp"

namespace Yelo
{
	namespace Flags
	{
		enum auxilary_overlay_type : short
		{
			_auxilary_overlay_type_team_icon,

			_auxilary_overlay_type,
		};

		enum hud_use_on_map_type : short
		{
			_hud_use_on_map_type_any,
			_hud_use_on_map_type_solo,
			_hud_use_on_map_type_multiplayer,

			_hud_use_on_map_type,
		};

		enum unit_hud_sound_flags : long_flags
		{
			_unit_hud_sound_shield_recharging_bit,
			_unit_hud_sound_shield_damaged_bit,
			_unit_hud_sound_shield_low_bit,
			_unit_hud_sound_shield_empty_bit,
			_unit_hud_sound_health_low_bit,
			_unit_hud_sound_health_empty_bit,
			_unit_hud_sound_health_minor_damage_bit,
			_unit_hud_sound_health_major_damage_bit,
		};
	};

	namespace TagGroups
	{
		struct auxilary_overlay_definition : public s_hud_element_overlay
		{
			Yelo::Enums::auxilary_overlay_type type;
			Flags::hud_auxilary_overlay_flags flags;
			long:8 * sizeof(long) * 6;
		}; static_assert( sizeof(auxilary_overlay_definition) == 0x84 ); // max count: 16

		struct auxilary_meter_definition
		{
			Enums::hud_auxilary_meter_type type;
			unsigned short : 16;
			long:8 * sizeof(long) * 4;

			s_hud_element_overlay background;
			s_hud_element_meter meter;
			real minimum_fraction_cutoff;
			Flags::hud_auxilary_panel_flags flags;
			long:8 * sizeof(long) * 6;
			long:8 * sizeof(long) * 16;
		}; static_assert( sizeof(auxilary_meter_definition) == 0x144 ); // max count: 16

		struct unit_hud_interface_definition
		{
			enum { k_group_tag = 'unhi' };

			s_hud_absolute_placement placement;

			s_hud_element_overlay hud_background;

			struct {
				s_hud_element_overlay background;

				struct {
					s_hud_element_meter element;
					rgb_color overcharge_minimum_color;
					rgb_color overcharge_maximum_color;
					rgb_color overcharge_flash_color;
					rgb_color overcharge_empty_color;
					long:8 * sizeof(long) * 4;
				}meter;
			}shield_panel;

			struct {
				s_hud_element_overlay background;

				struct {
					s_hud_element_meter element;
					rgb_color medium_health_left_color;
					real max_color_health_fraction_cutoff;
					real min_color_health_fraction_cutoff;
					long:8 * sizeof(long) * 5;
				}meter; // same size as shield_panel.meter, so this may be union'd
			}health_panel;

			struct {
				s_hud_element_overlay background;
				s_hud_element_overlay forground;
				long:8 * sizeof(long) * 8;

				s_hud_element center; // The blips use this as a reference point
			}motion_sensor;

			struct {
				s_hud_absolute_placement placement;

				Yelo::TagBlock<const auxilary_overlay_definition> overlays;
			}auxilary_overlays;

			long:8 * sizeof(long) * 4;

			Yelo::TagBlock<const sound_hud_element_definition> sounds;

			Yelo::TagBlock<const auxilary_meter_definition> meters;

			long:8 * sizeof(long) * 89;
			long:8 * sizeof(long) * 12;
		}; static_assert( sizeof(unit_hud_interface_definition) == 0x56C );
	};
};
