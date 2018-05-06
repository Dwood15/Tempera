/*
	Project: haloforge
	File: objectcontroller.h
	Copyright � 2009 SilentK, Abyll
	Copyright � 2018 Dwood

	This file is part of tempera.

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

class ObjectController;

#pragma once

#include "core.h"
#include "objects.h"

class ObjectController {
	float MinDistance = 0.10f;
	float MaxDistance = 5.5f;
	float HoldDistance;

	bool debounce = false;
	bool holding; // a check to see if we grabbed an object (r_click=  hold in front) locks the length.

	object_header *nearest_h;

	object_header *selected_h;

	void SetHoldDistance(float dist);

public:
	explicit ObjectController(Core *core);

	Core *core;

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

	void ToggleHoldiong();

	bool SelectionIsValid();

	bool HasSelection();

	void UpdateHeldObject();

	void LogInfo();
};
