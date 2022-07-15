#include <glad/glad.h>  // NOLINT(llvm-include-order)
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>

#include "Util/EngineDefaults.h"
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "WazeDrones", nullptr, nullptr);
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
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    return window;
}

void BuildSimulationGeometry()
{
}

void BuildBuilding(std::vector<vec3> polygon, float height)
{
    TessellationHelper tessellationHelper{EngineDefaults::GetShader()};
    std::vector<vec3> topPolygon;
    topPolygon.reserve(polygon.size());
    for (auto& i : polygon)
    {
        topPolygon.emplace_back(i.x, i.y, height);
    }
    const uint16_t mainIndex = tessellationHelper.AddVertex(Vertex(polygon[0], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    const uint16_t mainIndexTop = tessellationHelper.AddVertex(Vertex(topPolygon[0], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    uint16_t previousIndex = tessellationHelper.AddVertex(Vertex(polygon[1], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    uint16_t previousIndexTop = tessellationHelper.AddVertex(Vertex(topPolygon[1], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
    tessellationHelper.AddTriangle(mainIndex);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
    tessellationHelper.AddTriangle(previousIndexTop);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
    for (size_t i = 2; i < polygon.size(); i++)
    {
        const uint16_t index = tessellationHelper.AddVertex(Vertex(polygon[i], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
        const uint16_t indexTop = tessellationHelper.AddVertex(Vertex(topPolygon[i], 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0));
        //bottom triangle
        tessellationHelper.AddTriangle(mainIndex);
        tessellationHelper.AddTriangle(previousIndex);
        tessellationHelper.AddTriangle(index);
        //top triangle
        tessellationHelper.AddTriangle(mainIndexTop);
        tessellationHelper.AddTriangle(previousIndexTop);
        tessellationHelper.AddTriangle(indexTop);
        //side triangles
        tessellationHelper.AddTriangle(previousIndexTop);
        tessellationHelper.AddTriangle(indexTop);
        tessellationHelper.AddTriangle(previousIndex);
        tessellationHelper.AddTriangle(index);
        tessellationHelper.AddTriangle(previousIndex);
        tessellationHelper.AddTriangle(indexTop);
        previousIndex = index;
        previousIndexTop = indexTop;
    }
}

int main(int /*argc*/, char* /*argv*/[])
{
    GLFWwindow* window = Init();
    if (window == nullptr)
    {
        return -1;
    }
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
