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
    float MouseSensitivity = 0.1F;
public:
    explicit CameraManager(GLFWwindow* window);
    void Tick();
};
