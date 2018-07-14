/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/AI.hpp"

#include "Objects/Objects.hpp"
#include "Objects/Units.hpp"

#include "Game/AI.Transform.inl"

namespace Yelo {
	namespace AI {
		s_ai_globals_data* AIGlobals()								DPTR_IMP_GET(ai_globals);
		actor_data_t& Actors()										DPTR_IMP_GET_BYREF(actors);
		swarm_data_t& Swarms()										DPTR_IMP_GET_BYREF(swarms);
		swarm_component_data_t& SwarmComponents()					DPTR_IMP_GET_BYREF(swarm_components);
		prop_data_t& Props()										DPTR_IMP_GET_BYREF(props);
		encounter_data_t& Encounters()								DPTR_IMP_GET_BYREF(encounters);
		squads_data_t& Squads()										DPTR_IMP_GET_BYREF(squads);
		platoons_data_t& Platoons()									DPTR_IMP_GET_BYREF(platoons);
		ai_pursuit_data_t& AIPursuits()								DPTR_IMP_GET_BYREF(ai_pursuits);

		//ai_communication_dialogue_events_t
		ai_communication_reply_events_t& AICommunicationReplies()	DPTR_IMP_GET_BYREF(ai_communication_replies);
		ai_conversation_data_t& AIConversations()					DPTR_IMP_GET_BYREF(ai_conversations);

		__declspec(naked) void ActorActionHandleVehicleExitHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN);

			_asm
			{
				push	eax
				push	edx
				xor		ecx, ecx
				mov		cl, [eax + 60h]

				push	ecx
				push	eax
				push	ebx
				call	AI::ActorPropShouldCauseExitVehicle
				add		esp, 0Ch

				mov		cl, al
				pop		edx
				pop		eax

				test    cl, cl

				jmp		RETN_ADDRESS
			};
		}

		__declspec(naked) void PropStatusRefreshHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(PROP_STATUS_REFRESH_RETN);

			_asm
			{
				push	ecx
				mov		cl, al
				push	ecx

				push	ebp
				call	AI::ActorShouldIgnoreSeatedProp
				add		esp, 4
				
				pop		ecx
				or		cl, al
				mov		[ebp + 133h], cl
				pop		ecx

				jmp		RETN_ADDRESS
			}
		}

		__declspec(naked) void ActorInputUpdateHook()
		{
			static uintptr_t RETN_ADDRESS = GET_FUNC_PTR(ACTOR_INPUT_UPDATE_RETN);

			_asm
			{
				push	eax

				push	ebx
				call	AI::ActorShouldPanicAboutMountedUnit
				add		esp, 4

				mov		byte ptr [esi+1B4h], al

				pop		eax
				jmp		RETN_ADDRESS
			}
		}

		void Initialize()
		{
			Memory::CreateHookRelativeCall(&AI::Update, GET_FUNC_VPTR(AI_UPDATE_HOOK), Enums::_x86_opcode_retn);
			Memory::WriteRelativeJmp(&ActorActionHandleVehicleExitHook, GET_FUNC_VPTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK), true);
			Memory::WriteRelativeJmp(&PropStatusRefreshHook, GET_FUNC_VPTR(PROP_STATUS_REFRESH_HOOK), true);
			Memory::WriteRelativeJmp(&ActorInputUpdateHook, GET_FUNC_VPTR(ACTOR_INPUT_UPDATE_HOOK), true);

			Transform::Initialize();
		}

		void Dispose()
		{
		}

		void InitializeForNewGameState()
		{
			Transform::InitializeForNewGameState();
		}
		
		void InitializeForNewMap()
		{
			Transform::InitializeForNewMap();
		}

		void DisposeFromOldMap()
		{
			Transform::DisposeFromOldMap();
		}

		void __cdecl Update()
		{

		}

		void HandleGameStateLifeCycle(shortlife_state)
		{
			Transform::HandleGameStateLifeCycle(life_state);
		}

		void ObjectsUpdate()
		{
			Objects::c_object_iterator iter(Enums::_object_type_mask_unit);

			for(auto object_index : iter)
			{
				Transform::UnitUpdate(object_index.index);
			}
		}

		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data* damage_data)
		{
			Transform::UnitDamaged(object_index, damage_data);
		}
	};
};
