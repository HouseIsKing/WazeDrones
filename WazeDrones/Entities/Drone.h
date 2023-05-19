#pragma once
#include <forward_list>
#include <queue>
#include <set>
#include <unordered_set>

#include "../Util/AStarNode.h"
class WorldManager;
struct AStarNode;

class Drone
{
    BoundingBox Collider;
    TessellationHelper DroneTransform;
    WorldManager* World;
    GraphNode* Start;
    GraphNode* End;
    std::unordered_map<OctreeNode*, bool> OctreeList; //Reperesnts which octree nodes the drone is inside.
    std::unordered_map<uint16_t, Drone*> DroneList; //Represents which drones the drone might collide with.
    float YNeeded = 0.0F;
    bool GoingUp = false;
    //vec3 Velocity;
    std::forward_list<GraphNode*> Path;
    unique_ptr<GraphNode> Temp;
    vec3 PreviousTarget;
    uint16_t Id;
    bool IsRemovingFromWorld = false;
    float ExpectedTimeTravel = 0.0F;
    float TimeTravel = 0.0F;
    void GenerateTessellationData();
    AStarNode* NeighborUpdateAStar(std::set<AStarNode*, AStarCompare>& cells, std::unordered_map<GraphNode*, unique_ptr<AStarNode>>& cellsSearched, AStarNode
                                   * current, bool useWeights) const;

public:
    Drone(WorldManager* worldManager, uint32_t idNodeStart, uint16_t id);
    void Tick(float deltaTime);
    void Draw();
    void SetDestination(uint32_t idNodeEnd);
    void PlanPath(bool expectedTimeCalculation);
    void DrawPath(TessellationHelper& lineTessellation);
    void AddDroneCollision(Drone* drone);
    void RemoveDroneCollision(const Drone* drone);
    float GetDistanceFromNoY(const vec3& point);
    [[nodiscard]] uint16_t GetId() const;
    [[nodiscard]] vec3 GetPosition();
    [[nodiscard]] int GetNumLeavesDroneAt() const;
    void DroneRemoved(const Drone* drone);
};
