#pragma once

#include <precompile.h>
#include "../../cseries/base.h"
#include "../../memory/datum_index.h"
#include "../../game/configuration.h"
#include "../../models/animations/model.hpp"

namespace Yelo {
	namespace GameUI {
		//Because _of course we're in macro hell.
		struct s_first_person_weapon : TStructImpl(0x1EA0) {
			bool * GetIsVisible() { return GetDataPtr<bool, 0x0>(); }
			bool const * GetIsVisible() const { return GetDataPtr<bool, 0x0>(); }

			datum_index const *GetUnitIndex() const { return GetDataPtr<datum_index, 0x4>(); }

			//
			datum_index const *GetWeaponIndex() const { return GetDataPtr<datum_index, 0x8>(); }

			// // 1 = o-h-enter, 3 = overheated, 4 = charging, 5 = posing, 6 = firing1, 10 = meleeing,
			// // 13 = reload empty, 14 = reload full, 19 = switching/ready, 22 = o-h-exit, 23 = firing2
			TStructGetPtrImpl(short, AnimationState, 0xC);
			// // 0xE = unknown 2 bytes
			TStructGetPtrImpl(game_time_t, IdleTime, 0x10);
			//
			 TStructGetPtrImpl(Objects::s_animation_state, CurrentBaseAnimation, 0x12); // frame index of current base animation (idle or moving)
			 TStructGetPtrImpl(Objects::s_animation_state, CurrentAnimation, 0x16);
			//
			 TStructGetPtrImpl(Objects::s_animation_state, CurrentMovingOverlayAnimation, 0x1A);
			TStructGetPtrImpl(Objects::s_animation_state, , 0x1E);

			//TStructGetPtrImpl(real, , 0x28); // frame index (real) for the above anim state
			//TStructGetPtrImpl(real, , 0x2C);
			// 0x30?
			//TStructGetPtrImpl(real, , 0x34);
			// 0x38?
			// 0x3C?
			//TStructGetPtrImpl(real, , 0x40);
			//TStructGetPtrImpl(real, , 0x44);
			// 0x48?
			// 0x4C?
			// 0x50 bool
			// 0x54 real_vector3d forward?
			// 0x60 real_euler_angles2d
			// 0x68 real_euler_angles2d
			// 0x70 real_point3d
			// 0x7C real_point3d?
			//////////////////////////////////////////////////////////////////////////
			// 0x88 short frame index
			// 0x8A short frame count
			// 0x8C real_orientation3d node orientations[128]
			// 0x108C real_matrix4x3 node matrices[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1D8C bool remapped weapon?
			// unsigned char : 8;
			// 0x1D8E short node_remappings[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1E0E bool remapped hands
			// unsigned char : 8;
			// 0x1E10 short node_remappings[64]
			//////////////////////////////////////////////////////////////////////////
			// 0x1E90 bool
			// 0x1E94 short shotgun_reload_type;
			//		_shotgun_reload_type_last_round = 1
			//		_shotgun_reload_type_first_and_last_round = 2
			TStructGetPtrImpl(datum_index, AnimationSoundIndex, 0x1E98);
			TStructGetPtrImpl(short, AnimationStateDuringSound, 0x1E9C);
		};

		struct s_first_person_weapons {
			s_first_person_weapon local_players[Enums::k_maximum_number_of_local_players];
		}; static_assert(sizeof(s_first_person_weapons) == 0x1EA0);

		s_first_person_weapons *FirstPersonWeapons();
	};

	namespace blam {
		void __cdecl first_person_weapons_initialize_for_new_map() {
			for (auto &player : GameUI::FirstPersonWeapons()->local_players) {
				std::memset(&player, 0, sizeof(player));
				*player.GetUnitIndex()                 = datum_index::null();
				// NOTE: added this to stop needless "local player_update %d, weapon (0x0), deleted unexpectedly" messages in debug builds
				*player.GetWeaponIndex()               = datum_index::null();
				*player.GetAnimationSoundIndex()       = datum_index::null();
				*player.GetAnimationStateDuringSound() = NONE;
			}
		}
	};
};
