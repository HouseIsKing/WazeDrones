#pragma once
#include <forward_list>

#include "../Util/AStarNode.h"
#include "../Util/Graph.h"
#include "../Util/Transform.h"
struct AStarNode;

class Drone
{
    BoundingBox Collider;
    TessellationHelper DroneTransform;
    Graph DroneGraph;
    GraphNode* Start;
    GraphNode* End;
    vec3 Destination;
    //vec3 Velocity;
    std::forward_list<const GraphNode*> Path;
    void GenerateTessellationData();
    void SetupPath();
    AStarNode* NeighborUpdateAStar(std::unordered_map<GraphNode*, AStarNode*>& cellsSearched, AStarNode* current);
public:
    Drone(Graph baseGraph, float x, float y, float z);
    void Tick(float deltaTime);
    void Draw();
    void SetDestination(vec3 destination);
    void PlanPath();
    void DrawPath(TessellationHelper& lineTessellation);
};
