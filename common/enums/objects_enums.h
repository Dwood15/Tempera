#pragma once

namespace Yelo::Enums {
	enum unit_speech_priority : short {
		_unit_speech_none,
		_unit_speech_idle,
		_unit_speech_pain,
		_unit_speech_talk,
		_unit_speech_communicate,
		_unit_speech_shout,
		_unit_speech_script,
		_unit_speech_involuntary,
		_unit_speech_exclaim,
		_unit_speech_scream,
		_unit_speech_death,

		k_number_of_unit_speech_priorities, // NUMBER_OF_UNIT_SPEECH_PRIORITIES
	};

	enum unit_scream_type : short {
		_unit_scream_type_fear,
		_unit_scream_type_enemy_grenade, // _dialogue_vocalization_hurt_enemy_grenade
		_unit_scream_type_pain,
		_unit_scream_type_maimed_limb,
		_unit_scream_type_mained_head,
		_unit_scream_type_resurrection,

		k_number_of_unit_scream_types, // NUMBER_OF_UNIT_SCREAM_TYPES
	};

	enum unit_camo_regrowth : short {
		_unit_camo_regrowth_off,
		_unit_camo_regrowth_on, // they fired their weapon, requiring active_camo_regrowth_rate to be applied
	};

	enum powered_seat {
		_powered_seat_driver,
		_powered_seat_gunner,

		k_number_of_powered_seats
	};

	enum group_tags : unsigned int {
		sbsp_group_tag = 'sbsp'
	};
};
