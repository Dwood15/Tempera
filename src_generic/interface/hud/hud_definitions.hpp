/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_waypoints = 16,
		};

		enum hud_anchor : short
		{
			_hud_anchor_top_left,
			_hud_anchor_top_right,
			_hud_anchor_bottom_left,
			_hud_anchor_bottom_right,
			_hud_anchor_center,

			k_number_of_hud_anchors,
		};


		enum hud_multitexture_overlay_effector_type : short
		{
			_hud_multitexture_overlay_effector_type_tint,
			_hud_multitexture_overlay_effector_type_horizontal_offset,
			_hud_multitexture_overlay_effector_type_vertical_offset,
			_hud_multitexture_overlay_effector_type_fade,

			_hud_multitexture_overlay_effector_type,
		};
		enum hud_multitexture_overlay_effector_destination : short
		{
			_hud_multitexture_overlay_effector_destination_geometry_offset,
			_hud_multitexture_overlay_effector_destination_primary_map,
			_hud_multitexture_overlay_effector_destination_secondary_map,
			_hud_multitexture_overlay_effector_destination_tertiary_map,

			_hud_multitexture_overlay_effector_destination,
		};
		enum hud_multitexture_overlay_effector_source : short
		{
			_hud_multitexture_overlay_effector_source_player_pitch,
			_hud_multitexture_overlay_effector_source_player_pitch_tangent,
			_hud_multitexture_overlay_effector_source_player_yaw,
			_hud_multitexture_overlay_effector_source_weapon_ammo_total,
			_hud_multitexture_overlay_effector_source_weapon_ammo_loaded,
			_hud_multitexture_overlay_effector_source_weapon_heat,
			_hud_multitexture_overlay_effector_source_explicit_uses_low_bound,
			_hud_multitexture_overlay_effector_source_weapon_zoom_level,

			_hud_multitexture_overlay_effector_source,
		};

		enum hud_multitexture_overlay_anchor : short
		{
			_hud_multitexture_overlay_anchor_texture,
			_hud_multitexture_overlay_anchor_screen,

			_hud_multitexture_overlay_anchor,
		};
		enum hud_multitexture_overlay_blend_mode : short
		{
			_hud_multitexture_overlay_blend_mode_add,
			_hud_multitexture_overlay_blend_mode_subtract,
			_hud_multitexture_overlay_blend_mode_multiply,
			_hud_multitexture_overlay_blend_mode_multiply2x,
			_hud_multitexture_overlay_blend_mode_dot,

			_hud_multitexture_overlay_blend_mode,
		};
		enum hud_multitexture_overlay_address : short
		{
			_hud_multitexture_overlay_address_clamp,
			_hud_multitexture_overlay_address_wrap,

			_hud_multitexture_overlay_address,
		};
	};

	namespace Flags
	{
		enum hud_scaling_flags : unsigned short
		{
			_hud_scaling_dont_scale_offset_bit,
			_hud_scaling_dont_scale_size_bit,
			_hud_scaling_use_high_res_scale_bit,
		};

		enum hud_flash_flags : unsigned short
		{
			_hud_flash_reverse_colors_bit, // reverse default/flashing
		};

		enum hud_meter_flags : byte_flags
		{
			_hud_meter_use_min_max_for_state_changes_bit,
			_hud_meter_interpolate_between_min_max_flash_colors_as_state_changes_bit,
			_hud_meter_interpolate_color_along_hsv_space_bit,
			_hud_meter_more_colors_for_hsv_interpolation_bit,
			_hud_meter_invert_interpolation_bit,
		};

		enum hud_auxilary_overlay_flags : unsigned short
		{
			_hud_auxilary_overlay_use_team_color_bit,
		};
		enum hud_auxilary_panel_flags : long_flags
		{
			_hud_auxilary_panel_show_only_when_active_bit,
			_hud_auxilary_panel_flash_once_if_activated_while_disabled_bit,
		};
	};

	namespace TagGroups
	{
		struct s_hud_absolute_placement
		{
			short anchor;
			unsigned short : 16;
			long:8 * sizeof(long) * 8;
		}; static_assert( sizeof(s_hud_absolute_placement) == 0x24 );

		struct s_hud_element // same size as s_hud_absolute_placement...I think this may be union'd
		{
			point2d anchor_offset;
			real width_scale;
			real height_scale;
			Flags::hud_scaling_flags scaling_flags;
			unsigned short : 16;
			long:8 * sizeof(long) * 5;
		}; static_assert( sizeof(s_hud_element) == 0x24 );

		struct s_hud_color_flash // aka global_hud_color
		{
			argb_color default_color;
			argb_color flashing_color;
			real flash_period;
			real flash_delay;
			short number_of_flashes;
			Flags::hud_flash_flags flash_flags;
			real flash_length;
			argb_color disabled_color;
			//////////////////////////////////////////////////////////////////////////
			// only seen objective colors expose these fields
			short uptime_ticks;
			short fade_ticks;
		}; static_assert( sizeof(s_hud_color_flash) == 0x20 );

		struct icon_hud_element_definition
		{
			short sequence_index;
			short width_offset;
			point2d offset_from_reference_corner;
			argb_color override_icon_color;
			sbyte frame_rate;
			byte_flags flags;
			short text_index;
		}; static_assert( sizeof(icon_hud_element_definition) == 0x10 );


		struct hud_messaging_definition
		{
			s_hud_absolute_placement placement;
			s_hud_element element;
			tag_reference single_player_font;
			tag_reference multiplayer_font;
			real up_time;
			real fade_time;
			real_argb_color icon_color;
			real_argb_color text_color;
			real text_spacing;
			tag_reference item_message_text;
			tag_reference icon_bitmap;
			tag_reference alternate_icon_text;
			Yelo::TagBlock<const icon_hud_element_definition> button_icons;
		}; static_assert( sizeof(hud_messaging_definition) == 0xD0 );

		struct hud_waypoint_arrow
		{
			tag_string name;
			PAD64;
			rgb_color color;
			real opacity;
			real translucency;
			short on_screen_sequence_index;
			short off_screen_sequence_index;
			short occluded_sequence_index;
			unsigned short : 16;
			long:8 * sizeof(long) * 4;
			unsigned long flags;
			long:8 * sizeof(long) * 6;
		}; static_assert( sizeof(hud_waypoint_arrow) == 0x68 );

		struct hud_globals_definition
		{
			hud_messaging_definition hud_messaging;
			s_hud_color_flash hud_help_text_color;
			tag_reference hud_messages;
			s_hud_color_flash objective_colors;
			struct {
				real_rectangle2d offset;
				tag_reference:8 * sizeof(tag_reference) * 2;
				tag_reference arrow_bitmap;
				Yelo::TagBlock<const hud_waypoint_arrow> arrows;
				long:8 * sizeof(long) * 20;
			}waypoint;
			struct {
				real hud_scale;
				tag_reference:8 * sizeof(tag_reference) * 16;
			}multiplayer;
			struct {
				tag_reference weapon_hud;
				real motion_sensor_range;
				real motion_sensor_velocity_sensitivity;
				real motion_sensor_scale;
				rectangle2d chapter_title_bounds;
				long:8 * sizeof(long) * 11;
			}defaults;
			struct {
				struct { // short_rectangle2d
					short x0, x1;
					short y0, y1;
				}offset;
				tag_reference:8 * sizeof(tag_reference) * 2;
				tag_reference bitmap;
				short sequence_index;
				short multiplayer_sequence_index;
				argb_color color;
				tag_reference:8 * sizeof(tag_reference) * 1;
			}damage_indicators;
			struct {
				s_hud_color_flash not_much_time_left;
				s_hud_color_flash out_of_time;
				long:8 * sizeof(long) * 10;
			}hud_timer;
			tag_reference carnage_report_bitmap;
			struct {
				struct {
					short begin_text;
					short end_text;
				}loading_text,
				 checkpoint_text;
				tag_reference checkpoint_sound;
				long:8 * sizeof(long) * 24;
			}misc;
		}; static_assert( sizeof(hud_globals_definition) == 0x450 );


		//////////////////////////////////////////////////////////////////////////
		// generic hud elements
		struct s_hud_element_number : public s_hud_element // aka global_hud_element
		{
			s_hud_color_flash color;
			sbyte maximum_number_of_digits;
			byte_flags flags;
			sbyte number_of_fractional_digits;
			unsigned char : 8;
			tag_block:8 * sizeof(tag_block) * 1;
		}; static_assert( sizeof(s_hud_element_number) == 0x54 );

		struct s_hud_element_static : public s_hud_element // aka global_hud_interface_element
		{
			tag_reference interface_bitmap;
			s_hud_color_flash color;
		}; static_assert( sizeof(s_hud_element_static) == 0x54 );

		struct s_hud_element_meter : public s_hud_element // aka global_hud_meter
		{
			tag_reference interface_bitmap;
			rgb_color color_at_meter_minimum;
			rgb_color color_at_meter_maximum;
			rgb_color flash_color;
			argb_color empty_color;
			Flags::hud_meter_flags flags;
			byte minumum_meter_value;
			short sequence_index;
			byte alpha_multiplier;
			byte alpha_bias;
			short value_scale;
			real opacity;
			real translucency;
			argb_color disabled_color;
			tag_block:8 * sizeof(tag_block) * 1;
			unsigned long : 32;
		}; static_assert( sizeof(s_hud_element_meter) == 0x68 );

		struct hud_screen_effect_definition
		{
			unsigned long : 32;
			struct {
				unsigned short flags;
				unsigned short : 16;
				tag_reference:8 * sizeof(tag_reference) * 1;
				tag_reference fullscreen_bitmap;
				tag_reference splitscreen_bitmap;
				long:8 * sizeof(long) * 2;
			}mask;
			struct {
				unsigned short flags;
				unsigned short : 16;
				angle_bounds fov_in_bounds;
				real_bounds radius_out_bounds;
				long:8 * sizeof(long) * 6;
			}convolution;
			struct {
				unsigned short flags;
				short script_source;
				real intensity;
				long:8 * sizeof(long) * 6;
			}night_vision;
			struct {
				unsigned short flags;
				short script_source;
				real intensity;
				real_rgb_color tint;
				long:8 * sizeof(long) * 6;
			}desaturation;
		}; static_assert( sizeof(hud_screen_effect_definition) == 0xB8 );

		struct sound_hud_element_definition
		{
			tag_reference sound;
			unsigned long latched_to; // unit_hud_sound_flags, grenade_hud_sound_flags
			real scale;
			long:8 * sizeof(long) * 8;
		}; static_assert( sizeof(sound_hud_element_definition) == 0x38 );


		//////////////////////////////////////////////////////////////////////////
		// hud overlays
		struct multitexture_overlay_hud_element_effector_definition // aka global_hud_multitexture_overlay_effector_definition
		{
			long:8 * sizeof(long) * 16;

			////////////////////////////////////////////////////////////////
			// source/destination
			// These describe the relationship that causes the effect.
			// * destination type is the type of variable you want to be effected
			// * destination tells which texture map (or geometry offset) to apply it to
			// * source says which value to look at when computing the effect
			short destination_type;
			short destination;
			short source;
			unsigned short : 16;

			////////////////////////////////////////////////////////////////
			// in/out bounds
			// When the source is at the lower inbound, the destination ends up the lower outbound and vice-versa applies for the upper values.
			real_bounds in_bounds;
			real_bounds out_bounds;
			long:8 * sizeof(long) * 16;

			////////////////////////////////////////////////////////////////
			// tint color bounds
			// If destination is tint, these values are used instead of the out bounds.
			real_rgb_color tint_color_lower_bound;
			real_rgb_color tint_color_upper_bound;

			////////////////////////////////////////////////////////////////
			// periodic functions
			// If you use a periodic function as the source, this lets you tweak it.
			short periodic_function;
			unsigned short : 16;
			real function_period;
			real function_phase;
			long:8 * sizeof(long) * 8;
		}; static_assert( sizeof(multitexture_overlay_hud_element_effector_definition) == 0xDC ); // max count: 30
		struct multitexture_overlay_hud_element_definition // aka 'global_hud_multitexture_overlay_definition'
		{
			unsigned short : 16;
			short type;
			short framebuffer_blend_func;
			unsigned short : 16;
			long:8 * sizeof(long) * 8;

			////////////////////////////////////////////////////////////////
			// anchors
			// where you want the origin of the texture.
			// *"texture" uses the texture coordinates supplied
			// *"screen" uses the origin of the screen as the origin of the texture
			short primary_anchor;
			short secondary_anchor;
			short tertiary_anchor;

			////////////////////////////////////////////////////////////////
			// blending function
			// how to blend the textures together
			short _0_to_1_blend_func;
			short _1_to_2_blend_func;
			unsigned short : 16;

			////////////////////////////////////////////////////////////////
			// map scales
			// how much to scale the textures
			real_point2d primary_scale;
			real_point2d secondary_scale;
			real_point2d tertiary_scale;

			////////////////////////////////////////////////////////////////
			// map offsets
			// how much to offset the origin of the texture
			real_point2d primary_offset;
			real_point2d secondary_offset;
			real_point2d tertiary_offset;

			////////////////////////////////////////////////////////////////
			// map
			// which maps to use
			tag_reference primary;
			tag_reference secondary;
			tag_reference tertiary;
			short primary_wrap_mode;
			short secondary_wrap_mode;
			short tertiary_wrap_mode;
			unsigned short : 16;
			long:8 * sizeof(long) * 46;
			Yelo::TagBlock<const multitexture_overlay_hud_element_effector_definition> effectors;
			long:8 * sizeof(long) * 32;
		}; static_assert( sizeof(multitexture_overlay_hud_element_definition) == 0x1E0 ); // max count: 30

		struct s_hud_element_overlay : public s_hud_element_static
		{
			short sequence_index;
			unsigned short : 16;
			Yelo::TagBlock<const multitexture_overlay_hud_element_definition> multitex_overlay;
			unsigned long : 32;
		}; static_assert( sizeof(s_hud_element_overlay) == 0x68 );
	};

	namespace blam
	{
		extern const char * g_hud_anchor_names[Enums::k_number_of_hud_anchors];
	};
};
