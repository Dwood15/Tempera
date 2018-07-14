#pragma once

#include <versions.h>
#include <dinput.h>
#include <macros_generic.h>
#include "../../../src/hs/structures.hpp"
#include "../../../src/scenario/structures.h"

namespace feature_management::engines {
	class CE110 : public IEngine<CE110> {

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

		static auto *const global_collision_bsp  = reinterpret_cast<Yelo::TagGroups::collision_bsp **>(0x6E2258);
		static auto *const global_structure_bsp  = reinterpret_cast<Yelo::TagGroups::structure_bsp **>(0x6E225C);
		static auto *const global_scenario_index = reinterpret_cast<datum_index *>(0x6397CC);
		static auto *const structure_bsp_index   = reinterpret_cast<short *>(0x6397D0);

		//////////////////////////////////////////////////////////////////////////
		// Scenario.cpp
		static auto **const scenario_globals = reinterpret_cast<Yelo::Scenario::s_scenario_globals **>(0x6E2254);
		static auto *const global_scenario   = reinterpret_cast<Yelo::TagGroups::scenario **>(0x6E224C);
		static auto *const global_bsp3d      = reinterpret_cast<Yelo::TagGroups::collision_bsp **>(0x6E2250);
		static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ  = 0x4F8207;
		static auto const OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE = 0x4F83CE;
		static auto const OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS    = 0x4F84E2;

		static byte *ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK = (reinterpret_cast<byte *>(0x485D77));

		static unsigned int *K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST[] = {
			(reinterpret_cast<uint *>(0x485D7B)),
		};

		static unsigned int *K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST[] = {
			//CAST_PTR(uint*, PLATFORM_VALUE(0x485E93, 0x47D783)), // don't modify this one, modify the size check using the address below
			(reinterpret_cast<uint *>(0x485DCA)),
		};

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
		static auto **const recorded_animations = (reinterpret_cast<Yelo::Scripting::recorded_animations_data_t **>(0x64B940));
		static auto **const object_list_header  = (reinterpret_cast<Yelo::Scripting::object_list_header_data_t **>(0x815904));

		static auto **const list_object_reference = (reinterpret_cast<Yelo::Scripting::list_object_reference_data_t **>(0x815908));
		static auto **const hs_syntax             = (reinterpret_cast<Yelo::Scripting::hs_syntax_data_t **>(0x815914));
		static auto **const hs_globals            = (reinterpret_cast<Yelo::Scripting::hs_globals_data_t **>(0x81590C));
		static auto **const hs_threads            = (reinterpret_cast<Yelo::Scripting::hs_thread_data_t **>(0x815910));

		static auto const HS_UPDATE_HOOK = 0x48CE78;

		//////////////////////////////////////////////////////////////////////////
		// script globals related
		static auto *const hs_external_globals_count = reinterpret_cast<long *>(0x5F9D0C);

		static auto **const hs_external_globals = reinterpret_cast<Yelo::Scripting::hs_global_definition **>(0x626988);

		static short *K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_16bit[] = {
			(reinterpret_cast<short *>(0x4860F1)),
		};

		static long  *K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_32bit[] = {
			reinterpret_cast<long *>(0x4865AA),
			reinterpret_cast<long *>(0x48BCDA),
			reinterpret_cast<long *>(0x48CAFB),
			reinterpret_cast<long *>(0x48CC0F),
			reinterpret_cast<long *>(0x48CC6D),
			reinterpret_cast<long *>(0x48CD70),
			reinterpret_cast<long *>(0x48D38A),
		};
		static void  *K_HS_EXTERNAL_GLOBALS_REFERENCES[]             = {
			reinterpret_cast<void *>(0x48607C),
			reinterpret_cast<void *>(0x4860AC),
			reinterpret_cast<void *>(0x4860D9),
			reinterpret_cast<void *>(0x486410),
			reinterpret_cast<void *>(0x4865A5),
			reinterpret_cast<void *>(0x4891E2),
			reinterpret_cast<void *>(0x48BC6E),
			reinterpret_cast<void *>(0x48D1E6),
			reinterpret_cast<void *>(0x48DB1B),
			reinterpret_cast<void *>(0x48DC87),
		};

		//////////////////////////////////////////////////////////////////////////


		static auto const HS_COMPILE_AND_EVALUATE      = 0x487030; // currently unused
		static auto const HS_NULL_EVALUATE             = 0x483F40;
		static auto const HS_NULL_WITH_PARAMS_EVALUATE = 0x483430;

		static IDirectInput8A *GetDInput8Device()          { return *(IDirectInput8A **) 0x64C52C; }

		static IDirectInputDevice8A *GetKeyboardInput()    { return *(IDirectInputDevice8A **) 0x64C730; }

		static IDirectInputDevice8A *GetMouseInput()      { return *(IDirectInputDevice8A **) 0x64C734; }

		static IDirectInputDevice8A **GetJoystickInputs() { return (IDirectInputDevice8A **) 0x64C778; }

		static void OnPlayerActionUpdate();

		static void OnUnitControlUpdate(int client_update_idx);

		static void WriteHooks();

		static auto GetHsFunctionTableReferenceList();

		__trueinline static auto GetHsFunctionTable();
		__trueinline static auto GetHsFunctionTableCount();
		__trueinline static auto GetHsFunctionTableCountReferences16();
		__trueinline static auto GetHsFunctionTableCountReferences32();
	};
};
