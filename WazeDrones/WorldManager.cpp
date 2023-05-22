#include "WorldManager.h"

#include "Util/CustomRandomEngine.h"
#include "Util/EngineDefaults.h"

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
    for (const auto& i : DroneList | std::views::values)
    {
        i->Tick(static_cast<float>(tickTimer * 20.0));
    }
    for (const auto& i : DroneListToRemove)
    {
        std::lock_guard lock(DroneListMutex);
        for (const auto& drone : DroneList | std::views::values)
        {
            drone->DroneRemoved(DroneList.at(i).get());
        }
        DroneList.erase(i);
        DroneIdPool.push(i);
    }
    DroneListToRemove.clear();
}

void WorldManager::AddDrone(Drone* drone)
{
    std::lock_guard lock(DroneListMutex);
    DroneList.emplace(drone->GetId(), drone);
}

void WorldManager::RemoveDrone(const uint16_t id)
{
    DroneListToRemove.push_back(id);
}

void WorldManager::DroneArrived(const float timePercentage)
{
    AverageTimePercentage += timePercentage;
    DroneArrivedCount++;
}

void WorldManager::Draw() const
{
    for (const auto& i : DroneList | std::views::values)
    {
        i->Draw();
    }
}

void WorldManager::DrawStats() const
{
    const auto stat = AverageTimePercentage / static_cast<float>(DroneArrivedCount);
    std::cout << "Average time percentage: " << stat << "%" << std::endl;
}

size_t WorldManager::GetDroneCount()
{
    std::lock_guard lock(DroneListMutex);
    return DroneList.size();
}

vec3 WorldManager::GetRandomDronePosition()
{
    std::lock_guard lock(DroneListMutex);
    auto iterator = DroneList.cbegin();
    std::advance(iterator, static_cast<uint16_t>(EngineDefaults::GetNextInt(static_cast<int>(DroneList.size()))));
    return iterator->second->GetPosition();
}
