#include "WorldManager.h"

WorldManager::WorldManager() : Root(nullptr)
{
    for (int i = 0xFFFF; i >= 0; i--)
    {
        DroneIdPool.push(static_cast<uint16_t>(i));
    }
}

uint16_t WorldManager::GetNextAvailableDroneId()
{
    const uint16_t id = DroneIdPool.top();
    DroneIdPool.pop();
    return id;
}

OctreeNode* WorldManager::GetRoot()
{
    return &Root;
}

Graph* WorldManager::GetGraph()
{
    return &MainGraph;
}

void WorldManager::Tick(const double tickTimer)
{
    for (const auto& i : DroneListToRemove)
    {
        DroneList.erase(i);
        DroneIdPool.push(i);
    }
    for (const auto& i : DroneList | std::views::values)
    {
        i->Tick(static_cast<float>(tickTimer * 20.0));
    }
}

void WorldManager::AddDrone(Drone* drone)
{
    DroneList.emplace(drone->GetId(), drone);
}

void WorldManager::RemoveDrone(const uint16_t id)
{
    DroneListToRemove.push_back(id);
}

void WorldManager::Draw() const
{
    for (const auto& i : DroneList | std::views::values)
    {
        i->Draw();
    }
}
