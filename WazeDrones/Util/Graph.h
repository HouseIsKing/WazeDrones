#pragma once
#include <memory>
#include <stack>
#include <unordered_map>

#include "GraphNode.h"

class Graph
{
    std::stack<uint32_t> IdStack;
    std::unordered_map<uint32_t, std::unique_ptr<GraphNode>> Nodes;
public:
    GraphNode* GetNode(uint32_t id);
    GraphNode* AddNode(const OctreeNode* representativeNode);
    void RemoveNode(uint32_t id);
    void Draw(TessellationHelper& lineTessellation);
};
