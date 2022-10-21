#include "Drone.h"

#include <ranges>

#include "../Util/EngineDefaults.h"

Drone::Drone(Graph baseGraph, float x, float y, float z) : Collider(-3.0F, -3.0F, -3.0F, 3.0F, 3.0F, 3.0F),
                                                           DroneTransform(EngineDefaults::GetShader()), DroneGraph(std::move(baseGraph)),
                                                           Start(nullptr), End(nullptr), Destination()
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

void Drone::SetDestination(const vec3 destination)
{
    Destination = destination;
    PlanPath();
}

void Drone::SetupPath()
{
    if (Start != nullptr)
    {
        DroneGraph.RemoveNode(Start->GetId());
    }
    if (End != nullptr)
    {
        DroneGraph.RemoveNode(End->GetId());
    }
    Start = DroneGraph.AddNode(DroneTransform.GetTransform(0).GetPosition());
    End = DroneGraph.AddNode(Destination);
    float minDistanceStart = std::numeric_limits<float>::max();
    float minDistanceEnd = std::numeric_limits<float>::max();
    const GraphNode* closestNodeStart = nullptr;
    GraphNode* closestNodeEnd = nullptr;
    for (const auto& node : DroneGraph.GetNodes() | std::views::values)
    {
        if (node.get() != Start)
        {
            if (vec3 temp = node->GetPosition() - Start->GetPosition(); dot(temp, temp) < minDistanceStart)
            {
                minDistanceStart = dot(temp, temp);
                closestNodeStart = node.get();
            }
        }
        if (node.get() != End)
        {
            if (vec3 temp = node->GetPosition() - End->GetPosition(); dot(temp, temp) < minDistanceEnd)
            {
                minDistanceEnd = dot(temp, temp);
                closestNodeEnd = node.get();
            }
        }
    }
    Start->AddConnection(closestNodeStart->GetId());
    closestNodeEnd->AddConnection(End->GetId());
}

AStarNode* Drone::NeighborUpdateAStar(std::unordered_map<GraphNode*, AStarNode*>& cellsSearched, AStarNode* current)
{
    for (const auto& connection : current->Node->GetConnections())
    {
        GraphNode* neighbor = DroneGraph.GetNode(connection);
        try
        {
            AStarNode* cell = cellsSearched.at(neighbor);
            if (cell->G > current->G + neighbor->Distance(*current->Node))
            {
                cell->G = current->G + neighbor->Distance(*current->Node);
                cell->Parent = current;
            }
        }
        catch (...)
        {
            cellsSearched.emplace(neighbor, new AStarNode(neighbor, current, current->G + neighbor->Distance(*current->Node), neighbor->Distance(*End)));
            if (neighbor == End)
            {
                return cellsSearched.at(neighbor);
            }
        }
    }
    return nullptr;
}

void Drone::PlanPath()
{
    SetupPath();
    std::vector<unique_ptr<AStarNode>> cells;
    std::unordered_map<GraphNode*, AStarNode*> cellsSearched;
    cells.emplace_back(new AStarNode(Start, nullptr, 0, Start->Distance(*End)));
    cellsSearched.emplace(Start, cells.back().get());
    AStarNode* current = nullptr;
    AStarNode* pathFound = nullptr;
    while (pathFound == nullptr)
    {
        float minScore = std::numeric_limits<float>::max();
        for (const auto& cell : cellsSearched | std::views::values)
        {
            if (cell->G + cell->H < minScore)
            {
                minScore = cell->G + cell->H;
                current = cell;
            }
        }
        const GraphNode* temp = current->Node;
        pathFound = NeighborUpdateAStar(cellsSearched, current);
        cellsSearched.erase(const_cast<GraphNode*>(temp));
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
