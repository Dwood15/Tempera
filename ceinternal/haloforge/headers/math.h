/*
	Project: haloforge
	File: math.h
	Copyright © 2009 SilentK, Abyll

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

#ifndef MATH_H
#define MATH_H
//-------------------------
#include <windows.h>
#include <d3dx9math.h>
//-------------------------
//#define vect float
#define vect2 D3DXVECTOR2
#define vect3 D3DXVECTOR3
//-------------------------
struct HaloColor
{
	float a;
	float r;
	float g;
	float b;
	HaloColor(float a, float r, float g, float b);
};
//-------------------------
#define hGreen HaloColor(1,0,1,0)
#define hBlue HaloColor(1,0,0,1)
#define hRed HaloColor(1,1,0,0)
//-------------------------
class CMath
{
public:

	float Get2dDistance(float x, float y);
	float GetAngleDistance(vect3 local, vect3 toTarget);
	vect2 GetAngleToTarget(vect3 local, vect3 target);
};
//-------------------------
#endif /* MATH_H */