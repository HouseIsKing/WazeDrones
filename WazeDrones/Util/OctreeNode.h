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
    inline static float MinSize = 20.0F;
    static BoundingBox GetChildrenBound(const BoundingBox& boundary, size_t boundingBoxIndex);
public:
    explicit OctreeNode(BoundingBox boundary);
    OctreeNode();
    OctreeNode(const OctreeNode&) = delete;
    OctreeNode(OctreeNode&&) = delete;
    OctreeNode& operator=(const OctreeNode& other) = delete;
    OctreeNode& operator=(OctreeNode&& other) noexcept = delete;
    ~OctreeNode() = default;
    void Insert(const BoundingBox& box);
    [[nodiscard]] BoundingBox GetBoundary() const;
    void Draw(TessellationHelper& lineTessellation) const;
    void Init(const BoundingBox& boundary);
};
