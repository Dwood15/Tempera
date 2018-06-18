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

#include "objectcontroller.h"

void ObjectController::SetHoldDistance(float dist) {
	HoldDistance = dist;

	//I don't like this way of doing it, but not sure how else I could do it...
	if (MaxDistance > HoldDistance && HoldDistance > MinDistance) {
		debounce = false;
		return;
	}

	if (HoldDistance > MaxDistance) {
		HoldDistance = MaxDistance;
	} else if (HoldDistance < MinDistance) {
		HoldDistance = MinDistance;
	}

	if (!debounce) {
		core->ConsoleText(hRed, "Unable to change distance of object further!\n");
		debounce = true;
	}
}

ObjectController::ObjectController(Core *newcore) : core(newcore) {
	nearest_h    = NULL;
	selected_h   = NULL;
	holding      = false;
	HoldDistance = 4.0f;
}

void ObjectController::MoveObjXY() {
	if (!CanHoldObject()) {
		return;
	}

	if (core->ObjectControl->IsHoldingObject()) {
		return;
	}

	float z_diff     = selected_h->address->World.z - core->camera->vWorld.z;
	float multiplier = abs(z_diff / core->camera->vLookAt.z);

	selected_h->address->World.x = core->camera->vWorld.x + core->camera->vLookAt.x * multiplier;
	selected_h->address->World.y = core->camera->vWorld.y + core->camera->vLookAt.y * multiplier;
}

void ObjectController::MoveObjFront() {
	if (!CanHoldObject()) {
		core->ConsoleText(hRed, "Didn't grab anything: nothing has been selected!");
		return;
	}

	if (!holding) {
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

void ObjectController::IncreaseHoldDistance(unsigned int numSteps) {
	SetHoldDistance(HoldDistance + (StepSize * numSteps));
}

void ObjectController::DecreaseHoldDistance(unsigned int numSteps) {
	SetHoldDistance(HoldDistance - (StepSize * numSteps));
}

bool ObjectController::IsHoldingObject() {
	return holding;
}

bool ObjectController::HasSelection() {
	return selected_h != NULL && (int) selected_h != -1;
}

bool ObjectController::SelectionIsValid() {
	return selected_h->address != NULL && (int) selected_h->address != -1;
}

bool ObjectController::CanHoldObject() {
	return (HasSelection() && SelectionIsValid());
}

bool ObjectController::HasNearest() {
	return ( nearest_h != NULL && (int) nearest_h != -1);
}

bool ObjectController::NearestIsValid() {
	return ( nearest_h->address != NULL && (int) nearest_h->address != -1);
}

bool ObjectController::CanSelectObject() {
	return (HasNearest() && NearestIsValid());
}

void ObjectController::RemoveSelection() {
	core->ObjectControl->selected_h = (object_header *) -1;
	core->ConsoleText(hGreen, "Removed selection.");
}

void ObjectController::DropHeldObject() {
	if (!IsHoldingObject()) {
		return;
	}

	core->ConsoleText(hBlue, "Got set holding = false");
	holding = false;
}

void ObjectController::UpdateHeldObject() {
	if (!IsHoldingObject()) {
		ToggleHoldiong();
		core->ConsoleText(hBlue, "Got set holding = true");
	}

	MoveObjFront();
}

void ObjectController::ToggleHoldiong() {
	holding = !holding;
}

void ObjectController::UpdateSelection() {
	// For choosing the 'selected' object: aka the one closest to where we're aiming.
	// Nearest is already set from the D3Dhook
	if (HasSelection()) {
		RemoveSelection();
	}

	SetSelectedObject();
}

void ObjectController::SetNearest(object_header *objh) {
	if (objh != NULL && (int) objh != -1) {
		nearest_h = objh;
	}
}

bool ObjectController::IsNearest(object_header *objh) {
	return objh == nearest_h;
}

bool ObjectController::IsSelected(object_header *objh) {
	return objh == selected_h;
}

void ObjectController::SetSelectedObject() {
	if (!CanSelectObject()) {
		core->ConsoleText(hRed, "Unable to update single selection.");
		return;
	}
	core->ConsoleText(hGreen, "Added single selection.");
	selected_h = nearest_h;
}

void ObjectController::LogInfo() {
	if (selected_h == NULL || (int) selected_h->address == NULL) {
		return;
	}
	core->ConsoleText(hBlue, "Selected object name: %s", core->GetObjectName(selected_h->address));
	core->MyCamera->ScreenPos(selected_h->address->World, true);
}
