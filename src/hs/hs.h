#pragma once

#include "enums/ai_enums.h"
#include "enums/hud_enums.h"
#include "enums/generic_enums.h"
#include "../memory/datum_index.h"
#include "structures.h"
#include "runtime.h"

#include "../scenario/yelo/yelo_global_definitions.h"
#include "macros_generic.h"
#include "../cseries/yelo_base.h"

namespace Yelo::blam {
	using namespace Yelo::Scripting;

	void hs_nodes_garbage_collect();

	// Scripting::hs_function_definition *hs_function_get(short function_index);
	//
	// Scripting::hs_global_definition *hs_global_external_get(short global_index);
	//
	// Yelo::Enums::hs_type hs_global_get_type(short global_index);

	const char *hs_global_get_name(short global_index);

	short hs_find_global_by_name(const char *name);

	short hs_find_function_by_name(const char *name);

	short hs_find_tag_reference_by_index(datum_index tag_index);

	Yelo::Scripting::s_hs_script_container_datum_index hs_find_script_by_name(const char *name);

	bool hs_evaluate_by_script_name(const char *name);

	Yelo::Scripting::hs_syntax_node *hs_syntax_get(datum_index expression_index); //HsSyntax()[expression_index.handle]; }

	static bool hs_type_valid(short type) { return type >= Enums::_hs_type_void && type < Enums::k_number_of_hs_types; }

	static bool hs_type_is_enum(short type) { return type >= Enums::_hs_type_enum_first && type < Enums::_hs_type_enum_last; }

	static bool hs_type_is_tag_reference(short type) { return type >= Enums::_hs_type_tag_reference_first && type < Enums::_hs_type_tag_reference_last; }

};

//#define HS_//TYPE(hstype) Enums::_hs_type_##hstype
#define HS_TYPE(hstype) Enums::_hs_type_##hstype


//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes no arguments
#define DECLARE_HS_FUNCTION(name) extern Yelo::Scripting::hs_function_definition function_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes various arguments for input
#define DECLARE_HS_FUNCTION_WITH_PARAMS(name) extern hs_function_definition function_##name##_definition

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

namespace Yelo::Scripting {
	typedef void *(__stdcall *proc_hs_yelo_function)();

	typedef void *(__stdcall *proc_hs_yelo_function_with_params)(void **arguments);

	// This is the name of a blam global which isn't used in release builds of the game.
	// We use this global to expose the build version to scripts without causing harm when not using OS.
	// It was chosen due to the hs_type being a 'real', which allows us to specify the version number as "MAJ.MIN"
	// See: K_OPENSAUCE_VERSION
	static const char *k_external_global_opensauce_override_name = "ai_debug_path_maximum_radius";

	constexpr size_t GetTotalScenarioHsSyntaxData() { return Yelo::Enums::k_total_scenario_hs_syntax_data; }

	constexpr size_t GetTotalScenarioHsSyntaxDataUpgrade() { return Enums::k_total_scenario_hs_syntax_data_upgrade; }

	// Interpret [data] as [type] data. Takes the [data].pointer and sets [data] to the dereferenced value.
	// If [data].pointer is NULL, then this sets [data] to [type]'s NONE equivalent.
	void UpdateTypeHolderFromPtrToData(TypeHolder &data, const Enums::hs_type type);

	// Interpret [ptr] as a [type] pointer. Takes [ptr], deferences it and sets [data] to the value.
	// [data] is 'const' as this doesn't modify the pointer, but the data which it points to.
	void UpdateTypeHolderDataFromPtr(const TypeHolder &data, const Enums::hs_type type, void *ptr);

	// currently defined in Halo1_CE's ScriptLibrary.cpp
	bool DefinitionsMatch(const TagGroups::s_scripting_definitions &data);
};


