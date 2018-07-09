/* See LICENSE.md for specific license information */
#pragma once

#include <d3dx9math.inl>
#include "hs.h"
#include "runtime.inl"
#include "../effects/damage/damage_effect_definitions.hpp"
#include "../models/animations/definitions.hpp"
#include "../ai/actors/actor_definitions.hpp"
#include "../effects/effect_definitions.hpp"
#include "../effects/sound/sound_definitions.hpp"
#include "../game/game.hpp"
#include "../game/allegiance.h"
#include "../ai/actors/actors.hpp"
#include "../ai/actors/actor_types.hpp"
#include "../interface/hud/hud_definitions.hpp"
#include "../memory/upgrades/blam_memory_upgrades.hpp"

namespace Yelo {
	namespace Scripting {
		typedef s_hs_value_union (__stdcall*proc_hs_typecast)(s_hs_value_union value);

		static proc_hs_typecast k_hs_typecasting_procedures[Enums::k_number_of_hs_types][Enums::k_number_of_hs_types];

		static void HsTypesInitializeTypeCastingProcedures() {
			using namespace Enums;
			using namespace blam;

			typedef proc_hs_typecast (&typecasting_procedures_t)[k_number_of_hs_types];

			typecasting_procedures_t
				void_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_void],
				bool_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_bool],
				real_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_real],
				short_procedures  = k_hs_typecasting_procedures[hs_type::_hs_type_short],
				long_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_long],
				string_procedures = k_hs_typecasting_procedures[hs_type::_hs_type_string],

				object_list_procedures = k_hs_typecasting_procedures[_hs_type_object_list];

			// _hs_type_void
			for (short type = _hs_type_data__first; type < _hs_type_data__last + 1; type++) {
				void_procedures[type] = hs_value_to_void;
			}

			// _hs_type_bool
			bool_procedures[_hs_type_real]   = hs_long_to_boolean; // yes long, (this is what the engine does)
			bool_procedures[_hs_type_short]  = hs_short_to_boolean;
			bool_procedures[_hs_type_long]   = hs_long_to_boolean;
			bool_procedures[_hs_type_string] = hs_string_to_boolean;

			// _hs_type_real
			real_procedures[_hs_type_bool]  = hs_bool_to_real;
			real_procedures[_hs_type_short] = hs_short_to_real;
			real_procedures[_hs_type_long]  = hs_long_to_real;
			//			real_procedures[_hs_type_string] =	hs_string_to_real;
			for (short type = _hs_type_enum__first; type < _hs_type_enum__last + 1; type++) {
				real_procedures[type] = hs_enum_to_real;
			}

			// _hs_type_short
			short_procedures[_hs_type_bool] = hs_bool_to_short;
			short_procedures[_hs_type_real] = hs_real_to_short;
			short_procedures[_hs_type_long] = hs_long_to_short;
			//			short_procedures[_hs_type_string] =	hs_string_to_short;

			// _hs_type_long
			long_procedures[_hs_type_bool]  = hs_bool_to_long;
			long_procedures[_hs_type_real]  = hs_real_to_long;
			// NOTE: due to a typo, the engine actually didn't support casting from short to long
			long_procedures[_hs_type_short] = hs_short_to_long;
			//			long_procedures[_hs_type_string] =	hs_string_to_long;

			// _hs_type_string

			// _hs_type_object_list
			object_list_procedures[_hs_type_ai] = object_list_from_ai_reference;
			for (short type = _hs_type_object__first; type < _hs_type_object__last + 1; type++) {
				object_list_procedures[type] = hs_object_to_object_list;
			}

			for (short type = _hs_type_object_name__first; type < _hs_type_object_name__last + 1; type++) {
				object_list_procedures[type] = hs_object_name_to_object_list;
			}
		}

		void HsTypesInitialize() {
			HsTypesInitializeTypeCastingProcedures();
		}

		void HsTypesDispose() {

		}

		typedef bool (__stdcall *proc_hs_type_parse)(Enums::hs_type type, datum_index expression_index);

		typedef void (__stdcall *proc_hs_type_inspect)(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size);

		struct c_hs_type_void_abi;

		class c_hs_type_abi;

		static const c_hs_type_abi *asb1 = &{k_hs_unparsed_abi, Yelo::Enums::_hs_unparsed;};
		static const c_hs_type_abi *asb2 = &{k_hs_special_form_abi, Yelo::Enums::_hs_special_form;};
		static const c_hs_type_abi *asb3 = &{k_hs_function_name_abi, Yelo::Enums::_hs_function_name;};
		static const c_hs_type_abi *asb4 = &{k_hs_passthrough_abi, Yelo::Enums::_hs_passthrough;};
		static const c_hs_type_abi *asb5 = &{k_hs_type_void_abi, Yelo::Enums::_hs_type_void;};
		static const c_hs_type_abi *asb6 = &{k_hs_type_bool_abi, Yelo::Enums::_hs_type_bool;};

		class c_hs_type_abi {
		public:
			static const c_hs_type_abi *Get(Enums::hs_type type) {
				static std::array<const c_hs_type_abi *, Enums::k_number_of_hs_types> hs_type_abis = {
					{asb1},
					{asb2},
					{asb3},
					{asb4},
					{asb5},
					{asb6}
				};

				return hs_type_abis[type];
			}

		protected:
			Enums::hs_type m_type;
			unsigned short : 16;

		public:
			c_hs_type_abi(Enums::hs_type type) : m_type(type) {}

			Enums::hs_type GetType() const { return m_type; }

			const char * GetTypeName() const { return blam::hs_type_names[m_type]; }

			virtual s_hs_value_union GetDefaultValue();

			virtual s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union *source) const;

			virtual void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union &destination) const;

			virtual size_t GetSize() const;

			virtual bool Parse(Enums::hs_type type, datum_index expression_index) const;

			virtual bool Inspectable() const;

			virtual void Inspect(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) const;

			virtual bool CastableToValueUnion() const { return CastableTo_hs_type_long(); }

			virtual s_hs_value_union GetValue(s_hs_value_union value) { return GetValue(value.long); }

			virtual bool CastableTo_hs_type_bool() const { return false; }

			virtual s_hs_value_union GetValue(bool value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_real() const { return false; }

			virtual s_hs_value_union GetValue(real value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_short() const { return false; }

			virtual s_hs_value_union GetValue(short value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_long() const { return false; }

			virtual s_hs_value_union GetValue(long value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_string() const { return false; }

			virtual s_hs_value_union GetValue(const char * value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_script() const { return false; }

			virtual s_hs_value_union GetValue(hs_script_index_t index) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_game_difficulty() const { return false; }

			virtual s_hs_value_union GetValue(Enums::game_difficulty_level value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_team() const { return false; }

			virtual s_hs_value_union GetValue(Enums::game_team value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_ai_default_state() const { return false; }

			virtual s_hs_value_union GetValue(Enums::actor_default_state value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_actor_type() const { return false; }

			virtual s_hs_value_union GetValue(Enums::actor_type value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_hud_corner() const { return false; }

			virtual s_hs_value_union GetValue(Enums::hud_anchor value) const { return k_none_value_union; }
		};

		// Yes, I declared as structs, to avoid having to explicitly say 'public' for inheritance and members

		struct c_hs_type_void_abi final : c_hs_type_abi {
			inline c_hs_type_void_abi(Enums::hs_type type) : c_hs_type_abi(type) {}

			s_hs_value_union GetDefaultValue() const override { return k_none_value_union; }

			s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union *source) const override { return k_none_value_union; }

			void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union &destination) const override {}

			size_t GetSize() const override { return 0; }

			bool Parse(Enums::hs_type type, datum_index expression_index) const override { return false; }

			bool Inspectable() const override { return false; }

			void Inspect(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) const override {}
		};

		struct c_hs_type_value_abi : c_hs_type_abi {
		protected:
			s_hs_value_union     m_default_value;
			proc_hs_type_parse   m_parse;
			proc_hs_type_inspect m_inspect;

		public:
			c_hs_type_value_abi(Enums::hs_type type, s_hs_value_union default_value, proc_hs_type_parse parse, proc_hs_type_inspect inspect)
				: c_hs_type_abi(type), m_default_value(default_value), m_parse(parse), m_inspect(inspect) {}

			s_hs_value_union GetDefaultValue() const override {
				return m_default_value;
			}

			bool Parse(Enums::hs_type type, datum_index expression_index) const override {
				return m_parse(type, expression_index);
			}

			bool Inspectable() const override { return m_inspect != nullptr; }

			void Inspect(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) const override {
				m_inspect(type, value, buffer, buffer_size);
			}
		};

		struct c_hs_type_bool_abi final : c_hs_type_value_abi {
			c_hs_type_bool_abi(Enums::hs_type type, s_hs_value_union default_value, proc_hs_type_parse parse, proc_hs_type_inspect inspect)
				: c_hs_type_value_abi(type, default_value, parse, inspect) {}

			size_t GetSize() const override { return sizeof(bool); }

			s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union *source) const override;

			void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union &destination) const override;
		};
	};

	namespace blam {
		using namespace Scripting;

		const std::array<unsigned short, Enums::k_number_of_hs_object_types> k_hs_object_type_masks = {
			Enums::_object_type_mask_all, // engine actually uses 0xFFFF here
			Enums::_object_type_mask_unit,
			FLAG(Enums::_object_type_vehicle),
			FLAG(Enums::_object_type_weapon),
			Enums::_object_type_mask_device,
			FLAG(Enums::_object_type_scenery),
		};

		const std::array<tag, Enums::_hs_type_tag_reference__count> k_hs_tag_reference_type_group_tags = {
			TagGroups::sound_definition::k_group_tag,
			TagGroups::effect_definition::k_group_tag,
			TagGroups::s_damage_effect_definition::k_group_tag,   // damage
			TagGroups::looping_sound_definition::k_group_tag,
			TagGroups::model_animation_graph::k_group_tag,
			TagGroups::s_actor_variant_definition::k_group_tag,
			TagGroups::s_damage_effect_definition::k_group_tag,   // damage_effect
			TagGroups::s_object_definition::k_group_tag,
		};

		std::array<const char *, Enums::k_number_of_hs_types> hs_type_names = {
			"unparsed",
			"special_form",      // NOTE: changed spaces to underscores
			"function_name",   // NOTE: changed spaces to underscores
			"passthrough",
			"void",

			"boolean",
			"real",
			"short",
			"long",
			"string",
			"script",

			"trigger_volume",
			"cutscene_flag",
			"cutscene_camera_point",
			"cutscene_title",
			"cutscene_recording",
			"device_group",
			"ai",
			"ai_command_list",
			"starting_profile",
			"conversation",

			"navpoint",
			"hud_message",

			"object_list",

			"sound",
			"effect",
			"damage",
			"looping_sound",
			"animation_graph",
			"actor_variant",
			"damage_effect",
			"object_definition",

			"game_difficulty",
			"team",
			"ai_default_state",
			"actor_type",
			"hud_corner",

			"object",
			"unit",
			"vehicle",
			"weapon",
			"device",
			"scenery",

			"object_name",
			"unit_name",
			"vehicle_name",
			"weapon_name",
			"device_name",
			"scenery_name",
		};

		std::array<const char *, Enums::k_number_of_hs_script_types> hs_script_type_names = {
			"startup",
			"dormant",
			"continuous",
			"static",
			"stub",
		};

		// should be in game/game.cpp
		const char *                                                    g_game_difficulty_level_names[Enums::k_number_of_game_difficulty_levels] = {
			"easy",
			"normal",
			"hard",
			"impossible",
		};
		// should be in game/game_allegiance.cpp
		const char *                                                    g_game_team_names[Enums::k_number_of_game_teams]                         = {
			"default",
			"player_update",
			"human",
			"covenant",
			"flood",
			"sentinel",
			"unused6",
			"unused7",
			"unused8",
			"unused9",
		};
		// should be in ai/ai_scenario_definitions.cpp
		const char *                                                    g_ai_default_state_names[Enums::k_number_of_actor_default_states]        = {
			"none",
			"sleep",
			"alert",
			"move_repeat",
			"move_loop",
			"move_loop_back_and_forth",
			"move_loop_random",
			"move_random",
			"guard",
			"guard_at_position",
			"search",
			"flee",
		};
		// should be in ai/actor_types.cpp
		const char *                                                    g_actor_type_names[Enums::k_number_of_actor_types]                       = {
			"elite",
			"jackal",
			"grunt",
			"hunter",
			"engineer",
			"assassin",
			"player_update",
			"marine",
			"crew",
			"combat_form",
			"infection_form",
			"carrier_form",
			"monitor",
			"sentinel",
			"none",
			"mounted_weapon",
		};
		// should be in interface/hud_definitions.cpp
		const char *                                                    g_hud_anchor_names[Enums::k_number_of_hud_anchors]                       = {
			"top_left",
			"top_right",
			"bottom_left",
			"bottom_right",
			"center",
		};
		std::array<const string_list, Enums::_hs_type_enum__count> hs_enum_table                                                            = {
			string_list{std::size(g_game_difficulty_level_names), &g_game_difficulty_level_names[0]},
			string_list{std::size(g_game_team_names), &g_game_team_names[0]},
			string_list{std::size(g_ai_default_state_names), &g_ai_default_state_names[0]},
			string_list{std::size(g_actor_type_names), &g_actor_type_names[0]},
			string_list{std::size(g_hud_anchor_names), &g_hud_anchor_names[0]},
		};
	};
};
