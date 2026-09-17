#pragma once

#include "Vector2.h"

struct Vector3 {
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

	Vector3 operator+(const Vector3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	Vector3 operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	Vector3 operator*(const Vector3& other) const {
		return { x * other.x, y * other.y, z * other.z };
	}

	inline operator Vector2& () { return *(Vector2*)this; }
	inline operator Vector2() const { return { x, y }; }

	inline static float Magnitude(Vector3 vector) { return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z); }
	inline float Magnitude() const { return sqrt(x * x + y * y + z * z); }

	inline bool operator==(Vector3 v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool operator!=(Vector3 v) const { return x != v.x || y != v.y || z != v.z; }
	inline Vector3& operator+=(Vector3 inV) { x += inV.x; y += inV.y; z += inV.z; return *this; }
	inline Vector3& operator-=(Vector3 inV) { x -= inV.x; y -= inV.y; z -= inV.z; return *this; }
	inline Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	inline Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
	inline Vector3& operator/=(Vector3 inV) { x /= inV.x; y /= inV.y; z /= inV.z; return *this; }
	inline friend Vector3 operator+(Vector3 lhs, Vector3 rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
	inline friend Vector3 operator-(Vector3 lhs, Vector3 rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
	inline friend Vector3 operator*(Vector3 v, float s) { return { v.x * s, v.y * s, v.z * s }; }
	inline friend Vector3 operator*(float s, Vector3 v) { return { v.x * s, v.y * s, v.z * s }; }
	inline friend Vector3 operator*(Vector3 lhs, Vector3 rhs) { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }
	inline friend Vector3 operator/(Vector3 v, float s) { Vector3 temp(v); temp /= s; return temp; }
	inline friend Vector3 operator/(Vector3 lhs, Vector3 rhs) { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }
	inline Vector3 operator-() const { return { -x, -y, -z }; }
};

