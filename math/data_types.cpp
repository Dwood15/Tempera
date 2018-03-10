#include <cmath>
#include "data_types.h"

interpolate_vector_fn interpolate_vector_objects = interpolate_vector;

ColorRGB::ColorRGB() noexcept {}
ColorRGB::ColorRGB(float r, float g, float b) noexcept :
    red(r),
    green(g),
    blue(b) {};
ColorRGB::ColorRGB(const ColorByte &other) noexcept :
    red(static_cast<float>(other.red) / 255),
    green(static_cast<float>(other.green) / 255),
    blue(static_cast<float>(other.blue) / 255) {};
ColorRGB::ColorRGB(const ColorARGB &other) noexcept :
    red(other.red),
    green(other.green),
    blue(other.blue) {}
ColorRGB::ColorRGB(const ColorRGB &other) noexcept :
    red(other.red),
    green(other.green),
    blue(other.blue) {}

ColorARGB::ColorARGB() noexcept {};
ColorARGB::ColorARGB(float a, float r, float g, float b) noexcept :
    alpha(a),
    red(r),
    green(g),
    blue(b) {};
ColorARGB::ColorARGB(const ColorByte &other) noexcept :
    alpha(static_cast<float>(other.alpha) / 255),
    red(static_cast<float>(other.red) / 255),
    green(static_cast<float>(other.green) / 255),
    blue(static_cast<float>(other.blue) / 255) {};
ColorARGB::ColorARGB(const ColorRGB &other) noexcept :
    red(other.red),
    green(other.green),
    blue(other.blue) {}
ColorARGB::ColorARGB(const ColorARGB &other) noexcept :
    alpha(other.alpha),
    red(other.red),
    green(other.green),
    blue(other.blue) {}

ColorByte::ColorByte() noexcept {};
ColorByte::ColorByte(float a, float r, float g, float b) noexcept :
    blue(b * 255),
    green(g * 255),
    red(r * 255),
    alpha(a * 255) {};
ColorByte::ColorByte(unsigned char a, unsigned char r, unsigned char g, unsigned char b) noexcept :
    blue(b),
    green(g),
    red(r),
    alpha(a) {};
ColorByte::ColorByte(const ColorRGB &other) noexcept :
    blue(other.blue * 255),
    green(other.green * 255),
    red(other.red * 255) {};
ColorByte::ColorByte(const ColorARGB &other) noexcept :
    blue(other.blue * 255),
    green(other.green * 255),
    red(other.red * 255),
    alpha(other.alpha * 255) {};
ColorByte::ColorByte(const ColorByte &other) noexcept :
    blue(other.blue),
    green(other.green),
    red(other.red),
    alpha(other.alpha) {}

Quaternion::Quaternion() noexcept {};

// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
Quaternion::Quaternion(const RotationMatrix &matrix) noexcept {
    float tr = matrix.v[0].x + matrix.v[1].y + matrix.v[2].z;
    if(tr > 0) {
        float S = sqrt(tr+1.0) * 2; // S=4*qw
        this->w = 0.25 * S;
        this->x = (matrix.v[2].y - matrix.v[1].z) / S;
        this->y = (matrix.v[0].z - matrix.v[2].x) / S;
        this->z = (matrix.v[1].x - matrix.v[0].y) / S;
    }
    else if((matrix.v[0].x > matrix.v[1].y) & (matrix.v[0].x > matrix.v[2].z)) {
        float S = sqrt(1.0 + matrix.v[0].x - matrix.v[1].y - matrix.v[2].z) * 2; // S=4*qx
        this->w = (matrix.v[2].y - matrix.v[1].z) / S;
        this->x = 0.25 * S;
        this->y = (matrix.v[0].y + matrix.v[1].x) / S;
        this->z = (matrix.v[0].z + matrix.v[2].x) / S;
    } else if(matrix.v[1].y > matrix.v[2].z) {
        float S = sqrt(1.0 + matrix.v[1].y - matrix.v[0].x - matrix.v[2].z) * 2; // S=4*qy
        this->w = (matrix.v[0].z - matrix.v[2].x) / S;
        this->x = (matrix.v[0].y + matrix.v[1].x) / S;
        this->y = 0.25 * S;
        this->z = (matrix.v[1].z + matrix.v[2].y) / S;
    } else {
        float S = sqrt(1.0 + matrix.v[2].z - matrix.v[0].x - matrix.v[1].y) * 2; // S=4*qz
        this->w = (matrix.v[1].x - matrix.v[0].y) / S;
        this->x = (matrix.v[0].z + matrix.v[2].x) / S;
        this->y = (matrix.v[1].z + matrix.v[2].y) / S;
        this->z = 0.25 * S;
    }
}

Quaternion::Quaternion(const Quaternion& copy) noexcept : x(copy.x), y(copy.y), z(copy.z), w(copy.w) {}

RotationMatrix::RotationMatrix() noexcept {}

// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
RotationMatrix::RotationMatrix(const Quaternion &quaternion) noexcept {
    float sqw = quaternion.w*quaternion.w;
    float sqx = quaternion.x*quaternion.x;
    float sqy = quaternion.y*quaternion.y;
    float sqz = quaternion.z*quaternion.z;

    // invs (inverse square length) is only required if quaternion is not already normalised
    float invs = 1 / (sqx + sqy + sqz + sqw);
    this->v[0].x = ( sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
    this->v[1].y = (-sqx + sqy - sqz + sqw)*invs;
    this->v[2].z = (-sqx - sqy + sqz + sqw)*invs;

    float tmp1 = quaternion.x*quaternion.y;
    float tmp2 = quaternion.z*quaternion.w;
    this->v[1].x = 2.0 * (tmp1 + tmp2)*invs;
    this->v[0].y = 2.0 * (tmp1 - tmp2)*invs;

    tmp1 = quaternion.x*quaternion.z;
    tmp2 = quaternion.y*quaternion.w;
    this->v[2].x = 2.0 * (tmp1 - tmp2)*invs;
    this->v[0].z = 2.0 * (tmp1 + tmp2)*invs;
    tmp1 = quaternion.y*quaternion.z;
    tmp2 = quaternion.x*quaternion.w;
    this->v[2].y = 2.0 * (tmp1 + tmp2)*invs;
    this->v[1].z = 2.0 * (tmp1 - tmp2)*invs;
}

RotationMatrix::RotationMatrix(const RotationMatrix &copy) noexcept {
    this->v[0] = copy.v[0];
    this->v[1] = copy.v[1];
    this->v[2] = copy.v[2];
}

// special thanks to MosesOfEgypt for the rotation interpolation stuff here
void interpolate_quat(const Quaternion &in_before, const Quaternion &in_after, Quaternion &out, float scale) noexcept {
    auto &w1 = in_before.w;
    auto &x1 = in_before.x;
    auto &y1 = in_before.y;
    auto &z1 = in_before.z;
    auto w0 = in_after.w;
    auto x0 = in_after.x;
    auto y0 = in_after.y;
    auto z0 = in_after.z;
    float cos_half_theta = w0*w1 + x0*x1 + y0*y1 + z0*z1;
    if(cos_half_theta < 0) {
        w0*=-1;
        x0*=-1;
        y0*=-1;
        z0*=-1;
        cos_half_theta *= -1;
    }
    if(cos_half_theta < 0.01) return;

    float half_theta;
    if(fabs(cos_half_theta) >= 1.0) {
        half_theta = 0.0;
    }
    else {
        half_theta = acos(cos_half_theta);
    }

    float sin_half_theta = 0;
    float m = (1 - cos_half_theta*cos_half_theta);
    if(m > 0) sin_half_theta = m;

    float r0 = 1 - scale;
    float r1 = scale;
    if(sin_half_theta > 0.00001) {
        r0 = sin((1 - scale) * half_theta) / sin_half_theta;
        r1 = sin(scale * half_theta) / sin_half_theta;
    }

    out.w = w0*r1 + w1*r0;
    out.x = x0*r1 + x1*r0;
    out.y = y0*r1 + y1*r0;
    out.z = z0*r1 + z1*r0;
}

void interpolate_vector_rotation(const Vector3D &before, const Vector3D &after, Vector3D &output, float scale) noexcept {
    float radius = sqrt(before.x*before.x + before.y*before.y + before.z*before.z);
    auto &xr = output.x;
    auto &yr = output.y;
    auto &zr = output.z;
    float rr;

    xr = before.x + (after.x - before.x) * scale;
    yr = before.y + (after.y - before.y) * scale;
    zr = before.z + (after.z - before.z) * scale;
    rr = sqrt(xr*xr + yr*yr + zr*zr);

    int highest = 0;
    float xrs = fabs(xr);
    float yrs = fabs(yr);
    float zrs = fabs(zr);
    if(yrs > xrs) highest = 1;
    else if(zrs > xrs) highest = 2;

    float a,b,c;
    float donegative = 1;
    if(highest == 0) {
        a = xr;
        b = yr;
        c = zr;
    }
    else if(highest == 1) {
        a = yr;
        b = xr;
        c = zr;
    }
    else {
        a = zr;
        b = yr;
        c = xr;
    }
    if(a < 0) donegative = -1;

    a = sqrt(fabs(radius*radius - b*b - c*c)) * donegative;

    if(highest == 0) {
        xr = a;
    }
    else if(highest == 1) {
        yr = a;
    }
    else if(highest == 2) {
        zr = a;
    }
}

void interpolate_vector(const Vector3D &before, const Vector3D &after, Vector3D &output, float scale) noexcept {
    output.x = before.x + (after.x - before.x) * scale;
    output.y = before.y + (after.y - before.y) * scale;
    output.z = before.z + (after.z - before.z) * scale;
}

void interpolate_vector_predict(const Vector3D &before, const Vector3D &after, Vector3D &output, float scale) noexcept {
    output.x = after.x + (after.x - before.x) * scale;
    output.y = after.y + (after.y - before.y) * scale;
    output.z = after.z + (after.z - before.z) * scale;
}

float distance(float x1, float y1, float z1, float x2, float y2, float z2) noexcept {
    return sqrt(distance_squared(x1,y1,z1,x2,y2,z2));
}

float distance(float x1, float y1, float x2, float y2) noexcept {
    return sqrt(distance_squared(x1,y1,x2,y2));
}

float distance(const Vector3D &a, const Vector3D &b) noexcept {
    return distance(a.x, a.y, a.z, b.x, b.y, b.z);
}

float distance_squared(float x1, float y1, float z1, float x2, float y2, float z2) noexcept {
    float x = x1 - x2;
    float y = y1 - y2;
    float z = z1 - z2;
    return x*x + y*y + z*z;
}

float distance_squared(float x1, float y1, float x2, float y2) noexcept {
    float x = x1 - x2;
    float y = y1 - y2;
    return x*x + y*y;
}

float distance_squared(const Vector3D &a, const Vector3D &b) noexcept {
    return distance_squared(a.x, a.y, a.z, b.x, b.y, b.z);
}

double counter_time_elapsed(const LARGE_INTEGER &before) noexcept {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return counter_time_elapsed(before, now);
}

double counter_time_elapsed(const LARGE_INTEGER &before, const LARGE_INTEGER &after) noexcept {
    static LARGE_INTEGER performance_frequency = {};
    if(performance_frequency.QuadPart == 0) QueryPerformanceFrequency(&performance_frequency);
    return static_cast<double>(after.QuadPart - before.QuadPart) / performance_frequency.QuadPart;
}
