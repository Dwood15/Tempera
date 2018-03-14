/*
	Project: haloforge
	File: camera.cpp
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

#include <windows.h>
#include "headers/camera.h"

float g_Aspect = 4.0f / 3.0f;

CMyCamera::CMyCamera() {
	Camera = (_camera *) CAMERA_ADDRESS;
}

CMyCamera::~CMyCamera() {
}

_camera *CMyCamera::GetCameraStaticAddress() {
	return (_camera *) CAMERA_ADDRESS;
}

void CMyCamera::CreateViewMatrix(vect3 Eye, vect3 LookAt, vect3 Up) {
	vEye = Eye;
	vLookAt = LookAt;
	vUp = Up;
	D3DXMatrixLookAtLH(&matView, &vEye, &vLookAt, &vUp);
}

void CMyCamera::CreateProjectionMatrix(float Fov, float Aspect, float Near, float Far) {
	D3DXMatrixPerspectiveFovLH(&matProj, Fov, Aspect, Near, Far);
}

void CMyCamera::CreateWorldMatrix(vect3 World) {
	D3DXMatrixTranslation(&matWorld, World.x, World.y, World.z);
}

float CMyCamera::GetAspectRatio() {
	return g_Aspect;
}

vect3 CMyCamera::ScreenPos(vect3 coord, bool log) {
	_camera *pCamera = GetCameraStaticAddress();
	if (log)
		DEBUG("\nCam Look: %f, %f, %f\nObj coord: %f, %f, %f\nCam coord: %f, %f, %f", pCamera->vLookAt.x, pCamera->vLookAt.y, pCamera->vLookAt.z, coord.x, coord.y, coord.z, pCamera->vWorld.x, pCamera->vWorld.y, pCamera->vWorld.z);

	//PCamera->vWorld;
	//PCamera->vLookAt;
	//PCamera->Fov;
	float Y_fov = pCamera->Fov / GetAspectRatio();

	vect3 cam_to_obj(coord.x - pCamera->vWorld.x, coord.y - pCamera->vWorld.y, coord.z - pCamera->vWorld.z);
	if (log)
		DEBUG("Relative vect: %f, %f, %f\n", cam_to_obj.x, cam_to_obj.y, cam_to_obj.z);

	float dist_to_obj = sqrt(cam_to_obj.x * cam_to_obj.x + cam_to_obj.y * cam_to_obj.y + cam_to_obj.z * cam_to_obj.z);

	D3DXVec3Normalize(&cam_to_obj, &cam_to_obj);

	// All angles are in radians, -PI<angle<PI, and no i dont know which could be equal to. but who cares - it's a float so it's impossible.
	float cam_yaw = atan2f(pCamera->vLookAt.y, pCamera->vLookAt.x);
	float obj_yaw = atan2f(cam_to_obj.y, cam_to_obj.x);

	// Relative(to cam) yaw ends up from -2 PI < yaw < 2 PI, but we want it from -PI<yaw<PI
	float relative_yaw = obj_yaw - cam_yaw;
	if (relative_yaw > D3DX_PI) // yaw>180 degrees. convert to negative, smaller.
		relative_yaw -= 2 * D3DX_PI;
	if (relative_yaw < -D3DX_PI)
		relative_yaw += 2 * D3DX_PI;
	// [/Difference]

	float obj_pitch = asin(cam_to_obj.z);
	float cam_pitch = asin(pCamera->vLookAt.z);

	float relative_pitch = cam_pitch - obj_pitch;

	if (log) {
		DEBUG("Cam yaw/pitch: %f,  %f\n", cam_yaw, cam_pitch);
		DEBUG("Obj yaw/pitch: %f,  %f\n", obj_yaw, obj_pitch);
		DEBUG("Rel yaw/pitch: %f,  %f\n", relative_yaw, relative_pitch);
	}

	float x_pos = -relative_yaw * 2 / pCamera->Fov; // radian angle measurement cancels here.

	float y_pos = relative_pitch * 2 / Y_fov; // and that's the (relative pitch) / (fov / 2)

	x_pos = (x_pos + 1) / 2; // Lastly, change from range (-1,1) to (0,1)  Also, it CAN be outside of that range - if it's outside of the FOV.
	y_pos = (y_pos + 1) / 2;

	vect3 onscreen(x_pos, y_pos, dist_to_obj);
	return onscreen;
}
