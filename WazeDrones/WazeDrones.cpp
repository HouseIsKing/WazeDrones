#include <fstream>

#include "Util/EngineDefaults.h"
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <iostream>
#include <ranges>
#include <vector>

#include "CameraManager.h"
#include "PerformanceGui.h"
#include "Entities/Drone.h"
#include "Util/Graph.h"
#include "Util/OctreeNode.h"
#include "Util/TessellationHelper.h"
#include "WorldManager.h"

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
    glfwWindowHint(GLFW_SAMPLES, 8);
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

void BuildPolygon(std::vector<vec3>& polygon, float height, TessellationHelper& tessellationHelper, const uint16_t texture)
{
    if (polygon.size() < 3)
    {
        return;
    }
    std::vector<vec3> topPolygon;
    topPolygon.reserve(polygon.size());
    for (auto& i : polygon)
    {
        topPolygon.emplace_back(i.x, height, i.z);
    }
    const uint32_t mainIndex = tessellationHelper.AddVertex(Vertex(polygon[0], 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, texture));
    const uint32_t mainIndexTop = tessellationHelper.AddVertex(Vertex(topPolygon[0], 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, texture));
    uint32_t previousIndex = tessellationHelper.AddVertex(Vertex(polygon[1], 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, texture));
    uint32_t previousIndexTop = tessellationHelper.AddVertex(Vertex(topPolygon[1], 0.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, texture));
    tessellationHelper.AddTriangle(mainIndex);
    tessellationHelper.AddTriangle(previousIndex);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndexTop);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
    float u = 1.0F;
    float v = 0.0F;
    for (size_t i = 2; i < polygon.size(); i++)
    {
        const uint32_t index = tessellationHelper.AddVertex(Vertex(polygon[i], u, v, 1.0F, 1.0F, 1.0F, 1.0F, texture));
        const uint32_t indexTop = tessellationHelper.AddVertex(Vertex(topPolygon[i], u, v, 1.0F, 1.0F, 1.0F, 1.0F, texture));
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
        u = 1 - u;
        v = 1 - v;
    }
    tessellationHelper.AddTriangle(previousIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(mainIndex);
    tessellationHelper.AddTriangle(mainIndexTop);
    tessellationHelper.AddTriangle(previousIndex);
}

void BuildPolygons(std::ifstream& file, TessellationHelper& tessellationHelper, const uint16_t texture, bool buildCollider, std::vector<BoundingBox>& colliders)
{
    size_t polygonCount;
    file >> polygonCount;
    if (buildCollider)
    {
        colliders.reserve(colliders.size() + polygonCount);
    }
    for (size_t i = 0; i < polygonCount; i++)
    {
        size_t numPoints;
        file >> numPoints;
        std::vector<vec3> points;
        points.reserve(numPoints);
        float minX = 10000.0F;
        float minZ = 10000.0F;
        float maxX = -1000.0F;
        float maxZ = -1000.0F;
        for (size_t j = 0; j < numPoints - 1; j++)
        {
            float x;
            float y;
            file >> x >> y;
            if (x < minX)
            {
                minX = x;
            }
            if (x > maxX)
            {
                maxX = x;
            }
            if (y < minZ)
            {
                minZ = y;
            }
            if (y > maxZ)
            {
                maxZ = y;
            }
            points.emplace_back(x, 0, y);
        }
        float temp;
        file >> temp >> temp;
        float height;
        file >> height;
        if (buildCollider)
        {
            colliders.emplace_back(minX, 0.0F, minZ, maxX, height, maxZ);
        }
        BuildPolygon(points, height, tessellationHelper, texture);
    }
    file.close();
}

void BuildLine(const vector<vec3>& points, TessellationHelper& tessellationHelper)
{
    if (points.size() < 2)
    {
        return;
    }
    uint32_t previousIndex = 0;
    bool start = true;
    for (const vec3& point : points)
    {
        const uint32_t currentIndex = tessellationHelper.AddVertex(Vertex(point, 0.0F, 0.0F, 0.1F, 0.1F, 0.1F, 1.0F, 0));
        if (!start)
        {
            tessellationHelper.AddTriangle(previousIndex);
            tessellationHelper.AddTriangle(currentIndex);
        }
        else
        {
            start = false;
        }
        previousIndex = currentIndex;
    }
}

void BuildLines(std::ifstream& file, TessellationHelper& tessellationHelper)
{
    size_t lineCount;
    file >> lineCount;
    for (size_t i = 0; i < lineCount; i++)
    {
        size_t numPoints;
        file >> numPoints;
        std::vector<vec3> points;
        points.reserve(numPoints);
        for (size_t j = 0; j < numPoints - 1; j++)
        {
            float x;
            float y;
            file >> x >> y;
            points.emplace_back(x, 0, y);
        }
        float temp;
        file >> temp >> temp;
        float height;
        file >> height;
        BuildLine(points, tessellationHelper);
    }
    file.close();
}

void BuildSimulationGeometry(TessellationHelper& tessellationHelperTriangles, TessellationHelper& tessellationHelperLines, OctreeNode& root, std::vector<BoundingBox>& colliders)
{
    std::ifstream file("Data/buildings_tel_aviv.txt");
    const uint16_t grassTexture = EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/grass.jpg"));
    const uint16_t buildingTexture = EngineDefaults::RegisterTexture(Texture::LoadTexture("Textures/building.jpg"));
    float bottomX;
    float bottomZ;
    float topX;
    float topZ;
    file >> bottomX >> topZ >> topX >> bottomZ;
    const float maxSize = std::max(abs(bottomX - topX), abs(bottomZ - topZ));
    root.Init(BoundingBox{-4591.323F, 0, -6544.746F, -4591.323F + maxSize, 12238.192F, -6544.746F + maxSize});
    EngineDefaults::BuildTextureUbo();
    BuildPolygons(file, tessellationHelperTriangles, buildingTexture, true, colliders);
    file.open("Data/parks_tel_aviv.txt");
    BuildPolygons(file, tessellationHelperTriangles, grassTexture, false, colliders);
    file.open("Data/roads_tel_aviv.txt");
    BuildLines(file, tessellationHelperLines);
}

std::vector<GraphNode*> BuildOctreeAndWaypointGraph(OctreeNode& root, std::vector<BoundingBox>& colliders, Graph& graph)
{
    std::vector<GraphNode*> helper;
    for (BoundingBox& collider : colliders)
    {
        root.Insert(collider);
        if (collider.GetMaxX() - collider.GetMinX() > 1.0F && collider.GetMaxZ() - collider.GetMinZ() > 1.0F)
        {
            helper.push_back(graph.AddNode(vec3((collider.GetMaxX() - collider.GetMinX()) / 2.0F + collider.GetMinX(), collider.GetMaxY() + 0.5F, (collider.GetMaxZ() - collider.GetMinZ()) / 2.0F + collider.GetMinZ())));
        }
    }
    std::vector<OctreeNode*> rootLeaves = root.GetAllLeavesNotColliding();
    std::ranges::sort(rootLeaves);
    unordered_map<OctreeNode*, GraphNode*> nodeToGraphNode;
    for (OctreeNode* leaf : rootLeaves)
    {
        GraphNode* node = graph.AddNode(leaf);
        nodeToGraphNode.emplace(leaf, node);
    }
    for (OctreeNode* leaf : rootLeaves)
    {
        for (int i = 0; i < 6; i++)
        {
            if (OctreeNode* result = leaf->GetNeighborGreaterOrEqual(static_cast<Direction>(i)); result != nullptr && nodeToGraphNode[result] != nullptr)
            {
                nodeToGraphNode[leaf]->AddConnection(nodeToGraphNode[result]->GetId());
                nodeToGraphNode[result]->AddConnection(nodeToGraphNode[leaf]->GetId());
            }
        }
    }
    for (GraphNode* node : helper)
    {
        OctreeNode* leaf = root.GetLeafAt(node->GetPosition());
        if (!leaf->GetIsColliding())
        {
            node->AddConnection(nodeToGraphNode[leaf]->GetId());
            nodeToGraphNode[leaf]->AddConnection(node->GetId());
            continue;
        }
        bool found = false;
        while (!found)
        {
            if (leaf->GetParent() == nullptr)
            {
                break;
            }
            leaf = leaf->GetParent();
            float distance = std::numeric_limits<float>::max();
            GraphNode* closest = nullptr;
            for (int i = 0; i < 6; i++)
            {
                if (OctreeNode* child = leaf->GetChild(static_cast<Position>(i)); !child->GetIsColliding() && node->Distance(*nodeToGraphNode[child]) < distance)
                {
                    closest = nodeToGraphNode[child];
                    distance = node->Distance(*nodeToGraphNode[child]);
                    found = true;
                }
            }
            if (found)
            {
                node->AddConnection(closest->GetId());
                closest->AddConnection(node->GetId());
            }
        }
    }
    return helper;
}

void CalcDensity(GLFWwindow* window, const std::vector<OctreeNode*>& leaves, PerformanceGui& gui)
{
    float safetyVolume = 100.0F * 100.0F * 15.0F;
    float maxNumDrones = 0;
    float averageNumDrones = 0.0F;
    for (const auto* const leaf : leaves)
    {
        const float value = static_cast<float>(leaf->GetDronesInsideNode().size()) / (leaf->GetVolume() / safetyVolume);
        averageNumDrones += value;
        if (value > maxNumDrones)
        {
            maxNumDrones = value;
        }
    }
    averageNumDrones /= static_cast<float>(leaves.size());
    const std::string textToDraw1 = "Max entropy of drones in a safety leaf: " + std::to_string(maxNumDrones);
    const std::string textToDraw2 = "Average number of drones in a safety leaf: " + std::to_string(averageNumDrones);
    gui.DrawStringAt(window, textToDraw1, 5.0F, 5.0F, 0.0F, 0.8F, 0.8F, 0.8F, 1.0F);
    gui.DrawStringAt(window, textToDraw2, 5.0F, 25.0F, 0.0F, 0.8F, 0.8F, 0.8F, 1.0F);
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
    CameraManager cameraManager{window};
    TessellationHelper worldTessellation{EngineDefaults::GetShader()};
    TessellationHelper linesTessellation{EngineDefaults::GetShader(), GL_LINES};
    TessellationHelper linesTessellationDrones{EngineDefaults::GetShader(), GL_LINES};
    std::vector<BoundingBox> colliders;
    WorldManager worldManager;
    PerformanceGui gui;
    BuildSimulationGeometry(worldTessellation, linesTessellation, *worldManager.GetRoot(), colliders);
    std::vector<GraphNode*> startEndPos = BuildOctreeAndWaypointGraph(*worldManager.GetRoot(), colliders, *worldManager.GetGraph());
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    std::vector<OctreeNode*> leaves = worldManager.GetRoot()->GetAllLeavesNotColliding();
    std::unordered_set<uint32_t> spawnedLocationsUsed{};
    for (int i = 0; i < 2000; i++)
    {
        GraphNode* node = startEndPos[static_cast<size_t>(EngineDefaults::GetNextInt(static_cast<int>(startEndPos.size())))];
        while (spawnedLocationsUsed.contains(node->GetId()))
        {
            node = startEndPos[static_cast<size_t>(EngineDefaults::GetNextInt(static_cast<int>(startEndPos.size())))];
        }
        spawnedLocationsUsed.emplace(node->GetId());
        auto* drone = new Drone(&worldManager, node->GetId(), worldManager.GetNextAvailableDroneId());
        worldManager.AddDrone(drone);
        uint32_t destNodeId = startEndPos[static_cast<size_t>(EngineDefaults::GetNextInt(static_cast<int>(startEndPos.size())))]->GetId();
        while (destNodeId == node->GetId())
        {
            destNodeId = startEndPos[static_cast<size_t>(EngineDefaults::GetNextInt(static_cast<int>(startEndPos.size())))]->GetId();
        }
        drone->SetDestination(destNodeId);
    }
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    double timePrev = glfwGetTime();
    while (glfwWindowShouldClose(window) == 0)
    {
        linesTessellationDrones.Reset();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (tickTimer > 1.0 / 20.0)
        {
            gui.Rebuild();
            worldManager.Tick(tickTimer);
            CalcDensity(window, leaves, gui);
            tickTimer -= 1.0 / 20.0;
        }
        cameraManager.Tick();
        worldTessellation.Draw();
        worldManager.Draw();
        gui.Render();
        linesTessellationDrones.Draw();
        glLineWidth(6);
        linesTessellation.Draw();
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
