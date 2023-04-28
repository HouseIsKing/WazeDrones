#pragma once
#include <map>
#include <mutex>

#include "Entities/Drone.h"
#include "Util/Graph.h"
#include "Util/OctreeNode.h"

class WorldManager
{
    OctreeNode Root;
    Graph MainGraph;
    std::stack<uint16_t> DroneIdPool;
    std::map<uint16_t, unique_ptr<Drone>> DroneList;
    std::vector<uint16_t> DroneListToRemove;
    std::mutex DroneListMutex;
    float AverageTimePercentage = 0.0F;
    uint32_t DroneArrivedCount = 0;

public:
    WorldManager();
    uint16_t GetNextAvailableDroneId();
    [[nodiscard]] OctreeNode* GetRoot();
    [[nodiscard]] Graph* GetGraph();
    void Tick(double tickTimer);
    void AddDrone(Drone* drone);
    void RemoveDrone(uint16_t id);
    void DroneArrived(float timePercentage);
    void Draw() const;
    [[nodiscard]] size_t GetDroneCount();
    [[nodiscard]] vec3 GetRandomDronePosition();
};
