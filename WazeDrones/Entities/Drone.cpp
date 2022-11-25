#include "Drone.h"

#include <ranges>

#include "../Util/EngineDefaults.h"

Drone::Drone(Graph baseGraph, const float x, const float y, const float z, const uint32_t idNodeStart) : Collider(-3.0F, -3.0F, -3.0F, 3.0F, 3.0F, 3.0F),
    DroneTransform(EngineDefaults::GetShader()), DroneGraph(std::move(baseGraph)),
    Start(DroneGraph.GetNode(idNodeStart)), End(nullptr)
{
    DroneTransform.GetTransform(0).SetPosition(x, y, z);
    GenerateTessellationData();
}

void Drone::GenerateTessellationData()
{
    const uint32_t index1 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMinY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index2 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMinY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index3 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMaxY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index4 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMaxY(), Collider.GetMinZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index5 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMinY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index6 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMinY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index7 = DroneTransform.AddVertex(Vertex(Collider.GetMaxX(), Collider.GetMaxY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    const uint32_t index8 = DroneTransform.AddVertex(Vertex(Collider.GetMinX(), Collider.GetMaxY(), Collider.GetMaxZ(), 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1));
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index8);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index8);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index8);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index3);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index2);
    DroneTransform.AddTriangle(index7);
    DroneTransform.AddTriangle(index6);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index1);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index4);
    DroneTransform.AddTriangle(index5);
    DroneTransform.AddTriangle(index8);
}

void Drone::Tick(const float deltaTime)
{
    if (Path.empty())
    {
        return;
    }
    vec3 currentGoal = Path.front()->GetPosition();
    vec3 currentPos = DroneTransform.GetTransform(0).GetPosition();
    float distanceFactor = 0.0F;
    if (const float temp = distance(currentPos, currentGoal); temp < deltaTime * 0.41665F)
    {
        DroneTransform.GetTransform(0).SetPosition(currentGoal);
        distanceFactor = temp;
        Path.pop_front();
        if (Path.empty())
        {
            return;
        }
        currentGoal = Path.front()->GetPosition();
        currentPos = DroneTransform.GetTransform(0).GetPosition();
    }
    DroneTransform.GetTransform(0).SetPosition(currentPos + normalize(currentGoal - currentPos) * (deltaTime * 0.41665F - distanceFactor));
}

void Drone::Draw()
{
    DroneTransform.Draw();
}

void Drone::SetDestination(const uint32_t idNodeEnd)
{
    End = DroneGraph.GetNode(idNodeEnd);
    PlanPath();
}

AStarNode* Drone::NeighborUpdateAStar(std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarCompare>& cells, std::unordered_map<GraphNode*, unique_ptr<AStarNode>>& cellsSearched, AStarNode* current)
{
    for (const auto& connection : current->Node->GetConnections())
    {
        GraphNode* neighbor = DroneGraph.GetNode(connection);
        try
        {
            if (AStarNode* cell = cellsSearched.at(neighbor).get(); cell->G > current->G + neighbor->Distance(*current->Node))
            {
                cell->G = current->G + neighbor->Distance(*current->Node);
                cell->Parent = current;
            }
        }
        catch (...)
        {
            cellsSearched.emplace(neighbor, new AStarNode(neighbor, current, current->G + neighbor->Distance(*current->Node), neighbor->Distance(*End)));
            cells.emplace(cellsSearched.at(neighbor).get());
            if (neighbor == End)
            {
                return cellsSearched.at(neighbor).get();
            }
        }
    }
    return nullptr;
}

void Drone::PlanPath()
{
    std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarCompare> cellQueue;
    std::unordered_map<GraphNode*, unique_ptr<AStarNode>> cellsSearched;
    cellsSearched.emplace(Start, new AStarNode(Start, nullptr, 0, Start->Distance(*End)));
    cellQueue.emplace(cellsSearched.at(Start).get());
    AStarNode* pathFound = nullptr;
    while (pathFound == nullptr)
    {
        AStarNode* current = cellQueue.top();
        cellQueue.pop();
        pathFound = NeighborUpdateAStar(cellQueue, cellsSearched, current);
    }
    while (pathFound->Parent != nullptr)
    {
        Path.emplace_front(pathFound->Node);
        pathFound = pathFound->Parent;
    }
}

void Drone::DrawPath(TessellationHelper& lineTessellation)
{
    if (Path.empty())
    {
        return;
    }
    uint32_t prevIndex = lineTessellation.AddVertex(Vertex(DroneTransform.GetTransform(0).GetPosition(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 1));
    for (const auto& node : Path)
    {
        const uint32_t currentIndex = lineTessellation.AddVertex(Vertex(node->GetPosition(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 1));
        lineTessellation.AddTriangle(prevIndex);
        lineTessellation.AddTriangle(currentIndex);
        prevIndex = currentIndex;
    }
}
