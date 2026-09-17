#pragma once

#include "Vector3.h"

struct Quaternion {
    union {
        struct { float x, y, z, w; };
        float data[4]{ 0.f, 0.f, 0.f, 0.f };
    };

    inline Quaternion() : x(0), y(0), z(0), w(1) {};
    inline Quaternion(float data[]) noexcept : x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}
    inline Quaternion(Vector3 vector, float scalar) noexcept : x(vector.x), y(vector.y), z(vector.z), w(scalar) {};
    inline Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {};
    inline Quaternion(float Pitch, float Yaw, float Roll) {
        *this = Quaternion::FromEuler(Yaw, Pitch, Roll);
    };

    inline static Vector3 Up(Quaternion q);
    inline static Vector3 Down(Quaternion q);
    inline static Vector3 Left(Quaternion q);
    inline static Vector3 Right(Quaternion q);
    inline static Vector3 Forward(Quaternion q);
    inline static Vector3 Back(Quaternion q);

    inline static float Angle(Quaternion a, Quaternion b);
    inline static Quaternion Conjugate(Quaternion rotation);
    inline static float Dot(Quaternion lhs, Quaternion rhs);
    inline static Quaternion FromAngleAxis(float angle, Vector3 axis);
    inline static Quaternion FromEuler(Vector3 rotation);
    inline static Quaternion FromEuler(float yaw, float pitch, float roll, bool fromDeg = true);
    inline static Quaternion FromToRotation(Vector3 fromVector, Vector3 toVector);
    inline static Quaternion Inverse(Quaternion rotation);
    inline static Quaternion Lerp(Quaternion a, Quaternion b, float t);
    inline static Quaternion LerpUnclamped(Quaternion a, Quaternion b, float t);
    inline static Quaternion LookRotation(Vector3 forward);
    inline static Quaternion LookRotation(Vector3 forward, Vector3 upwards);
    inline static float Norm(Quaternion rotation);
    inline static Quaternion Normalize(Quaternion rotation);
    inline static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxRadiansDelta);
    inline static Quaternion Slerp(Quaternion a, Quaternion b, float t);
    inline static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
    inline static void ToAngleAxis(Quaternion rotation, float& angle, Vector3& axis);
    inline static Vector3 ToEuler(Quaternion q, bool toDeg = true);

    inline Vector3 euler() { return ToEuler(*this); }
    inline Quaternion normalized() { return Normalize(*this); }

    inline std::string str() const { return std::to_string(x) + (oxorany(", ")) + std::to_string(y) + (oxorany(", ")) + std::to_string(z) + (oxorany(", ")) + std::to_string(w); }

    inline Quaternion& operator+=(Quaternion q) { x += q.x; y += q.y; z += q.z; w += q.w; return *this; }
    inline Quaternion& operator-=(Quaternion q) { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return *this; }
    inline Quaternion& operator*=(Quaternion rhs);
    inline Quaternion& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    inline Quaternion& operator/=(Quaternion rhs);
    inline Quaternion& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
    inline friend Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs) { Quaternion q(lhs); return q += rhs; }
    inline friend Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs) { Quaternion t(lhs); return t -= rhs; }
    inline friend Quaternion operator*(const float s, const Quaternion& q) { Quaternion t(q); return t *= s; }
    inline friend Quaternion operator/(const Quaternion& q, const float s) { Quaternion t(q); return t /= s; }
    inline Quaternion operator-() const { return { -x, -y, -z, -w }; }
    inline Quaternion operator*(float s) const { return { x * s, y * s, z * s, w * s }; }

    inline friend Quaternion operator*(Quaternion lhs, Quaternion rhs) {
        return { lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
                lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
                lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z };
    }
    inline friend Quaternion operator/(Quaternion lhs, Quaternion rhs) {
        return { lhs.w / rhs.x + lhs.x / rhs.w + lhs.y / rhs.z - lhs.z / rhs.y,
                lhs.w / rhs.y + lhs.y / rhs.w + lhs.z / rhs.x - lhs.x / rhs.z,
                lhs.w / rhs.z + lhs.z / rhs.w + lhs.x / rhs.y - lhs.y / rhs.x,
                lhs.w / rhs.w - lhs.x / rhs.x - lhs.y / rhs.y - lhs.z / rhs.z };
    }
    inline static Vector3 RotateVectorByQuaternion(Quaternion lhs, Vector3 rhs);

    static const Quaternion identity;
};

inline const Quaternion Quaternion::identity = Quaternion(0, 0, 0, 1);

inline Quaternion& Quaternion::operator*=(Quaternion rhs) {
    float tempX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
    float tempY = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
    float tempZ = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
    float tempW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
    x = tempX; y = tempY; z = tempZ; w = tempW;
    return *this;
}

inline Quaternion& Quaternion::operator/=(Quaternion rhs) {
    float tempX = w / rhs.x + x / rhs.w + y / rhs.z - z / rhs.y;
    float tempY = w / rhs.y + y / rhs.w + z / rhs.x - x / rhs.z;
    float tempZ = w / rhs.z + z / rhs.w + x / rhs.y - y / rhs.x;
    float tempW = w / rhs.w - x / rhs.x - y / rhs.y - z / rhs.z;
    x = tempX; y = tempY; z = tempZ; w = tempW;
    return *this;
}

inline Vector3 operator*(Quaternion lhs, const Vector3 rhs) {
    Vector3 u = Vector3(lhs.x, lhs.y, lhs.z);
    return u * (Vector3::Dot(u, rhs) * 2.0f) + rhs * (lhs.w * lhs.w - Vector3::Dot(u, u)) + Vector3::Cross(u, rhs) * (2.0f * lhs.w);
}

Vector3 Quaternion::Up(Quaternion q) { return q * Vector3::up; }
Vector3 Quaternion::Down(Quaternion q) { return q * Vector3::down; }
Vector3 Quaternion::Left(Quaternion q) { return q * Vector3::left; }
Vector3 Quaternion::Right(Quaternion q) { return q * Vector3::right; }
Vector3 Quaternion::Forward(Quaternion q) { return q * Vector3::forward; }
Vector3 Quaternion::Back(Quaternion q) { return q * Vector3::back; }

float Quaternion::Angle(Quaternion a, Quaternion b) {
    float dot = Dot(a, b);
    return acosf(fminf(fabs(dot), 1)) * 2;
}

Quaternion Quaternion::Conjugate(Quaternion rotation) {
    return { -rotation.x, -rotation.y, -rotation.z, rotation.w };
}

float Quaternion::Dot(Quaternion lhs, Quaternion rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

Quaternion Quaternion::FromAngleAxis(float angle, Vector3 axis) {
    Quaternion q;
    float m = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    float s = sinf(angle / 2) / m;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    q.w = cosf(angle / 2);
    return q;
}

Quaternion Quaternion::FromEuler(Vector3 rotation) {
    return FromEuler(rotation.x, rotation.y, rotation.z);
}

Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll, bool fromDeg) {
    constexpr float deg2Rad = M_PI / 180.f;

    if (fromDeg) {
        roll *= (float)deg2Rad;
        pitch *= (float)deg2Rad;
        yaw *= (float)deg2Rad;
    }
    float cY(cosf(yaw / 2.0f));
    float sY(sinf(yaw / 2.0f));

    float cP(cosf(pitch / 2.0f));
    float sP(sinf(pitch / 2.0f));

    float cR(cosf(roll / 2.0f));
    float sR(sinf(roll / 2.0f));

    /*
    Упрощённое перемножение этих данных:
    Simplified multiplication of this data:
      Quaternion qX(sY, 0.f, 0.f, cY);
      Quaternion qY(0.f, sP, 0.f, cP);
      Quaternion qZ(0.f, 0.f, sR, cR);
      return (qY * qX) * qZ;
    */
    return { cP * sY * cR + sP * cY * sR,
            sP * cY * cR - cP * sY * sR,
            cP * cY * sR - sP * sY * cR,
            cP * cY * cR + sP * sY * sR };
}

Vector3 Quaternion::ToEuler(Quaternion q, bool toDeg) {
    Vector3 rot{};

    float xy = q.x * q.y, xw = q.x * q.w;
    float yz = q.y * q.z;
    float zw = q.z * q.w;

    float singularity_test = yz - xw;

    rot.x = -1.f * asinf(std::clamp(2.0f * singularity_test, -1.0f, 1.0f));

    if (abs(singularity_test) < 0.499999f) {
        float xx = q.x * q.x, yy = q.y * q.y, yw = q.y * q.w, zz = q.z * q.z, ww = q.w * q.w;
        rot.y = atan2f(2.0f * (q.x * q.z + yw), zz - xx - yy + ww);
        rot.z = atan2f(2.0f * (xy + zw), yy - zz - xx + ww);
    }
    else {
        float a = xy + zw;
        float b = -yz + xw;
        float c = xy - zw;
        float e = yz + xw;
        rot.y = atan2f(a * e + b * c, b * e - a * c);
        rot.z = 0;
    }

    constexpr float rad2deg = 180.f / M_PI;
    if (toDeg) rot *= (float)rad2deg;

    return rot;
}
Quaternion Quaternion::FromToRotation(Vector3 fromVector, Vector3 toVector) {
    float dot = Vector3::Dot(fromVector, toVector);
    float k = sqrtf(Vector3::SqrMagnitude(fromVector) * Vector3::SqrMagnitude(toVector));
    if (fabsf(dot / k + 1) < 0.00001) {
        Vector3 ortho = Vector3::Orthogonal(fromVector);
        return { Vector3::Normalize(ortho), 0 };
    }
    Vector3 cross = Vector3::Cross(fromVector, toVector);
    return Normalize(Quaternion(cross, dot + k));
}

Quaternion Quaternion::Inverse(Quaternion rotation) {
    float n = Norm(rotation);
    return Conjugate(rotation) / (n * n);
}

Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, float t) {
    if (t < 0) return Normalize(a);
    else if (t > 1) return Normalize(b);
    return LerpUnclamped(a, b, t);
}

Quaternion Quaternion::LerpUnclamped(Quaternion a, Quaternion b, float t) {
    Quaternion quaternion{};
    if (Dot(a, b) >= 0) quaternion = a * (1 - t) + b * t;
    else quaternion = a * (1 - t) - b * t;
    return Normalize(quaternion);
}

Quaternion Quaternion::LookRotation(Vector3 forward) {
    return LookRotation(forward, Vector3(0, 1, 0));
}

Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 upwards) {
    forward = Vector3::Normalize(forward);
    upwards = Vector3::Normalize(upwards);
    constexpr float smallFloat = 0.0000000001f;
    if (Vector3::SqrMagnitude(forward) < smallFloat || Vector3::SqrMagnitude(upwards) < smallFloat) return Quaternion::identity;
    if (1 - fabs(Vector3::Dot(forward, upwards)) < smallFloat) return FromToRotation(Vector3::forward, forward);
    Vector3 right = Vector3::Normalize(Vector3::Cross(upwards, forward));
    upwards = Vector3::Cross(forward, right);
    Quaternion quaternion{};
    float radicand = right.x + upwards.y + forward.z;
    if (radicand > 0) {
        quaternion.w = sqrtf(1.0f + radicand) * 0.5f;
        float recip = 1.0f / (4.0f * quaternion.w);
        quaternion.x = (upwards.z - forward.y) * recip;
        quaternion.y = (forward.x - right.z) * recip;
        quaternion.z = (right.y - upwards.x) * recip;
    }
    else if (right.x >= upwards.y && right.x >= forward.z) {
        quaternion.x = sqrtf(1.0f + right.x - upwards.y - forward.z) * 0.5f;
        float recip = 1.0f / (4.0f * quaternion.x);
        quaternion.w = (upwards.z - forward.y) * recip;
        quaternion.z = (forward.x + right.z) * recip;
        quaternion.y = (right.y + upwards.x) * recip;
    }
    else if (upwards.y > forward.z) {
        quaternion.y = sqrtf(1.0f - right.x + upwards.y - forward.z) * 0.5f;
        float recip = 1.0f / (4.0f * quaternion.y);
        quaternion.z = (upwards.z + forward.y) * recip;
        quaternion.w = (forward.x - right.z) * recip;
        quaternion.x = (right.y + upwards.x) * recip;
    }
    else {
        quaternion.z = sqrtf(1.0f - right.x - upwards.y + forward.z) * 0.5f;
        float recip = 1.0f / (4.0f * quaternion.z);
        quaternion.y = (upwards.z + forward.y) * recip;
        quaternion.x = (forward.x + right.z) * recip;
        quaternion.w = (right.y - upwards.x) * recip;
    }
    return quaternion;
}

float Quaternion::Norm(Quaternion rotation) {
    return sqrtf(rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w);
}

Quaternion Quaternion::Normalize(Quaternion rotation) {
    return rotation / Norm(rotation);
}

Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, float maxRadiansDelta) {
    float angle = Angle(from, to);
    if (angle == 0) return to;
    maxRadiansDelta = fmaxf(maxRadiansDelta, angle - (float)M_PI);
    float t = fminf(1, maxRadiansDelta / angle);
    return SlerpUnclamped(from, to, t);
}

Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, float t) {
    if (t < 0) return Normalize(a);
    else if (t > 1) return Normalize(b);
    return SlerpUnclamped(a, b, t);
}

Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, float t) {
    float dot = Dot(a, b);
    if (dot < 0.f) {
        b = -b;
        dot = -dot;
    }

    if (dot < 0.95f) {
        float angle = acosf(dot);
        return (a * sinf(angle * (1.0f - t)) + b * sinf(angle * t)) * (1.0f / sinf(angle));
    }
    return LerpUnclamped(a, b, t);
}

void Quaternion::ToAngleAxis(Quaternion rotation, float& angle, Vector3& axis) {
    if (rotation.w > 1) rotation = Normalize(rotation);
    angle = 2 * acosf(rotation.w);
    float s = sqrt(1 - rotation.w * rotation.w);
    if (s < 0.00001) {
        axis.x = 1;
        axis.y = 0;
        axis.z = 0;
    }
    else {
        axis.x = rotation.x / s;
        axis.y = rotation.y / s;
        axis.z = rotation.z / s;
    }
}
inline Vector3 Quaternion::RotateVectorByQuaternion(Quaternion lhs, Vector3 rhs) {
    float x = lhs.x * 2.f;
    float y = lhs.y * 2.f;
    float z = lhs.z * 2.f;
    float xx = lhs.x * x;
    float yy = lhs.y * y;
    float zz = lhs.z * z;
    float xy = lhs.x * y;
    float xz = lhs.x * z;
    float yz = lhs.y * z;
    float wx = lhs.w * x;
    float wy = lhs.w * y;
    float wz = lhs.w * z;

    Vector3 res;
    res.x = (1.0f - (yy + zz)) * rhs.x + (xy - wz) * rhs.y + (xz + wy) * rhs.z;
    res.y = (xy + wz) * rhs.x + (1.0f - (xx + zz)) * rhs.y + (yz - wx) * rhs.z;
    res.z = (xz - wy) * rhs.x + (yz + wx) * rhs.y + (1.0f - (xx + yy)) * rhs.z;

    return res;
}

float NormalizeAngle(float angle) {
    while (angle > 360)
        angle -= 360;
    while (angle < 0)
        angle += 360;
    return angle;
}

Vector3 NormalizeAngles(Vector3 angles) {
    angles.x = NormalizeAngle(angles.x);
    angles.y = NormalizeAngle(angles.y);
    angles.z = NormalizeAngle(angles.z);
    return angles;
}

Quaternion FromEuler(float x, float y, float z)
{
    float cx = cos(x * 0.5);
    float cy = cos(y * 0.5);
    float cz = cos(z * 0.5);
    float sx = sin(x * 0.5);
    float sy = sin(y * 0.5);
    float sz = sin(z * 0.5);
    Quaternion q;
    q.x = cx * sy * sz + cy * cz * sx;
    q.y = cx * cz * sy - cy * sx * sz;
    q.z = cx * cy * sz - cz * sx * sy;
    q.w = sx * sy * sz + cx * cy * cz;
    return q;
}

Quaternion FromEuler(Vector3 rotation)
{
    return FromEuler(rotation.x, rotation.y, rotation.z);
}


Vector3 ToEulerRad(Quaternion q1) {
    float Rad2Deg1 = 360.0f / (M_PI * 2.0f);
    float sqw = q1.w * q1.w;
    float sqx = q1.x * q1.x;
    float sqy = q1.y * q1.y;
    float sqz = q1.z * q1.z;
    float unit = sqx + sqy + sqz + sqw;
    float test = q1.x * q1.w - q1.y * q1.z;
    Vector3 v;

    if (test > 0.4995f * unit) {
        v.y = 2.0f * atan2f(q1.y, q1.x);
        v.x = M_PI / 2.0f;
        v.z = 0;
        return NormalizeAngles(v * Rad2Deg1);
    }

    if (test < -0.4995f * unit) {
        v.y = -2.0f * atan2f(q1.y, q1.x);
        v.x = -M_PI / 2.0f;
        v.z = 0;
        return NormalizeAngles(v * Rad2Deg1);
    }

    Quaternion q(q1.w, q1.z, q1.x, q1.y);
    v.y = atan2f(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1 - 2.0f * (q.z * q.z + q.w * q.w)); // yaw
    v.x = asinf(2.0f * (q.x * q.z - q.w * q.y)); // pitch
    v.z = atan2f(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1 - 2.0f * (q.y * q.y + q.z * q.z)); // roll
    return NormalizeAngles(v * Rad2Deg1);
}