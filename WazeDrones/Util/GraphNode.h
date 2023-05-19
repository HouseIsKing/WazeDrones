#pragma once
#include <vector>

#include "OctreeNode.h"

class GraphNode
{
    const vec3 GraphNodePosition;
    const uint32_t Id;
    std::vector<uint32_t> Connections;

public:
    int AmountOfDronesPassingThrough;
    GraphNode(uint32_t id, vec3 position);
    GraphNode(const GraphNode& other) = delete;
    GraphNode(GraphNode&& other) = delete;
    GraphNode& operator=(const GraphNode& other) = delete;
    GraphNode& operator=(GraphNode&& other) = delete;
    void AddConnection(uint32_t node);
    void RemoveConnection(uint32_t node);
    [[nodiscard]] uint32_t GetId() const;
    [[nodiscard]] const std::vector<uint32_t>& GetConnections() const;
    [[nodiscard]] const vec3& GetPosition() const;
    [[nodiscard]] float Distance(const GraphNode& other) const;
    ~GraphNode() = default;
};
