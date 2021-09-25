#include "objectcontroller.h"
#include "../../RuntimeManager.h"

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
		CurrentRuntime->ConsoleText(hRed, "Unable to change distance of object further!\n");
		debounce = true;
	}
}

ObjectController::ObjectController() {
	nearest_h    = NULL;
	selected_h   = NULL;
	holding      = false;
	HoldDistance = 4.0f;
}

void ObjectController::MoveObjXY() {
	if (!CanHoldObject()) {
		return;
	}

	if (CurrentRuntime->ObjectControl->IsHoldingObject()) {
		return;
	}

	auto camera = CurrentRuntime->camera;

	float z_diff     = selected_h->address->World.z -camera->vWorld.z;
	float multiplier = abs(z_diff /camera->vLookAt.z);

	selected_h->address->World.x =camera->vWorld.x +camera->vLookAt.x * multiplier;
	selected_h->address->World.y = camera->vWorld.y + camera->vLookAt.y * multiplier;
}

void ObjectController::MoveObjFront() {
	auto camera = CurrentRuntime->camera;

	if (!CanHoldObject()) {
		CurrentRuntime->ConsoleText(hRed, "Didn't grab anything: nothing has been selected!");
		return;
	}

	if (!holding) {
		float z_diff = selected_h->address->World.z - camera->vWorld.z;
		float y_diff = selected_h->address->World.y - camera->vWorld.y;
		float x_diff = selected_h->address->World.x - camera->vWorld.x;
		HoldDistance = sqrt(z_diff * z_diff + y_diff * y_diff + x_diff * x_diff);
		holding      = true;
	}

	selected_h->address->World.x = camera->vWorld.x + camera->vLookAt.x * HoldDistance;
	selected_h->address->World.y = camera->vWorld.y + camera->vLookAt.y * HoldDistance;
	selected_h->address->World.z = camera->vWorld.z + camera->vLookAt.z * HoldDistance;
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
	CurrentRuntime->ObjectControl->selected_h = (object_header *) -1;
	CurrentRuntime->ConsoleText(hGreen, "Removed selection.");
}

void ObjectController::DropHeldObject() {
	if (!IsHoldingObject()) {
		return;
	}

	CurrentRuntime->ConsoleText(hBlue, "Got set holding = false");
	holding = false;
}

void ObjectController::UpdateHeldObject() {
	if (!IsHoldingObject()) {
		ToggleHoldiong();
		CurrentRuntime->ConsoleText(hBlue, "Got set holding = true");
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
		CurrentRuntime->ConsoleText(hRed, "Unable to update single selection.");
		return;
	}
	CurrentRuntime->ConsoleText(hGreen, "Added single selection.");
	selected_h = nearest_h;
}

void ObjectController::LogInfo() {
	if (!selected_h || !selected_h->address) {
		return;
	}

	CurrentRuntime->ConsoleText(hBlue, "Selected object name: %s", CurrentRuntime->GetObjectName(selected_h->address));
	CurrentRuntime->MyCamera->ScreenPos(selected_h->address->World, true);
}
