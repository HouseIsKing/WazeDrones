#pragma once
#include <memory>
#include <stack>
#include <unordered_map>

#include "GraphNode.h"

class Graph
{
    std::stack<uint32_t> IdStack;
    std::unordered_map<uint32_t, std::shared_ptr<GraphNode>> Nodes;
public:
    GraphNode* GetNode(uint32_t id);
    GraphNode* AddNode(const OctreeNode* representativeNode);
    GraphNode* AddNode(vec3 position);
    void RemoveNode(uint32_t id);
    void Draw(TessellationHelper& lineTessellation);
    [[nodiscard]] const std::unordered_map<uint32_t, std::shared_ptr<GraphNode>>& GetNodes();
};
