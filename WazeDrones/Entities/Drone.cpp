#include "Drone.h"

#include <ranges>

Drone::Drone(Graph baseGraph) : DroneGraph(std::move(baseGraph)), Start(nullptr), End(nullptr)
{
}

void Drone::SetDestination(const vec3 destination)
{
    Destination = destination;
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
    Start = DroneGraph.AddNode(DroneTransform.GetPosition());
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

void Drone::Neighbor(const GraphNode* node)

void Drone::PlanPath()
{
    SetupPath();
    
}
