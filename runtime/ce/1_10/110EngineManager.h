#pragma once

#include <versions.h>

#include <macros_generic.h>
#include "../../../src/scenario/structures.h"
#include "../../../src/scenario/definitions.h"
#include "../../../src/models/collision_bsp.h"
#include "../../../src/hs/structures.h"
#include "../../../src/gamestate/camera.h"
#include "../../../src/RuntimeManager.h"

//TODO: Detect if msvc and only compile with c++latest
//#if defined(_MSC_VER)
//	#define F abc
//	#define B def
//	#define FB(arg) #arg
//	#define FB1(arg) FB(arg)
//	#include FB1(_MSVC_LANG)
//#endif

static void InitializeLibraryFixups();

namespace feature_management::engines {

	namespace overrides {
		void override_main_get_window_count_calls();
	}

	class CE110 : public IEngine {
	private:
		//static void InitializeHSMemoryUpgrades();
	public:
		CE110() {
			Major = major::CE;
			Minor = minor::halo_1_10;
			Supported = features::EVERYTHING;
		}

		bool IsCustomEd() {
			return true;
		}

		features SupportedFeatures() {
			return Supported;
		}
		//TODO: Make using this _not_ suck.
		static auto const HS_VALID_ACCESS_FLAGS = 0x486220;

		//////////////////////////////////////////////////////////////////////////
		// InitializeCreateScriptFunction()
		static auto const HS_ARGUMENTS_EVALUATE = 0x48D480;
		static auto const HS_RETURN             = 0x48D270;

		const char* GetLogFileName() {
			return "tempera.hce.1_10.debug.log";
		}

		LPCoreAddressList GetCoreAddressList();
		defined_functionrange *GetFunctionMap();
		static void __stdcall OnUnitControlUpdate();

		void WriteHooks();

		void GetMallocAddresses(uint &cpu_alloc_size, uint &game_state_globals_ptr, uint &game_state_globals_crc) {
			game_state_globals_ptr = 0x67DD88;
			cpu_alloc_size = 0x67DD8C;
			game_state_globals_crc = 0x67DD94;
		}
		const Yelo::TagGroups::coll::collision_bsp **global_collision_bsp  = (const Yelo::TagGroups::coll::collision_bsp **)0x6E2258;

		const Yelo::TagGroups::structure_bsp **global_structure_bsp  = (const Yelo::TagGroups::structure_bsp **)(0x6E225C);
//		static inline Yelo::TagGroups::structure_bsp ** global_structure_bsp  = reinterpret_cast<Yelo::TagGroups::structure_bsp **>(0x6E225C);

		const Yelo::datum_index * global_scenario_index = (const Yelo::datum_index *)(0x6397CC);
//		static inline Yelo::datum_index * global_scenario_index = reinterpret_cast<Yelo::datum_index *>(0x6397CC);

		const short * structure_bsp_index   = (const short *)(0x6397D0);
//		static inline short * structure_bsp_index   = reinterpret_cast<short *>(0x6397D0);

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
		Yelo::Camera::s_cinematic_globals_data ** GetCinematicGlobals() { return reinterpret_cast<Yelo::Camera::s_cinematic_globals_data **>(0x68C83C); }
		Yelo::Scenario::s_scenario_globals ** GetScenarioGlobals() { return reinterpret_cast<Yelo::Scenario::s_scenario_globals **>(0x6E2254); }
		Yelo::TagGroups::scenario ** GetGlobalScenario() { return reinterpret_cast<Yelo::TagGroups::scenario **>(0x6E224C); }
		Yelo::TagGroups::coll::collision_bsp ** GetGlobalBsp3d() { return reinterpret_cast<Yelo::TagGroups::coll::collision_bsp **>(0x6E2250); }

		static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ  = 0x4F8207;
		static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE = 0x4F83CE;
		static auto const OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS    = 0x4F84E2;

		//////////////////////////////////////////////////////////////////////////
		// Render
		static auto const RENDER_FRAME  = 0x50F7B0;
		static auto const RENDER_WINDOW_FN = 0x50F8C0;

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
		// Not yet fully ported from Open Sauce
		//		static void UpdateGlobalHSFunctionCounts(long count);
		//
		//		static void UpdateHSFunctionCounts(short count);
		//
		//		static void MemoryUpgradesInitialize();


		//////////////////////////////////////////////////////////////////////////


		static auto const HS_COMPILE_AND_EVALUATE      = 0x487030; // currently unused
		static auto const HS_NULL_EVALUATE             = 0x483F40;
		static auto const HS_NULL_WITH_PARAMS_EVALUATE = 0x483430;

#define PLATFORM_VALUE(ce_value) ce_value
#define PLATFORM_VALUE_HACK_(args_list) PLATFORM_VALUE args_list
#define FUNC_PTR(name, ...)	enum FUNC_PTR_##name { PTR_##name = PLATFORM_VALUE_HACK_((__VA_ARGS__)) };

		static IDirectInput8A *GetDInput8Device()          { return *(IDirectInput8A **) 0x64C52C; }

		static IDirectInputDevice8A *GetKeyboardInput()    { return *(IDirectInputDevice8A **) 0x64C730; }

		static IDirectInputDevice8A *GetMouseInput()      { return *(IDirectInputDevice8A **) 0x64C734; }

		static IDirectInputDevice8A **GetJoystickInputs() { return (IDirectInputDevice8A **) 0x64C778; }

		typedef void (__cdecl* proc_hs_parse)(int32 function_index, Yelo::datum_index expression_index);
		typedef void (__cdecl* proc_hs_evaluate)(int32 function_index, Yelo::datum_index thread_index, bool initialize_stack);
	};
};
