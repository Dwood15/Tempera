/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/hud_definitions.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>
#include "../../tags/group/markup.h"
#include "../../tags/group/tag_groups_base_yelo.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum weapon_hud_state
		{
			_weapon_hud_state_total_ammo,
			_weapon_hud_state_loaded_ammo,
			_weapon_hud_state_heat,
			_weapon_hud_state_age,
			_weapon_hud_state_secondary_weapon_total_ammo,
			_weapon_hud_state_secondary_weapon_loaded_ammo,
			_weapon_hud_state_dist_to_target,
			_weapon_hud_state_elevation_to_target,

			k_number_of_weapon_hud_states,
		};

		enum weapon_crosshair_type
		{
			_weapon_crosshair_type_aim,
			_weapon_crosshair_type_zoom,
			_weapon_crosshair_type_charge,
			_weapon_crosshair_type_should_reload,
			_weapon_crosshair_type_flash_heat,
			_weapon_crosshair_type_flash_total_ammo,
			_weapon_crosshair_type_flash_battery,
			_weapon_crosshair_type_reload_or_overheat,
			_weapon_crosshair_type_flash_when_firing_and_no_ammo,
			_weapon_crosshair_type_flash_when_firing_and_no_grenade,
			_weapon_crosshair_type_low_ammo_and_none_left_to_reload,
			_weapon_crosshair_type_should_reload_secondary_trigger,
			_weapon_crosshair_type_flash_secondary_total_ammo,
			_weapon_crosshair_type_flash_secondary_reload,
			_weapon_crosshair_type_flash_when_firing_secondary_trigger_with_no_ammo,
			_weapon_crosshair_type_low_secondary_ammo_and_none_left_to_reload,
			_weapon_crosshair_type_primary_trigger_ready,
			_weapon_crosshair_type_secondary_trigger_ready,
			_weapon_crosshair_type_flash_when_firing_with_depleted_battery,

			k_number_of_weapon_crosshair_types,
		};
	};

	namespace Flags
	{
		enum
		{
			_weapon_hud_element_runtime_invalid_bit, // set by postprocessing code
		};

		enum
		{
			_weapon_hud_overlay_type_show_on_flashing_bit,
			_weapon_hud_overlay_type_show_on_empty_bit,
			_weapon_hud_overlay_type_show_on_reload_bit, // or overheating
			_weapon_hud_overlay_type_show_on_default_bit,
			_weapon_hud_overlay_type_show_always_bit,
		};
		enum
		{
			_weapon_hud_overlay_flashes_when_active_bit,
			_weapon_hud_overlay_invalid_bit, // not exposed. set during postprocess, when overlay references invalid sequence
		};

		enum grenade_hud_sound_flags : long_flags
		{
			_grenade_hud_sound_low_grenade_count_bit,
			_grenade_hud_sound_no_grenades_left_bit,
			_grenade_hud_sound_throw_on_no_grenades_bit,
		};
	};

	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// weapon hud interface
		struct weapon_hud_element
		{
			Enums::weapon_hud_state state_attached_to;
			byte_flags runtime_flags;
			unsigned char : 8;
			Enums::hud_use_on_map_type can_use_on_map_type; // actually treated as byte_flags
			unsigned short : 16;
			long:8 * sizeof(long) * 7;
		}; static_assert( sizeof(weapon_hud_element) == 0x24 );

		struct weapon_hud_static_element : public weapon_hud_element
		{
			s_hud_element_overlay overlay;
			long:8 * sizeof(long) * 10;
		}; static_assert( sizeof(weapon_hud_static_element) == 0xB4 );

		struct weapon_hud_meter_element : public weapon_hud_element
		{
			s_hud_element_meter element;
			long:8 * sizeof(long) * 10;
		}; static_assert( sizeof(weapon_hud_meter_element) == 0xB4 );

		struct weapon_hud_number_element : public weapon_hud_element
		{
			s_hud_element_number element;
			unsigned short weapon_flags;
			unsigned short : 16;
			long:8 * sizeof(long) * 9;
		}; static_assert( sizeof(weapon_hud_number_element) == 0xA0 );

		struct weapon_hud_crosshair_item
		{
			s_hud_element element;
			s_hud_color_flash flash;
			short frame_rate;
			short sequence_index;
			unsigned long flags;

			long:8 * sizeof(long) * 8;
		}; static_assert( sizeof(weapon_hud_crosshair_item) == 0x6C );
		struct weapon_hud_crosshairs_element
		{
			Enums::weapon_crosshair_type crosshair_type;
			byte_flags runtime_flags;
			unsigned char : 8;
			Enums::hud_use_on_map_type can_use_on_map_type; // actually byte_enum
			unsigned short : 16;
			long:8 * sizeof(long) * 7;
			tag_reference crosshair_bitmap;
			Yelo::TagBlock<const weapon_hud_crosshair_item> crosshair_overlays;
			long:8 * sizeof(long) * 10;
		}; static_assert( sizeof(weapon_hud_crosshairs_element) == 0x68 );

		struct weapon_hud_overlay_item
		{
			s_hud_element element;
			s_hud_color_flash flash;
			short frame_rate;
			unsigned short : 16;
			short sequence_index;
			unsigned short type;
			unsigned long flags;

			tag_reference:8 * sizeof(tag_reference) * 1;
			long:8 * sizeof(long) * 10;
		}; static_assert( sizeof(weapon_hud_overlay_item) == 0x88 );
		struct weapon_hud_overaly
		{
			tag_reference overlay_bitmap;
			Yelo::TagBlock<const weapon_hud_overlay_item> overlays;
		};
		struct weapon_hud_overlays_element : public weapon_hud_element
		{
			weapon_hud_overaly overlay;
			long:8 * sizeof(long) * 10;
		}; static_assert( sizeof(weapon_hud_overlays_element) == 0x68 );

		struct weapon_hud_messaging_information
		{
			short sequence_index;
			short width_offset;
			point2d offset_from_reference_corner;
			argb_color override_icon_color;
			sbyte frame_rate;
			byte_flags flags;
			short text_index;

			long:8 * sizeof(long) * 12;
		};

		struct weapon_hud_interface_definition
		{
			enum { k_group_tag = 'wphi' };

			tag_reference child_hud;
			struct {
				unsigned short flags;
				unsigned short : 16;
				short inventory_ammo;
				short loaded_ammo;
				short heat;
				short age;

				long:8 * sizeof(long) * 8;
			}flash_cutoffs;

			s_hud_absolute_placement placement;

			Yelo::TagBlock<const weapon_hud_static_element> static_elements;
			Yelo::TagBlock<const weapon_hud_meter_element> meter_elements;
			Yelo::TagBlock<const weapon_hud_number_element> number_elements;
			Yelo::TagBlock<const weapon_hud_crosshairs_element> crosshairs;
			Yelo::TagBlock<const weapon_hud_overlays_element> overlay_elements;
			unsigned long runtime_used_crosshairs[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_weapon_crosshair_types)];
			tag_block:8 * sizeof(tag_block) * 1;
			Yelo::TagBlock<const hud_screen_effect_definition> screen_effect;
			tag_block:8 * sizeof(tag_block) * 11;
			weapon_hud_messaging_information messaging_information;
		}; static_assert( sizeof(weapon_hud_interface_definition) == 0x17C );


		//////////////////////////////////////////////////////////////////////////
		// grenade hud interface
		struct grenade_hud_interface_definition
		{
			enum { k_group_tag = 'grhi' };
			
			s_hud_absolute_placement placement;
			s_hud_element_overlay hud_background;
			s_hud_element_overlay total_grenades_background;
			struct {
				s_hud_element_number element;
				short flash_cutoff;
				unsigned short : 16;
			}total_grenades_numbers;
			weapon_hud_overaly total_grenade_overlays;
			Yelo::TagBlock<const sound_hud_element_definition> warning_sounds;
			long:8 * sizeof(long) * 17;
			weapon_hud_messaging_information messaging_information;
		}; static_assert( sizeof(grenade_hud_interface_definition) == 0x1F8 );
	};
};
