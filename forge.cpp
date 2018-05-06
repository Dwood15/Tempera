/*
	Project: haloforge
	File: main.cpp
	Copyright � 2009 SilentK, Abyll
 	Copyright 	 2018 Dwood

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.
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

static Core *core;
static CD3D cd3d;

static void updateGlobals() {
	if (last_respawn_count != *to_respawn_count) {
		DEBUG("Number of people to respawn from: %d to: %d\n", last_respawn_count, *to_respawn_count);
		last_respawn_count = *to_respawn_count;
	}

	if (last_spawn_count != *spawn_count) {
		DEBUG("Number of people to respawn from: %d to: %d\n", last_spawn_count, *spawn_count);
		last_spawn_count = *spawn_count;
	}

	if (last_render_window_count != *render_window_count) {
		DEBUG("Updated number players to spawn from: %d to: %d\n", last_render_window_count, *render_window_count);
		last_render_window_count = *render_window_count;
	}
}

int __stdcall forgeMain() {
	core = new Core();
	SetCore(core);
	cd3d.hkD3DHook(NULL);

	Sleep(120);
	PrintHelp();

	while (1) {
		Sleep(30);
		updateGlobals();

		if (*at_main_menu && (GetAsyncKeyState(VK_F1) & 1)) {
			*(short *) 0x624A9C = (short) 0x2;
			core->ConsoleText(hGreen, "Number players to spawn in next sp map: 2!");
		}

		if (GetAsyncKeyState(VK_F2) & 1) {
			players_global_data->DumpData(!*at_main_menu, core);
		} else if (GetAsyncKeyState(VK_F11) & 1) {
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

		if (GetAsyncKeyState(VK_F7) & 1) {
			core->TryLogPlayer(0, true);
			core->TryLogPlayer(1, true);

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
	}
}

//		TODO: Scrolling to set selected item's distance from player.
// 	MSG msg;
//		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
//		{
//			if(msg.message == WM_MOUSEWHEEL)
//			{
//				short zDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
//				/*
//				The return value is the high-order word representing the wheel-delta value.
//				It indicates the distance that the wheel is rotated, expressed in multiples
//				or divisions of WHEEL_DELTA, which is 120. A positive value indicates that
//				the wheel was rotated forward, away from the user; a negative value indicates
//				that the wheel was rotated backward, toward the user.
//				*/
//				core->ObjectControl->HoldDistance += zDelta;
//				DEBUG("dist: %d", zDelta);
//			}
//			else
//			{
//				DEBUG("other: %d", msg.message);
//			}
//		}

