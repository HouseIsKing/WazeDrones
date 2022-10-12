#include "GraphNode.h"

#include "Graph.h"

GraphNode::GraphNode(Graph* graph, const uint32_t id, const OctreeNode* octreeNodeRepresentative) : OctreeNodeRepresentative(octreeNodeRepresentative), Root(graph), Id(id)
{
}

void GraphNode::AddConnection(const uint32_t node)
{
    Connections.push_back(node);
}

void GraphNode::RemoveConnection(const uint32_t node)
{
    std::erase(Connections, node);
}

uint32_t GraphNode::GetId() const
{
    return Id;
}

const OctreeNode* GraphNode::GetOctreeNodeRepresentative() const
{
    return OctreeNodeRepresentative;
}

const std::vector<uint32_t>& GraphNode::GetConnections() const
{
    return Connections;
}
