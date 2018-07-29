
#pragma once

#include "macros_generic.h"
#include "../../models/bsp_definitions.h"
#include "../../tags/group/base.h"

namespace Yelo::TagGroups {
	struct scenario_object_name {
		tag_string name;
		short      object_type;
		short      scenario_object_index;
	};

	STAT_ASSERT(scenario_object_name, 0x24);

	struct scenario_device_group {
		tag_string    name;
		real          initial_value;
		unsigned long flags;
		long          padA[3];
	};

	STAT_ASSERT(scenario_device_group, 0x34);

	struct scenario_object_palette_entry {
		tag_reference object_definition;
		unsigned long:32;
		unsigned long:32;
		unsigned long:32;
		unsigned long:32;
		unsigned long:32;
		unsigned long:32;
		unsigned long:32;
		unsigned long:32;
	};
	STAT_ASSERT(scenario_object_palette_entry, 0x30);

	//////////////////////////////////////////////////////////////////////////
	// object
	struct s_scenario_object_header {
		short palette_index;
		short name_index;
	};
	STAT_ASSERT(s_scenario_object_header, 0x4);

	struct s_scenario_object_datum {
		unsigned short      placement_flags;
		short               desired_permutation;
		real_point3d        position;
		real_euler_angles3d rotation;
	};

	STAT_ASSERT(s_scenario_object_datum, 0x1C);

	struct s_scenario_object_permutation {
		unsigned __int64:64;
		unsigned __int64:64;
		unsigned __int64:64;
		unsigned __int64:64;
	};

	STAT_ASSERT(s_scenario_object_permutation, sizeof(__int64) * 4);

	struct s_scenario_scenery_datum {
		unsigned __int64:64;
		s_scenario_object_permutation permutation;
	};

	STAT_ASSERT(s_scenario_scenery_datum, 0x20 + sizeof(__int64));

	struct s_scenario_sound_scenery_datum {
		unsigned __int64:64;
	};
	STAT_ASSERT(s_scenario_sound_scenery_datum, 0x8);
	//////////////////////////////////////////////////////////////////////////
	// units
	struct s_scenario_unit_datum {
		unsigned __int64:64;
		s_scenario_object_permutation permutation;
		real                          body_vitality;
		unsigned long                 flags;
		unsigned __int64:64;
	};

	struct s_scenario_biped_datum {
		long padA[8];
	};

	STAT_ASSERT(s_scenario_biped_datum, sizeof(long) * 8);

	struct s_scenario_vehicle_datum {
		long padA[8];
	};
	STAT_ASSERT(s_scenario_vehicle_datum, sizeof(long) * 8);

	//////////////////////////////////////////////////////////////////////////
	// items
	struct __attribute__((packed)) s_scenario_weapon_datum {
		unsigned __int64:64;
		s_scenario_object_permutation permutation;

		short          rounds_left;
		short          rounds_total;
		unsigned short flags;
		unsigned short padA;
		long           padB[3];
	};

	struct s_scenario_equipment_datum {
		unsigned short padA;
		unsigned short flags;
		unsigned long  padB;
	};

	//////////////////////////////////////////////////////////////////////////
	// devices
	struct s_scenario_device_datum {
		unsigned __int64:64;
		short         power_group_index;
		short         position_group_index;
		unsigned long flags;
	};

	struct s_scenario_machine_datum {
		unsigned long flags;
		long          padA[3];
	};

	struct s_scenario_control_datum {
		unsigned long flags;
		short         custom_name_index;
		unsigned short : 16;
		unsigned __int64:64;
	};
	struct s_scenario_light_fixture_datum {
		real_rgb_color color;
		real           intensity;
		angle          falloff;
		angle          cutoff;
		unsigned __int64:64;
		unsigned __int64:64;
	};

	STAT_ASSERT(s_scenario_light_fixture_datum, 0x10 + sizeof(real_rgb_color) + sizeof(real) + (sizeof(angle) * 2));

	struct __attribute__((packed)) s_scenario_object {
		s_scenario_object_header header;
		s_scenario_object_datum  object;
	};

	STAT_ASSERT(s_scenario_object, sizeof(s_scenario_object_header) + sizeof(s_scenario_object_datum));

	struct s_scenario_device : public s_scenario_object {
		s_scenario_device_datum device;
	};
	struct s_scenario_unit : public s_scenario_object {
		s_scenario_unit_datum unit;
	};

	struct s_scenario_scenery : public s_scenario_object {
		s_scenario_scenery_datum scenery;
	}; static_assert(sizeof(s_scenario_scenery) == 0x48);

	//////////////////////////////////////////////////////////////////////////
	// units
	struct s_scenario_biped : public s_scenario_unit {
		s_scenario_biped_datum biped;
	}; static_assert(sizeof(s_scenario_biped) == 0x78);
	struct s_scenario_vehicle : public s_scenario_unit {
		s_scenario_vehicle_datum vehicle;
	};
	static_assert(sizeof(s_scenario_vehicle) == 0x78);
	//////////////////////////////////////////////////////////////////////////
	// items
	struct s_scenario_equipment : public s_scenario_object {
		s_scenario_equipment_datum equipment;
	};
	static_assert(sizeof(s_scenario_equipment) == 0x28);


	struct __attribute__((packed)) s_scenario_weapon : public s_scenario_object {
		s_scenario_weapon_datum weapon;
	};

	STAT_ASSERT(s_scenario_weapon, 0x5C);

	//////////////////////////////////////////////////////////////////////////
	// devices
	struct s_scenario_machine : public s_scenario_device {
		s_scenario_machine_datum machine;
	};
	static_assert(sizeof(s_scenario_machine) == 0x40);
	struct s_scenario_control : public s_scenario_device {
		s_scenario_control_datum control;
	};
	static_assert(sizeof(s_scenario_control) == 0x40);
	struct s_scenario_light_fixture : public s_scenario_device {
		s_scenario_light_fixture_datum light_fixture;
	};
	static_assert(sizeof(s_scenario_light_fixture) == 0x58);

	struct s_scenario_sound_scenery : public s_scenario_object {
		s_scenario_sound_scenery_datum sound_scenery;
	};
	static_assert(sizeof(s_scenario_sound_scenery) == 0x28);
};
