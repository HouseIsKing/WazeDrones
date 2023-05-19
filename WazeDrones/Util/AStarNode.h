#pragma once
#include "GraphNode.h"

struct AStarNode
{
    AStarNode* Parent;
    GraphNode* Node;
    float G;
    float H;
    float SpecialValue = 0.0F;

    AStarNode(GraphNode* node, AStarNode* parent, const float g, const float h) : Parent(parent), Node(node), G(g), H(h)
    {
    }
};

struct AStarCompare
{
    bool operator()(const AStarNode* lhs, const AStarNode* rhs) const
    {
        return lhs->G + lhs->H + lhs->SpecialValue < rhs->G + rhs->H + lhs->SpecialValue;
    }
};
