/*
	Project: haloforge
	File: objectcontroller.cpp
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

#include "headers/objectcontroller.h"

ObjectController::ObjectController(Core*newcore) : core(newcore) {
	nearest_h    = NULL;
	selected_h   = NULL;
	holding      = false;
	HoldDistance = 5.0f;
}

void ObjectController::MoveObjXY() {
	if( selected_h == NULL ) {
		return;
	}
	if( selected_h->address == NULL ) {
		return;
	}

	float z_diff     = selected_h->address->World.z - core->camera->vWorld.z;
	float multiplier = z_diff / core->camera->vLookAt.z;
	if( multiplier < 0 ) {
		multiplier *= -1;
	}

	selected_h->address->World.x = core->camera->vWorld.x + core->camera->vLookAt.x * multiplier;
	selected_h->address->World.y = core->camera->vWorld.y + core->camera->vLookAt.y * multiplier;
}

void ObjectController::MoveObjFront() {
	if( selected_h == NULL ) {
		core->ConsoleText(hRed, "Didn't grab anything: nothing has been selected!");
		return;
	} else if( selected_h->address == NULL ) {
		core->ConsoleText(hRed, "Didn't grab anything: last selected object's address is null");
		return;
	}

	if( !holding ) {
		float z_diff = selected_h->address->World.z - core->camera->vWorld.z;
		float y_diff = selected_h->address->World.y - core->camera->vWorld.y;
		float x_diff = selected_h->address->World.x - core->camera->vWorld.x;
		HoldDistance = sqrt(z_diff * z_diff + y_diff * y_diff + x_diff * x_diff);
		holding      = true;
	}

	selected_h->address->World.x = core->camera->vWorld.x + core->camera->vLookAt.x * HoldDistance;
	selected_h->address->World.y = core->camera->vWorld.y + core->camera->vLookAt.y * HoldDistance;
	selected_h->address->World.z = core->camera->vWorld.z + core->camera->vLookAt.z * HoldDistance;
}

void ObjectController::LogInfo() {
	if( selected_h == NULL || selected_h->address == NULL ) {
		return;
	}
	core->ConsoleText(hBlue, "Selected object name: %s", core->GetObjectName(selected_h->address));
	core->MyCamera->ScreenPos(selected_h->address->World, true);
}
