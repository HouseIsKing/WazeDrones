#pragma once
#include <forward_list>

#include "../Util/Graph.h"
#include "../Util/Transform.h"

class Drone
{
    constexpr BoundingBox Size = {0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F};
    BoundingBox Collider;
    Transform DroneTransform;
    Graph DroneGraph;
    GraphNode* Start;
    GraphNode* End;
    vec3 Destination;
    //vec3 Velocity;
    std::forward_list<GraphNode*> Path;
    void SetupPath();
public:
    explicit Drone(Graph baseGraph);
    void Draw(TessellationHelper& tessellationHelper);
    void Tick(float deltaTime);
    void SetDestination(vec3 destination);
    void PlanPath();
};
