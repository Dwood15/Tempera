/*
	Project: haloforge
	File: main.cpp
	Copyright � 2009 SilentK, Abyll

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

int __stdcall hkMain() {
	// new: Global access to the Server control class.
	Core *core = new Core();
	SetCore(core);
	CD3D cd3d;
	cd3d.hkD3DHook(NULL);

	while (1) {
		// Modified to toggle the builtin flycam, also loading your last camera's location.

		if (GetAsyncKeyState(VK_F1) & 1) {
			core->ToggleFlycam();
		}
		// Object control: for now, just F2 to lock onto an object.
		if (GetAsyncKeyState(VK_SHIFT) & 1) {    // for choosing the 'selected' object: aka the one closest to where we're aiming. Nearest is already set from the D3Dhook
			core->ConsoleText(hRed, "Setting selected!");
			continue;
			core->ObjectControl->selected_h = core->ObjectControl->nearest_h;
		}
		if (GetAsyncKeyState(VK_F5) & 1) {
			core->ObjectControl->LogInfo();
		}
		if (GetAsyncKeyState(VK_F4) & 1) {
			core->ConsoleCMD((char *) "debug_camera_save");
		}
		if (!core->ObjectControl->holding && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) { // XY plane
			//left mouse button
			continue;
			core->ObjectControl->MoveObjXY();

		} else if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {    // STRAIGHT in front of camera plane.
			continue;
			//Right mouse button
			//server->ObjectControl->holding = true;
//			core->ConsoleText(hBlue, "Got set holding = true");
//			core->ObjectControl->MoveObjFront();
		} else if (!(GetAsyncKeyState(VK_RBUTTON))) {
			if (core->ObjectControl->holding) {
//				core->ConsoleText(hBlue, "Got set holding = false");
//				core->ObjectControl->holding = false;
			}
		}
		if (!(GetAsyncKeyState(VK_RBUTTON))) {
			if (GetAsyncKeyState(VK_F6) & 1) { // Change the object's sector
				if (core->ObjectControl->selected_h == NULL) {
					core->ConsoleText(hBlue, "Could not select an object; Player sector: %d", core->GetPlayer(0)->Sector);
				} else {
					core->ConsoleText(hBlue, "Obj sector: %d, Player sector: %d", core->ObjectControl->selected_h->sector, core->GetPlayer(0)->Sector);
					core->ObjectControl->selected_h->sector = (short) core->GetPlayer((short) 0)->Sector;
				}
				//DEBUG("player sector: %d", core->GetPlayer(0)->Sector);
			}
			// Now then, object MOVE away/towards.
			if (GetAsyncKeyState(VK_UP)) {
				core->ObjectControl->HoldDistance += 0.03f;
			} else if (GetAsyncKeyState(VK_DOWN)) {
				core->ObjectControl->HoldDistance -= 0.03f;
			}
		}
//		MSG msg;
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

		Sleep(90);
	}
}
