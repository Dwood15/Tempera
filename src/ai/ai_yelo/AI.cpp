#include "macros_generic.h"
#include "engine_interface.h"
#include "../../memory/memory_interface_base.hpp"
#include "../actors/actor_structures.h"
#include "AI.hpp"
#include "../../memory/datum_index.h"
#include "../../gamestate/objects/damage.h"
namespace Yelo {
	namespace AI {

		void Initialize() {
			//HALO CE HOOKS
			auto AI_UPDATE_HOOK= reinterpret_cast<void *>(0x42A949);

			memory::CreateHookRelativeCall(&AI::Update, AI_UPDATE_HOOK, Enums::_x86_opcode_retn);
		}

		void Dispose() {
		}

		void InitializeForNewGameState() {

		}

		void InitializeForNewMap() {
		}

		void DisposeFromOldMap() {
		}

		void __cdecl Update() {

		}

		void HandleGameStateLifeCycle(short life_state) {
		}

		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data *damage_data) {
		}
	};
};
