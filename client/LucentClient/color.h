#pragma once

struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color() { r = 0; g = 0; b = 0; a = 0; }
    Color(float value[4]) { r = value[0]; g = value[1]; b = value[2]; a = value[3]; }
    Color(const float value[4]) { r = value[0]; g = value[1]; b = value[2]; a = value[3]; }
    Color(float _r, float _g, float _b, float _a) { r = _r; g = _g; b = _b; a = _a; }
};