#include "OctreeNode.h"

#include <memory>

BoundingBox OctreeNode::GetChildrenBound(const BoundingBox& boundary, const size_t boundingBoxIndex)
{
    const float x = abs(boundary.GetMinX() - boundary.GetMaxX()) / 2;
    const float y = abs(boundary.GetMinY() - boundary.GetMaxY()) / 2;
    const float z = abs(boundary.GetMinZ() - boundary.GetMaxZ()) / 2;
    switch (boundingBoxIndex)
    {
    case 0:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY(), boundary.GetMinZ(), boundary.GetMaxX() - x, boundary.GetMaxY() - y, boundary.GetMaxZ() - z};
    case 1:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY(), boundary.GetMinZ(), boundary.GetMaxX(), boundary.GetMaxY() - y, boundary.GetMaxZ() - z};
    case 2:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY() + y, boundary.GetMinZ(), boundary.GetMaxX() - x, boundary.GetMaxY(), boundary.GetMaxZ() - z};
    case 3:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY() + y, boundary.GetMinZ(), boundary.GetMaxX(), boundary.GetMaxY(), boundary.GetMaxZ() - z};
    case 4:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY(), boundary.GetMinZ() + z, boundary.GetMaxX() - x, boundary.GetMaxY() - y, boundary.GetMaxZ()};
    case 5:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY(), boundary.GetMinZ() + z, boundary.GetMaxX(), boundary.GetMaxY() - y, boundary.GetMaxZ()};
    case 6:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY() + y, boundary.GetMinZ() + z, boundary.GetMaxX() - x, boundary.GetMaxY(), boundary.GetMaxZ()};
    case 7:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY() + y, boundary.GetMinZ() + z, boundary.GetMaxX(), boundary.GetMaxY(), boundary.GetMaxZ()};
    default:
        return BoundingBox{};
    }
}

OctreeNode::OctreeNode(const BoundingBox boundary) : Boundary(boundary)
{
    Init(Boundary);
}

OctreeNode::OctreeNode() : Boundary()
{
}

void OctreeNode::Insert(const BoundingBox& box)
{
    if (Boundary.GetMaxX() - Boundary.GetMinX() >= MinSize && box.IsIntersecting(Boundary))
    {
        for (size_t i = 0; i < 8; i++)
        {
            if (Children[i] == nullptr)
            {
                Children[i] = std::make_unique<OctreeNode>(GetChildrenBound(Boundary, i));
            }
            Children[i]->Insert(box);
        }
    }
}

BoundingBox OctreeNode::GetBoundary() const
{
    return Boundary;
}

void OctreeNode::Draw(TessellationHelper& lineTessellation) const
{
    if (Children[0] != nullptr)
    {
        for (size_t i = 0; i < 8; i++)
        {
            Children[i]->Draw(lineTessellation);
        }
    }
    else
    {
        const uint32_t index1 = lineTessellation.AddVertex(Vertex{Boundary.GetMinX(), Boundary.GetMinY(), Boundary.GetMinZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        const uint32_t index2 = lineTessellation.AddVertex(Vertex{Boundary.GetMaxX(), Boundary.GetMinY(), Boundary.GetMinZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        const uint32_t index3 = lineTessellation.AddVertex(Vertex{Boundary.GetMinX(), Boundary.GetMaxY(), Boundary.GetMinZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        const uint32_t index4 = lineTessellation.AddVertex(Vertex{Boundary.GetMaxX(), Boundary.GetMaxY(), Boundary.GetMinZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        const uint32_t index5 = lineTessellation.AddVertex(Vertex{Boundary.GetMinX(), Boundary.GetMinY(), Boundary.GetMaxZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        const uint32_t index6 = lineTessellation.AddVertex(Vertex{Boundary.GetMaxX(), Boundary.GetMinY(), Boundary.GetMaxZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        const uint32_t index7 = lineTessellation.AddVertex(Vertex{Boundary.GetMinX(), Boundary.GetMaxY(), Boundary.GetMaxZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        const uint32_t index8 = lineTessellation.AddVertex(Vertex{Boundary.GetMaxX(), Boundary.GetMaxY(), Boundary.GetMaxZ(), 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0});
        lineTessellation.AddTriangle(index1);
        lineTessellation.AddTriangle(index2);
        lineTessellation.AddTriangle(index1);
        lineTessellation.AddTriangle(index3);
        lineTessellation.AddTriangle(index1);
        lineTessellation.AddTriangle(index5);
        lineTessellation.AddTriangle(index2);
        lineTessellation.AddTriangle(index4);
        lineTessellation.AddTriangle(index2);
        lineTessellation.AddTriangle(index6);
        lineTessellation.AddTriangle(index3);
        lineTessellation.AddTriangle(index4);
        lineTessellation.AddTriangle(index3);
        lineTessellation.AddTriangle(index7);
        lineTessellation.AddTriangle(index4);
        lineTessellation.AddTriangle(index8);
        lineTessellation.AddTriangle(index5);
        lineTessellation.AddTriangle(index6);
        lineTessellation.AddTriangle(index5);
        lineTessellation.AddTriangle(index7);
        lineTessellation.AddTriangle(index6);
        lineTessellation.AddTriangle(index8);
        lineTessellation.AddTriangle(index7);
        lineTessellation.AddTriangle(index8);
    }
}

void OctreeNode::Init(const BoundingBox& boundary)
{
    Boundary = boundary;
    Children.clear();
    Children.reserve(8);
    for (int i = 0; i < 8; i++)
    {
        Children.emplace_back(nullptr);
    }
}
