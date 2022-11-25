#pragma once
#include <forward_list>
#include <queue>

#include "../Util/AStarNode.h"
#include "../Util/Graph.h"
struct AStarNode;

class Drone
{
    BoundingBox Collider;
    TessellationHelper DroneTransform;
    Graph DroneGraph;
    GraphNode* Start;
    GraphNode* End;
    //vec3 Velocity;
    std::forward_list<const GraphNode*> Path;
    void GenerateTessellationData();
    AStarNode* NeighborUpdateAStar(std::priority_queue<AStarNode*, std::vector<AStarNode*>, AStarCompare>& cells, std::unordered_map<GraphNode*, unique_ptr<AStarNode>>& cellsSearched, AStarNode* current);
public:
    Drone(Graph baseGraph, float x, float y, float z, uint32_t idNodeStart);
    void Tick(float deltaTime);
    void Draw();
    void SetDestination(uint32_t idNodeEnd);
    void PlanPath();
    void DrawPath(TessellationHelper& lineTessellation);
};
