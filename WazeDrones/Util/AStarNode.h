#pragma once
#include "GraphNode.h"

struct AStarNode
{
    AStarNode* Parent;
    const GraphNode* Node;
    float G;
    float H;

    AStarNode(const GraphNode* node, AStarNode* parent, const float g, const float h) : Parent(parent), Node(node), G(g), H(h)
    {
    }
};
