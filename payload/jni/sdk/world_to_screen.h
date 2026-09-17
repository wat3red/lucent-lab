#pragma once

#include "functions.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <cmath>

double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

float deg2rad(float deg) {
    return deg * M_PI / 180.0;
}

__attribute__((__always_inline__))
inline Vector3 WorldToLocal(Vector3 aCamPos, Quaternion aCamRot, Vector3 aPos)
{
    return (Quaternion::Inverse(aCamRot)).normalized() * (aPos - aCamPos).normalized();
}

__attribute__((__always_inline__))
inline Vector3 Project(Vector3 aPos, float aFov, float aAspect)
{
    if (aPos.z == (0) || aAspect == (0)) return aPos;
    float t = tanf(deg2rad(aFov)  * (0.5f));
    if (t == (0)) return aPos;
    float f = (1.0f) / t;
    f /= aPos.z;
    aPos.x *= (f / aAspect);
    aPos.y *= f;
    return aPos;
}

__attribute__((__always_inline__))
inline Vector3 ClipSpaceToViewport(Vector3 aPos)
{
    aPos.x = aPos.x * (0.5) + (0.5);
    aPos.y = aPos.y * (0.50f) + (0.5f);
    return aPos;
}

__attribute__((__always_inline__))
inline Vector3 WorldToViewport(Vector3 aCamPos, Quaternion aCamRot, float aFov, float aAspect, Vector3 aPos)
{
    Vector3 p = WorldToLocal(aCamPos, aCamRot, aPos);
    p = Project(p, aFov, aAspect);
    return ClipSpaceToViewport(p);
}

__attribute__((__always_inline__))
inline Vector3 WorldToScreenPos(Vector3 aCamPos, Quaternion aCamRot, float aFov, float aScrWidth, float aScrHeight, float aspwct, Vector3 aPos) {
    if (aScrHeight == (0)) return aPos;
    Vector3 p = WorldToViewport(aCamPos, aCamRot, aFov, aspwct, aPos);
    p.x *= aScrWidth;
    p.y *= aScrHeight;
    p.y = aScrHeight - p.y;

    return p;
}

Vector3 WorldToScreen_Internal(Vector3 position, Vector3 cameraPosition, Quaternion cameraRotation, float fieldOfView)
{
    float widthFixed = Screen_get_width();
    float heightFixed = Screen_get_height();
    float aspect = widthFixed / heightFixed;

    return WorldToScreenPos(cameraPosition, cameraRotation, fieldOfView, widthFixed, heightFixed, aspect, position);
}