#include <math.h>
#include "colors.h"
#include "real_math.h"

//HaloColor::HaloColor constexpr(float alpha, float red, float green, float blue) : a(alpha), r(red), g(green), b(blue) { }
HaloColor::HaloColor(float alpha, float red, float green, float blue) : a(alpha), r(red), g(green), b(blue) {
}

float CMath::Get2dDistance(float x, float y) {
	return (float) sqrt((x * x) + (y * y));
}

float CMath::GetAngleDistance(vect3 local, vect3 toTarget) {
	return (float) (60.0f * acos(sin(local.y) * sin(toTarget.y) + cos(local.y) * cos(toTarget.y) * cos(toTarget.x - local.x)));
}

vect2 CMath::GetAngleToTarget(vect3 local, vect3 target) {
	vect2 rot;
	vect3 dist;
	// Get distance from target
	dist = target - local;
	// Get X angle
	rot.x = atanf(dist.y / dist.x);
	// Get Y angle
	rot.y = atan2f(dist.z, Get2dDistance(dist.x, dist.y));

	// negative angle?
	if (rot.x < 0.0f)
		rot.x *= -1.0f;

	// Some adjustments
	if (rot.x < 0.0f && rot.y >= 0.0f)
		rot.x = D3DX_PI - rot.x;

	if (rot.x < 0.0f && rot.y < 0.0f)
		rot.x = D3DX_PI + rot.x;

	if (rot.x > 0.0f && rot.y < 0.0f)
		rot.x = 2.0f * D3DX_PI - rot.x;

	return rot;
}
