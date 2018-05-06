/*
	Project: haloforge
	File: math.h
	Copyright � 2009 SilentK, Abyll
	Copyright � 2018 Dwood

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

#pragma once

#include <windows.h>
#include <d3dx9math.h>
#include "../ce_base_types.h"
#include "../haloforge/headers/addlog.h"

//TODO: Investigate why this is the way it is.
struct argb_color;

// 0 - 255, represents a color
struct rgb_color {
	byte red;
	byte green;
	byte blue;
};
STAT_ASSRT(rgb_color, 0x3);

// 0 - 255, represents a color
struct argb_color {
	byte      alpha;
	rgb_color rgb;
};
STAT_ASSRT(argb_color, 0x4);

// represents a color, range: 0 - 1
struct real_rgb_color {
	float red;
	float green;
	float blue;
};
STAT_ASSRT(real_rgb_color, 0xC);

// represents a color, range: 0 - 1
struct real_argb_color {
	float          alpha;
	real_rgb_color rgb;
};
STAT_ASSRT(real_argb_color, 0x10);

//#define vect float
struct vect2 : public D3DXVECTOR2 {
	using D3DXVECTOR2::D3DXVECTOR2;

	void DumpData(bool toConsole, const char *Preceding = nullptr) {
		if (Preceding) {
			Print(toConsole, "%s\tx: %.3f, y: %.3f\n", Preceding, this->x, this->y);
			return;
		}

		Print(toConsole, "\tx: %.3f, y: %.3f\n", this->x, this->y);
	}
};

STAT_ASSRT(vect2, sizeof(D3DXVECTOR2));

// #define vect3 D3DXVECTOR3

struct vect3 : public D3DXVECTOR3 {
	using D3DXVECTOR3::D3DXVECTOR3;
	// vect3() : D3DXVECTOR3() {}
	//
	// vect3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}

	void DumpData(bool toConsole, const char *Preceding = nullptr) {
		if (Preceding) {
			Print(toConsole, "%s\tx: %.3f, y: %.3f z: %.3f\n", Preceding, this->x, this->y, this->z);
			return;
		}

		Print(toConsole, "\tx: %.3f, y: %.3f z: %.3f\n", this->x, this->y, this->z);
	}

};

STAT_ASSRT(vect3, sizeof(D3DXVECTOR3));

//-------------------------
struct HaloColor {
	float a;
	float r;
	float g;
	float b;

	HaloColor(float a, float r, float g, float b);
};

#define hGreen HaloColor(1,0,1,0)
#define hBlue HaloColor(1,0,0,1)
#define hRed HaloColor(1,1,0,0)

class CMath {
public:

	float Get2dDistance(float x, float y);

	float GetAngleDistance(vect3 local, vect3 toTarget);

	vect2 GetAngleToTarget(vect3 local, vect3 target);
};
