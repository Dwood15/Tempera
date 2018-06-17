/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/GameUI.hpp"

#include <blamlib/Halo1/game/player_structures.hpp>
#include <blamlib/Halo1/interface/first_person_weapons.hpp>
#include <blamlib/Halo1/interface/hud.hpp>
#include <blamlib/Halo1/interface/hud_chat.hpp>
#include <blamlib/Halo1/interface/hud_messaging.hpp>
#include <blamlib/Halo1/interface/hud_unit.hpp>
#include <blamlib/Halo1/interface/hud_nav_points.hpp>
#include <blamlib/Halo1/interface/hud_weapon.hpp>
#include <blamlib/Halo1/interface/interface.hpp>
#include <blamlib/Halo1/interface/motion_sensor.hpp>
#include <blamlib/Halo1/items/weapon_definitions.hpp>
#include <blamlib/Halo1/models/model_animations.hpp>
#include <blamlib/Halo1/text/draw_string.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Settings/Settings.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/Players.hpp"
#include "Networking/MessageDeltas.hpp"
#include "Objects/Objects.hpp"

namespace Yelo
{
#include "Memory/_EngineLayout.inl"

	namespace GameUI
	{
		static void HudHideInterfaceElementsUpdate()
		{
			auto* yelo_globals = Scenario::GetYeloGlobals();

			if (Players::PlayerControlGlobals()->local_players[0].zoom_level >= 0)
			{
				if (TEST_FLAG(yelo_globals->flags, Flags::_project_yellow_globals_hide_health_when_zoomed_bit))
					SET_FLAG(HudUnitInterface()->flags, Flags::_unit_interface_show_health_bit, true);
				if (TEST_FLAG(yelo_globals->flags, Flags::_project_yellow_globals_hide_shield_when_zoomed_bit))
					SET_FLAG(HudUnitInterface()->flags, Flags::_unit_interface_show_shield_bit, true);
				if (TEST_FLAG(yelo_globals->flags, Flags::_project_yellow_globals_hide_motion_sensor_when_zoomed_bit))
					SET_FLAG(HudUnitInterface()->flags, Flags::_unit_interface_show_motion_sensor_bit, true);
			}
			else
			{
				if (TEST_FLAG(yelo_globals->flags, Flags::_project_yellow_globals_hide_health_when_zoomed_bit))
					SET_FLAG(HudUnitInterface()->flags, Flags::_unit_interface_show_health_bit, false);
				if (TEST_FLAG(yelo_globals->flags, Flags::_project_yellow_globals_hide_shield_when_zoomed_bit))
					SET_FLAG(HudUnitInterface()->flags, Flags::_unit_interface_show_shield_bit, false);
				if (TEST_FLAG(yelo_globals->flags, Flags::_project_yellow_globals_hide_motion_sensor_when_zoomed_bit))
					SET_FLAG(HudUnitInterface()->flags, Flags::_unit_interface_show_motion_sensor_bit, false);
			}
		}

		s_hud_scripted_globals* HudScriptedGlobals()		DPTR_IMP_GET(hud_scripted_globals);
		s_hud_messaging* HudMessaging()						DPTR_IMP_GET(hud_messaging);
		s_hud_unit_interface* HudUnitInterface()			DPTR_IMP_GET(hud_unit_interface);
		s_hud_weapon_interface* HudWeaponInterface()		DPTR_IMP_GET(hud_weapon_interface);
		s_hud_nav_points* HudNavPoints()					DPTR_IMP_GET(hud_nav_points);
		s_motion_sensor* MotionSensor()						DPTR_IMP_GET(motion_sensor);
		s_first_person_weapons* FirstPersonWeapons()		DPTR_IMP_GET(first_person_weapons);


		s_text_bounds_globals_data* TextBoundsGlobals()		PTR_IMP_GET2(text_bounds_globals);
		s_font_drawing_globals_data* FontDrawingGlobals()	PTR_IMP_GET2(font_drawing_globals);

		s_hud_chat_globals* HudChatGlobals()				PTR_IMP_GET2(hud_chat_globals);
		long HudChatLineCount()							PTR_IMP_GET(hud_chat_line_count);


		static TagGroups::s_weapon_definition const* FirstPersonWeaponGetWeaponDefinition(s_first_person_weapon* fp_weapon)
		{
			datum_index weapon_index = *fp_weapon->GetWeaponIndex();

			if(!weapon_index.IsNull())
				return Objects::GetObjectDefinition<TagGroups::s_weapon_definition>(weapon_index);

			return nullptr;
		}
		static datum_index FirstPersonWeaponGetAnimationGraphIndex(s_first_person_weapon* fp_weapon)
		{
			datum_index anim_graph_index = datum_index::null();

			TagGroups::s_weapon_definition const* weapon = FirstPersonWeaponGetWeaponDefinition(fp_weapon);

			if(weapon != nullptr)
				anim_graph_index = weapon->weapon.first_person_animations.tag_index;

			return anim_graph_index;
		}
		static void FirstPersonWeaponSetStatePermutateAnimation(s_first_person_weapon* fp_weapon, shortanimation_state, short animation_current_index)
		{
			datum_index animation_graph_index = FirstPersonWeaponGetAnimationGraphIndex(fp_weapon);

			if(!animation_graph_index.IsNull())
			{
				animation_current_index = blam::animation_choose_random_permutation_internal(Enums::_animation_update_kind_render_only,
					animation_graph_index, animation_current_index);
			}

			*fp_weapon->GetAnimationState() = animation_state;
			fp_weapon->GetCurrentAnimation()->animation_index = animation_current_index;
		}
		__declspec(naked) static void __cdecl FirstPersonWeaponSetStateHook()
		{
			API_FUNC_NAKED_START_()
				push	edi
				push	ebx
				push	esi
				call	FirstPersonWeaponSetStatePermutateAnimation
			API_FUNC_NAKED_END_()
		}
		static void FirstPersonWeaponSetStateHookCreateHook()
		{
			// Our hook only overwrites the AnimationState and AnimationCurrentIndex setting code. 
			// Leaves the AnimationCurrentFrameIndex set alone (ie, frame index will always be zero)
			static const byte k_null_bytes[8] = { Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,};

			Memory::WriteMemory(GET_FUNC_VPTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK), k_null_bytes);
			Memory::WriteRelativeCall(&FirstPersonWeaponSetStateHook, GET_FUNC_VPTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK), true);
		}

		void Initialize()
		{
			Hud::Initialize();

			Memory::WriteRelativeCall(GameUI::Update, GET_FUNC_VPTR(HUD_UPDATE_CALL));
			Memory::WriteRelativeCall(GameUI::FirstPersonWeaponsUpdate, GET_FUNC_VPTR(FIRST_PERSON_WEAPONS_UPDATE_CALL));
			FirstPersonWeaponSetStateHookCreateHook();
		}

		void Dispose()
		{
			Hud::Dispose();
		}

		__declspec(naked) void __cdecl Update()
		{
			static const uintptr_t HUD_UPDATE = GET_FUNC_PTR(HUD_UPDATE);

			API_FUNC_NAKED_START_()
				call	HUD_UPDATE

				call	Hud::Update
				call	HudHideInterfaceElementsUpdate
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void __cdecl FirstPersonWeaponsUpdate()
		{
			static const uintptr_t FIRST_PERSON_WEAPONS_UPDATE = GET_FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE);

			API_FUNC_NAKED_START_()
				call	FIRST_PERSON_WEAPONS_UPDATE
			API_FUNC_NAKED_END_()
		}


#ifndef YELO_NO_NETWORK
		bool SendHudChatToEveryonePredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index)
		{
			return true;
		}
		static bool SendHutChatToTeamPredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index)
		{
			return player->network_player.team_index == src_player->network_player.team_index;
		}
		static bool SendHudChatToPlayersInVehiclePredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index)
		{
			datum_index player_vehicle_index = player->GetVehicleIndex();
			if(player_vehicle_index.IsNull() || player_vehicle_index != src_player_vehicle_index)
				return false;

			return false;
		}

		static bool SendHudChatToPlayers(byte src_player_number, proc_send_hud_chat_predicate send_predicate,
			long message_size)
		{
			if(send_predicate == SendHudChatToEveryonePredicate)
				return MessageDeltas::SvSendMessageToAllIngame(message_size, 
					true, false, true, 3);
			else
			{
				Players::s_player_datum* src_player = Players::GetPlayerFromNumber(src_player_number);
				datum_index src_player_vehicle_index = datum_index::null();
				if(src_player != nullptr) src_player_vehicle_index = src_player->GetVehicleIndex();

				for(auto player : Players::Players())
				{
					long player_machine_index = player->network_player.machine_index;
					if(	player_machine_index == NONE ||
						!send_predicate(player.datum, src_player, src_player_vehicle_index))
						continue;

					if(!MessageDeltas::SvSendMessageToMachine(player_machine_index, message_size,
						true, false, true, 3))
					{
						YELO_DEBUG_FORMAT("SendHudChatToPlayers failed for %S", player->network_player.name);
					}
				}
			}

			return true;
		}

		static bool SendHudChatServer(byte src_player_number, proc_send_hud_chat_predicate send_predicate,
			long message_size, Enums::hud_chat_type msg_type)
		{
			if(send_predicate == nullptr)
			{
				switch(msg_type)
				{
				case Enums::_hud_chat_type_team: send_predicate = SendHutChatToTeamPredicate; break;
				case Enums::_hud_chat_type_vehicle: send_predicate = SendHutChatToTeamPredicate; break;

				default: send_predicate = SendHudChatToEveryonePredicate;
				}
			}

			return SendHudChatToPlayers(src_player_number, send_predicate, message_size);
		}

		static bool SendHudChatClient(long message_size)
		{
			return MessageDeltas::ClientSendMessageToServer(message_size, true);
		}
		void SendHudChat(Enums::hud_chat_type msg_type, wconst char * message, byte src_player_number, proc_send_hud_chat_predicate send_predicate)
		{
			if(msg_type > Enums::_hud_chat_type_none && msg_type <= Enums::_hud_chat_type_info_msg)
			{
				if(msg_type == Enums::_hud_chat_type_server || msg_type == Enums::_hud_chat_type_info_msg)
					src_player_number = NONE;

				MessageDeltas::hud_chat_network_data network_data;
				network_data.msg_type = msg_type;
				network_data.player_number = src_player_number;
				network_data.message = message;

				long bits_encoded = MessageDeltas::EncodeStateless(
					Enums::_message_delta_hud_chat,
					nullptr, &network_data);

				bool result = false;
				if(bits_encoded > 0)
				{
						 if(GameState::IsServer()) result = SendHudChatServer(src_player_number, send_predicate, bits_encoded, msg_type);
					else if(GameState::IsClient()) result = SendHudChatClient(bits_encoded);
				}

				if(!result)
					YELO_DEBUG("SendHudChat failed", true);
			}
		}
	};
};

#include "Interface/GameUI_Hud.inl"
