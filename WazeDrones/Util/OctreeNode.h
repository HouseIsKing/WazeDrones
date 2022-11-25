#pragma once
#include <memory>
#include <unordered_map>

#include "BoundingBox.h"
#include "TessellationHelper.h"

enum Direction { Left, Right, Up, Down, Front, Back };

enum Position { LeftBottomFront, LeftBottomBack, LeftTopFront, LeftTopBack, RightBottomFront, RightBottomBack, RightTopFront, RightTopBack };

class OctreeNode
{
    OctreeNode* Parent;
    BoundingBox Boundary;
    bool IsColliding;
    std::unordered_map<Position, std::unique_ptr<OctreeNode>> Children;
    inline static float MinSize = 20.0F;
    static BoundingBox GetChildrenBound(const BoundingBox& boundary, int boundingBoxIndex);
    [[nodiscard]] Position GetPosition() const;
    [[nodiscard]] OctreeNode* GetNeighborGreaterOrEqualLeft() const;
    [[nodiscard]] OctreeNode* GetNeighborGreaterOrEqualRight() const;
    [[nodiscard]] OctreeNode* GetNeighborGreaterOrEqualUp() const;
    [[nodiscard]] OctreeNode* GetNeighborGreaterOrEqualDown() const;
    [[nodiscard]] OctreeNode* GetNeighborGreaterOrEqualFront() const;
    [[nodiscard]] OctreeNode* GetNeighborGreaterOrEqualBack() const;
public:
    std::vector<OctreeNode*> GetAllLeavesNotColliding();
    explicit OctreeNode(BoundingBox boundary, OctreeNode* parent);
    explicit OctreeNode(OctreeNode* parent);
    OctreeNode(const OctreeNode&) = delete;
    OctreeNode(OctreeNode&&) = delete;
    OctreeNode& operator=(const OctreeNode& other) = delete;
    OctreeNode& operator=(OctreeNode&& other) noexcept = delete;
    bool operator<(const OctreeNode& other) const;
    ~OctreeNode() = default;
    void Insert(const BoundingBox& box);
    [[nodiscard]] BoundingBox GetBoundary() const;
    void Draw(TessellationHelper& lineTessellation) const;
    void Init(const BoundingBox& boundary);
    [[nodiscard]] OctreeNode* GetParent() const;
    [[nodiscard]] OctreeNode* GetNeighborGreaterOrEqual(Direction d) const;
    [[nodiscard]] vec3 GetCenter() const;
    [[nodiscard]] OctreeNode* GetLeafAt(const vec3& position);
    [[nodiscard]] bool GetIsColliding() const;
    [[nodiscard]] OctreeNode* GetChild(Position pos) const;
};
