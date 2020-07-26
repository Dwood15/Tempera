#pragma once



#include "../../ai/ai_script.h"
#include "../hs.h"

namespace Yelo
{
	namespace Scripting
	{
		template<Enums::hs_type type>
		struct hs_type_traits;

		struct hs_type_default_traits_for_scenario_datum
		{
			typedef short ctype;
		};
		struct hs_type_default_traits_for_game_state_datum
		{
			typedef datum_index ctype;
		};
		struct hs_type_default_traits_for_tag_index
		{
			typedef datum_index ctype;
		};


		template<>
		struct hs_type_traits<Enums::_hs_type_void>
		{
			typedef void ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_bool>
		{
			typedef bool ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_real>
		{
			typedef real ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_short>
		{
			typedef short ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_long>
		{
			typedef long ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_string>
		{
			typedef const char * ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_script>
		{
			typedef s_hs_script_container_datum_index ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_trigger_volume> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_cutscene_flag> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_cutscene_camera_point> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_cutscene_title> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_cutscene_recording> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_device_group> : hs_type_default_traits_for_scenario_datum {};

		template<>
		struct hs_type_traits<Enums::_hs_type_ai>
		{
			typedef AI::s_ai_index ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_ai_command_list> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_starting_profile> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_conversation> : hs_type_default_traits_for_scenario_datum {};

		// these technically aren't immediate scenario datum, but can be reconciled from a tag reference in the scenario
		template<>
		struct hs_type_traits<Enums::_hs_type_navpoint> : hs_type_default_traits_for_scenario_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_hud_message> : hs_type_default_traits_for_scenario_datum {};

		template<>
		struct hs_type_traits<Enums::_hs_type_object_list> : hs_type_default_traits_for_game_state_datum {};

		template<>
		struct hs_type_traits<Enums::_hs_type_sound> : hs_type_default_traits_for_tag_index {};
		template<>
		struct hs_type_traits<Enums::_hs_type_effect> : hs_type_default_traits_for_tag_index {};
		template<>
		struct hs_type_traits<Enums::_hs_type_damage> : hs_type_default_traits_for_tag_index {};
		template<>
		struct hs_type_traits<Enums::_hs_type_looping_sound> : hs_type_default_traits_for_tag_index {};
		template<>
		struct hs_type_traits<Enums::_hs_type_animation_graph> : hs_type_default_traits_for_tag_index {};
		template<>
		struct hs_type_traits<Enums::_hs_type_actor_variant> : hs_type_default_traits_for_tag_index {};
		template<>
		struct hs_type_traits<Enums::_hs_type_damage_effect> : hs_type_default_traits_for_tag_index {};
		template<>
		struct hs_type_traits<Enums::_hs_type_object_definition> : hs_type_default_traits_for_tag_index {};

		template<>
		struct hs_type_traits<Enums::_hs_type_enum_game_difficulty>
		{
			typedef Enums::game_difficulty_level ctype;
		};
		template<>
		struct hs_type_traits<Enums::_hs_type_enum_team>
		{
			typedef Enums::game_team ctype;
		};
		template<>
		struct hs_type_traits<Enums::_hs_type_enum_ai_default_state>
		{
			typedef Enums::actor_default_state ctype;
		};
		template<>
		struct hs_type_traits<Enums::_hs_type_enum_actor_type>
		{
			typedef Enums::actor_type ctype;
		};
		template<>
		struct hs_type_traits<Enums::_hs_type_enum_hud_corner>
		{
			typedef Enums::hud_anchor ctype;
		};

		template<>
		struct hs_type_traits<Enums::_hs_type_object> : hs_type_default_traits_for_game_state_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_unit> : hs_type_default_traits_for_game_state_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_vehicle> : hs_type_default_traits_for_game_state_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_weapon> : hs_type_default_traits_for_game_state_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_device> : hs_type_default_traits_for_game_state_datum {};
		template<>
		struct hs_type_traits<Enums::_hs_type_scenery> : hs_type_default_traits_for_game_state_datum {};

		struct hs_type_default_traits_for_object_name
		{
			typedef short ctype;
		};
		template<>
		struct hs_type_traits<Enums::_hs_type_object_name> : hs_type_default_traits_for_object_name {};
		template<>
		struct hs_type_traits<Enums::_hs_type_unit_name> : hs_type_default_traits_for_object_name {};
		template<>
		struct hs_type_traits<Enums::_hs_type_vehicle_name> : hs_type_default_traits_for_object_name {};
		template<>
		struct hs_type_traits<Enums::_hs_type_weapon_name> : hs_type_default_traits_for_object_name {};
		template<>
		struct hs_type_traits<Enums::_hs_type_device_name> : hs_type_default_traits_for_object_name {};
		template<>
		struct hs_type_traits<Enums::_hs_type_scenery_name> : hs_type_default_traits_for_object_name {};
	};
};
