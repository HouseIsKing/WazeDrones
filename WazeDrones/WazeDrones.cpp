#include "Util/EngineDefaults.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>

#include "CameraManager.h"
#include "Util/TessellationHelper.h"

void FramebufferSizeCallback(GLFWwindow* /*window*/, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "WazeDrones", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    return window;
}

void BuildSimulationGeometry()
{
}

void BuildBuilding(std::vector<vec3>& polygon, float height, TessellationHelper& tessellationHelper)
{
    std::vector<vec3> topPolygon;
    topPolygon.reserve(polygon.size());
    for (auto& i : polygon)
    {
        topPolygon.emplace_back(i.x, height, i.z);
    }
    const uint16_t mainIndex = tessellationHelper.AddVertex(Vertex(polygon[0], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    const uint16_t mainIndexTop = tessellationHelper.AddVertex(Vertex(topPolygon[0], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    uint16_t previousIndex = tessellationHelper.AddVertex(Vertex(polygon[1], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    uint16_t previousIndexTop = tessellationHelper.AddVertex(Vertex(topPolygon[1], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    tessellationHelper.AddTriangle(mainIndex);
    tessellationHelper.AddTriangle(previousIndex);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndexTop);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
    for (size_t i = 2; i < polygon.size(); i++)
    {
        const uint16_t index = tessellationHelper.AddVertex(Vertex(polygon[i], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
        const uint16_t indexTop = tessellationHelper.AddVertex(Vertex(topPolygon[i], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
        //bottom triangle
        tessellationHelper.AddTriangle(mainIndex);
        tessellationHelper.AddTriangle(index);
        tessellationHelper.AddTriangle(previousIndex);
        //top triangle
        tessellationHelper.AddTriangle(mainIndexTop);
        tessellationHelper.AddTriangle(previousIndexTop);
        tessellationHelper.AddTriangle(indexTop);
        //side triangles
        tessellationHelper.AddTriangle(index);
        tessellationHelper.AddTriangle(indexTop);
        tessellationHelper.AddTriangle(previousIndex);
        tessellationHelper.AddTriangle(previousIndexTop);
        tessellationHelper.AddTriangle(previousIndex);
        tessellationHelper.AddTriangle(indexTop);
        previousIndex = index;
        previousIndexTop = indexTop;
    }
    tessellationHelper.AddTriangle(previousIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(mainIndex);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
}

int main(int /*argc*/, char* /*argv*/[])
{
    GLFWwindow* window = Init();
    if (window == nullptr)
    {
        return -1;
    }
    glfwSwapInterval(0);
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    int counter = 0;
    double timePassed = 0.0;
    double tickTimer = 0.0;
    double timePrev = glfwGetTime();
    CameraManager cameraManager{window};
    vector<vec3> data;
    data.reserve(6);
    data.emplace_back(-3.0F, 0.0F, 3.0F);
    data.emplace_back(3.0F, 0.0F, 3.0F);
    data.emplace_back(6.0F, 0.0F, 0.0F);
    data.emplace_back(3.0F, 0.0F, -3.0F);
    data.emplace_back(-3.0F, 0.0F, -3.0F);
    data.emplace_back(-6.0F, 0.0F, 0.0F);
    TessellationHelper worldTessellation{EngineDefaults::GetShader()};
    BuildBuilding(data, 10.0F, worldTessellation);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_CULL_FACE);
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        if (tickTimer > 1.0 / 144.0)
        {
            tickTimer -= 1.0 / 144.0;
            cameraManager.Tick();
        }
        worldTessellation.Draw();
        glfwSwapBuffers(window);
        double timeNow = glfwGetTime();
        counter++;
        timePassed += timeNow - timePrev;
        tickTimer += timeNow - timePrev;
        timePrev = timeNow;
        if (timePassed >= 1.0)
        {
            timePassed -= 1.0;
            std::cout << "FPS: " << counter << std::endl;
            counter = 0;
        }
        glfwPollEvents();
        
    }
    glfwTerminate();
    return 0;
}
