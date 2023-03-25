#pragma once
#include <forward_list>
#include <queue>
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
    std::unordered_set<Drone*> DroneList; //Represents which drones the drone might collide with.
    float YNeeded = 0.0F;
    bool GoingUp = false;
    //vec3 Velocity;
    std::forward_list<const GraphNode*> Path;
    unique_ptr<GraphNode> Temp;
    vec3 PreviousTarget;
    uint16_t Id;
    bool IsRemovingFromWorld = false;
    void GenerateTessellationData();
    AStarNode* NeighborUpdateAStar(std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarCompare>& cells, std::unordered_map<GraphNode*, unique_ptr<AStarNode>>& cellsSearched, AStarNode* current) const;

public:
    Drone(WorldManager* worldManager, uint32_t idNodeStart, uint16_t id);
    void Tick(float deltaTime);
    void Draw();
    void SetDestination(uint32_t idNodeEnd);
    void PlanPath();
    void DrawPath(TessellationHelper& lineTessellation);
    void AddDroneCollision(Drone* drone);
    void RemoveDroneCollision(Drone* drone);
    float GetDistanceFromNoY(const vec3& point);
    [[nodiscard]] uint16_t GetId() const;
};
