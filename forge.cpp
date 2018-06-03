/*
	Project: tempera
	File: forge.cpp
	Copyright � 2009 SilentK, Abyll
 	Copyright 	 2018 Dwood

	This file is part of tempera.

   You should have received a copy of the GNU General Public License
   along with tempera.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Windows.h>
#include <stdio.h>

#include "haloforge/headers/forge.h"
#include "haloforge/headers/camera.h"
#include "haloforge/headers/addlog.h"
#include "haloforge/headers/core.h"
#include "Direct3D/d3d9hook.h"
#include "gamestate/player_types.h"

static short last_respawn_count       = 0x0;
static short last_spawn_count         = 0x0;
static short last_render_window_count = 0x0;

//Live-updating number of players game should spawn.
static short *to_respawn_count = (short *) 0x6B4802;

//Number of players to spawn - set on map initialization/unloading.
static short *spawn_count         = (short *) 0x624A9C;
///num windows to render. Fills with black for invalid.    f
static short *render_window_count = (short *) 0x6B4098;
bool         *at_main_menu        = (bool *) 0x6B4051;

static void updateGlobals() {
	if (last_respawn_count != *to_respawn_count) {
		Print(true, "Number of people to respawn from: %d to: %d\n", last_respawn_count, *to_respawn_count);
		last_respawn_count = *to_respawn_count;
	}

	if (last_spawn_count != *spawn_count) {
		Print(true, "Number of people to respawn from: %d to: %d\n", last_spawn_count, *spawn_count);
		last_spawn_count = *spawn_count;
	}

	if (last_render_window_count != *render_window_count) {
		Print(true, "Updated number players to spawn from: %d to: %d\n", last_render_window_count, *render_window_count);
		last_render_window_count = *render_window_count;
	}
}

static void handleMainMenuOptions() {
	if(!core) {
		//Don't want to mess with shit if it's not initialized properly.
		return;
	}

	if(GetAsyncKeyState(VK_F1) & 1) {
		*(short *) 0x624A9C = (short) 0x2;
		core->ConsoleText(hGreen, "Number players to spawn in next sp map: 3!");
	}
}

static void handleSPInput() {

}

static void handleMPInput() {

}

static void handleMPSPInput() {

}

//TODO: Launch thread later on in the load cycle
int __stdcall forgeMain() {
	Sleep(600);
	//automatically spawn the maximum number of local players at the beginning of the game.
	//*(short *) 0x624A9C = (short)MAX_PLAYER_COUNT_LOCAL;

	core = new Core();
	SetCore(core);
	cd3d.hkD3DHook(NULL);
	// core->ConsoleText(hGreen, "Number players to spawn in next sp map: 3!");
	core->ConsoleText(hGreen, "Tempera initialized.");
	PrintHelp();

	while (1) {
		Sleep(20);
		updateGlobals();

		if (*at_main_menu) {
			handleMainMenuOptions();
		}

		if (GetAsyncKeyState(VK_F2) & 1) {
			// if(core) {
			//TODO: Get players_global data dynamically, ie via core
			// 	players_global_data->DumpData(true, core);
			// }
		}

		if (GetAsyncKeyState(VK_F11) & 1) {
			PrintHelp();
			continue;
		}

		if (*at_main_menu) {
			Sleep(45);
			continue;
		}

		if (GetAsyncKeyState(VK_F5) & 1) {
			core->ObjectControl->LogInfo();
		}

		if ((core->IsPlayerSpawned(1)) && GetAsyncKeyState(VK_F7) & 1) {
			ident plyr_datum = core->GetPlayerObjectIdent(1);

			if (plyr_datum.index > -1) {
				//object_header * objh = core->GetObjectHeader(plyr_datum.index);
				object_data *objd = core->GetGenericObject(plyr_datum.index);

				Print(true, "\n\t\t***~~~*** Player 2 Data Dump ***~~~***\n");
				objd->DumpData(true);
				Print(true, "\n\t\t***~~~*** END Player 2 Data Dump ***~~~***\n");

				//objd->Velocity.x += .15;
				//objd->Velocity.y  = 0;

				++objd->Animation.state.animation_index;

				Print(true, "Player 2 animation state: %d", objd->Animation.state.animation_index);
				// core->ConsoleText(hBlue, );
			}
		} else if (core->IsPlayerSpawned(0) && GetAsyncKeyState(VK_F8) & 1) {
			ident plyr_datum = core->GetPlayerObjectIdent(0);
			if (plyr_datum.index > -1) {
				//object_header * objh = core->GetObjectHeader(plyr_datum.index);
				object_data *objd = core->GetGenericObject(plyr_datum.index);

				Print(true, "\n\t***~~~*** Player 1 Data Dump ***~~~***\n");
				objd->DumpData(true);
				Print(true, "\n\t***~~~*** END Player 1 Data Dump ***~~~***\n");

				objd->Velocity.x += .15;
				objd->Velocity.y  = 0;

				core->ConsoleText(hBlue, "Increased Player 1 velocity! %.3f", objd->Velocity.x);
			}
		} else if (GetAsyncKeyState(VK_UP)) {
			core->ObjectControl->IncreaseHoldDistance();         // Object MOVE away.

		} else if (GetAsyncKeyState(VK_DOWN)) {
			core->ObjectControl->DecreaseHoldDistance();         // Object MOVE closer

		} else if (GetAsyncKeyState(VK_SHIFT) & 1) {
			// SHIFT to lock on object.
			core->ObjectControl->UpdateSelection();

		} else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { // XY plane
			//left mouse button
			core->ObjectControl->MoveObjXY();
		} else if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {    // STRAIGHT in front of camera plane.
			//Right mouse button
			core->ObjectControl->UpdateHeldObject();
		} else if (!(GetAsyncKeyState(VK_RBUTTON))) {
			core->ObjectControl->DropHeldObject();
		}

		//		TODO: Scrolling to set selected item's distance from player.
		// 	MSG msg;
		//Isn't doing anything?
		// LPMSG msg = NULL;
		// if (PeekMessage(msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE)) {
		// 	if (msg->message == WM_MOUSEWHEEL) {
		// 		short zDelta = GET_WHEEL_DELTA_WPARAM(msg->wParam);
		// 		/*
		// 		The return value is the high-order word representing the wheel-delta value.
		// 		It indicates the distance that the wheel is rotated, expressed in multiples
		// 		or divisions of WHEEL_DELTA, which is 120.
		//
		// 		A positive value == the wheel was rotated away from the user;.
		// 		A negative value indicates that the wheel was rotated toward the user.
		// 		*/
		// 		if (zDelta < 0) {
		// 			core->ObjectControl->DecreaseHoldDistance(zDelta);
		// 		} else {
		// 			core->ObjectControl->IncreaseHoldDistance(zDelta);
		// 		}
		// 		Print(false, "MouseWheel dist: %d", zDelta);
		// 	} else {
		// 		Print(true, "other: %d", msg->message);
		// 	}
		// }

	}
}


