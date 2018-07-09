#pragma once


#include "../memory/datum_index.h"
#include "../scenario/scenario.h"
#include "structures.hpp"
#include "runtime.hpp"
#include "../cseries/handle_abi.hpp"
#include "../game/game.hpp"
#include "../game/allegiance.h"
#include "../ai/actors/actors.hpp"
#include "../memory/upgrades/blam_memory_upgrades.hpp"
#include "../ai/actors/actor_types.hpp"
#include "../interface/hud/hud_definitions.hpp"
#include "../scenario/yelo/yelo_global_definitions.h"
#include "../cseries/base.h"

namespace Yelo::Scripting {
	auto item = Yelo::Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade;
	using hs_syntax_data_t = Memory::DataArray<hs_syntax_node, Yelo::Enums::k_maximum_hs_syntax_nodes_per_scenario, Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade>;

	union script_idx {
		short script_index;
		short global_index;
	};

	struct s_hs_script_container_datum_index {
		script_idx idx;
		//			short script_container_index;

		bool IsNull() const { return HandleIsNone(*this); }

		// TODO: remove default parameter when containers come online
		static s_hs_script_container_datum_index CreateIndex(short index, short script_container_index = 0) {
			return {
				index,
				//script_container_index // TODO: uncomment when containers come online
			};
		}

		static const s_hs_script_container_datum_index k_null = {
			NONE,
			//NONE // TODO: uncomment when containers come online
		};
	};

	static_assert(sizeof(s_hs_script_container_datum_index) == 2);

	typedef s_hs_script_container_datum_index hs_script_index_t;
	typedef s_hs_script_container_datum_index hs_global_index_t;

	union s_hs_value_union {
		uintptr_t bits;
		void      *pointer;

		bool    boolean;
		real    real;
		short   short;
		long   long;
		const char *  string;

		hs_script_index_t script;
		//			hs_global_index_t global;

		Enums::game_difficulty_level difficulty_level;
		Enums::game_team             team;
		Enums::actor_default_state   ai_default_state;
		Enums::actor_type            actor_type;
		Enums::hud_anchor            hud_corner;

		datum_index tag_index;
		datum_index datum;

		string_id string_id;

		datum_index ToExpressionIndex() const { return datum; }

		datum_index ToTagIndex() const { return datum; }

		datum_index ToObjectIndex() const { return datum; }
	};

	static_assert(sizeof(s_hs_value_union) == sizeof(void *));
	extern const s_hs_value_union k_null_value_union = {NULL};
	extern const s_hs_value_union k_none_value_union = {NONE};

	hs_syntax_data_t &HsSyntax();
};

namespace Yelo::blam {
	void hs_nodes_garbage_collect() {
		// NOTE: engine seems to use a for(;;) using data_next_index() on syntax nodes
		for (auto node : HsSyntax()) {
			if (node->IsGarbageCollectable())
				continue;

			HsSyntax().Delete(node.index);
		}
	}

	Scripting::hs_function_definition *hs_function_get(short function_index) {
		return c_hs_library::GetFunction(function_index);
	}

	Scripting::hs_global_definition *hs_global_external_get(short global_index) {
		return c_hs_library::GetGlobal(global_index);
	}

	Yelo::Enums::hs_type hs_global_get_type(short global_index) {
		assert(global_index != NONE); // engine doesn't do this

		bool is_internal = (global_index & Enums::_hs_global_index_is_external_mask) == 0;
		global_index &= Enums::_hs_global_index_mask;

		if (is_internal) {
			return global_scenario_get()->globals[global_index].global.type;
		}

		return hs_global_external_get(global_index)->type;
	}

	const char *  hs_global_get_name(short global_index) {
		assert(global_index != NONE); // engine doesn't do this

		bool is_internal = (global_index & Enums::_hs_global_index_is_external_mask) == 0;
		global_index &= Enums::_hs_global_index_mask;

		if (is_internal) {
			return global_scenario_get()->globals[global_index].name;
		}

		return hs_global_external_get(global_index)->name;
	}

	short hs_find_global_by_name(const char *  name) {
		// TODO: update code when containers come online

		// search the globals external from the scenario first
		long     global_index = 0;
		for (auto *external_global : c_hs_library::GetExternalGlobals()) {
			if (!_stricmp(name, external_global->name)) {
				global_index |= Enums::_hs_global_index_is_external_mask;
				// engine casts down as well, so we will too
				return CAST(short, global_index);
			}

			global_index++;
		}

		auto *scenario = global_scenario_try_and_get();
		// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
		if (scenario == nullptr)
			return NONE;

		// now search the globals internal to the scenario
		global_index = 0;
		for (auto &internal_global : scenario->globals) {
			if (!_stricmp(name, internal_global.name)) {
				// engine casts down as well, so we will too
				return CAST(short, global_index);
			}

			global_index++;
		}

		// no global by the given name was found
		return NONE;
	}

	short hs_find_function_by_name(const char *  name) {
		long     function_index = 0;
		for (auto *function : c_hs_library::GetFunctionsTable()) {
			if (!_stricmp(name, function->name)) {
				// engine casts down as well, so we will too
				return CAST(short, function_index);
			}

			function_index++;
		}

		// no function by the given name was found
		return NONE;
	}

	short hs_find_tag_reference_by_index(datum_index tag_index) {
		// TODO: update code when containers come online

		auto *scenario = global_scenario_try_and_get();
		// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
		if (scenario == nullptr) {
			return -1;
		}

		long     reference_index = 0;
		for (auto &reference : scenario->references) {
			if (reference.reference.tag_index == tag_index) {
				// engine casts down as well, so we will too
				return static_cast<short>(reference_index);
			}

			reference_index++;
		}

		// no reference with the given tag_index was found
		return -1;
	}

	Scripting::hs_script_index_t hs_find_script_by_name(const char *  name) {
		// TODO: update code when containers come online

		auto *scenario = global_scenario_try_and_get();
		// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
		if (scenario == nullptr) {
			return hs_script_index_t::k_null;
		}

		long     script_index = 0;
		for (auto &script : scenario->scripts) {
			if (!_stricmp(name, script.name)) {
				return hs_script_index_t::CreateIndex(static_cast<short>(script_index));
			}

			script_index++;
		}

		// no script by the given name was found
		return hs_script_index_t::k_null;
	}

	bool hs_evaluate_by_script_name(const char *  name) {
		// TODO: update code when containers come online

		auto script = hs_find_script_by_name(name);
		if (script.IsNull())
			return false;

		auto *scenario = global_scenario_get();

		hs_runtime_evaluate(scenario->scripts[script.idx.script_index].script.root_expression_index);
		return true;
	}

	Scripting::hs_syntax_node *hs_syntax_get(datum_index expression_index) { return HsSyntax()[expression_index.handle]; }
};

// these are now defined in hs_types.cpp
namespace Yelo::blam {
	extern const std::array<unsigned short, Yelo::Enums::k_number_of_hs_object_types> k_hs_object_type_masks;
	extern const std::array<tag, Enums::_hs_type_tag_reference__count>      k_hs_tag_reference_type_group_tags;

	extern std::array<const char * , Enums::k_number_of_hs_types>           hs_type_names;
	extern std::array<const char * , Enums::k_number_of_hs_script_types>    hs_script_type_names;
	extern std::array<const string_list, Enums::_hs_type_enum__count> hs_enum_table;

	bool hs_type_valid(short type) { return type >= Enums::_hs_type_void && type < Enums::k_number_of_hs_types; }

	bool hs_type_is_enum(short type) { return type >= Enums::_hs_type_enum__first && type < Enums::_hs_type_enum__last; }

	bool hs_type_is_tag_reference(short type) { return type >= Enums::_hs_type_tag_reference__first && type < Enums::_hs_type_tag_reference__last; }

	bool hs_type_is_object(short type) { return type >= Enums::_hs_type_object__first && type < Enums::_hs_type_object__last; }

	bool hs_type_is_object_name(short type) { return type >= Yelo::Enums::_hs_type_object_name__first && type < Enums::_hs_type_object_name__last; }
};



namespace Yelo::Enums {
	// stock sizes
	enum {
		k_total_scenario_hs_syntax_data         = sizeof(Memory::s_data_array) + (sizeof(Scripting::hs_syntax_node) * Enums::k_maximum_hs_syntax_nodes_per_scenario),
		k_total_scenario_hs_syntax_data_upgrade = sizeof(Memory::s_data_array) + (sizeof(Scripting::hs_syntax_node) * Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade),
	};
};


//#define HS_//TYPE(hstype) Enums::_hs_type_##hstype
#define HS_TYPE(hstype) Enums::_hs_type_##hstype


//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes no arguments
#define DECLARE_HS_FUNCTION(name) extern Yelo::Scripting::hs_function_definition function_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes various arguments for input
#define DECLARE_HS_FUNCTION_WITH_PARAMS(name) extern hs_function_definition function_##name##_definition

#define GET_HS_FUNCTION(name) Yelo::Scripting::function_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a normal hs global
#define DECLARE_HS_GLOBAL(name) extern Yelo::Scripting::hs_global_definition global_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining an hs global with special flags
#define DECLARE_HS_GLOBAL_EX(name) extern Yelo::Scripting::hs_global_definition global_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs global whose value is stored
// in the engine itself. Was useful in the case of 'gravity'
#define DECLARE_HS_GLOBAL2(name) extern Yelo::Scripting::hs_global_definition global_##name##_definition

#define GET_HS_GLOBAL(name) Yelo::Scripting::global_##name##_definition

namespace Yelo
{
	namespace Flags
	{
		enum hs_yelo_definition_flags : unsigned short
		{
			_hs_yelo_definition_internal_bit,
			// changes to the global are sync'd
			_hs_yelo_definition_is_synchronized_bit,
			// hs_function is actually an hs_function_definition_yelo
			_hs_yelo_definition_is_yelo_function_bit,
		};
	};

	namespace Scripting
	{
		typedef void* (__stdcall* proc_hs_yelo_function)();
		typedef void* (__stdcall* proc_hs_yelo_function_with_params)(void** arguments);

		// This is the name of a blam global which isn't used in release builds of the game.
		// We use this global to expose the build version to scripts without causing harm when not using OS.
		// It was chosen due to the hs_type being a 'real', which allows us to specify the version number as "MAJ.MIN"
		// See: K_OPENSAUCE_VERSION
		static const const char *  k_external_global_opensauce_override_name = "ai_debug_path_maximum_radius";

		size_t GetTotalScenarioHsSyntaxData()  { return Enums::k_total_scenario_hs_syntax_data; }
		size_t GetTotalScenarioHsSyntaxDataUpgrade() { return Enums::k_total_scenario_hs_syntax_data_upgrade; }

		// Interpret [data] as [type] data. Takes the [data].pointer and sets [data] to the dereferenced value.
		// If [data].pointer is NULL, then this sets [data] to [type]'s NONE equivalent.
		void UpdateTypeHolderFromPtrToData(TypeHolder& data, const Enums::hs_type type) {
			if (data.pointer != nullptr) {
				TypeHolder copy;
				copy.pointer = data.pointer;
				data.pointer = nullptr;
				switch (type) {
					//case HS_TYPE(byte):
					case Enums::_hs_type_bool:
						data.byte = *copy.ptr.byte;
						break;
						//////////////////////////////////////////////////////////////////////////
						// block index based types
					case Enums::_hs_type_script:
					case Enums::_hs_type_trigger_volume:
					case Enums::_hs_type_cutscene_flag:
					case Enums::_hs_type_cutscene_camera_point:
					case Enums::_hs_type_cutscene_title:
					case Enums::_hs_type_cutscene_recording:
					case Enums::_hs_type_device_group:
					case Enums::_hs_type_ai_command_list:
					case Enums::_hs_type_starting_profile:
					case Enums::_hs_type_conversation:
						//////////////////////////////////////////////////////////////////////////
					case Enums::_hs_type_object_name:
					case Enums::_hs_type_short:
						data.unsigned short = *copy.ptr.unsigned short;
						break;
					case Enums::_hs_type_real:
					case Enums::_hs_type_ai:
						//////////////////////////////////////////////////////////////////////////
						// datum_index based types
					case Enums::_hs_type_object_list:
					case Enums::_hs_type_sound:
					case Enums::_hs_type_effect:
					case Enums::_hs_type_damage:
					case Enums::_hs_type_looping_sound:
					case Enums::_hs_type_animation_graph:
					case Enums::_hs_type_actor_variant:
					case Enums::_hs_type_damage_effect:
					case Enums::_hs_type_object_definition:
					case Enums::_hs_type_object:
					case Enums::_hs_type_unit:
					case Enums::_hs_type_vehicle:
					case Enums::_hs_type_weapon:
					case Enums::_hs_type_device:
					case Enums::_hs_type_scenery:
						//////////////////////////////////////////////////////////////////////////
					case Enums::_hs_type_long:
						data.uint = *copy.ptr.uint;
						break;

						//////////////////////////////////////////////////////////////////////////
						//				case HS_TYPE(string): data.ptr.ascii = copy.ptr.ascii; break;
				}
			} else // the pointer is null, so default to NONE
			{
				data.pointer = nullptr;

				switch (type) {
					//case HS_TYPE(byte):
					case Enums::_hs_type_bool:
						data.byte = NONE;
						break;
						//////////////////////////////////////////////////////////////////////////
						// block index based types
					case Enums::_hs_type_script:
					case Enums::_hs_type_trigger_volume:
					case Enums::_hs_type_cutscene_flag:
					case Enums::_hs_type_cutscene_camera_point:
					case Enums::_hs_type_cutscene_title:
					case Enums::_hs_type_cutscene_recording:
					case Enums::_hs_type_device_group:
					case Enums::_hs_type_ai_command_list:
					case Enums::_hs_type_starting_profile:
					case Enums::_hs_type_conversation:
						//////////////////////////////////////////////////////////////////////////
					case Enums::_hs_type_object_name:
					case Enums::_hs_type_short:
						data.short = NONE;
						break;
					case Enums::_hs_type_real:
						data.real = -1.0f;
						break;
					case Enums::_hs_type_ai:
						//////////////////////////////////////////////////////////////////////////
						// datum_index based types
					case Enums::_hs_type_object_list:
					case Enums::_hs_type_sound:
					case Enums::_hs_type_effect:
					case Enums::_hs_type_damage:
					case Enums::_hs_type_looping_sound:
					case Enums::_hs_type_animation_graph:
					case Enums::_hs_type_actor_variant:
					case Enums::_hs_type_damage_effect:
					case Enums::_hs_type_object_definition:
					case Enums::_hs_type_object:
					case Enums::_hs_type_unit:
					case Enums::_hs_type_vehicle:
					case Enums::_hs_type_weapon:
					case Enums::_hs_type_device:
					case Enums::_hs_type_scenery:
						//////////////////////////////////////////////////////////////////////////
					case Enums::_hs_type_long:
						data.long = NONE;
						break;

						//////////////////////////////////////////////////////////////////////////
						//				case HS_TYPE(string): data.ptr.ascii = nullptr; break;
				}
			}
		}
		// Interpret [ptr] as a [type] pointer. Takes [ptr], deferences it and sets [data] to the value.
		// [data] is 'const' as this doesn't modify the pointer, but the data which it points to.
		void UpdateTypeHolderDataFromPtr(const TypeHolder& data, const Enums::hs_type type, void* ptr);


		// currently defined in Halo1_CE's ScriptLibrary.cpp
		bool DefinitionsMatch(const TagGroups::s_scripting_definitions& data);
	};
};

namespace Yelo::Scripting {
	void UpdateTypeHolderDataFromPtr(const TypeHolder &data, const Enums::hs_type type, void *ptr) {
		if (data.pointer != nullptr && ptr != nullptr) {
			switch (type) {
				//case HS_TYPE(byte):
				case Enums::_hs_type_bool:
					*data.ptr.byte = *reinterpret_cast<byte *>(ptr);
					break;
					//////////////////////////////////////////////////////////////////////////
					// block index based types
				case Enums::_hs_type_script:
				case Enums::_hs_type_trigger_volume:
				case Enums::_hs_type_cutscene_flag:
				case Enums::_hs_type_cutscene_camera_point:
				case Enums::_hs_type_cutscene_title:
				case Enums::_hs_type_cutscene_recording:
				case Enums::_hs_type_device_group:
				case Enums::_hs_type_ai_command_list:
				case Enums::_hs_type_starting_profile:
				case Enums::_hs_type_conversation:
					//////////////////////////////////////////////////////////////////////////
				case Enums::_hs_type_object_name:
				case Enums::_hs_type_short:
					*data.ptr.unsigned short = *reinterpret_cast<unsigned short *>(ptr);
					break;
				case Enums::_hs_type_real:
				case Enums::_hs_type_ai:
					//////////////////////////////////////////////////////////////////////////
					// datum_index based types
				case Enums::_hs_type_object_list:
				case Enums::_hs_type_sound:
				case Enums::_hs_type_effect:
				case Enums::_hs_type_damage:
				case Enums::_hs_type_looping_sound:
				case Enums::_hs_type_animation_graph:
				case Enums::_hs_type_actor_variant:
				case Enums::_hs_type_damage_effect:
				case Enums::_hs_type_object_definition:
				case Enums::_hs_type_object:
				case Enums::_hs_type_unit:
				case Enums::_hs_type_vehicle:
				case Enums::_hs_type_weapon:
				case Enums::_hs_type_device:
				case Enums::_hs_type_scenery:
					//////////////////////////////////////////////////////////////////////////
				case Enums::_hs_type_long:
					*data.ptr.uint = *reinterpret_cast<uint *>(ptr);
					break;

					//////////////////////////////////////////////////////////////////////////
					//				case HS_TYPE(string): *data.ptr.ascii = CAST_PTR(const char *, ptr); break;
			}
		}
	}
};
