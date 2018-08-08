#include "macros_generic.h"
#include "../cseries/yelo_base.h"
#include "enums/hs_enums.h"

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
					*data.ptr.uint16 = *reinterpret_cast<unsigned short *>(ptr);
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
					*data.ptr.int32 = *reinterpret_cast<uint *>(ptr);
					break;

					//////////////////////////////////////////////////////////////////////////
					//				case HS_TYPE(string): *data.ptr.ascii = CAST_PTR(const char *, ptr); break;
			}
		}
	}

	void UpdateTypeHolderFromPtrToData(TypeHolder &data, const Enums::hs_type type) {
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
					data.uint16 = *copy.ptr.uint16;
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
					data.uint32 = *copy.ptr.uint32;
					break;

					//////////////////////////////////////////////////////////////////////////
					//				case HS_TYPE(string): data.ptr.ascii = copy.ptr.ascii; break;
			}
			// the pointer is null, so default to NONE
		} else {
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
					data.int16 = NONE;
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
					data.uint32 = NONE;
					break;

					//////////////////////////////////////////////////////////////////////////
					//				case HS_TYPE(string): data.ptr.ascii = nullptr; break;
			}
		}
	}
};
#include "structures.h"
namespace Yelo::blam {
	using namespace Yelo::Scripting;

	void hs_nodes_garbage_collect() {
		// NOTE: engine seems to use a for(;;) using data_next_index() on syntax nodes
		//for (auto node : HsSyntax()) {
		//	if (node->IsGarbageCollectable())
		//		continue;

		//	HsSyntax().Delete(node.index);
		//}
	}

	Scripting::hs_function_definition *hs_function_get(short function_index) {
		// return c_hs_library::GetFunction(function_index);
		return nullptr;
	}

	Scripting::hs_global_definition *hs_global_external_get(short global_index) {
		// return c_hs_library::GetGlobal(global_index);
		return nullptr;
	}

	// Yelo::Enums::hs_type hs_global_get_type(short global_index) {
	// 	//assert(global_index != NONE); // engine doesn't do this
	//
	// 	bool is_internal = (global_index & Enums::_hs_global_index_is_external_mask) == 0;
	// 	global_index &= Enums::_hs_global_index_mask;
	//
	// 	if (is_internal) {
	// 		return global_scenario_get()->globals[global_index].global.type;
	// 	}
	//
	// 	return hs_global_external_get(global_index)->type;
	// }

	// const char *hs_global_get_name(short global_index) {
	// 	//assert(global_index != NONE); // engine doesn't do this
	//
	// 	bool is_internal = (global_index & Enums::_hs_global_index_is_external_mask) == 0;
	// 	global_index &= Enums::_hs_global_index_mask;
	//
	// 	if (is_internal) {
	// 		return global_scenario_get()->globals[global_index].name;
	// 	}
	//
	// 	return hs_global_external_get(global_index)->name;
	// }

	// short hs_find_global_by_name(const char *name) {
	// 	// TODO: update code when containers come online
	//
	// 	// search the globals external from the scenario first
	// 	long      global_index = 0;
	// 	for (auto *external_global : c_hs_library::GetExternalGlobals()) {
	// 		if (!_stricmp(name, external_global->name)) {
	// 			global_index |= Enums::_hs_global_index_is_external_mask;
	// 			// engine casts down as well, so we will too
	// 			return CAST(short, global_index);
	// 		}
	//
	// 		global_index++;
	// 	}
	//
	// 	auto *scenario = global_scenario_try_and_get();
	// 	// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
	// 	if (scenario == nullptr)
	// 		return NONE;
	//
	// 	// now search the globals internal to the scenario
	// 	global_index = 0;
	// 	for (auto &internal_global : scenario->globals) {
	// 		if (!_stricmp(name, internal_global.name)) {
	// 			// engine casts down as well, so we will too
	// 			return CAST(short, global_index);
	// 		}
	//
	// 		global_index++;
	// 	}
	//
	// 	// no global by the given name was found
	// 	return NONE;
	// }
	//
	// short hs_find_function_by_name(const char *name) {
	// 	long      function_index = 0;
	// 	// for (auto *function : c_hs_library::GetFunctionsTable()) {
	// 	// 	if (!_stricmp(name, function->name)) {
	// 	// 		// engine casts down as well, so we will too
	// 	// 		return CAST(short, function_index);
	// 	// 	}
	// 	//
	// 	// 	function_index++;
	// 	// }
	// 	throw "WUSH OH CALLED COMMENTED OUT FUNCTION";
	// 	// no function by the given name was found
	// 	return NONE;
	// }

	// short hs_find_tag_reference_by_index(datum_index tag_index) {
	// 	// TODO: update code when containers come online
	//
	// 	auto *scenario = global_scenario_try_and_get();
	// 	// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
	// 	if (scenario == nullptr) {
	// 		return -1;
	// 	}
	//
	// 	long      reference_index = 0;
	// 	for (auto &reference : scenario->references) {
	// 		if (reference.reference.tag_index == tag_index) {
	// 			// engine casts down as well, so we will too
	// 			return static_cast<short>(reference_index);
	// 		}
	//
	// 		reference_index++;
	// 	}
	//
	// 	// no reference with the given tag_index was found
	// 	return -1;
	// }

	// Yelo::Scripting::s_hs_script_container_datum_index hs_find_script_by_name(const char *name) {
	// 	// TODO: update code when containers come online
	//
	// 	auto *scenario = global_scenario_try_and_get();
	// 	// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
	// 	if (scenario == nullptr) {
	// 		return s_hs_script_container_datum_index::k_null;
	// 	}
	//
	// 	long      script_index = 0;
	// 	for (auto &script : scenario->scripts) {
	// 		if (!_stricmp(name, script.name)) {
	// 			return s_hs_script_container_datum_index::CreateIndex(static_cast<short>(script_index));
	// 		}
	//
	// 		script_index++;
	// 	}
	//
	// 	// no script by the given name was found
	// 	return s_hs_script_container_datum_index::k_null;
	// }

	// bool hs_evaluate_by_script_name(const char *name) {
	// 	// TODO: update code when containers come online
	//
	// 	auto script = hs_find_script_by_name(name);
	// 	if (script.IsNull())
	// 		return false;
	//
	// 	auto *scenario = global_scenario_get();
	//
	// 	hs_runtime_evaluate(scenario->scripts[script.idx.script_index].script.root_expression_index);
	// 	return true;
	// }
	Scripting::hs_syntax_node *hs_syntax_get(datum_index expression_index) { return nullptr; } //HsSyntax()[expression_index.handle]; }
};

bool Yelo::Scripting::s_hs_script_container_datum_index::IsNull() const {
	throw "WTF IsNull Function impossible to refactor";
}
