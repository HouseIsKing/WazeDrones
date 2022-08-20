#pragma once
#include <array>
#include <memory>
#include <vector>

#include "BoundingBox.h"
#include "TessellationHelper.h"

class OctreeNode
{
    BoundingBox Boundary;
    std::vector<std::unique_ptr<OctreeNode>> Children;
    std::array<BoundingBox, 8> ChildrenBounds;
    const float MinSize = 5.0F;
public:
    explicit OctreeNode(BoundingBox boundary);
    OctreeNode();
    OctreeNode(const OctreeNode&) = delete;
    OctreeNode(OctreeNode&&) = delete;
    OctreeNode& operator=(const OctreeNode& other) = delete;
    OctreeNode& operator=(OctreeNode&& other) noexcept = delete;
    ~OctreeNode() = default;
    void Insert(BoundingBox box);
    [[nodiscard]] BoundingBox GetBoundary() const;
    void Draw(TessellationHelper& lineTessellation) const;
    void Init(BoundingBox boundary);
};
