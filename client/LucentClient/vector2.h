#pragma once

struct Vector2 {
	float x, y;

	Vector2() : x(0), y(0) {}
	Vector2(float x_, float y_) : x(x_), y(y_) {}

	Vector2 operator+(const Vector2& other) const {
		return { x + other.x, y + other.y };
	}

	Vector2 operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}

	Vector2 operator*(const Vector2& other) const {
		return { x * other.x, y * other.y };
	}

	inline float Magnitude() const { return sqrt(x * x + y * y); }
};

