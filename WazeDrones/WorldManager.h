#pragma once
#include "Entities/Drone.h"
#include "Util/Graph.h"
#include "Util/OctreeNode.h"

class WorldManager
{
    OctreeNode Root;
    Graph MainGraph;
    std::vector<unique_ptr<Drone>> DroneList;

public:
    WorldManager();
    [[nodiscard]] OctreeNode* GetRoot();
    [[nodiscard]] Graph* GetGraph();
    void Tick(double tickTimer) const;
    void AddDrone(Drone* drone);
    void Draw() const;
};
