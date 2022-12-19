#include "WorldManager.h"

WorldManager::WorldManager() : Root(nullptr)
{
}

OctreeNode* WorldManager::GetRoot()
{
    return &Root;
}

Graph* WorldManager::GetGraph()
{
    return &MainGraph;
}

void WorldManager::Tick(const double tickTimer) const
{
    for (const auto& i : DroneList)
    {
        i->Tick(static_cast<float>(tickTimer * 20.0));
    }
}

void WorldManager::AddDrone(Drone* drone)
{
    DroneList.emplace_back(drone);
}

void WorldManager::Draw() const
{
    for (const auto& i : DroneList)
    {
        i->Draw();
    }
}
