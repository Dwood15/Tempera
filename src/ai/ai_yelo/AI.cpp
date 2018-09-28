#include "macros_generic.h"
#include "../../memory/memory_interface_base.hpp"
#include "../actors/actor_structures.h"
#include "AI.hpp"
#include "AI.Transform.inl"
#include "../../memory/datum_index.h"
#include "../../gamestate/objects/damage.h"

namespace Yelo {
	namespace AI {
		// s_ai_globals_data* AIGlobals()								DPTR_IMP_GET(ai_globals);
		actor_data_t &Actors() {
			actor_data_t *actors = reinterpret_cast<actor_data_t *>(0x81B8A0);
			return *actors;
		};

		// swarm_data_t& Swarms()										DPTR_IMP_GET_BYREF(swarms);
		// swarm_component_data_t& SwarmComponents()					DPTR_IMP_GET_BYREF(swarm_components);
		// prop_data_t& Props()										DPTR_IMP_GET_BYREF(props);
		// encounter_data_t& Encounters()								DPTR_IMP_GET_BYREF(encounters);
		// squads_data_t& Squads()										DPTR_IMP_GET_BYREF(squads);
		// platoons_data_t& Platoons()									DPTR_IMP_GET_BYREF(platoons);
		// ai_pursuit_data_t& AIPursuits()								DPTR_IMP_GET_BYREF(ai_pursuits);

		//ai_communication_dialogue_events_t
		// ai_communication_reply_events_t& AICommunicationReplies()	DPTR_IMP_GET_BYREF(ai_communication_replies);
		// ai_conversation_data_t& AIConversations()					DPTR_IMP_GET_BYREF(ai_conversations);

		naked void ActorActionHandleVehicleExitHook() {

			static uintptr_t ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN = 0x40B10A;

			__asm {
			push   eax
			push   edx
			xor      ecx, ecx
			mov      cl,[eax + 60h]

			push   ecx
			push   eax
			push   ebx
			call   AI::ActorPropShouldCauseExitVehicle
			add      esp, 0Ch

			mov      cl, al
			pop      edx
			pop      eax

			test    cl, cl

			jmp      ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN
			};
		}

		naked void PropStatusRefreshHook() {
			static uintptr_t PROP_STATUS_REFRESH_RETN = 0x41C9D4;

			__asm
			{
			push   ecx
			mov      cl, al
			push   ecx

			push   ebp
			call   AI::ActorShouldIgnoreSeatedProp
			add      esp, 4

			pop      ecx
			or      cl, al
			mov[ebp + 133h], cl
			pop      ecx

			jmp      PROP_STATUS_REFRESH_RETN
			}
		}

		naked void ActorInputUpdateHook() {
			static uintptr_t ACTOR_INPUT_UPDATE_RETN = 0x429CAE;

			__asm
			{
			push   eax

			push   ebx
			call   AI::ActorShouldPanicAboutMountedUnit
			add      esp, 4

			mov      byte ptr[esi+1B4h], al

			pop      eax
			jmp      ACTOR_INPUT_UPDATE_RETN
			}
		}


		void Initialize() {
			//HALO CE HOOKS
			auto AI_UPDATE_HOOK= reinterpret_cast<void *>(0x42A949);
			auto ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK = reinterpret_cast<void *>(0x40B105);
			auto PROP_STATUS_REFRESH_HOOK = reinterpret_cast<void *>(0x41C9CE);
			auto ACTOR_INPUT_UPDATE_HOOK = reinterpret_cast<void *>(0x429CA7);
			Memory::CreateHookRelativeCall(&AI::Update, AI_UPDATE_HOOK, Enums::_x86_opcode_retn);
			Memory::WriteRelativeJmp(&ActorActionHandleVehicleExitHook, ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK, true);
			Memory::WriteRelativeJmp(&PropStatusRefreshHook, PROP_STATUS_REFRESH_HOOK, true);
			Memory::WriteRelativeJmp(&ActorInputUpdateHook, ACTOR_INPUT_UPDATE_HOOK, true);

			Transform::Initialize();
		}

		void Dispose() {
		}

		void InitializeForNewGameState() {
			Transform::InitializeForNewGameState();
		}

		void InitializeForNewMap() {
			Transform::InitializeForNewMap();
		}

		void DisposeFromOldMap() {
			Transform::DisposeFromOldMap();
		}

		void __cdecl Update() {

		}

		void HandleGameStateLifeCycle(short life_state) {
			Transform::HandleGameStateLifeCycle(life_state);
		}

		void ObjectsUpdate() {
			auto iter(object_type::_object_type_mask_unit);

			for (auto object_index : iter) {
				Transform::UnitUpdate(object_index.index);
			}
		}

		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data *damage_data) {
			Transform::UnitDamaged(object_index, damage_data);
		}
	};
};
