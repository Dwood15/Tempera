#pragma once

#include <macros_generic.h>

namespace Yelo::Enums {
	enum pp_component_status : uint {
		pp_component_status_initialised,
		pp_component_status_uninitialised,
		pp_component_status_initialisation_failed,

		_pp_component_status
	};

	enum postprocess_render_stage : short {
		_postprocess_render_stage_pre_blur,
		_postprocess_render_stage_pre_alpha_blended,
		_postprocess_render_stage_pre_hud,
		_postprocess_render_stage_pre_ui,
		_postprocess_render_stage_post_ui,
		_postprocess_render_stage_blur,

		_postprocess_render_stage
	};

	enum shader_variable_base_type : short {
		_shader_variable_base_type_boolean,
		_shader_variable_base_type_integer,
		_shader_variable_base_type_float,

		_shader_variable_base_type_argb_color,
		_shader_variable_base_type_matrix,
		_shader_variable_base_type_texture,

		_shader_variable_base_type
	};

	enum effect_activation_state : short {
		_effect_activation_state_initially_active,
		_effect_activation_state_is_in_cutscene,
		_effect_activation_state_player_is_zoomed,
		_effect_activation_state_player_using_a_vehicle,

		_effect_activation_control_state
	};

	enum parameter_process_type : short
	{
		_parameter_process_type_none,
		_parameter_process_type_static,
		_parameter_process_type_animated,
		_parameter_process_type_runtime_value,
		_parameter_process_type_texture,
		_parameter_process_type
	};
}
