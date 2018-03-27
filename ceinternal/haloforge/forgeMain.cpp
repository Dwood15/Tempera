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

#include <windows.h>
#include <stdio.h>

#include "headers/forgeMain.h"
#include "headers/math.h"
#include "headers/camera.h"
#include "headers/addlog.h"
#include "headers/core.h"
#include "Direct3D/d3d9hook.h"
#include "../shitty_header_ports.h"

static short last_respawn_count = 0x0;

typedef int retIntGivenVoid(void);

static Core *core;
static CD3D cd3d;

static void dumpPlayerGlobalsData() {
	retIntGivenVoid              *find_unused_local_player_index = (retIntGivenVoid*)0x4762f0;
	int                          unused_plyr_idx                 = find_unused_local_player_index();
	static s_players_globals_data*players_global_data            = *(s_players_globals_data**)0x815918;

	core->ConsoleText(hGreen, "Unused Player Index: %d", unused_plyr_idx);

	printf("unused_after_initialize_unk %d\n", players_global_data->unused_after_initialize_unk);
	printf("local_player_count: %d\n", players_global_data->local_player_count);
	printf("double speed tix remaining: %d\n", players_global_data->double_speed_ticks_remaining);
	printf("Are all dead?: %d\n", players_global_data->are_all_dead);
	printf("Input disabled: %d\n", players_global_data->input_disabled);
	printf("BSP switch trigger: %d\n", players_global_data->_bsp_switch_trigger_idx);

	DEBUG("unused_after_initialize_unk %d\n", players_global_data->unused_after_initialize_unk);
	DEBUG("local_player_count: %d\n", players_global_data->local_player_count);
	DEBUG("double speed tix remaining: %d\n", players_global_data->double_speed_ticks_remaining);
	DEBUG("Are all dead?: %d\n", players_global_data->are_all_dead);
	DEBUG("Input disabled: %d\n", players_global_data->input_disabled);
	DEBUG("BSP switch trigger: %d\n", players_global_data->_bsp_switch_trigger_idx);
}

int __stdcall hkMain() {
	core = new Core();
	SetCore(core);
	cd3d.hkD3DHook(NULL);

	while(1) {
		short to_respawn_count = *(short*)0x6B4802;
		if( last_respawn_count != to_respawn_count ) {
			DEBUG("Updating to_respawn: %d\n");
		}

		Sleep(45);
		continue;

		if( GetAsyncKeyState(VK_F1) & 1 ) {
			dumpPlayerGlobalsData();

		} else if( GetAsyncKeyState(VK_F5) & 1 ) {

			core->ObjectControl->LogInfo();

		} else if( GetAsyncKeyState(VK_F4) & 1 ) {

			core->ConsoleCMD((char*)"debug_camera_save");

		} else if( GetAsyncKeyState(VK_F6) & 1 ) { // Change the object's sector

			if( core->ObjectControl->selected_h == NULL || (int)core->ObjectControl->selected_h == -1 ) {

				core->ConsoleText(hBlue, "Could not select an object; Player sector: %d\n\tSend live_projekt log to Dwood + relevant details.", core->GetPlayer(0)->Sector);
				DEBUG("player sector: %d", core->GetPlayer(0)->Sector);

			} else {

				core->ConsoleText(hBlue, "Obj sector: %d, Player sector: %d", core->ObjectControl->selected_h->sector, core->GetPlayer(0)->Sector);
				core->ObjectControl->selected_h->sector = (short)core->GetPlayer((short)0)->Sector;

			}
		} else

		if( GetAsyncKeyState(VK_UP)) {
			// Object MOVE away.
			core->ObjectControl->HoldDistance += 0.03f;

		} else if( GetAsyncKeyState(VK_DOWN)) {
			// Object MOVE closer
			if( core->ObjectControl->HoldDistance > 0.0f) {
				core->ObjectControl->HoldDistance -= 0.03f;
			}

		} else if( GetAsyncKeyState(VK_SHIFT) & 1 ) {

			// Object control: for now, SHIFT to lock onto an object.
			// for choosing the 'selected' object: aka the one closest to where we're aiming. Nearest is already set from the D3Dhook
			if((int)core->ObjectControl->selected_h != -1 ) {

				core->ConsoleText(hGreen, "Removing selection! - Press Shift again to reselect.");
				core->ObjectControl->selected_h = (object_header*)-1;

			} else {

				core->ConsoleText(hGreen, "Setting selected! - Press Shift again to deselect.");
				core->ObjectControl->selected_h = core->ObjectControl->nearest_h;

			}
		} else if( !core->ObjectControl->holding && ( GetAsyncKeyState(VK_LBUTTON) & 0x8000 )) { // XY plane
			//left mouse button
			if((int)core->ObjectControl->selected_h != -1 ) {

				core->ObjectControl->MoveObjXY();

			}
			continue;
		} else if(( GetAsyncKeyState(VK_RBUTTON) & 0x8000 )) {    // STRAIGHT in front of camera plane.
			//Right mouse button
			if( !core->ObjectControl->holding ) {

				core->ObjectControl->holding = true;
				core->ConsoleText(hBlue, "Got set holding = true");

			}

			core->ObjectControl->MoveObjFront();
			continue;

		} else if( !( GetAsyncKeyState(VK_RBUTTON)) && core->ObjectControl->holding ) {
			core->ConsoleText(hBlue, "Got set holding = false");
			core->ObjectControl->holding = false;
		}


//		TODO: Get this working properly.
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
	}
}
