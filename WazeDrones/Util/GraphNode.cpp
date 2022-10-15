#include "GraphNode.h"

GraphNode::GraphNode(const uint32_t id, const vec3 position) : GraphNodePosition(position), Id(id)
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

const std::vector<uint32_t>& GraphNode::GetConnections() const
{
    return Connections;
}

const vec3& GraphNode::GetPosition() const
{
    return GraphNodePosition;
}
