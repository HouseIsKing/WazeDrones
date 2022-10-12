#pragma once
#include <vector>

#include "OctreeNode.h"

class Graph;

class GraphNode
{
    const OctreeNode* OctreeNodeRepresentative;
    Graph* Root;
    const uint32_t Id;
    std::vector<uint32_t> Connections;
public:
    GraphNode(Graph* graph, uint32_t id, const OctreeNode* octreeNodeRepresentative);
    GraphNode(const GraphNode& other) = delete;
    GraphNode(GraphNode&& other) = delete;
    GraphNode& operator=(const GraphNode& other) = delete;
    GraphNode& operator=(GraphNode&& other) = delete;
    void AddConnection(uint32_t node);
    void RemoveConnection(uint32_t node);
    [[nodiscard]] uint32_t GetId() const;
    [[nodiscard]] const OctreeNode* GetOctreeNodeRepresentative() const;
    [[nodiscard]] const std::vector<uint32_t>& GetConnections() const;
    ~GraphNode() = default;
};
