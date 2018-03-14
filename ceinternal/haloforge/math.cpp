/*
	Project: haloforge
	File: math.cpp
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

#include <math.h>
#include "headers/math.h"

HaloColor::HaloColor(float alpha, float red, float green, float blue): a(alpha), r(red), g(green), b(blue)
{
}

float CMath::Get2dDistance(float x, float y)
{
	return (float)sqrt((x * x) + (y * y));
}

float CMath::GetAngleDistance(vect3 local, vect3 toTarget)
{
	return (float)(60.0f * acos(sin(local.y) * sin(toTarget.y) + cos(local.y) * cos(toTarget.y) * cos(toTarget.x - local.x)));
}

vect2 CMath::GetAngleToTarget(vect3 local, vect3 target)
{
	vect2 rot;
	vect3 dist;
	// Get distance from target
	dist = target - local;
	// Get X angle
	rot.x = atanf( dist.y / dist.x );
	// Get Y angle
	rot.y = atan2f( dist.z, Get2dDistance( dist.x, dist.y ) );

	// negative angle?
	if( rot.x < 0.0f )
		rot.x *= -1.0f;

	// Some adjustments
    if ( rot.x < 0.0f && rot.y >= 0.0f )
		rot.x = D3DX_PI - rot.x;

    if ( rot.x < 0.0f && rot.y < 0.0f )
		rot.x = D3DX_PI + rot.x;

    if ( rot.x > 0.0f && rot.y < 0.0f )
		rot.x = 2.0f * D3DX_PI - rot.x;

	return rot;
}
