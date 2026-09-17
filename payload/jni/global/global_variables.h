#pragma once

#include "../sdk/Vector3.h"
#include "../sdk/Quaternion.h"

namespace Global {
    namespace OverwrittenValues {
        Vector3 g_CameraEulerAngles;
        Vector3 g_CameraPosition;
        Quaternion g_CameraRotation;
    }
}
