#include "OctreeNode.h"

#include <memory>
#include <ranges>

BoundingBox OctreeNode::GetChildrenBound(const BoundingBox& boundary, const int boundingBoxIndex)
{
    const float x = abs(boundary.GetMinX() - boundary.GetMaxX()) / 2;
    const float y = abs(boundary.GetMinY() - boundary.GetMaxY()) / 2;
    const float z = abs(boundary.GetMinZ() - boundary.GetMaxZ()) / 2;
    switch (boundingBoxIndex)
    {
    case LeftBottomBack:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY(), boundary.GetMinZ(), boundary.GetMaxX() - x, boundary.GetMaxY() - y, boundary.GetMaxZ() - z};
    case RightBottomBack:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY(), boundary.GetMinZ(), boundary.GetMaxX(), boundary.GetMaxY() - y, boundary.GetMaxZ() - z};
    case LeftTopBack:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY() + y, boundary.GetMinZ(), boundary.GetMaxX() - x, boundary.GetMaxY(), boundary.GetMaxZ() - z};
    case RightTopBack:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY() + y, boundary.GetMinZ(), boundary.GetMaxX(), boundary.GetMaxY(), boundary.GetMaxZ() - z};
    case LeftBottomFront:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY(), boundary.GetMinZ() + z, boundary.GetMaxX() - x, boundary.GetMaxY() - y, boundary.GetMaxZ()};
    case RightBottomFront:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY(), boundary.GetMinZ() + z, boundary.GetMaxX(), boundary.GetMaxY() - y, boundary.GetMaxZ()};
    case LeftTopFront:
        return BoundingBox{boundary.GetMinX(), boundary.GetMinY() + y, boundary.GetMinZ() + z, boundary.GetMaxX() - x, boundary.GetMaxY(), boundary.GetMaxZ()};
    case RightTopFront:
        return BoundingBox{boundary.GetMinX() + x, boundary.GetMinY() + y, boundary.GetMinZ() + z, boundary.GetMaxX(), boundary.GetMaxY(), boundary.GetMaxZ()};
    default:
        return BoundingBox{};
    }
}

Position OctreeNode::GetPosition() const
{
    if (Parent != nullptr)
    {
        for (auto& [fst, snd] : Parent->Children)
        {
            if (snd.get() == this)
            {
                return fst;
            }
        }
    }
    return Position{};
}

OctreeNode* OctreeNode::GetNeighborGreaterOrEqualLeft() const
{
    if (Parent == nullptr)
    {
        return nullptr;
    }
    switch (const Position myPosition = GetPosition())
    {
    case RightBottomBack:
        return Parent->Children[LeftBottomBack].get();
    case RightBottomFront:
        return Parent->Children[LeftBottomFront].get();
    case RightTopBack:
        return Parent->Children[LeftTopBack].get();
    case RightTopFront:
        return Parent->Children[LeftTopFront].get();
    default:
        OctreeNode* parentNeighbor = Parent->GetNeighborGreaterOrEqualLeft();
        if (parentNeighbor == nullptr || parentNeighbor->Children[LeftBottomBack] == nullptr)
        {
            return parentNeighbor;
        }
        switch (myPosition)
        {
        case LeftBottomFront:
            return parentNeighbor->Children[RightBottomFront].get();
        case LeftTopBack:
            return parentNeighbor->Children[RightTopBack].get();
        case LeftTopFront:
            return parentNeighbor->Children[RightTopFront].get();
        case LeftBottomBack:
            return parentNeighbor->Children[RightBottomBack].get();
        default:
            return nullptr;
        }
    }
}

OctreeNode* OctreeNode::GetNeighborGreaterOrEqualRight() const
{
    if (Parent == nullptr)
    {
        return nullptr;
    }
    switch (const Position myPosition = GetPosition())
    {
    case LeftBottomBack:
        return Parent->Children[RightBottomBack].get();
    case LeftBottomFront:
        return Parent->Children[RightBottomFront].get();
    case LeftTopBack:
        return Parent->Children[RightTopBack].get();
    case LeftTopFront:
        return Parent->Children[RightTopFront].get();
    default:
        OctreeNode* parentNeighbor = Parent->GetNeighborGreaterOrEqualRight();
        if (parentNeighbor == nullptr || parentNeighbor->Children[LeftBottomBack] == nullptr)
        {
            return parentNeighbor;
        }
        switch (myPosition)
        {
        case RightBottomFront:
            return parentNeighbor->Children[LeftBottomFront].get();
        case RightTopBack:
            return parentNeighbor->Children[LeftTopBack].get();
        case RightTopFront:
            return parentNeighbor->Children[LeftTopFront].get();
        case RightBottomBack:
            return parentNeighbor->Children[LeftBottomBack].get();
        default:
            return nullptr;
        }
    }
}

OctreeNode* OctreeNode::GetNeighborGreaterOrEqualUp() const
{
    if (Parent == nullptr)
    {
        return nullptr;
    }
    switch (const Position myPosition = GetPosition())
    {
    case LeftBottomBack:
        return Parent->Children[LeftTopBack].get();
    case LeftBottomFront:
        return Parent->Children[LeftTopFront].get();
    case RightBottomBack:
        return Parent->Children[RightTopBack].get();
    case RightBottomFront:
        return Parent->Children[RightTopFront].get();
    default:
        OctreeNode* parentNeighbor = Parent->GetNeighborGreaterOrEqualUp();
        if (parentNeighbor == nullptr || parentNeighbor->Children[LeftBottomBack] == nullptr)
        {
            return parentNeighbor;
        }
        switch (myPosition)
        {
        case LeftTopFront:
            return parentNeighbor->Children[LeftBottomFront].get();
        case RightTopBack:
            return parentNeighbor->Children[RightBottomBack].get();
        case RightTopFront:
            return parentNeighbor->Children[RightBottomFront].get();
        case LeftTopBack:
            return parentNeighbor->Children[LeftBottomBack].get();
        default:
            return nullptr;
        }
    }
}

OctreeNode* OctreeNode::GetNeighborGreaterOrEqualDown() const
{
    if (Parent == nullptr)
    {
        return nullptr;
    }
    switch (const Position myPosition = GetPosition())
    {
    case LeftTopBack:
        return Parent->Children[LeftBottomBack].get();
    case LeftTopFront:
        return Parent->Children[LeftBottomFront].get();
    case RightTopBack:
        return Parent->Children[RightBottomBack].get();
    case RightTopFront:
        return Parent->Children[RightBottomFront].get();
    default:
        OctreeNode* parentNeighbor = Parent->GetNeighborGreaterOrEqualDown();
        if (parentNeighbor == nullptr || parentNeighbor->Children[LeftBottomBack] == nullptr)
        {
            return parentNeighbor;
        }
        switch (myPosition)
        {
        case LeftBottomFront:
            return parentNeighbor->Children[LeftTopFront].get();
        case RightBottomBack:
            return parentNeighbor->Children[RightTopBack].get();
        case RightBottomFront:
            return parentNeighbor->Children[RightTopFront].get();
        case LeftBottomBack:
            return parentNeighbor->Children[LeftTopBack].get();
        default:
            return nullptr;
        }
    }
}

OctreeNode* OctreeNode::GetNeighborGreaterOrEqualFront() const
{
    if (Parent == nullptr)
    {
        return nullptr;
    }
    switch (const Position myPosition = GetPosition())
    {
    case LeftBottomBack:
        return Parent->Children[LeftBottomFront].get();
    case RightBottomBack:
        return Parent->Children[RightBottomFront].get();
    case LeftTopBack:
        return Parent->Children[LeftTopFront].get();
    case RightTopBack:
        return Parent->Children[RightTopFront].get();
    default:
        OctreeNode* parentNeighbor = Parent->GetNeighborGreaterOrEqualFront();
        if (parentNeighbor == nullptr || parentNeighbor->Children[LeftBottomBack] == nullptr)
        {
            return parentNeighbor;
        }
        switch (myPosition)
        {
        case RightBottomFront:
            return parentNeighbor->Children[RightBottomBack].get();
        case RightTopFront:
            return parentNeighbor->Children[RightTopBack].get();
        case LeftTopFront:
            return parentNeighbor->Children[LeftTopBack].get();
        case LeftBottomFront:
            return parentNeighbor->Children[LeftBottomBack].get();
        default:
            return nullptr;
        }
    }
}

OctreeNode* OctreeNode::GetNeighborGreaterOrEqualBack() const
{
    if (Parent == nullptr)
    {
        return nullptr;
    }
    switch (const Position myPosition = GetPosition())
    {
    case LeftBottomFront:
        return Parent->Children[LeftBottomBack].get();
    case RightBottomFront:
        return Parent->Children[RightBottomBack].get();
    case LeftTopFront:
        return Parent->Children[LeftTopBack].get();
    case RightTopFront:
        return Parent->Children[RightTopBack].get();
    default:
        OctreeNode* parentNeighbor = Parent->GetNeighborGreaterOrEqualBack();
        if (parentNeighbor == nullptr || parentNeighbor->Children[LeftBottomBack] == nullptr)
        {
            return parentNeighbor;
        }
        switch (myPosition)
        {
        case RightBottomBack:
            return parentNeighbor->Children[RightBottomFront].get();
        case RightTopBack:
            return parentNeighbor->Children[RightTopFront].get();
        case LeftTopBack:
            return parentNeighbor->Children[LeftTopFront].get();
        case LeftBottomBack:
            return parentNeighbor->Children[LeftBottomFront].get();
        default:
            return nullptr;
        }
    }
}

std::vector<OctreeNode*> OctreeNode::GetAllLeavesNotColliding()
{
    std::vector<OctreeNode*> leaves;
    if (Children[LeftBottomFront] == nullptr)
    {
        if (!IsColliding)
        {
            leaves.push_back(this);
        }
    }
    else
    {
        for (const auto& val : Children | std::views::values)
        {
            auto childLeaves = val->GetAllLeavesNotColliding();
            leaves.insert(leaves.end(), childLeaves.begin(), childLeaves.end());
        }
    }
    return leaves;
}

OctreeNode::OctreeNode(const BoundingBox boundary, OctreeNode* parent) : Parent(parent), Boundary(boundary), IsColliding(false)
{
    Init(Boundary);
}

OctreeNode::OctreeNode(OctreeNode* parent) : Parent(parent), Boundary(), IsColliding(false)
{
}

bool OctreeNode::operator<(const OctreeNode& other) const
{
    return Boundary.GetVolume() < other.Boundary.GetVolume();
}

void OctreeNode::Insert(const BoundingBox& box)
{
    if (box.IsIntersecting(Boundary))
    {
        if (Boundary.GetMaxX() - Boundary.GetMinX() >= MinSize)
        {
            for (int p = LeftBottomFront; p <= RightTopBack; p++)
            {
                if (Children.at(static_cast<Position>(p)) == nullptr)
                {
                    Children[static_cast<Position>(p)] = std::make_unique<OctreeNode>(GetChildrenBound(Boundary, p), this);
                }
                Children[static_cast<Position>(p)]->Insert(box);
            }
        }
        IsColliding = true;
    }
}

BoundingBox OctreeNode::GetBoundary() const
{
    return Boundary;
}

void OctreeNode::Draw(TessellationHelper& lineTessellation) const
{
    if (Children.at(LeftBottomFront) != nullptr)
    {
        for (int p = LeftBottomFront; p <= RightTopBack; p++)
        {
            Children.at(static_cast<Position>(p))->Draw(lineTessellation);
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
    for (int p = LeftBottomFront; p <= RightTopBack; p++)
    {
        Children.emplace(static_cast<Position>(p), nullptr);
    }
}

OctreeNode* OctreeNode::GetParent() const
{
    return Parent;
}

OctreeNode* OctreeNode::GetNeighborGreaterOrEqual(const Direction d) const
{
    switch (d)
    {
    case Left:
        return GetNeighborGreaterOrEqualLeft();
    case Right:
        return GetNeighborGreaterOrEqualRight();
    case Front:
        return GetNeighborGreaterOrEqualFront();
    case Back:
        return GetNeighborGreaterOrEqualBack();
    case Up:
        return GetNeighborGreaterOrEqualUp();
    case Down:
        return GetNeighborGreaterOrEqualDown();
    }
    return nullptr;
}

vec3 OctreeNode::GetCenter() const
{
    return vec3{Boundary.GetMinX() + (Boundary.GetMaxX() - Boundary.GetMinX()) / 2.0F, Boundary.GetMinY() + (Boundary.GetMaxY() - Boundary.GetMinY()) / 2.0F, Boundary.GetMinZ() + (Boundary.GetMaxZ() - Boundary.GetMinZ()) / 2.0F};
}
