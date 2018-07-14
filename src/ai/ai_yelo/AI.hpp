/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/ai/actions.hpp>
#include <blamlib/Halo1/ai/actor_types.hpp>
#include <blamlib/Halo1/ai/ai.hpp>
#include <blamlib/Halo1/ai/ai_communication.hpp>
#include <blamlib/Halo1/ai/encounters.hpp>
#include <blamlib/Halo1/ai/props.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_damage_data;
	};

	namespace AI
	{
		encounter_data_t&				Encounters();
		squads_data_t&					Squads();
		platoons_data_t&				Platoons();
		ai_pursuit_data_t&				Pursuits();

		//ai_communication_dialogue_events_t*	AICommunicationDialogue();
		ai_communication_reply_events_t&	AICommunicationReplies();
		ai_conversation_data_t&				AIConversations();



		void Initialize();
		void Dispose();

		void InitializeForNewGameState();

		void InitializeForNewMap();
		void DisposeFromOldMap();

		void __cdecl Update();

		void HandleGameStateLifeCycle(shortlife_state);

		void ObjectsUpdate();
		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data* damage_data);

		namespace Transform
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get/Sets whether transforms are enabled. </summary>
			///
			/// <returns>	A bool reference; </returns>
			bool& TransformsEnabled();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Halo script ai transform actor function. </summary>
			///
			/// <param name="arguments">	[in] The function arguments. </param>
			///
			/// <returns>	True if the transform succeeds, otherwise false. </returns>
			void* HS_AITransformActor(void** arguments);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Halo script ai transform actors function. </summary>
			///
			/// <param name="arguments">	[in] The function arguments. </param>
			///
			/// <returns>	True if the transform succeeds, otherwise false. </returns>
			void* HS_AITransformActors(void** arguments);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Halo script ai transform actors by type function. </summary>
			///
			/// <param name="arguments">	[in] The function arguments. </param>
			///
			/// <returns>	True if the transform succeeds, otherwise false. </returns>
			void* HS_AITransformActorsByType(void** arguments);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Halo script to get whether an actor is transforming. </summary>
			///
			/// <param name="arguments">	[in] The function arguments. </param>
			///
			/// <returns>	True if the unit is transforming, otherwise false. </returns>
			void* HS_AIActorIsTransforming(void** arguments);
		}
	};
};
