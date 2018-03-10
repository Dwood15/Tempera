#pragma once

#include <windows.h>

#define STRA(x) #x
#define STR(x) STRA(x)

//#define ALSO_OUTPUT_WHAT_HAPPENED_WHEN_IT_BAILED

#ifdef ALSO_OUTPUT_WHAT_HAPPENED_WHEN_IT_BAILED
#define assert_or_bail(v) if(!(v)) { console_outconsole_out_warning(STR(v)); return; }
#else
#define assert_or_bail(v) if(!(v)) return;
#endif

#define CONCAT(x, y) x ## y
#define CONCAT2(x, y) CONCAT(x, y)
#define PAD_CHARS(s) char CONCAT2(padding, __LINE__)[s];
#define PAD_BITS(type,s) type CONCAT2(padding, __LINE__) : s;

struct ColorARGB;
struct ColorByte;

struct ColorRGB {
    float red = 1.0;
    float green = 1.0;
    float blue = 1.0;

    ColorRGB() noexcept;
    ColorRGB(float r, float g, float b) noexcept;
    ColorRGB(const ColorByte &other) noexcept;
    ColorRGB(const ColorARGB &other) noexcept;
    ColorRGB(const ColorRGB &other) noexcept;
};

struct ColorARGB {
    float alpha = 1.0;
    float red = 1.0;
    float green = 1.0;
    float blue = 1.0;

    ColorARGB() noexcept;
    ColorARGB(float a, float r, float g, float b) noexcept;
    ColorARGB(const ColorByte &other) noexcept;
    ColorARGB(const ColorRGB &other) noexcept;
    ColorARGB(const ColorARGB &other) noexcept;
};

struct ColorByte {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha = 255;

    ColorByte() noexcept;
    ColorByte(float a, float r, float g, float b) noexcept;
    ColorByte(unsigned char a, unsigned char r, unsigned char g, unsigned char b) noexcept;
    ColorByte(const ColorRGB &other) noexcept;
    ColorByte(const ColorARGB &other) noexcept;
    ColorByte(const ColorByte &other) noexcept;
};

struct Vector3D {
    float x;
    float y;
    float z;
};

struct Euler3DPYR {
    float pitch;
    float yaw;
    float roll;
};

struct Euler3DYPR {
    float yaw;
    float pitch;
    float roll;
};

struct RotationMatrix;
struct Quaternion {
    float x;
    float y;
    float z;
    float w;
    Quaternion() noexcept;
    Quaternion(const RotationMatrix &matrix) noexcept;
    Quaternion(const Quaternion &copy) noexcept;
};

struct RotationMatrix {
    Vector3D v[3];
    RotationMatrix() noexcept;
    RotationMatrix(const Quaternion &quaternion) noexcept;
    RotationMatrix(const RotationMatrix &copy) noexcept;
};

/// Interpolate a quaternion.
void interpolate_quat(const Quaternion &in_before, const Quaternion &in_after, Quaternion &out, float scale) noexcept;

/// Interpolate a normalized 3D vector.
void interpolate_vector_rotation(const Vector3D &before, const Vector3D &after, Vector3D &output, float scale) noexcept;

typedef void (*interpolate_vector_fn)(const Vector3D&,const Vector3D&,Vector3D&,float);

/// Interpolate a 3D vector.
void interpolate_vector(const Vector3D &before, const Vector3D &after, Vector3D &output, float scale) noexcept;

/// Interpolate a 3D vector, but add the delta to the after vector instead of the before vector. This is to remove the
/// "one tick behind" effect that interpolate_vector normally causes, but at the cost of accuracy.
void interpolate_vector_predict(const Vector3D &before, const Vector3D &after, Vector3D &output, float scale) noexcept;

/// Calculate the distance between two 2D points.
float distance(float x1, float y1, float x2, float y2) noexcept;

/// Calculate the distance between two 3D points.
float distance(float x1, float y1, float z1, float x2, float y2, float z2) noexcept;

/// Calculate the distance between two 3D points.
float distance(const Vector3D &a, const Vector3D &b) noexcept;

/// Calculate the distance between two 3D points without taking the square root.
float distance_squared(float x1, float y1, float z1, float x2, float y2, float z2) noexcept;

/// Calculate the distance between two 2D points without taking the square root.
float distance_squared(float x1, float y1, float x2, float y2) noexcept;

/// Calculate the distance between two 3D points without taking the square root.
float distance_squared(const Vector3D &a, const Vector3D &b) noexcept;

/// Get the time elapsed since a counter.
double counter_time_elapsed(const LARGE_INTEGER &before) noexcept;

/// Get the time elapsed between two counters.
double counter_time_elapsed(const LARGE_INTEGER &before, const LARGE_INTEGER &after) noexcept;
