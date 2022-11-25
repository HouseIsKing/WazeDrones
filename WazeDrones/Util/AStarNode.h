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

struct AStarCompare
{
    bool operator()(const AStarNode* lhs, const AStarNode* rhs) const
    {
        return lhs->G + lhs->H > rhs->G + rhs->H;
    }
};
