#pragma once

namespace Yelo::Enums {
	enum periodic_function : short {
		_periodic_function_one,
		_periodic_function_zero,
		_periodic_function_cosine,
		_periodic_function_cosine_variable,
		_periodic_function_diagonal_wave,
		_periodic_function_diagonal_wave_variable,
		_periodic_function_slide,
		_periodic_function_slide_variable,
		_periodic_function_noise,
		_periodic_function_jitter,
		_periodic_function_wander,
		_periodic_function_spark,

		k_number_of_periodic_functions,
	};

	enum transition_function : short {
		_transition_function_linear,
		_transition_function_early,
		_transition_function_very_early,
		_transition_function_late,
		_transition_function_very_late,
		_transition_function_cosine,

		k_number_of_transition_functions,
	};
};
