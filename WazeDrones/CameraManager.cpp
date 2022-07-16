#include "CameraManager.h"

CameraManager::CameraManager(GLFWwindow* window) : CameraInstance(vec3(), 16.0F / 9.0F), Window(window), PrevMouseX(0), PrevMouseY(0)
{
}

void CameraManager::Tick()
{
    int state = glfwGetKey(Window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        CameraInstance.Position += CameraInstance.GetFrontVector() * 0.05F;
    }
    state = glfwGetKey(Window, GLFW_KEY_S);
    if (state == GLFW_PRESS)
    {
        CameraInstance.Position -= CameraInstance.GetFrontVector() * 0.05F;
    }
    state = glfwGetKey(Window, GLFW_KEY_A);
    if (state == GLFW_PRESS)
    {
        CameraInstance.Position -= CameraInstance.GetRightVector() * 0.05F;
    }
    state = glfwGetKey(Window, GLFW_KEY_D);
    if (state == GLFW_PRESS)
    {
        CameraInstance.Position += CameraInstance.GetRightVector() * 0.05F;
    }
    double x;
    double y;
    glfwGetCursorPos(Window, &x, &y);
    const float mouseX = static_cast<float>(x) - PrevMouseX;
    const float mouseY = static_cast<float>(y) - PrevMouseY;
    PrevMouseX = static_cast<float>(x);
    PrevMouseY = static_cast<float>(y);
    CameraInstance.Yaw += mouseX * MouseSensitivity;
    CameraInstance.Pitch += -mouseY * MouseSensitivity;
    if (CameraInstance.Pitch > 89.0F)
    {
        CameraInstance.Pitch = 89.0F;
    }
    if (CameraInstance.Pitch < -89.0F)
    {
        CameraInstance.Pitch = -89.0F;
    }
    Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("view"), CameraInstance.GetViewMatrix());
    Shader::SetMat4(EngineDefaults::GetShader()->GetUniformInt("projection"), CameraInstance.GetProjectionMatrix());
}
