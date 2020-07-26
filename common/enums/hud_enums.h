#pragma once

namespace Yelo::Enums {
	enum {
		k_maximum_number_of_waypoints = 16,
	};

	enum hud_anchor : unsigned short {
		_hud_anchor_top_left,
		_hud_anchor_top_right,
		_hud_anchor_bottom_left,
		_hud_anchor_bottom_right,
		_hud_anchor_center,

		k_number_of_hud_anchors,
	};

	enum hud_multitexture_overlay_effector_type : unsigned short {
		_hud_multitexture_overlay_effector_type_tint,
		_hud_multitexture_overlay_effector_type_horizontal_offset,
		_hud_multitexture_overlay_effector_type_vertical_offset,
		_hud_multitexture_overlay_effector_type_fade,

		_hud_multitexture_overlay_effector_type,
	};
	enum hud_multitexture_overlay_effector_destination : unsigned short {
		_hud_multitexture_overlay_effector_destination_geometry_offset,
		_hud_multitexture_overlay_effector_destination_primary_map,
		_hud_multitexture_overlay_effector_destination_secondary_map,
		_hud_multitexture_overlay_effector_destination_tertiary_map,

		_hud_multitexture_overlay_effector_destination,
	};
	enum hud_multitexture_overlay_effector_source : unsigned short {
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

	enum hud_multitexture_overlay_anchor : unsigned short {
		_hud_multitexture_overlay_anchor_texture,
		_hud_multitexture_overlay_anchor_screen,

		_hud_multitexture_overlay_anchor,
	};
	enum hud_multitexture_overlay_blend_mode : unsigned short {
		_hud_multitexture_overlay_blend_mode_add,
		_hud_multitexture_overlay_blend_mode_subtract,
		_hud_multitexture_overlay_blend_mode_multiply,
		_hud_multitexture_overlay_blend_mode_multiply2x,
		_hud_multitexture_overlay_blend_mode_dot,

		_hud_multitexture_overlay_blend_mode,
	};
	enum hud_multitexture_overlay_address : unsigned short {
		_hud_multitexture_overlay_address_clamp,
		_hud_multitexture_overlay_address_wrap,

		_hud_multitexture_overlay_address,
	};
};

namespace Yelo::Flags {
	enum hud_scaling_flags : unsigned short {
		_hud_scaling_dont_scale_offset_bit,
		_hud_scaling_dont_scale_size_bit,
		_hud_scaling_use_high_res_scale_bit,
	};

	enum hud_flash_flags : unsigned short {
		_hud_flash_reverse_colors_bit, // reverse default/flashing
	};

	enum hud_meter_flags : unsigned char {
		_hud_meter_use_min_max_for_state_changes_bit,
		_hud_meter_interpolate_between_min_max_flash_colors_as_state_changes_bit,
		_hud_meter_interpolate_color_along_hsv_space_bit,
		_hud_meter_more_colors_for_hsv_interpolation_bit,
		_hud_meter_invert_interpolation_bit,
	};

	enum hud_auxilary_overlay_flags : unsigned short {
		_hud_auxilary_overlay_use_team_color_bit,
	};
	enum hud_auxilary_panel_flags : unsigned int {
		_hud_auxilary_panel_show_only_when_active_bit,
		_hud_auxilary_panel_flash_once_if_activated_while_disabled_bit,
	};
};
