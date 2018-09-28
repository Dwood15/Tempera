#pragma once

#include "engine_functions.h"

// comment the following to just use the parameters as-is
// instead of copying to the stack. I believe there was some
// concurrency issues originally which caused me to use local
// buffers instead...
enum { k_engine_function_string_buffer_size = 512, };

// End the code to a naked function which takes arguments
// and is also __stdcall
#define API_FUNC_NAKED_END(arg_count)	\
 		__asm pop	ebp 				\
		__asm retn	(arg_count * 4) 	}

namespace Yelo {
	namespace Physics {
		struct s_collision_result;
	};

	namespace Players {
		struct s_unit_camera_info;
	};

	namespace TagGroups {
		struct s_bitmap_data;

		struct s_sound_permutation;

		struct predicted_resource;
	};

	namespace Engine {
	namespace Cache {
		__declspec(naked) bool FileReadRequest(/*datum_index tag_index,*/
			uint32 offset_, uint32 size_, void *buffer, const Yelo::Cache::s_cache_file_request_params &params, bool block, Enums::cache_file_request_source source) {
			static const uintptr_t FUNCTION = GET_FUNC_PTR(CACHE_FILE_READ_REQUEST);

			__asm push ebp 
			__asm mov ebp, esp

			__asm{
				push esi
				xor eax, eax

				movzx eax, source
				push eax
				movzx eax, block
				push eax
				mov esi, params
				push buffer
				push size_
				push offset_
				call FUNCTION
				add esp, 4 * 5

				pop esi
				pop ebp
				retn(6*4)
			}
			// not including the unused tag_index
		}
	};
};

namespace Effects {
	void __declspec(naked) NewOnObjectMarker(datum_index effect_definition_index, datum_index object_index, cstring marker_name) {
		static const uintptr_t FUNCTION = GET_FUNC_PTR(EFFECT_NEW_ON_OBJECT_MARKER);

		__asm { 
	push	ebp 
	mov	ebp, esp

		push esi
		push edi
		push ecx

		mov ecx, marker_name
		push ecx
		mov esi, object_index
		mov edi, effect_definition_index
		call FUNCTION
		add esp,
		4 * 1

		pop ecx
		pop edi
		pop esi
		API_FUNC_NAKED_END(3)
		}
	};

	namespace HS {
		void ObjectListAdd(datum_index object_list, datum_index object_index) {
			static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_LIST_ADD);

			if (object_list.IsNull()) return;

			__asm {
			push	object_index
			mov 	eax, object_list
			call	FUNCTION
			add 	esp, 4 * 1
			}
		}
	};

	namespace Objects {
#include "Game/EngineFunctions.Objects.inl"
};

namespace Players {
#include "Game/EngineFunctions.Players.inl"

		};

		namespace Scenario {
			bool TriggerVolumeTestPoint(int32 trigger_volume_index, const real_point3d &point) {
				static const uintptr_t FUNCTION = GET_FUNC_PTR(SCENARIO_TRIGGER_VOLUME_TEST_POINT);
				__asm {
					mov ecx, point 
					mov eax, trigger_volume_index 
					call FUNCTION 
				}
			}

			bool TriggerVolumeTestObject(int32 trigger_volume_index, datum_index object) {
				if (object.IsNull()) 	{
					return false;
				}
				Yelo::Objects::s_object_data *obj = Yelo::Objects::ObjectHeader()[object]->_object;
				return TriggerVolumeTestPoint(trigger_volume_index, obj->center);
			}
		};

		namespace TagGroups {
			void PredictResources(TagBlock <Yelo::TagGroups::predicted_resource> &resources_block) { 
			__asm { 
					push esi
					mov esi, resources_block 
					call FUNCTION
					pop	esi 
				}
			}

			datum_index TagLoaded(tag group_tag, cstring name) {
				static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_LOADED);
				char local[256]; 
				memset(local, 0, sizeof(local)); 
				strcpy_s(local, name);
				__asm {
					push edi 
					lea eax, local 
					push eax 
					mov edi, group_tag 
					call FUNCTION 
					add esp, 4 * 1
					pop edi 
				}
			}

			wcstring UnicodeStringListGetString(datum_index ustr, int32 index) { 
			static const uintptr_t FUNCTION = GET_FUNC_PTR(UNICODE_STRING_LIST_GET_STRING);
			__asm { 
				mov edx, index 
				mov ecx, ustr 
				call FUNCTION 
				}
			}
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// ai
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// actors.c
		__declspec(naked) void actor_delete(datum_index 			actor_index, 	bool is_dead) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_DELETE);

			__asm { 
	push	ebp 
	mov	ebp, esp

			pushad 

			movzx eax, is_dead
			push eax
			mov 	ebx, actor_index
			call FUNCTION
			add esp, 4

			popad
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void actor_update(const datum_index actor_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_UPDATE);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax
			push ecx
			push edx
			push esi

			mov esi, actor_index
			call FUNCTION

			pop 	esi
			pop edx
			pop ecx
			pop 	eax
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void actor_customize_unit(const datum_index actor_variant, const datum_index unit_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_CUSTOMIZE_UNIT);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax
			push ecx
			push edx

			mov 	eax, actor_variant
			push unit_index
			call FUNCTION
			add esp, 4

			pop edx
			pop ecx
			pop 	eax
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void actor_set_active(const datum_index actor_index, const bool active) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_SET_ACTIVE);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax
			push edi

			xor eax, eax
			mov 	al, active
			mov 	edi, actor_index
			call FUNCTION

			pop 	edi
			pop 	eax
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void actor_set_dormant(const datum_index actor_index, const bool dormant) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_SET_DORMANT);

			__asm { 
	push	ebp 
	mov	ebp, esp
			pushad

			xorebx, ebx
			mov 	bl, dormant
			mov 	eax, actor_index
			call FUNCTION

			popad
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void actor_delete_props(const datum_index actor_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_DELETE_PROPS);

			_asm
			jmp FUNCTION;
		}

		__declspec(naked) void actor_freeze(const datum_index actor_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_FREEZE);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax
			push ecx
			push edx

			mov 	eax, actor_index
			call FUNCTION

			pop edx
			pop ecx
			pop 	eax
			pop ebp
			API_FUNC_NAKED_END_()
		}

		void actor_braindead(const datum_index actor_index, const bool braindead) {
			const auto actor_data = AI::Actors()[actor_index];

			if (braindead) { actor_data->current_state = Enums::_actor_default_state_none; actor_data->state.action  = Enums::_actor_action_none;
 actor_delete_props(actor_index); actor_freeze(actor_index); actor_set_dormant(actor_index, 0);
			} else { if (actor_data->current_state == Enums::_actor_default_state_none) { 	actor_data->current_state = Enums::_actor_default_state_alert; }
			}
		}

		__declspec(naked) datum_index

		actor_create_for_unit(const bool is_swarm, const datum_index unit_index, const datum_index actor_variant, const datum_index encounter_index, const int32 squad_index, 					 const int32 arg7, 					 const int32 arg6, const bool magic_sight_after_timer, const Enums::actor_default_state initial_state, const Enums::actor_default_state return_state, 					 const int32 command_list_index, const int32 sequence_id) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_CREATE_FOR_UNIT);

			_asm
			jmp FUNCTION;
		}

		__declspec(naked) void actor_action_change(const datum_index actor_index, const Enums::actor_action new_action_type, const byte *new_action_data) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(ACTOR_ACTION_CHANGE);

			_asm
			jmp FUNCTION;
		}

		//////////////////////////////////////////////////////////////////////////
		// ai_script.c
		__declspec(naked) void ai_scripting_attach_free(datum_index unit_index, 						 datum_index actor_variant_definition_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(AI_SCRIPTING_ATTACH_FREE);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push eax

			mov 	eax, actor_variant_definition_index
			push unit_index
			call FUNCTION
			add esp,
			4

			pop 	eax
			pop ebp

			API_FUNC_NAKED_END_()
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// bink
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// bink_playback.c
		__declspec(naked) void bink_playback_start(cstring bik_path) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(BINK_PLAYBACK_START);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push bik_path
			call FUNCTION
			add esp, 4
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// cache
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// cache_files_windows.c
		bool cache_file_read_request(/*datum_index tag_index,*/
			uint32 	offset,
			uint32 size,
			void *buffer,
			const Cache::s_cache_file_request_params &params,
			bool block, Enums::cache_file_request_source
			source) {
			return Engine::Cache::FileReadRequest(offset, size, buffer, params, block, source );
		}

		//////////////////////////////////////////////////////////////////////////
		// pc_sound_cache.c
		bool sound_cache_sound_request(TagGroups::s_sound_permutation 	 *sound_perm, 	 bool block_thread, 	 bool load, 	 bool reference) {
			return Engine::SoundCacheRequestSound(sound_perm, block_thread, load, reference );
		}

		//////////////////////////////////////////////////////////////////////////
		// pc_texture_cache.c
		IDirect3DBaseTexture9 ** texture_cache_bitmap_get_hardware_format(TagGroups::s_bitmap_data 		  *bitmap,
			// block 		  bool block_thread, 		  bool load) {
			return Engine::TextureCacheBitmapGetHardwareFormat(bitmap, block_thread, load );
		}

		//////////////////////////////////////////////////////////////////////////
		// physical_memory_map.c
		void predicted_resources_precache(TagBlock <TagGroups::predicted_resource> &resources) {
			return Engine::TagGroups::PredictResources(resources);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// camera
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// director.c
		__declspec(naked) void director_save_camera() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DIRECTOR_SAVE_CAMERA);

			__asm {
			call	FUNCTION
			retn
			}
		}

		__declspec(naked) void director_load_camera() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DIRECTOR_LOAD_CAMERA);

			__asm {
			call	FUNCTION
			retn
			}
		}

		__declspec(naked) int16

		director_desired_perspective(const datum_index unit_index, Enums::game_perspective &desired_perspective) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DIRECTOR_DESIRED_PERSPECTIVE);

			__asm { 
	push	ebp 
	mov	ebp, esp
			mov 	ecx, unit_index
			mov 	edx,[desired_perspective]
			push edx
			call FUNCTION
			add esp, 4
			pop ebp
			API_FUNC_NAKED_END_()
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// game
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// cheats.c
		__declspec(naked) void cheat_all_weapons() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_ALL_WEAPONS);

			__asm {
			call	FUNCTION
			retn
			}
		}

		__declspec(naked) void cheat_spawn_warthog() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_SPAWN_WARTHOG);

			__asm {
			call	FUNCTION
			retn
			}
		}

		__declspec(naked) void cheat_teleport_to_camera() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_TELEPORT_TO_CAMERA);

			__asm {
			call	FUNCTION
			retn
			}
		}

		__declspec(naked) void cheat_active_camouflage() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE);

			__asm {
			call	FUNCTION
			retn
			}
		}

		__declspec(naked) void cheat_active_camouflage_local_player() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER);

			__asm {
			call	FUNCTION
			retn
			}
		}

		__declspec(naked) datum_index

		cheat_local_player() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CHEAT_PLAYER_INDEX);

			__asm {
			call	FUNCTION
			retn
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// game_allegiance.c
		bool game_team_is_enemy(long_enum 				 team, 				 long_enum team_to_test) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(GAME_TEAM_IS_ENEMY);

			__asm {
			mov 	ecx, team_to_test
			mov 	edx, team
			call	FUNCTION
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// game_engine.c
		datum_index find_closest_player_index(datum_index 			 player_index) {
			return Engine::Players::FindClosestPlayerIndex(player_index);
		}

		__declspec(naked) void game_engine_rasterize_message(wcstring  message,  real alpha) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(GAME_ENGINE_RASTERIZE_MESSAGE);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push alpha
			push message
			call FUNCTION
			add esp,
			4 * 2
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		//////////////////////////////////////////////////////////////////////////
		// game_engine_multiplayer_sounds.c
		__declspec(naked) void game_engine_play_multiplayer_sound(datum_index 		  player_index,	_enum multiplayer_sound_index,	bool should_replicate) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(GAME_ENGINE_PLAY_MULTIPLAYER_SOUND);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push esi
			push edi

			movzx
			esi, should_replicate
			push esi
			movsx
			esi, multiplayer_sound_index
			mov 	edi, player_index
			call FUNCTION
			add esp,
			4 * 1

			pop 	edi
			pop 	esi
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		//////////////////////////////////////////////////////////////////////////
		// players.c
		__declspec(naked) void player_examine_nearby_vehicle(datum_index 	player_index,  datum_index vehicle_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(PLAYER_EXAMINE_NEARBY_VEHICLE);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push vehicle_index
			push player_index
			call FUNCTION
			add esp,
			4 * 2
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		__declspec(naked) void player_set_action_result(datum_index	player_index, 						 datum_index action_object_index, int32	action_result, 						 int32 action_seat_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(PLAYER_SET_ACTION_RESULT);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push ebx

			push action_seat_index
			push action_result
			mov 	ebx, action_object_index
			mov 	eax, player_index
			call FUNCTION
			add esp,
			4 * 2

			pop 	ebx
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		__declspec(naked) void player_set_action_result_to_network(datum_index 			player_index, 	datum_index action_object_index, int32 			action_result_type, 	int32 action_result, int32 			action_seat_index, 	datum_index next_weapon_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(PLAYER_SET_ACTION_RESULT_TO_NETWORK);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push edi

			push next_weapon_index
			push action_seat_index
			push action_result
			push action_result_type
			mov 	edi, action_object_index
			mov 	ecx, player_index
			call FUNCTION
			add esp,
			4 * 4

			pop 	edi
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		datum_index player_index_from_unit_index(datum_index	unit_index) {
			return Engine::Players::IndexFromUnitIndex(unit_index);
		}

		bool player_teleport(datum_index 					 player_index, 			 datum_index source_unit_index, 			 const real_point3d &position) {
			return Engine::Players::Teleport(player_index, position, source_unit_index );
		}

		void player_over_shield_screen_effect(datum_index 		player_index) {
			Engine::Players::ScreenEffectOvershield(player_index);
		}

		void player_active_camo_screen_effect(datum_index 		player_index) {
			Engine::Players::ScreenEffectCamo(player_index);
		}

		void player_health_pack_screen_effect(datum_index 		player_index) {
			Engine::Players::ScreenEffectHealth(player_index);
		}

		//////////////////////////////////////////////////////////////////////////
		// player_control.c
		__declspec(naked) void player_control_get_unit_camera_info(const int16 player_index, Players::s_unit_camera_info &camera_info) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(PLAYER_CONTROL_GET_UNIT_CAMERA_INFO);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax
			push ecx
			push edx
			push esi

			mov 	ax, player_index
			mov esi, camera_info
			call FUNCTION

			pop 	esi
			pop edx
			pop ecx
			pop 	eax
			pop ebp
			API_FUNC_NAKED_END_()
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// hs
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// hs_library_external.c
		void hs_object_orient(datum_index object_index, 			  int32 cutscene_flag_index, 			  bool teleport, 			  bool set_facing) {
			Engine::Objects::ObjectOrient(object_index, cutscene_flag_index, teleport, set_facing );
		}

		void hs_effect_new_from_object_marker(datum_index 		effect_definition_index, datum_index object_index, cstring marker_name) {
			Engine::Effects::NewOnObjectMarker(effect_definition_index, object_index, marker_name );
		}

		//////////////////////////////////////////////////////////////////////////
		// object_lists.c
		void object_list_add(datum_index 					 object_list_index, 			 datum_index object_index) {
			Engine::HS::ObjectListAdd(object_list_index, object_index );
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// input
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// input_windows.c
		__declspec(naked) bool input_key_is_down(_enum 				key_code) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(INPUT_KEY_IS_DOWN);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push ecx

			mov 	cx, key_code
			call FUNCTION

			pop ecx
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// interface
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// hud.c
		__declspec(naked) wcstring hud_get_item_string(short reference_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(HUD_GET_ITEM_MESSAGE);

			__asm { 
	push	ebp 
	mov	ebp, esp

			movsx
			edx, reference_index
			call FUNCTION
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		//////////////////////////////////////////////////////////////////////////
		// hud_chat.c
		void hud_chat_to_network(long player_number, unsigned long chat_type, wcstring text) {
			Engine::Networking::EncodeHudChatNetworkData(player_number, chat_type, text);
		}

		__declspec(naked) void hud_chat_display_message(wcstring  message) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(KEYSTONE_CHAT_LOG_ADD_STRING);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push message
			call FUNCTION
			add esp,
			4 * 1
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		//////////////////////////////////////////////////////////////////////////
		// hud_draw.c
		__declspec(naked) void hud_draw_meter(int16 /*local_player_index*/, TagGroups::s_hud_absolute_placement
		*placement, 			TagGroups::s_hud_element_meter *element, 			byte 			arg_C, 			byte arg_10, long_flags 			flags, 			real arg_18, real 			arg_1C) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(HUD_DRAW_METER);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push esi

			push arg_1C
			push arg_18
			push flags
			movzx
			esi, arg_10
			push esi
			movzx
			esi, arg_C
			push esi
			mov esi, element
			push placement
			call FUNCTION
			add esp,
			4 * 6

			pop 	esi
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		__declspec(naked) void hud_draw_numbers(int16 			  local_player_index, 			  TagGroups::s_hud_absolute_placement *placement, TagGroups::s_hud_element_number 			  *element, 			  int32 number, int16 			  arg_10, 			  long_flags flags, game_ticks_t 			  first_render_time, 			  real arg_1C) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(HUD_DRAW_NUMBERS);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push esi

			push arg_1C
			push first_render_time
			push flags
			movsx
			esi, arg_10
			push esi
			push number
			push element
			push placement
			movsx
			esi, local_player_index
			push esi
			call FUNCTION
			add esp,
			4 * 8

			pop 	esi
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		__declspec(naked) void hud_draw_static_element(int16 						local_player_index, 						TagGroups::s_hud_absolute_placement *placement, TagGroups::s_hud_element_static 						*element, 						long_flags flags, game_ticks_t 						first_render_time) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(HUD_DRAW_STATIC_ELEMENT);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push esi

			push first_render_time
			push flags
			push element
			push placement
			movsx
			esi, local_player_index
			push esi
			call FUNCTION
			add esp,
			4 * 5

			pop 	esi
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		//////////////////////////////////////////////////////////////////////////
		// hud_messaging.c
		__declspec(naked) void hud_print_message(int16 				local_player_index, 				wcstring message) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(HUD_PRINT_MESSAGE);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push message
			movsx
			eax, local_player_index
			call FUNCTION
			add esp,
			4 * 1
			__asm pop	ebp 	 	 	
 	__asm retn 	 	 		
	}
		}

		//////////////////////////////////////////////////////////////////////////
		// ui_video_screen.c
		void ui_video_screen_add_resolution(uint32 	 width,	uint32 height, uint32 	 refresh_rate) {
			Engine::RasterizerAddResolution(width, height, refresh_rate );
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// items
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// weapons.c
		__declspec(naked) bool

		weapon_prevents_melee_attack(const datum_index weapon_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(WEAPON_PREVENTS_MELEE_ATTACK);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ecx

			mov 	ecx, weapon_index
			call FUNCTION

			pop ecx
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) bool

		weapon_prevents_grenade_throwing(const datum_index weapon_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(WEAPON_PREVENTS_GRENADE_THROWING);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ecx

			mov 	ecx, weapon_index
			call FUNCTION

			pop ecx
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void weapon_stop_reload(const datum_index weapon_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(WEAPON_STOP_RELOAD);

			_asm
			jmp FUNCTION;
		}

		__declspec(naked) void first_person_weapon_message_from_unit(const datum_index unit_index, const int32 weapon_message_type) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(FIRST_PERSON_WEAPON_MESSAGE_FROM_UNIT);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax
			push ecx
			push edx

			mov 	eax, unit_index
			push weapon_message_type
			call FUNCTION
			add esp, 4

			pop edx
			pop ecx
			pop 	eax
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) int16

		weapon_get_first_person_animation_time(const datum_index weapon_index, const int16 frame_type, Enums::first_person_weapon_animation animation, 		const int32 arg3) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(WEAPON_GET_FIRST_PERSON_ANIMATION_TIME);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ecx

			xor ecx, ecx
			movsx
			cx, frame_type
			push arg3
			push ecx
			mov 	eax, weapon_index
			movsx
			cx, animation
			call FUNCTION
			add esp, 8

			pop ecx
			pop ebp
			API_FUNC_NAKED_END_()
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// main
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// console.c
		__declspec(naked) bool console_process_command(long_flags 						access_flags, 						cstring command) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CONSOLE_PROCESS_COMMAND);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push edi

			mov 	edi, command
			push access_flags
			call FUNCTION
			add esp,
			4 * 1

			pop 	edi
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		static __declspec(naked) void console_printf_impl(bool clear_screen, cstring format) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CONSOLE_PRINTF);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push format
			mov 	al, clear_screen
			call FUNCTION
			add esp, 4 * 1
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		void console_printf(bool clear_screen, cstring format, ...) {
			char local[k_engine_function_string_buffer_size];
			memset(local, 0, k_engine_function_string_buffer_size);

			va_list args; va_start(args, format);
			vsprintf_s(local, format, args); va_end(args);

			console_printf_impl(clear_screen, local);
		}

		static __declspec(naked) void console_response_printf_impl(BOOL clear_screen, cstring format) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CONSOLE_RESPONSE_PRINTF);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push format
			push clear_screen
			call FUNCTION
			add esp,
			4 * 2
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		void console_response_printf(bool clear_screen, cstring format, ...) {
			char local[k_engine_function_string_buffer_size];
			memset(local, 0, k_engine_function_string_buffer_size);

			va_list args; va_start(args, format);
			vsprintf_s(local, format, args); va_end(args);

			console_response_printf_impl(clear_screen, local);
		}

		static __declspec(naked) void console_warning_impl(cstring 					format) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(CONSOLE_WARNING);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push format
			call FUNCTION
			add esp,
			4 * 1
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		void console_warning(cstring 			 format, ...) {
			char local[k_engine_function_string_buffer_size];
			memset(local, 	 0, k_engine_function_string_buffer_size);

			va_list args; va_start(args, format );
			vsprintf_s(local, format, args ); va_end(args);

			console_warning_impl(local);
		}

		bool console_process_remote_command(cstring command, int32 machine_index) {
			Console::TerminalGlobals()->rcon_machine_index = machine_index;
			bool result 	 	 	 	 	 	= console_process_command(0, command);
			Console::TerminalGlobals()->rcon_machine_index = NONE;

			return result;
		}

		//////////////////////////////////////////////////////////////////////////
		// main.c
		__declspec(naked) bool main_connect(cstring 		 address, 		 cstring password ) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(MAIN_CONNECT);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push password
			push address
			call FUNCTION
			add esp, 4 * 2
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		void main_menu_load() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(MAINMENU_LOAD);

			GameState::MainGlobals()->map.main_menu_scenario_load = true;

			__asm call	FUNCTION
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// math
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// periodic_functions.c
		__declspec(naked) real periodic_function_evaluate(Enums::periodic_function function_type, real input) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(PERIODIC_FUNCTION_EVALUATE);

			__asm { 
	push	ebp 
	mov	ebp, esp
			fld input
			sub esp, 4 * 2 		// allocate space for the 'input' parameter
			fstp qword
			ptr[esp] 	// store the input on the stack
			movzx eax, function_type
			call FUNCTION
			add esp, 4 * 2 		// deallocate. double type consumes two DWORDs of stack
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		__declspec(naked) real transition_function_evaluate(Enums::transition_function function_type, real input) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(TRANSITION_FUNCTION_EVALUATE);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push input
			movzx ecx, function_type
			call FUNCTION
			add esp, 4 * 1
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// memory
	namespace blam {
		using namespace Yelo::Memory;

		//////////////////////////////////////////////////////////////////////////
		// data.c
		__declspec(naked) s_data_array * data_new(cstring 	name, 	int32 maximum_count, size_t 	datum_size) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATA_NEW);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push ebx

			mov 	ebx, datum_size
			push maximum_count
			push name
			call FUNCTION
			add esp, 4 * 2

			pop 	ebx
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		void data_dispose(s_data_array *data) {
			if (data != nullptr) { 		CAST_PTR(Yelo::Memory::s_data_array *, GlobalFree(data));
			}
		}

		__declspec(naked) void data_delete_all(s_data_array *data) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATA_DELETE_ALL);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push esi

			mov esi, data
			call FUNCTION

			pop esi
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		datum_index data_next_index(s_data_array *data, datum_index cursor) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATA_NEXT_INDEX);

			if (data == nullptr || cursor.IsNull()) {
				return datum_index::null;
			}

			__asm {
			push	esi
			push	edi

			mov esi, cursor
			mov edi, data
			call	FUNCTION

			pop edi
			pop esi
			}
		}

		/*		void*  data_iterator_next(s_data_iterator& iterator) { 	static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATA_ITERATOR_NEXT);
 	__asm { 
	push	ebp 
	mov	ebp, esp 		push	edi
 		mov		edi, iterator 		call	FUNCTION
 		pop		edi 	pop	ebp 	 	 	
 	retn 	 	 		
	} }*/
		datum_index datum_new_at_index(s_data_array *data, datum_index index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATUM_NEW_AT_INDEX);

			if (data == nullptr || index.IsNull()) {
				return datum_index::null;
			}

			__asm {
			mov eax, index
			mov edx, data
			call	FUNCTION
			}
		}

		datum_index datum_new(s_data_array  *data) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATUM_NEW);

			if (data == nullptr)  {
				return datum_index::null();
			}

			__asm {
			mov edx, data
			call	FUNCTION
			}
		}

		void datum_delete(s_data_array	*data,	datum_index index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATUM_DELETE);

			if (data == nullptr || index.IsNull()) { 
				return;
			}

			__asm {
				mov edx, index
				mov eax, data
				call FUNCTION
			}
		}

		void * datum_try_and_get(s_data_array *data, datum_index index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(DATUM_TRY_AND_GET);

			if (data == nullptr) { 
				return nullptr;
			}

			__asm {
			push edx
			push esi

			mov edx, index
			mov esi, data
			call	FUNCTION

			pop esi
			pop edx
			}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// models
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// models.c
		__declspec(naked) int16 model_find_marker(const datum_index render_model_definition_index, cstring marker_name) {
			static const uintptr_t CALL_ADDRESS = Engine::GET_FUNC_PTR(MODEL_FIND_MARKER);

			__asm { 
	push	ebp 
	mov	ebp, esp
			mov eax, render_model_definition_index
			push render_model_definition_index
			push marker_name
			call CALL_ADDRESS
			add esp, 8

			pop ebp
			API_FUNC_NAKED_END_()
		}

		//////////////////////////////////////////////////////////////////////////
		// model_animations.c
		int16 animation_choose_random_permutation_internal(unsigned long render_or_affects_game_state, datum_index animation_graph_index, long animation_index) {
			return Engine::AnimationPickRandomPermutation(render_or_affects_game_state, animation_graph_index, animation_index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// objects
	namespace blam {
		using namespace Yelo::Objects;

		//////////////////////////////////////////////////////////////////////////
		// damage.c
		bool object_restore_body(datum_index object_index) {
			return Engine::Objects::RestoreBody(object_index);
		}

		void object_deplete_body(datum_index object_index) {
			Engine::Objects::DepleteBody(object_index);
		}

		void object_deplete_shield(datum_index object_index) {
			Engine::Objects::DepleteShield(object_index);
		}

		void object_double_charge_shield(datum_index object_index) {
			Engine::Objects::DoubleChargeShield(object_index);
		}

		void area_of_effect_cause_damage(s_damage_data &data, datum_index di) {
			// TODO
		}

		void object_cause_damage(s_damage_data &data, datum_index damaged_object_index, short node_index, short region_index, short damage_materials_element_index, real_vector3d *normal) {
			Engine::Objects::ObjectCauseDamage(data, damaged_object_index,   node_index, region_index, damage_materials_element_index, normal);
		}

		//////////////////////////////////////////////////////////////////////////
		// index_resolution.c
		long index_resolution_table_translate(MessageDeltas::s_index_resolution_table &table, datum_index local_index) {
			// TODO: get rid of these guards
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		// objects.c
		void object_reset(datum_index object_index) {
			Engine::Objects::Reset(object_index);
		}

		void object_set_position(datum_index object_index, real_point3d *new_position, real_vector3d *new_forward, real_vector3d *new_up) {
			Engine::Objects::SetPosition(object_index, new_position, new_forward, new_up );
		}

		void object_set_position_network(datum_index  object_index, real_point3d *new_position) {
			Engine::Objects::SetPositionNetwork(object_index, new_position);
		}

		void object_translate(datum_index object_index, const real_point3d &new_position, const s_scenario_location *new_location) {
			// TODO
		}

		void object_placement_data_new(s_object_placement_data &data, datum_index object_definition_index, datum_index owner_object_index) {
			Engine::Objects::PlacementDataNew(data, object_definition_index, owner_object_index);
		}

		datum_index object_new(s_object_placement_data &data) {
			return Engine::Objects::New(data);
		}

		datum_index object_new_with_role(s_object_placement_data &data, Enums::networked_datum role) {
			return Engine::Objects::NewWithRole(data, role );
		}

		void object_delete_to_network(datum_index	object_index) {
			Engine::Networking::EncodeObjectDeletionMessage(object_index);
		}

		void object_delete(datum_index 			  object_index) {
			Engine::Objects::Delete(object_index);
		}

		void object_reconnect_to_map(datum_index object_index, s_scenario_location *location_reference) {
			Engine::Objects::ReconnectToMap(object_index, location_reference);
		}

		void object_disconnect_from_map(datum_index object_index) {
			Engine::Objects::DisconnectFromMap(object_index);
		}

		__declspec(naked) short object_get_marker_by_name(const datum_index object_index, cstring marker_name, s_object_marker *markers, const int16 maximum_marker_count) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(OBJECT_GET_MARKER_BY_NAME);

			__asm jmp FUNCTION;
		}

		void object_attach_to_marker(datum_index	target_object_index, cstring target_marker_name, datum_index object_index, cstring marker_name) {
			Engine::Objects::Attach(target_object_index, target_marker_name, object_index, marker_name );
		}

		void object_detach(datum_index object_index) {
			Engine::Objects::Detach(object_index);
		}

		real_point3d &object_get_origin(datum_index object_index, real_point3d &return_origin) {
			return Engine::Objects::GetOrigin(object_index, return_origin);
		}

		void object_get_orientation(datum_index object_index,  real_vector3d *return_forward,  real_vector3d  *return_up) {
			Engine::Objects::GetOrientation(object_index, return_forward, return_up);
		}

		s_scenario_location & object_get_location(datum_index object_index, s_scenario_location &return_location) {
			return Engine::Objects::GetLocation(object_index, return_location );
		}

		void object_start_interpolation(datum_index object_index, long interpolation_frame_count) {
			Engine::Objects::StartInterpolation(object_index, interpolation_frame_count );
		}

		s_object_data * object_iterator_next(s_object_iterator  &iter) {
			return Engine::Objects::IteratorNext(iter);
		}

		short objects_in_sphere(Flags::objects_find_flags find_flags, unsigned int object_type_flags, const s_scenario_location &location, const real_point3d &center, real radius, datum_index object_indices[], int16 maximum_object_indices) {
			return Engine::Objects::FindInSphere(find_flags, object_type_flags, location, center, radius, object_indices, maximum_object_indices);
		}

		void object_definition_predict(datum_index object_index) {
			Engine::Objects::DefinitionPredict(object_index);
		}

		bool object_header_block_allocate(datum_index object_index, size_t block_reference_offset, size_t size) {
			return Engine::Objects::HeaderBlockAllocate(object_index, block_reference_offset, size);
		}

		void objects_scripting_set_scale(datum_index object_index, real scale, int32 ticks) {
			Engine::Objects::SetScale(object_index, scale, ticks );
		}

		real object_get_level_of_detail_pixels(datum_index object_index) {
			return Engine::Objects::GetLevelOfDetailPixels(object_index);
		}

		void object_render_state_refresh(datum_index object_render_state_index, datum_index object_index, real level_of_detail_pixels, byte arg4) {
			Engine::Objects::ObjectRenderStateRefresh(object_render_state_index, object_index, level_of_detail_pixels, arg4);
		}

		__declspec(naked) void objects_update() {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(OBJECTS_UPDATE);

			_asm jmp FUNCTION;
		}

		__declspec(naked) void object_update(datum_index object_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(OBJECT_UPDATE);

			_asm jmp FUNCTION;
		}

		__declspec(naked) void object_destroy(const datum_index object_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(OBJECT_DESTROY);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax
			push ecx
			push edx
			push edi

			mov eax, object_index
			call FUNCTION

			pop edi
			pop edx
			pop ecx
			pop eax
			pop ebp
			API_FUNC_NAKED_END_()
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// physics
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// collisions.c
		__declspec(naked) bool collision_test_vector(long_flags	flags,	real_point3d &location, real_vector3d	&vector,	datum_index object_index, Physics::s_collision_result	&collision) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(COLLISION_TEST_VECTOR);

			__asm jmp	FUNCTION;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// rasterizer
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// dx9/rasterizer_dx9.c
		bool rasterizer_set_texture_bitmap_data(_enum	stage,	TagGroups::s_bitmap_data *bitmap) {
			return Engine::SetTextureSamplerStage(bitmap, stage );
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// render
	namespace blam {
		void render_window(const uint16 local_player_index, void *render_camera, void *render_frustum, void *rasterizer_camera, void *rasterizer_frustum, void *rasterizer_target, const bool is_mirror) {
			CAST_PTR(void(*)( const uint16, void*, void*, void*, void*, void*, const bool), Engine::GET_FUNC_PTR(RENDER_WINDOW)) (local_player_index, render_camera, render_frustum, rasterizer_camera, rasterizer_frustum, rasterizer_target, is_mirror);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// scenario
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// scenario.c
		// After calling this function you should call Yelo::GameState::InitializeForNewBSP
		__declspec(naked) bool scenario_switch_structure_bsp(int16  bsp_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(SCENARIO_SWITCH_STRUCTURE_BSP);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push esi

			movsx
			esi, bsp_index
			call FUNCTION

			pop 	esi
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}

		bool scenario_trigger_volume_test_point(int32 		  trigger_volume_index,	const real_point3d &point) {
			return Engine::Scenario::TriggerVolumeTestPoint(trigger_volume_index, point );
		}

		bool scenario_trigger_volume_test_object(int32 			trigger_volume_index, 	datum_index object_index) {
			return Engine::Scenario::TriggerVolumeTestObject(trigger_volume_index, object_index );
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// shell
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// shell_windows.c
		__declspec(naked) bool shell_get_command_line_argument(cstring 			 param, 			 _Out_opt_ cstring
 			 *value) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(SHELL_GET_COMMAND_LINE_ARGUMENT);

			__asm { 
	push	ebp 
	mov	ebp, esp

			push edi

			mov  edi, value
			push param
			call FUNCTION
			add  esp,
			4 * 1

			pop edi
			pop	ebp 	 	 	
 	retn 	 	 		
	}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// tag_files
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// tag_groups.c
		datum_index tag_loaded(tag 	  group_tag, 	  cstring name) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(TAG_LOADED);

			char local[256];
			memset(local, 	 0, sizeof(local));
			strcpy_s(local, name );

			__asm {
			push	edi

			lea 	eax, local
			push	eax
			mov 	edi, group_tag
			call	FUNCTION
			add 	esp, 4 * 1

			pop 	edi
			}
		}

		void tag_iterator_new(TagGroups::s_tag_iterator 			  &iter, 			  const tag group_tag_filter) {
			memset(&iter, 	 0, sizeof(iter));
			iter. group_tag_filter = group_tag_filter;
		}

		datum_index tag_iterator_next(TagGroups::s_tag_iterator 				&iter) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(TAG_ITERATOR_NEXT);

			__asm {
			push	esi

			mov 	esi, iter
			call	FUNCTION

			pop 	esi
			}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// text
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// text_group.c
		wcstring unicode_string_list_get_string(datum_index unicode_string_list_definition_index, int32 reference_index) {
			return Engine::TagGroups::UnicodeStringListGetString(unicode_string_list_definition_index, reference_index );
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// units
	namespace blam {
		//////////////////////////////////////////////////////////////////////////
		// units.c
		__declspec(naked) void unit_update(const datum_index unit_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_UPDATE);

			_asm jmp FUNCTION;
		}

		void unit_set_animation(datum_index  unit_index, datum_index animation_graph_index, long animation_index) {
			Engine::Objects::UnitSetAnimation(unit_index, animation_graph_index, animation_index );
		}

		__declspec(naked) void unit_animation_start_action(const datum_index unit_index, const Enums::unit_replacement_animation_state action_type) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_ANIMATION_START_ACTION);

			_asm jmp FUNCTION;
		}

		short unit_find_nearby_seat(datum_index unit_index, datum_index target_unit_index, short &parent_seat_index) {
			return Engine::Objects::UnitFindNearbySeat(unit_index, target_unit_index, parent_seat_index);
		}

		bool unit_can_enter_seat(datum_index unit_index, datum_index target_unit_index, short seat_index, datum_index *unit_in_seat_index) {
			return Engine::Objects::UnitCanEnterSeat(unit_index, target_unit_index, seat_index, unit_in_seat_index);
		}

		bool unit_enter_seat(datum_index unit_index, datum_index target_unit_index, long seat_index) {
			return Engine::Objects::UnitEnterSeat(unit_index, target_unit_index, seat_index );
		}

		void unit_get_camera_position(datum_index unit_index, real_point3d *return_position) {
			Engine::Objects::UnitGetCameraPosition(unit_index, return_position );
		}

		void unit_kill(const datum_index unit_index) {
			auto *unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(unit_index);
			if (unit_datum) {
				SET_FLAG(unit_datum->object.damage.flags, Flags::_object_killed_bit, true);
			}
		}

		void unit_open(datum_index unit_index) {
			Engine::Objects::UnitOpen(unit_index);
		}

		void unit_close(datum_index unit_index) {
			Engine::Objects::UnitClose(unit_index);
		}

		void unit_scripting_exit_vehicle(datum_index unit_index) {
			Engine::Objects::UnitExitVehicle(unit_index);
		}

		__declspec(naked) bool unit_try_and_exit_seat(const datum_index unit_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_TRY_AND_EXIT_SEAT);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ecx
			push edx
			push edi

			mov edi, unit_index
			call FUNCTION

			pop edi
			pop edx
			pop ecx
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void unit_detach_from_parent(const datum_index unit_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_DETACH_FROM_PARENT);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push edi

			mov 	edi, unit_index
			call FUNCTION

			pop 	edi
			pop ebp
			API_FUNC_NAKED_END_()
		}

		void unit_exit_seat_end(datum_index unit_index, bool no_network_message, bool can_run_on_client_side, bool unknown) {
			Engine::Objects::UnitExitSeatEnd(unit_index, no_network_message, can_run_on_client_side, unknown);
		}

		__declspec(naked) bool unit_can_see_point(const datum_index unit_index, const real_point3d *point, const real view_radians) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_CAN_SEE_POINT);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ecx
			push edx
			push edi

			mov ecx, unit_index
			mov edi, point
			push view_radians
			call FUNCTION
			add esp, 4

			pop edi
			pop edx
			pop ecx
			pop ebp
			API_FUNC_NAKED_END_()
		}

		int16 unit_get_custom_animation_time(datum_index unit_index) {
			return Engine::Objects::UnitGetCustomAnimationTime(unit_index);
		}

		__declspec(naked) bool unit_start_user_animation(const datum_index unit_index, const datum_index animation_definition_index, cstring animation_name, const bool interpolate) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_START_USER_ANIMATION);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ebx
			push ecx
			push edx
			push edi

			mov eax, animation_name
			mov edi, animation_definition_index
			xor ebx, ebx
			mov bl, interpolate
			push ebx
			push unit_index
			call FUNCTION
			add esp, 8

			pop edi
			pop edx
			pop ecx
			pop ebx
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void unit_animation_set_state(const datum_index unit_index, const Enums::unit_animation_state state) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_ANIMATION_SET_STATE);

			_asm jmp FUNCTION;
		}

		__declspec(naked) void unit_drop_current_weapon(const datum_index unit_index, const bool force) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_DROP_CURRENT_WEAPON);

			_asm jmp FUNCTION;
		}

		__declspec(naked) datum_index unit_inventory_get_weapon(const datum_index unit_index, const int16 index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_INVENTORY_GET_WEAPON);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ecx
			push edx

			mov eax, unit_index
			xor ecx, ecx
			mov 	cx, index
			call FUNCTION

			pop edx
			pop ecx
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void unit_ready_desired_weapon(const datum_index unit_index, const bool force) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_READY_DESIRED_WEAPON);

			__asm jmp FUNCTION;
		}

		__declspec(naked) void unit_throw_grenade_release(const datum_index unit_index, const sbyte keyframe) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_THROW_GRENADE_RELEASE);

			__asm jmp FUNCTION;
		}

		__declspec(naked) void unit_cause_player_melee_damage(const datum_index unit_index) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_CAUSE_PLAYER_MELEE_DAMAGE);

			__asm jmp FUNCTION;
		}

		__declspec(naked) void unit_damage_aftermath(const datum_index unit_index, const Objects::s_damage_data *damage_data, const _enum damage_flags, 					 const real shield_amount, const real body_amount, void *arg6, 					 const int32 damage_part) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_DAMAGE_AFTERMATH);

			__asm jmp FUNCTION;
		}

		__declspec(naked) void unit_scripting_set_current_vitality(const datum_index unit_index, const real health, const real shield) {
			static const uintptr_t CALL_ADDRESS = Engine::GET_FUNC_PTR(UNIT_SCRIPTING_SET_CURRENT_VITALITY);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push eax

			mov eax, unit_index
			push shield
			push health
			call CALL_ADDRESS
			add esp, 8

			pop eax
			pop ebp
			API_FUNC_NAKED_END_()
		}

		__declspec(naked) void unit_set_actively_controlled(const datum_index unit_index, const bool controlled) {
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(UNIT_SET_ACTIVELY_CONTROLLED);

			__asm { 
	push	ebp 
	mov	ebp, esp
			push ecx
			xor ecx, ecx
			mov cl, controlled
			push unit_index
			call FUNCTION
			add esp, 4

			pop ecx
			pop ebp
			API_FUNC_NAKED_END_()
		}
	};
};
