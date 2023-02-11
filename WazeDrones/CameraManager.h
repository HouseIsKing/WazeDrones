#pragma once
#include "Camera.h"
#include "Util/EngineDefaults.h"
#include <GLFW/glfw3.h>

class CameraManager
{
    Camera CameraInstance;
    GLFWwindow* Window;
    float PrevMouseX;
    float PrevMouseY;
    float MouseSensitivity = 0.05F;

public:
    static vec3 TeleportToCollision;
    explicit CameraManager(GLFWwindow* window);
    void Tick();
};
