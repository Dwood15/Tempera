#pragma once

#include <versions.h>
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Weverything"
#include <dinput.h>
//#pragma clang diagnostic pop

#include <macros_generic.h>
#include "../../../src/scenario/structures.h"
#include "../../../src/scenario/definitions.h"
#include "../../../src/models/collision_bsp.h"
#include "../../../src/hs/structures.h"
#include "../../../src/hs/function_declarations.h"

namespace feature_management::engines {
	class CE110 : public IEngine<CE110> {
	private:
		static void InitializeHSMemoryUpgrades();

	public:
		CE110() {}
		~CE110() {}

		static const defined_functionrange *GetFunctionMap();
		static inline char *DEBUG_FILENAME = const_cast<char *>("tempera.hce.1_10.debug.log");

		static LPCoreAddressList GetCoreAddressList();

		static features SupportedFeatures() {
			return features::EVERYTHING;
		}
		//TODO: Make using this _not_ suck.
		static auto const HS_VALID_ACCESS_FLAGS = 0x486220;

		//////////////////////////////////////////////////////////////////////////
		// InitializeCreateScriptFunction()
		static auto const HS_ARGUMENTS_EVALUATE = 0x48D480;
		static auto const HS_RETURN             = 0x48D270;

		static inline Yelo::TagGroups::coll::collision_bsp ** global_collision_bsp  = reinterpret_cast<Yelo::TagGroups::coll::collision_bsp **>(0x6E2258);

		static inline Yelo::TagGroups::structure_bsp ** global_structure_bsp  = reinterpret_cast<Yelo::TagGroups::structure_bsp **>(0x6E225C);

		static inline datum_index * global_scenario_index = reinterpret_cast<datum_index *>(0x6397CC);

		static inline short * structure_bsp_index   = reinterpret_cast<short *>(0x6397D0);

		//TODO
		// recorded_animations_data_t &RecordedAnimations() { DPTR_IMP_GET_BYREF(recorded_animations); }

		// hs_syntax_data_t &HsSyntax() { DPTR_IMP_GET_BYREF(hs_syntax); }

		// object_list_header_data_t &ObjectListHeader() { DPTR_IMP_GET_BYREF(object_list_header); }

		// list_object_reference_data_t &ListObjectReference() { DPTR_IMP_GET_BYREF(list_object_reference); }

		// hs_thread_data_t &HsThreads() { DPTR_IMP_GET_BYREF(hs_threads); }
		//
		// hs_globals_data_t &HsGlobals() { DPTR_IMP_GET_BYREF(hs_globals); }

		//////////////////////////////////////////////////////////////////////////
		// Scenario.cpp
		static const Yelo::Scenario::s_scenario_globals ** GetScenarioGlobals() { return reinterpret_cast<const Yelo::Scenario::s_scenario_globals **>(0x6E2254); }
		static const Yelo::TagGroups::scenario ** GetGlobalScenario() { return reinterpret_cast<const Yelo::TagGroups::scenario **>(0x6E224C); }
		static const Yelo::TagGroups::coll::collision_bsp ** GetGlobalBsp3d() { return reinterpret_cast<const Yelo::TagGroups::coll::collision_bsp **>(0x6E2250); }

		static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ  = 0x4F8207;
		static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE = 0x4F83CE;
		static auto const OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS    = 0x4F84E2;

		//////////////////////////////////////////////////////////////////////////
		// Render
		static auto const RENDER_FRAME  = 0x50F7B0;
		static auto const RENDER_WINDOW = 0x50F8C0;

		//////////////////////////////////////////////////////////////////////////
		// Scenario
		static auto const SCENARIO_TRIGGER_VOLUME_TEST_POINT = 0x542380;

		//////////////////////////////////////////////////////////////////////////
		// TagGroups
		static auto const TAG_LOADED                     = 0x442990;
		static auto const TAG_ITERATOR_NEXT              = 0x442A10;
		static auto const UNICODE_STRING_LIST_GET_STRING = 0x55AD90;

		//////////////////////////////////////////////////////////////////////////
		// Scripting.cpp
		// static auto **const recorded_animations = (reinterpret_cast<Yelo::Scripting::recorded_animations_data_t **>(0x64B940));
		// static auto **const object_list_header  = (reinterpret_cast<Yelo::Scripting::object_list_header_data_t **>(0x815904));
		//
		// static auto **const list_object_reference = (reinterpret_cast<Yelo::Scripting::list_object_reference_data_t **>(0x815908));
		// static auto **const hs_syntax             = (reinterpret_cast<Yelo::Scripting::hs_syntax_data_t **>(0x815914));
		// static auto **const hs_globals            = (reinterpret_cast<Yelo::Scripting::hs_globals_data_t **>(0x81590C));
		// static auto **const hs_threads            = (reinterpret_cast<Yelo::Scripting::hs_thread_data_t **>(0x815910));

		static auto const HS_UPDATE_HOOK = 0x48CE78;

		//////////////////////////////////////////////////////////////////////////
		// script globals related
		const long * hs_external_globals_count = reinterpret_cast<long *>(0x5F9D0C);

		static const Yelo::Scripting::hs_global_definition ** hs_external_globals = reinterpret_cast<const Yelo::Scripting::hs_global_definition **>(0x626988);


		static void UpdateGlobalHSFunctionCounts(long count);

		static void UpdateHSFunctionCounts(short count);

		//////////////////////////////////////////////////////////////////////////


		static auto const HS_COMPILE_AND_EVALUATE      = 0x487030; // currently unused
		static auto const HS_NULL_EVALUATE             = 0x483F40;
		static auto const HS_NULL_WITH_PARAMS_EVALUATE = 0x483430;

#define PLATFORM_VALUE(ce_value) ce_value
#define PLATFORM_VALUE_HACK_(args_list) PLATFORM_VALUE args_list
#define FUNC_PTR(name, ...)	enum FUNC_PTR_##name { PTR_##name = PLATFORM_VALUE_HACK_((__VA_ARGS__)) };

		enum FUNC_PTR_HS_MACRO_FUNCTION_PARSE {
			PTR_HS_MACRO_FUNCTION_PARSE = 0x48A070
		};

		enum FUNC_PTR_HS_COMPILE_AND_EVALUATE { PTR_HS_COMPILE_AND_EVALUATE = PLATFORM_VALUE }; // currently unused
		enum FUNC_PTR_HS_NULL_EVALUATE { PTR_HS_NULL_EVALUATE = HS_NULL_EVALUATE };
		enum FUNC_PTR_HS_NULL_WITH_PARAMS_EVALUATE { PTR_HS_NULL_WITH_PARAMS_EVALUATE = PLATFORM_VALUE };

		static IDirectInput8A *GetDInput8Device()          { return *(IDirectInput8A **) 0x64C52C; }

		static IDirectInputDevice8A *GetKeyboardInput()    { return *(IDirectInputDevice8A **) 0x64C730; }

		static IDirectInputDevice8A *GetMouseInput()      { return *(IDirectInputDevice8A **) 0x64C734; }

		static IDirectInputDevice8A **GetJoystickInputs() { return (IDirectInputDevice8A **) 0x64C778; }

		typedef void (__cdecl* proc_hs_parse)(int32 function_index, datum_index expression_index);
		typedef void (__cdecl* proc_hs_evaluate)(int32 function_index, datum_index thread_index, bool initialize_stack);

		void NullifyScriptFunction(Yelo::Scripting::hs_function_definition &function);

		void NullifyScriptFunction(Yelo::Enums::hs_function_enumeration function);

		void NullifyScriptFunctionWithParams(Yelo::Scripting::hs_function_definition &function);

		void NullifyScriptFunctionWithParams(Yelo::Enums::hs_function_enumeration function);

		// Initialize the function's evaluator to one which we've defined
		// in our code. Evaluator takes no parameters but may return a value.
		void InitializeScriptFunction(Enums::hs_function_enumeration function, Yelo::Scripting::proc_hs_yelo_function proc) {
			if (function > NONE && function < Yelo::Enums::k_hs_function_enumeration_count) {
				ScriptFunctionSetEvaluteProc(*Yelo::Scripting::hs_yelo_functions[function], reinterpret_cast<Scripting::proc_hs_evaluate>(Yelo::Scripting::CreateScriptFunction(proc, false)));
			}
		}

		// Initialize the function's evaluator to one which we've defined
		// in our code. Evaluator expects parameters and may return a value.
		void InitializeScriptFunctionWithParams(Enums::hs_function_enumeration function, Yelo::Scripting::proc_hs_yelo_function_with_params proc) {
			if (function > NONE && function < Yelo::Enums::k_hs_function_enumeration_count) {
				ScriptFunctionWithParamsSetEvaluteProc(*Yelo::Scripting::hs_yelo_functions[function], reinterpret_cast<Scripting::proc_hs_evaluate>(Yelo::Scripting::CreateScriptFunction(proc, true)));
			}
		}

		static void OnPlayerActionUpdate();

		static void OnUnitControlUpdate(int client_update_idx);

		static void WriteHooks();

		static void InitializeMemoryUpgrades();
		static auto GetHsFunctionTableReferenceList();

		constexpr auto GetHsFunctionTable();
		constexpr auto GetHsFunctionTableCount();
		static __forceinline auto GetHsFunctionTableCountReferences16();
		static __forceinline auto GetHsFunctionTableCountReferences32();
	};
};
