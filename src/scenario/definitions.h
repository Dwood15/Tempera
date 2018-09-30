#pragma once

#include "enums/generic_enums.h"
#include "macros_generic.h"
#include "../math/real_math.h"
#include "../memory/datum_index.h"
#include "../math/int_math.h"
#include "../math/colors.h"
#include "../tags/group/markup.h"
#include "../tags/group/tagreference.h"
#include "../models/animations/recorded_animation_definitions.h"
#include "../game/objects/scenario_definitions_structures.h"
#include "enums/scenario_enums.h"

namespace Yelo::blam {
	bool __cdecl hs_scenario_postprocess(bool for_runtime);
};

namespace Yelo::TagGroups {
	struct hs_tag_reference {
		long          padA[6]; // 24
		tag_reference reference;
	};
	STAT_ASSERT(hs_tag_reference, 0x28)

	struct hs_source_file {
		tag_string name;
		tag_data   source;
	};
	STAT_ASSERT(hs_source_file, 0x34)

	struct scenario_starting_profile {
		tag_string    name;
		real_fraction starting_health_damage;
		real_fraction starting_shield_damage;
		struct {
			tag_reference weapon;
			short         rounds_loaded;
			short         rounds_total;
		}             weapons[2];
		byte          grenade_counts[4];
		long          padA[5];
	};

	STAT_ASSERT(scenario_starting_profile, 0x68)

	struct scenario_player {
		real_point3d position;
		angle        facing;
		short        team_designator;
		short        bsp_index;
		short        game_types[4];

		long padA[6];
	};
	STAT_ASSERT(scenario_player, 0x34)

	struct scenario_trigger_volume {
		// _scenario_trigger_volume_type_? = 0
		// _scenario_trigger_volume_type_? = 1
		short type; // This is actually treated as a skip-4 field in Halo1's defs
		unsigned short : 16;
		tag_string    name;
		real_vector3d data0;
		real_vector3d forward;
		real_vector3d up;
		real_point3d  position;
		real_vector3d transform;
	};
	STAT_ASSERT(scenario_trigger_volume, 0x60)

	struct scenario_netpoint {
		real_point3d  position;
		angle         facing_degrees;
		short         type;
		short         team_index;
		tag_reference weapon_group;
		long          padA[28];
	};
	STAT_ASSERT(scenario_netpoint, 0x94)

	struct scenario_netgame_equipment {
		unsigned long flags;
		short         game_types[4];
		short         team_index;
		short         respawn_time;
		datum_index   runtime_object_index; // initialized to NONE in scenario_load
		long          padA[11];
		real_point3d  position;
		angle         facing_degrees;
		tag_reference item_collection;
		long          padB[12];
	};
	STAT_ASSERT(scenario_netgame_equipment, 0x90)

	struct scenario_starting_equipment {
		unsigned long flags;
		short         game_types[4];
		long          padA[12];
		tag_reference item_collections[6];
		long          padB[12];
	};
	STAT_ASSERT(scenario_starting_equipment, 0xCC)


	struct scenario_cutscene_flag {
		unsigned long : 32;
		tag_string          name;
		real_point3d        position;
		real_euler_angles2d facing;
		long                padA[9];
	};
	STAT_ASSERT(scenario_cutscene_flag, 0x5C)

	struct scenario_cutscene_camera_point {
		unsigned long : 32;
		tag_string name;
		unsigned long : 32;
		real_point3d        position;
		real_euler_angles3d orientation;
		angle               field_of_view;
		long                padA[9];
	};
	STAT_ASSERT(scenario_cutscene_camera_point, 0x68)

	struct s_scenario_cutscene_title {
		unsigned long : 32;
		tag_string name;
		unsigned long : 32;
		rectangle2d text_bounds;
		short       string_index;
		unsigned short : 16;
		short justification;
		unsigned short : 16; // font enum in Halo2
		unsigned long  : 32;
		argb_color text_color;
		argb_color shadow_color;
		real       fade_in_time;
		real       up_time;
		real       fade_out_time;
		long       padA[4];
	};

	STAT_ASSERT(s_scenario_cutscene_title, 0x60);

	struct s_scenario_bsp_lightmap_set {
		tag_string        name;
		long              padA;
		tag_reference     standard_lightmap;
		tag_reference     directional_lightmap_1;
		tag_reference     directional_lightmap_2;
		tag_reference     directional_lightmap_3;
		tag_block padB[2];
	};

	struct s_scenario_bsp_sky_set_sky {
		unsigned short : 16;
		short         sky_index;
		tag_reference sky;
	};

	struct s_scenario_bsp_sky_set {
		tag_string                                       name;
		Yelo::TagBlock<const s_scenario_bsp_sky_set_sky> skies;
	};

	struct s_scenario_bsp_modifier {
		unsigned short : 16;
		short                                             bsp_index;
		Yelo::TagBlock<const s_scenario_bsp_lightmap_set> lightmap_sets;
		TagBlock<const s_scenario_bsp_sky_set>            sky_sets;
		tag_block                                 padA[3];
	};

	struct scenario_structure_bsp_reference {
		long                 cache_offset;
		long                 bsp_data_size;
		structure_bsp_header *header;
		unsigned long : 32;
		tag_reference structure_bsp;
	};
	static_assert(sizeof(scenario_structure_bsp_reference) == 0x20); // max count: 32

	struct scenario {
		// enum { k_group_tag = 'scnr' };

		tag_reference _dont_use;
		tag_reference _wont_use;
		tag_reference _cant_use;

		Yelo::TagBlock<tag_reference> skies;

		scenario_type type;
		unsigned short             flags;

		tag_block    padA; // scenario_child_scenario_reference
		angle             local_north;
		tag_data          padB;
		long              padAA[34]; // 136
		tag_block scenario_functionA;
		tag_block scenario_functionB;
		tag_data          padCC; // editor_scenario_data
		tag_block padDD; // editor_comment_definition
		tag_block h2_scenario_environment_object; // Halo2. scenario_environment_object
		long              padAB[53]; // 212

		Yelo::TagBlock<const scenario_object_name> object_names;

		Yelo::TagBlock<const s_scenario_scenery>            scenery;
		Yelo::TagBlock<const scenario_object_palette_entry> scenery_palette;
		//////////////////////////////////////////////////////////////////////////
		// units
		Yelo::TagBlock<const s_scenario_biped>              bipeds;
		Yelo::TagBlock<const scenario_object_palette_entry> bipeds_palette;
		Yelo::TagBlock<const s_scenario_vehicle>            vehicles;
		Yelo::TagBlock<const scenario_object_palette_entry> vehicles_palette;
		//////////////////////////////////////////////////////////////////////////
		// items
		Yelo::TagBlock<const s_scenario_equipment>          equipment;
		Yelo::TagBlock<const scenario_object_palette_entry> equipment_palette;
		Yelo::TagBlock<const s_scenario_weapon>             weapon;
		Yelo::TagBlock<const scenario_object_palette_entry> weapon_palette;
		//////////////////////////////////////////////////////////////////////////
		// devices
		Yelo::TagBlock<const scenario_device_group>         device_groups;
		Yelo::TagBlock<const s_scenario_machine>            machines;
		Yelo::TagBlock<const scenario_object_palette_entry> machines_palette;
		Yelo::TagBlock<const s_scenario_control>            controls;
		Yelo::TagBlock<const scenario_object_palette_entry> controls_palette;
		Yelo::TagBlock<const s_scenario_light_fixture>      light_fixtures;
		Yelo::TagBlock<const scenario_object_palette_entry> light_fixtures_palette;

		Yelo::TagBlock<const s_scenario_sound_scenery>      sound_scenery;
		Yelo::TagBlock<const scenario_object_palette_entry> sound_scenery_palette;

		tag_block padEE[2];

		long padUnk60[15]; // 60

		Yelo::TagBlock<const scenario_starting_profile>                      player_starting_profiles;
		Yelo::TagBlock<const scenario_player>                                player_starting_locations;
		Yelo::TagBlock<const scenario_trigger_volume>                        trigger_volumes;
		Yelo::TagBlock<const Yelo::TagGroups::recorded_animation_definition> recorded_animations;
		Yelo::TagBlock<const scenario_netpoint>                              netgame_flags;
		Yelo::TagBlock<scenario_netgame_equipment>                           netgame_equipment;
		Yelo::TagBlock<const scenario_starting_equipment>                    starting_equipment;
		tag_block                                                    tb_padA1;
		tag_block                                                    tb_padA2;
		tag_block                                                    tb_padA3;
		tag_block                                                    tb_padA4;

		long padkUnk36[9]; // 36

		tag_block pad_tbunkA[4];
		tag_block pad_tbunkB[7];

		tag_data hs_syntax_data;
		tag_data hs_string_data;

		Yelo::TagBlock<void *>          scripts;
		Yelo::TagBlock<void *> globals;
		Yelo::TagBlock<const hs_tag_reference>   references;
		Yelo::TagBlock<const hs_source_file>     source_files;

		tag_block h2_cs_script_data;
		tag_block tb_unknownASDF;

		Yelo::TagBlock<const scenario_cutscene_flag>         cutscene_flags;
		Yelo::TagBlock<const scenario_cutscene_camera_point> cutscene_camera_points;
		Yelo::TagBlock<const s_scenario_cutscene_title>      cutscene_titles;

		Yelo::TagBlock<const s_scenario_bsp_modifier> bsp_modifiers;

		long                                                   pad96[24]; // 96
		tag_reference                                          custom_object_names;
		tag_reference                                          ingame_help_text;
		tag_reference                                          hud_messages;
		Yelo::TagBlock<const scenario_structure_bsp_reference> structure_bsps;

		// Get the tag reference we redefined for users to reference yelo definitions
		tag_reference &GetYeloReferenceHack() { return _dont_use; }

		tag_reference const &GetYeloReferenceHack() const { return _dont_use; }

	};

	STAT_ASSERT(scenario, 0x5B0);
};
