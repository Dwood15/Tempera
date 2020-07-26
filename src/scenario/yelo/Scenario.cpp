#include <precompile.h>
// There are places which use a bit-vector for the structure-bsps
// In the stock code, the bit-vector is a 16-bit word
// We can only, reasonably, upgrade the code to a 32-bit word
static_assert( Yelo::Enums::k_maximum_structure_bsps_per_scenario_upgrade <= 32 );

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Scenario
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_SCENARIO
#define __EL_INCLUDE_FILE_ID	__EL_SCENARIO_SCENARIO
#include "Memory/_EngineLayout.inl"

		s_scenario_globals* ScenarioGlobals()						{ return *reinterpret_cast<Yelo::Scenario::s_scenario_globals**>(0x6E2254)};

		TagGroups::scenario* Scenario()								{ return(global_scenario); }
		TagGroups::collision_bsp* Bsp3d()							{ return(global_bsp3d); }
		TagGroups::collision_bsp* CollisionBsp()					{ return(global_collision_bsp); }
		TagGroups::structure_bsp* StructureBsp()					{ return(global_structure_bsp); }

		datum_index ScenarioIndex()									{ return(global_scenario_index); }
		short StructureBspIndex()									{ return(structure_bsp_index); }

		datum_index* GlobalScenarioIndexReference()					{ return global_scenario_index; }
		TagGroups::scenario** GlobalScenarioReference()				{ return global_scenario; }
		short* GlobalStructureBspIndexReference()					{ return structure_bsp_index; }
		TagGroups::structure_bsp** GlobalStructureBspReference()	{ return global_structure_bsp; }
		TagGroups::collision_bsp** GlobalBspReference()				{ return global_bsp3d; }
		TagGroups::collision_bsp** GlobalCollisionBspReference()	{ return global_collision_bsp; }

		static void InitializeMaximumStructureBspsPerScenarioUpgrade()
		{
			void* asm_bytes;

			// change 'movzx' to 'mov'
			asm_bytes = GET_FUNC_VPTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__READ);
			static const byte k_mod1_asm_bytes[] = {
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_mov_eax_y_offset
			};
			Memory::OverwriteMemorySansCopyArray(asm_bytes, k_mod1_asm_bytes);

			// change 'or offset, dx' to 'or offset, edx'
			asm_bytes = GET_FUNC_VPTR(OBJECT_TYPES_PLACE_OBJECTS_MOD_PROCESSED_BSPS__WRITE);
			static const byte k_mod2_asm_bytes[] = {
				Enums::_x86_opcode_nop,
			};
			Memory::OverwriteMemorySansCopyArray(asm_bytes, k_mod2_asm_bytes);

			// change 'mov offset, bx' to 'mov offset, ebx'
			asm_bytes = GET_FUNC_VPTR(OBJECTS_INITIALIZE_FOR_NEW_MAP_MOD_PROCESSED_BSPS);
			static const byte k_mod3_asm_bytes[] = {
				Enums::_x86_opcode_nop,
			};
			Memory::OverwriteMemorySansCopyArray(asm_bytes, k_mod3_asm_bytes);
		}
		void Initialize()
		{
			InitializeMaximumStructureBspsPerScenarioUpgrade();
		}

		void Dispose()
		{
		}
	};
};
