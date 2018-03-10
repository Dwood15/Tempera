#pragma once

#include "../../math/data_types.h"

struct CameraData {
    Vector3D position;
    float unknown[5];
    Vector3D orientation[2];
    float fov;
};

CameraData &camera_data() noexcept;

void interpolate_all_cam_before() noexcept;
void interpolate_all_cam_after() noexcept;
void on_camera_change() noexcept;
void unset_camera_change() noexcept;

float zoom_scale() noexcept;
