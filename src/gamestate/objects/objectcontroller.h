#pragma once
#include "../objects.h"

class ObjectController {
	float StepSize     = 0.05f;
	float MinDistance  = 0.25f;
	float MaxDistance  = 5.5f;
	float HoldDistance = 3.0f;

	bool debounce = false;
	bool holding;

	object_header *nearest_h;

	object_header *selected_h;

	void SetHoldDistance(float dist);

public:
	ObjectController();

	void MoveObjXY();

	void MoveObjFront();

	void IncreaseHoldDistance(unsigned int numSteps = 1);

	void DecreaseHoldDistance(unsigned int numSteps = 1);

	bool IsNearest(object_header *objh);

	void SetNearest(object_header *objh);

	bool IsSelected(object_header *objh);

	bool IsHoldingObject();

	bool CanHoldObject();

	void DropHeldObject();

	bool HasNearest();

	bool NearestIsValid();

	bool CanSelectObject();

	void RemoveSelection();

	void SetSelectedObject();

	void UpdateSelection();

	// a check to see if we grabbed an object (r_click= hold in front) locks the length.
	void ToggleHoldiong();

	bool SelectionIsValid();

	bool HasSelection();

	void UpdateHeldObject();

	void LogInfo();
};
