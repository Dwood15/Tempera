/*
	Project: haloforge
	File: camera.cpp
	Copyright � 2009 SilentK, Abyll
	Copyright � 2018 Dwood

	This file is part of Tempera.
*/

#include <Windows.h>
#include "../../common/addlog.h"
#include "camera.h"

float g_Aspect = 4.0f / 3.0f;

CMyCamera::CMyCamera(_camera *cam) {
	Camera = cam;
}

CMyCamera::~CMyCamera() {
}

_camera *CMyCamera::GetCameraStaticAddress() {
	return nullptr;//CAMERA_ADDRESS;
}

void CMyCamera::CreateViewMatrix(vect3 Eye, vect3 LookAt, vect3 Up) {
	vEye    = Eye;
	vLookAt = LookAt;
	vUp     = Up;
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
	_camera *pCamera = Camera;
	if (log) {
		DEBUG("\nCam Look: %f, %f, %f\nObj coord: %f, %f, %f\nCam coord: %f, %f, %f", pCamera->vLookAt.x, pCamera->vLookAt.y, pCamera->vLookAt.z, coord.x, coord.y, coord.z, pCamera->vWorld.x,
				pCamera->vWorld.y, pCamera->vWorld.z);
	}

	//PCamera->vWorld;
	//PCamera->vLookAt;
	//PCamera->Fov;
	float Y_fov = pCamera->Fov / GetAspectRatio();

	vect3 cam_to_obj(coord.x - pCamera->vWorld.x, coord.y - pCamera->vWorld.y, coord.z - pCamera->vWorld.z);
	if (log) {
		DEBUG("Relative vect: %f, %f, %f\n", cam_to_obj.x, cam_to_obj.y, cam_to_obj.z);
	}

	D3DXVec3Normalize(&cam_to_obj, &cam_to_obj);

	// All angles are in radians, -PI<angle<PI, and no i dont know which could be equal to. but who cares - it's a float so it's impossible.
	float cam_yaw = atan2f(pCamera->vLookAt.y, pCamera->vLookAt.x);
	float obj_yaw = atan2f(cam_to_obj.y, cam_to_obj.x);

	// Relative(to cam) yaw ends up from -2 PI < yaw < 2 PI, but we want it from -PI<yaw<PI
	float relative_yaw = obj_yaw - cam_yaw;
	// yaw>180 degrees. convert to negative, smaller.
	if (relative_yaw > D3DX_PI) {
		relative_yaw -= 2 * D3DX_PI;
	}
	if (relative_yaw < -D3DX_PI) {
		relative_yaw += 2 * D3DX_PI;
	}
	// [/Difference]

	float obj_pitch = asinf(cam_to_obj.z);
	float cam_pitch = asinf(pCamera->vLookAt.z);

	float relative_pitch = cam_pitch - obj_pitch;

	float dist_to_object = sqrtf(cam_to_obj.x * cam_to_obj.x + cam_to_obj.y * cam_to_obj.y + cam_to_obj.z * cam_to_obj.z);

	if (log) {
		Print<false>( "Cam yaw/pitch: %f,  %f\n", cam_yaw, cam_pitch);
		Print<false>( "Obj yaw/pitch: %f,  %f\n", obj_yaw, obj_pitch);
		Print<false>( "Rel yaw/pitch: %f,  %f\n", relative_yaw, relative_pitch);
		Print<false>( "Distance: %f\n", dist_to_object);
	}

	float x_pos = -relative_yaw * 2 / pCamera->Fov; // radian angle measurement cancels here.

	float y_pos = relative_pitch * 2 / Y_fov; // and that's the (relative pitch) / (fov / 2)

	// Lastly, change from range (-1,1) to (0,1)  Also, it CAN be outside of that range - if it's outside of the FOV.
	x_pos = (x_pos + 1) / 2;
	y_pos = (y_pos + 1) / 2;

	return vect3(x_pos, y_pos, dist_to_object);
}
