/**
 *	Project: haloforge
 *	File: camera.h
 *	Copyright � 2009 SilentK, Abyll
 *	Copyright � 2018 Dwood
 *
 *	This file is part of haloforge.
 *
 *  haloforge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  haloforge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with haloforge. If not, see <http://www.gnu.org/licenses/>.
 **/

#pragma once

#include <windows.h>
#include "math.h"
#include "addlog.h"
#include "../../headers/colors.h"

#define CAMERA_ADDRESS 0x6474E4
//0x006AC4DC;

#define FOV_XBOUND 0.024f
#define FOV_YBOUND 0.012f
#define OBJECT_CLIP 17

struct _camera {
	WORD  Unknown1;
	short wPerspective; // First Person( 0 ), Third Person( 1 ), Dead( 3 )
	DWORD Unknown2;

	vect3 vWorld; // x,y,z ( game units )
	vect3 fShift; // x,y,z ( forward/back,side,vertical )( game units )
	float fDepth;
	float Fov;      // in radians ( default = 70 degrees )
	vect3 vLookAt;
	vect3 fUp;
	vect3 fVelocity;

	DWORD Unknown3[26];

	vect3 fWorld2;
	vect3 fShift2; // x,y,z ( forward/back,side,vertical )( game units )
	float fDepth2;
	float Fov2;      // in radians ( default = 70 degrees )
	vect3 fForward2;
	vect3 fUp2;
	vect3 fVelocity2;

	DWORD Unknown4[9];
	vect3 fWorld3;
	DWORD Unknown5[5];
	vect3 fForward3;
	vect3 fUp3;
	float fFov3;

	vect3 fWorld4;
	DWORD Unknown6[4];
	float fFov4;
	vect3 fForward4;
	vect3 fUp4;
};

class CMyCamera {
public:
	CMyCamera();

	~CMyCamera();

	_camera *GetCameraStaticAddress();

	D3DXVECTOR3 vEye;
	D3DXVECTOR3 vLookAt;
	D3DXVECTOR3 vUp;

	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	D3DXMATRIX matWorld;

	D3DXVECTOR3 vScreen;
	D3DXVECTOR3 vWorld;

	float aspect;
	_camera *Camera;

	void CreateViewMatrix(vect3 fEye, vect3 vLookAt, vect3 fUp);

	void CreateProjectionMatrix(float fFov, float fAspect, float fNear, float fFar);

	void CreateWorldMatrix(vect3 fWorld);

	float GetAspectRatio();

	vect3 ScreenPos(vect3 object_world, bool log = false);
};
