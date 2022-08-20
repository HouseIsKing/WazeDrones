#include "OctreeNode.h"

#include <memory>

OctreeNode::OctreeNode(const BoundingBox boundary) : Boundary(boundary), ChildrenBounds()
{
    Init(Boundary);
}

OctreeNode::OctreeNode() : Boundary(), ChildrenBounds()
{
}

void OctreeNode::Insert(BoundingBox box)
{
    if (Boundary.GetMaxX() - Boundary.GetMinX() >= MinSize && box.IsIntersecting(Boundary))
    {
        for (size_t i = 0; i < 8; i++)
        {
            if (Children[i] == nullptr)
            {
                Children[i] = std::make_unique<OctreeNode>(ChildrenBounds[i]);
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
    for (const auto& element : Children)
    {
        if (element != nullptr)
        {
            element->Draw(lineTessellation);
        }
    }
}

void OctreeNode::Init(const BoundingBox boundary)
{
    Boundary = boundary;
    const float x = abs(Boundary.GetMinX() - Boundary.GetMaxX()) / 2;
    const float y = abs(Boundary.GetMinY() - Boundary.GetMaxY()) / 2;
    const float z = abs(Boundary.GetMinZ() - Boundary.GetMaxZ()) / 2;
    ChildrenBounds[0] = BoundingBox{boundary.GetMinX(), boundary.GetMinY(), boundary.GetMinZ(), boundary.GetMaxX() - x, boundary.GetMaxY() - y, boundary.GetMaxZ() - z};
    ChildrenBounds[1] = BoundingBox{boundary.GetMinX() + x, boundary.GetMinY(), boundary.GetMinZ(), boundary.GetMaxX(), boundary.GetMaxY() - y, boundary.GetMaxZ() - z};
    ChildrenBounds[2] = BoundingBox{boundary.GetMinX(), boundary.GetMinY() + y, boundary.GetMinZ(), boundary.GetMaxX() - x, boundary.GetMaxY(), boundary.GetMaxZ() - z};
    ChildrenBounds[3] = BoundingBox{boundary.GetMinX() + x, boundary.GetMinY() + y, boundary.GetMinZ(), boundary.GetMaxX(), boundary.GetMaxY(), boundary.GetMaxZ() - z};
    ChildrenBounds[4] = BoundingBox{boundary.GetMinX(), boundary.GetMinY(), boundary.GetMinZ() + z, boundary.GetMaxX() - x, boundary.GetMaxY() - y, boundary.GetMaxZ()};
    ChildrenBounds[5] = BoundingBox{boundary.GetMinX() + x, boundary.GetMinY(), boundary.GetMinZ() + z, boundary.GetMaxX(), boundary.GetMaxY() - y, boundary.GetMaxZ()};
    ChildrenBounds[6] = BoundingBox{boundary.GetMinX(), boundary.GetMinY() + y, boundary.GetMinZ() + z, boundary.GetMaxX() - x, boundary.GetMaxY(), boundary.GetMaxZ()};
    ChildrenBounds[7] = BoundingBox{boundary.GetMinX() + x, boundary.GetMinY() + y, boundary.GetMinZ() + z, boundary.GetMaxX(), boundary.GetMaxY(), boundary.GetMaxZ()};
    Children.clear();
    Children.reserve(8);
    for (int i = 0; i < 8; i++)
    {
        Children.emplace_back(nullptr);
    }
}
