/* See LICENSE.md for specific license information */
#pragma once

#include <array>
#include <d3dx9math.h>
#include "../structures.h"
//#include "../../ai/actors/actors.h"
#include "../../ai/actors/actor_definitions.h"
#include "enums/generic_enums.h"


namespace Yelo {
	namespace Scripting {
		typedef s_hs_value_union (__stdcall *proc_hs_typecast)(s_hs_value_union value);

		static proc_hs_typecast k_hs_typecasting_procedures[Enums::k_number_of_hs_types][Enums::k_number_of_hs_types];

		static s_hs_value_union hs_value_to_void(s_hs_value_union value) {
			return k_null_value_union;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_bool. nonstandard, the engine never defined any of these casts.
		static s_hs_value_union hs_bool_to_real(s_hs_value_union value) {
			value.real = value.boolean ? 1.0f : 0.0f;

			return value;
		}

		static s_hs_value_union hs_bool_to_short(s_hs_value_union value) {
			value.shurt = value.boolean ? 1 : 0;

			return value;
		}

		static s_hs_value_union hs_bool_to_long(s_hs_value_union value) {
			value.lung = value.boolean ? 1 : 0;

			return value;
		}

		// static void hs_inspect_boolean(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	// assert(type == Enums::_hs_type_bool);
		//
		// 	sprintf_s(buffer, buffer_size, "%s", value.boolean ? "true" : "false");
		// }

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_real
		static s_hs_value_union hs_real_to_short(s_hs_value_union value) {
			value.shurt = CAST(short, value.real);

			return value;
		}

		static s_hs_value_union hs_real_to_long(s_hs_value_union value) {
			value.lung = CAST(long, value.real);

			return value;
		}

		// NOTE: non-standard, engine doesn't support this
		// static s_hs_value_union hs_real_to_enum(s_hs_value_union value) {
		// 	// because hs_enum_to_real adds 1 before float conversion
		// 	value.short = CAST(short, value.real) - 1;
		//
		// 	return value;
		// }
		//
		// static void hs_inspect_real(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	assert(type == Enums::_hs_type_real);
		//
		// 	sprintf_s(buffer, buffer_size, "%f",
		// 				 value.real);
		// }

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_short
		static s_hs_value_union hs_short_to_boolean(s_hs_value_union value) {
			value.boolean = value.shurt != 0;

			return value;
		}

		static s_hs_value_union hs_short_to_real(s_hs_value_union value) {
			value.real = CAST(real, value.shurt);

			return value;
		}

		static s_hs_value_union hs_short_to_long(s_hs_value_union value) {
			value.lung = CAST(long, value.shurt);

			return value;
		}

		static void hs_inspect_short(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			assert(type == Enums::_hs_type_short);

			sprintf_s(buffer, buffer_size, "%d",
						 value.shurt);
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_long
		static s_hs_value_union hs_long_to_boolean(s_hs_value_union value) {
			value.boolean = value.lung != 0;

			return value;
		}

		static s_hs_value_union hs_long_to_real(s_hs_value_union value) {
			value.real = CAST(real, value.lung);

			return value;
		}

		static s_hs_value_union hs_long_to_short(s_hs_value_union value) {
			value.shurt = CAST(short, value.lung);

			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_string
		static s_hs_value_union hs_string_to_boolean(s_hs_value_union value) {
			value.lung = strlen(value.str);

			return hs_long_to_boolean(value);
		}

		static s_hs_value_union hs_string_to_real(s_hs_value_union value) {
			value.real = CAST(real,::std::atof(value.str));

			return value;
		}

		static void hs_inspect_string(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			//assert(type == Enums::_hs_type_string);

			sprintf_s(buffer, buffer_size, "%s", value.str);
		}

		static void hs_inspect_long(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			//sert(type == Enums::_hs_type_long);

			sprintf_s(buffer, buffer_size, "%ld", value.lung);
		}

		static void hs_inspection_on_none(char *buffer, size_t buffer_size) {
			sprintf_s(buffer, buffer_size, "%s", "NONE");
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_script
		static void hs_inspect_script(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_script);
			//
			// auto script_reference = value.script;
			// if (script_reference.IsNull())
			// 	hs_inspection_on_none(buffer, buffer_size);
			//
			// const auto &script = global_scenario_get()->scripts[script_reference.script_index];
			//
			// sprintf_s(buffer, buffer_size, "%s", script.name);
		}

		static void hs_inspect_trigger_volume(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_trigger_volume);

			// hs_inspect_scenario_datum<TagGroups::scenario_trigger_volume>(type, buffer, buffer_size, value.shurt, global_scenario_get()->trigger_volumes);
		}

		static void hs_inspect_cutscene_flag(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_cutscene_flag);

			// hs_inspect_scenario_datum<TagGroups::scenario_cutscene_flag>(type, buffer, buffer_size, value.shurt, global_scenario_get()->cutscene_flags);
		}

		static void hs_inspect_cutscene_camera_point(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_cutscene_camera_point);

			// hs_inspect_scenario_datum<TagGroups::scenario_cutscene_flag>(type, buffer, buffer_size, value.shurt, global_scenario_get()->cutscene_flags);
		}

		static void hs_inspect_cutscene_title(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_cutscene_title);

			// hs_inspect_scenario_datum<TagGroups::s_scenario_cutscene_title>(type, buffer, buffer_size, value.shurt, global_scenario_get()->cutscene_titles);
		}

		static void hs_inspect_cutscene_recording(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_cutscene_recording);

			// hs_inspect_scenario_datum<TagGroups::recorded_animation_definition>(type, buffer, buffer_size, value.shurt, global_scenario_get()->recorded_animations);
		}

		static void hs_inspect_device_group(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_device_group);

			// hs_inspect_scenario_datum<TagGroups::scenario_device_group>(type, buffer, buffer_size,	value.shurt, global_scenario_get()->device_groups);
		}

		static void hs_inspect_ai_command_list(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_ai_command_list);

			// hs_inspect_scenario_datum<TagGroups::ai_command_list_definition>(type, buffer, buffer_size, value.shurt, global_scenario_get()->ai_command_lists);
		}

		static void hs_inspect_starting_profile(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
			// assert(type == Enums::_hs_type_starting_profile);
			//
			// hs_inspect_scenario_datum<TagGroups::scenario_starting_profile>(type, buffer, buffer_size, value.shurt, global_scenario_get()->player_starting_profiles);
		}

		//////////////////////////////////////////////////////////////////////////
		// enums
		static s_hs_value_union hs_enum_to_real(s_hs_value_union value) {
			// yes, the engine adds 1 before converting to a float
			value.real = CAST(real, value.shurt + 1);

			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_object
		static s_hs_value_union hs_object_to_object_list(s_hs_value_union value) {
			if (value.datum.IsNull()) {
				value.datum = datum_index::null();
				return value;
			}

			datum_index list_index = blam::object_list_new();
			blam::object_list_add(list_index, value.ToObjectIndex());

			value.datum = list_index;
			return value;
		}

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_object_name
		static s_hs_value_union hs_object_name_to_object_list(s_hs_value_union value) {
			// datum_index object_index = object_index_from_name_index(value.shurt);
			// value.datum = object_index;
			return value;
			// return hs_object_to_object_list(value);
		}

		// static void hs_inspect_object_name(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	assert(hs_type_is_object_name(type));
		//
		// 	short name_index = value.shurt;
		// 	if (name_index == NONE) {
		// 		hs_inspection_on_none(buffer, buffer_size);
		// 	}
		//
		// 	const auto &object_name = global_scenario_get()->object_names[name_index];
		//
		// 	Yelo:: const char *name = object_name.name;
		//
		// 	sprintf_s(buffer, buffer_size, "%s", name);
		// }
		// static void hs_inspect_conversation(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	assert(type == Enums::_hs_type_conversation);
		//
		// 	hs_inspect_scenario_datum<TagGroups::ai_conversation>(
		// 		type, buffer, buffer_size,
		// 		value.
		// 			shurt, global_scenario_get()->ai_conversations);
		// }
		//
		// static void hs_inspect_navpoint(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	// assert(type == Enums::_hs_type_navpoint);
		//
		// 	// assert(false); // TODO
		// }
		//
		// static void hs_inspect_hud_message(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	// assert(type == Enums::_hs_type_hud_message);
		//
		// 	// assert(false); // TODO
		// }

		//////////////////////////////////////////////////////////////////////////
		// tag references
		// // NOTE: non-standard, engine doesn't support this
		// static s_hs_value_union hs_tag_reference_to_long(s_hs_value_union value) {
		// 	// can just fall through as long as this is true
		// 	static_assert(sizeof(value.lung) == sizeof(value.tag_index));
		//
		// 	return value;
		// }
		//
		// // NOTE: non-standard, engine doesn't support this
		// static s_hs_value_union hs_actor_variant_to_object_definition(s_hs_value_union value) {
		// 	assert(false); // TODO: lookup actor variant def and return its unit tag index
		//
		// 	return k_none_value_union;
		// }
		//
		// // NOTE: non-standard, engine doesn't support this
		// static s_hs_value_union hs_object_definition_to_animation_graph(s_hs_value_union value) {
		// 	assert(false); // TODO: lookup object def and return its animation tag index
		//
		// 	return k_none_value_union;
		// }

		// static void hs_inspect_tag_reference(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	assert(hs_type_is_tag_reference(type));
		//
		// 	datum_index tag_index = value.ToTagIndex();
		// 	if (tag_index.IsNull())
		// 		hs_inspection_on_none(buffer, buffer_size);
		//
		// 	const char *name = "NOT-YET-IMPLEMENTED"; // TODO
		//
		// 	sprintf_s(buffer, buffer_size, "%s", name);
		// }

		// static void hs_inspect_enum(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	assert(hs_type_is_enum(type));
		//
		// 	const auto &enum_definition = hs_enum_table[type - Enums::_hs_type_enum__first];
		// 	short      enum_value       = value.shurt;
		//
		// 	// NOTE: engine doesn't handle NONE cases, it asserts
		// 	if (enum_value == NONE)
		// 		hs_inspection_on_none(buffer, buffer_size);
		//
		// 	assert(enum_value >= 0 && enum_value < enum_definition.count);
		//
		// 	sprintf_s(buffer, buffer_size, "%s", enum_definition[enum_value]);
		// }

		//////////////////////////////////////////////////////////////////////////
		// _hs_type_ai
		// actually defined in ai_script.c
		// static s_hs_value_union object_list_from_ai_reference(s_hs_value_union value) {
		// if (value.lung == NONE) {
		// 	value.datum = datum_index::null();
		// 	return value;
		// }
		//
		// datum_index list_index   = object_list_new();
		// const auto  ai_reference = *reinterpret_cast<Yelo::AI::s_ai_index *>(&value.pointer);
		//
		// AI::s_ai_index_actor_iterator iterator;
		// ai_index_actor_iterator_new(ai_reference, iterator);
		// while (auto *actor = ai_index_actor_iterator_next(iterator)) {
		// 	datum_index unit_index = actor->meta.unit_index;
		// 	if (!unit_index.IsNull()) {
		// 		object_list_add(list_index, unit_index);
		// 	}
		//
		// 	// add all the units that make up this actor's swarm
		// 	for (datum_index swarm_unit_index = actor->meta.swarm_unit_index; !swarm_unit_index.IsNull();) {
		// 		auto *unit = object_get_and_verify_type<s_unit_datum>(swarm_unit_index);
		// 		object_list_add(list_index, swarm_unit_index);
		//
		// 		swarm_unit_index = unit->unit.swarm_next_unit_index;
		// 	}
		// }
		//
		// value.datum = list_index;
		// return value;
		// }

		// static void hs_inspect_object(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) {
		// 	assert(hs_type_is_object(type));
		//
		// 	datum_index object_index = value.ToObjectIndex();
		// 	if (object_index.IsNull())
		// 		hs_inspection_on_none(buffer, buffer_size);
		//
		// 	const char *name = "NOT-YET-IMPLEMENTED"; // TODO
		//
		// 	sprintf_s(buffer, buffer_size, "%s", name);
		// }

		//////////////////////////////////////////////////////////////////////////
		// scenario datums
		template <typename T, bool k_supports_none = true>
		static void hs_inspect_scenario_datum(Enums::hs_type type, char *buffer, size_t buffer_size, long index, const Yelo::TagBlock<const T> &datums) {
			static_assert(::std::is_same<tag_string, decltype(T::name)>::value, "expected the scenario datum to have a field called 'name' that is a tag_string");

			if (!k_supports_none) {
				assert(index != NONE);

			} else if (index == NONE) {
				hs_inspection_on_none(buffer, buffer_size);

			}

			const auto &datum        = datums[index];
			// T's name should be a tag_string
			const byte *name_address = reinterpret_cast<const byte *>(&datum) + offsetof(T, name);

			const char *name = reinterpret_cast<const char *>(name_address);

			sprintf_s(buffer, buffer_size, "%s", name);
		}

		static void HsTypesInitializeTypeCastingProcedures() {
			// using namespace Enums;
			// using namespace blam;
			//
			// typedef proc_hs_typecast (&typecasting_procedures_t)[k_number_of_hs_types];
			//
			// typecasting_procedures_t
			// 	void_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_void],
			// 	bool_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_bool],
			// 	real_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_real],
			// 	short_procedures  = k_hs_typecasting_procedures[hs_type::_hs_type_short],
			// 	long_procedures   = k_hs_typecasting_procedures[hs_type::_hs_type_long],
			// 	string_procedures = k_hs_typecasting_procedures[hs_type::_hs_type_string],
			//
			// 	object_list_procedures = k_hs_typecasting_procedures[_hs_type_object_list];

			// _hs_type_void
			// for (short type = _hs_type_data__first; type < _hs_type_data__last + 1; type++) {
			// 	void_procedures[type] = hs_value_to_void;
			// }

			// _hs_type_bool
			// bool_procedures[_hs_type_real]   = hs_long_to_boolean; // yes long, (this is what the engine does)
			// bool_procedures[_hs_type_short]  = hs_short_to_boolean;
			// bool_procedures[_hs_type_long]   = hs_long_to_boolean;
			// bool_procedures[_hs_type_string] = hs_string_to_boolean;
			//
			// // _hs_type_real
			// real_procedures[_hs_type_bool]  = hs_bool_to_real;
			// real_procedures[_hs_type_short] = hs_short_to_real;
			// real_procedures[_hs_type_long]  = hs_long_to_real;
			// //			real_procedures[_hs_type_string] =	hs_string_to_real;
			// for (short type = _hs_type_enum__first; type < _hs_type_enum__last + 1; type++) {
			// 	real_procedures[type] = hs_enum_to_real;
			// }
			//
			// // _hs_type_short
			// short_procedures[_hs_type_bool] = hs_bool_to_short;
			// short_procedures[_hs_type_real] = hs_real_to_short;
			// short_procedures[_hs_type_long] = hs_long_to_short;
			// //			short_procedures[_hs_type_string] =	hs_string_to_short;
			//
			// // _hs_type_long
			// long_procedures[_hs_type_bool]  = hs_bool_to_long;
			// long_procedures[_hs_type_real]  = hs_real_to_long;
			// // NOTE: due to a typo, the engine actually didn't support casting from short to long
			// long_procedures[_hs_type_short] = hs_short_to_long;
			// //			long_procedures[_hs_type_string] =	hs_string_to_long;
			//
			// // _hs_type_string
			//
			// // _hs_type_object_list
			// object_list_procedures[_hs_type_ai] = object_list_from_ai_reference;
			// for (short type = _hs_type_object__first; type < _hs_type_object__last + 1; type++) {
			// 	object_list_procedures[type] = hs_object_to_object_list;
			// }
			//
			// for (short type = _hs_type_object_name__first; type < _hs_type_object_name__last + 1; type++) {
			// 	object_list_procedures[type] = hs_object_name_to_object_list;
			// }
		}

		static void HsTypesInitialize() {
			HsTypesInitializeTypeCastingProcedures();
		}

		static void HsTypesDispose() {

		}

		typedef bool (__stdcall *proc_hs_type_parse)(Enums::hs_type type, datum_index expression_index);

		typedef void (__stdcall *proc_hs_type_inspect)(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size);

		struct c_hs_type_void_abi;

		class c_hs_type_abi;

		// static const c_hs_type_abi *asb1 = &{k_hs_unparsed_abi, Yelo::Enums::_hs_unparsed;};
		// static const c_hs_type_abi *asb2 = &{k_hs_special_form_abi, Yelo::Enums::_hs_special_form;};
		// static const c_hs_type_abi *asb3 = &{k_hs_function_name_abi, Yelo::Enums::_hs_function_name;};
		// static const c_hs_type_abi *asb4 = &{k_hs_passthrough_abi, Yelo::Enums::_hs_passthrough;};
		// static const c_hs_type_abi *asb5 = &{k_hs_type_void_abi, Yelo::Enums::_hs_type_void;};
		// static const c_hs_type_abi *asb6 = &{k_hs_type_bool_abi, Yelo::Enums::_hs_type_bool;};

		class c_hs_type_abi {
		public:
			static const c_hs_type_abi *Get(Enums::hs_type type) {
				// static::std::array<const c_hs_type_abi *, Enums::k_number_of_hs_types> hs_type_abis = {
				// 	{asb1},
				// 	{asb2},
				// 	{asb3},
				// 	{asb4},
				// 	{asb5},
				// 	{asb6}
				// };

				// return hs_type_abis[type]
				return nullptr;
			}

		protected:
			Enums::hs_type m_type;
			unsigned short : 16;

		public:
			c_hs_type_abi(Enums::hs_type type) : m_type(type) {}

			Enums::hs_type GetType() const { return m_type; }

			// const char * GetTypeName() const { return Yelo::blam::hs_type_names[m_type]; }

			virtual s_hs_value_union GetDefaultValue();

			virtual s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union *source) const;

			virtual void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union &destination) const;

			virtual size_t GetSize() const;

			virtual bool Parse(Enums::hs_type type, datum_index expression_index) const;

			virtual bool Inspectable() const;

			virtual void Inspect(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) const;

			virtual bool CastableToValueUnion() const { return CastableTo_hs_type_long(); }

			virtual s_hs_value_union GetValue(s_hs_value_union value) { return GetValue(value.lung); }

			virtual bool CastableTo_hs_type_bool() const { return false; }

			virtual s_hs_value_union GetValue(bool value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_real() const { return false; }

			virtual s_hs_value_union GetValue(real value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_short() const { return false; }

			virtual s_hs_value_union GetValue(short value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_long() const { return false; }

			virtual s_hs_value_union GetValue(long value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_string() const { return false; }

			virtual s_hs_value_union GetValue(const char *value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_script() const { return false; }

			virtual s_hs_value_union GetValue(s_hs_script_container_datum_index index) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_game_difficulty() const { return false; }

			// virtual s_hs_value_union GetValue(Enums::game_difficulty_level value) const { return k_none_value_union; }

			virtual bool CastableTo_hs_type_enum_team() const { return false; }

			// virtual s_hs_value_union GetValue(Enums::game_team value) const { return k_none_value_union; }

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

			s_hs_value_union GetDefaultValue() const { return k_none_value_union; }

			s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union *source) const { return k_none_value_union; }

			void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union &destination) const {}

			size_t GetSize() const { return 0; }

			bool Parse(Enums::hs_type type, datum_index expression_index) const { return false; }

			bool Inspectable() const { return false; }

			void Inspect(Enums::hs_type type, s_hs_value_union value, char *buffer, size_t buffer_size) const {}
		};

		struct c_hs_type_value_abi : c_hs_type_abi {
		protected:
			s_hs_value_union     m_default_value;
			proc_hs_type_parse   m_parse;
			proc_hs_type_inspect m_inspect;

		public:
			c_hs_type_value_abi(Enums::hs_type type, s_hs_value_union default_value, proc_hs_type_parse parse, proc_hs_type_inspect inspect)
				: c_hs_type_abi(type), m_default_value(default_value), m_parse(parse), m_inspect(inspect) {}

			s_hs_value_union GetDefaultValue() const {
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

			s_hs_value_union ReadValue(Enums::hs_type type, const s_hs_value_union *source) const override {
				if (source != nullptr)
					return GetValue(source->boolean);

				return GetDefaultValue();
			}

			void WriteValue(Enums::hs_type type, s_hs_value_union source, s_hs_value_union &destination) const override;
		};
	};

	namespace blam {
		using namespace Scripting;
		const::std::array<unsigned short, Yelo::Enums::k_number_of_hs_object_types> k_hs_object_type_masks = {
			// Yelo::Enums::_object_type_mask_all, // engine actually uses 0xFFFF here
			// Enums::_object_type_mask_unit,
			// FLAG(Enums::_object_type_vehicle),
			// FLAG(Enums::_object_type_weapon),
			// Enums::_object_type_mask_device,
			// FLAG(Enums::_object_type_scenery),
		};

		const::std::array<tag, Enums::_hs_type_tag_reference_count> k_hs_tag_reference_type_group_tags = {
			// TagGroups::sound_definition::k_group_tag,
			// TagGroups::effect_definition::k_group_tag,
			// // TagGroups::s_damage_effect_definition::k_group_tag,   // damage
			// TagGroups::looping_sound_definition::k_group_tag,
			// TagGroups::model_animation_graph::k_group_tag,
			// TagGroups::s_actor_variant_definition::k_group_tag,
			// // TagGroups::s_damage_effect_definition::k_group_tag,   // damage_effect
			// // TagGroups::s_object_definition::k_group_tag,
		};

	constexpr ::std::array<const char *, Enums::k_number_of_hs_types> hs_type_names = {{
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
		}};

	constexpr ::std::array<const char *, Enums::k_number_of_hs_script_types> hs_script_type_names = {{
			"startup",
			"dormant",
			"continuous",
			"static",
			"stub",
		}};

		// should be in game/game.cpp
	constexpr ::std::array<const char *, k_number_of_game_difficulty_levels> g_game_difficulty_level_names = {{
			"easy",
			"normal",
			"hard",
			"impossible"
		}};
		// should be in game/game_allegiance.cpp

	constexpr ::std::array<const char *, k_number_of_game_teams > g_game_team_names                         = {{
			"default",
			"player_update",
			"human",
			"covenant",
			"flood",
			"sentinel",
			"unused6",
			"unused7",
			"unused8",
			"unused9"
		}};
		// should be in ai/ai_scenario_definitions.cpp
		constexpr ::std::array<const char *, Enums::k_number_of_actor_default_states> g_ai_default_state_names = {{
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
			"flee"
		}};
		// should be in ai/actor_types.cpp
		constexpr ::std::array<const char *, Enums::k_number_of_actor_types> g_actor_type_names = {{
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
			"mounted_weapon"
		}};

		// should be in interface/hud_definitions.cpp
		constexpr ::std::array<const char *, Enums::k_number_of_hud_anchors> g_hud_anchor_names                = {{
			"top_left",
			"top_right",
			"bottom_left",
			"bottom_right",
			"center"
		}};

		//This isn't used anywhere so it's really not a big deal
#ifndef __GNUC__
		// static ::std::array<const string_list, Enums::_hs_type_enum__count> hs_enum_table = {{
		// 	string_list{static_cast<unsigned long>(::std::size(g_game_difficulty_level_names)), &g_game_difficulty_level_names[0]},
		// 	string_list{static_cast<unsigned long>(::std::size(g_game_team_names)), &g_game_team_names[0]},
		// 	string_list{static_cast<unsigned long>(::std::size(g_ai_default_state_names)), &g_ai_default_state_names[0]},
		// 	string_list{static_cast<unsigned long>(::std::size(g_actor_type_names)), &g_actor_type_names[0]},
		// 	string_list{static_cast<unsigned long>(::std::size(g_hud_anchor_names)), &g_hud_anchor_names[0]},
		// }};
#endif
	};
};
