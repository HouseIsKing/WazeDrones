#include "Graph.h"

#include <ranges>

GraphNode* Graph::GetNode(const uint32_t id)
{
    return Nodes[id].get();
}

/**
 * \brief Adds a node to the graph
 * \param representativeNode Octree node that represents the graph node
 * \return The newly created graph node
 */
GraphNode* Graph::AddNode(const OctreeNode* representativeNode)
{
    uint32_t id;
    if (IdStack.empty())
    {
        id = static_cast<uint32_t>(Nodes.size());
    }
    else
    {
        id = IdStack.top();
        IdStack.pop();
    }
    Nodes.emplace(id, new GraphNode(id, representativeNode->GetCenter()));
    return Nodes[id].get();
}

/**
 * \brief Adds a node to the graph
 * \param position Position of the node
 * \return The newly created graph node
 */
GraphNode* Graph::AddNode(const vec3 position)
{
    uint32_t id;
    if (IdStack.empty())
    {
        id = static_cast<uint32_t>(Nodes.size());
    }
    else
    {
        id = IdStack.top();
        IdStack.pop();
    }
    Nodes.emplace(id, new GraphNode(id, position));
    return Nodes[id].get();
}

/**
 * \brief Removes a node from the graph according to its id
 * \param id Node ID
 */
void Graph::RemoveNode(uint32_t id)
{
    try
    {
        for (uint32_t conn : Nodes.at(id)->GetConnections())
        {
            Nodes.at(conn)->RemoveConnection(id);
        }
        Nodes.erase(id);
        IdStack.emplace(id);
    }
    catch (std::exception&)
    {
    }
}

/**
 * \brief Draws the graph with all it's connections.
 * \param lineTessellation Drawing tool
 */
void Graph::Draw(TessellationHelper& lineTessellation)
{
    unordered_map<GraphNode*, uint32_t> nodeToIndex;
    for (auto& val : Nodes | std::views::values)
    {
        uint32_t index = lineTessellation.AddVertex(Vertex(val->GetPosition(), 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F, 0));
        nodeToIndex.emplace(val.get(), index);
    }
    for (auto& [index, node] : Nodes)
    {
        for (const uint32_t conn : node->GetConnections())
        {
            if (conn > index)
            {
                lineTessellation.AddTriangle(nodeToIndex.at(node.get()));
                lineTessellation.AddTriangle(nodeToIndex.at(GetNode(conn)));
            }
        }
    }
}

const std::unordered_map<uint32_t, std::shared_ptr<GraphNode>>& Graph::GetNodes()
{
    return Nodes;
}
