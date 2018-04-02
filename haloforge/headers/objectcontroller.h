/*
	Project: haloforge
	File: objectcontroller.h
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

class ObjectController;
#ifndef OBJECTCONTROLLER_H
#define OBJECTCONTROLLER_H

#include "core.h"
#include "objects.h"

class ObjectController
{
public:
	explicit ObjectController(Core* core);

	Core* core;

	object_header* nearest_h;
	object_header* selected_h;

	void MoveObjXY();
	void MoveObjFront();
	bool holding; // a check to see if we grabbed an object (rightclick - hold in front), so that the length is locked.
	float HoldDistance;

	void LogInfo();
};

#endif
