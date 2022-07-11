#pragma once
#include <limits>

using std::numeric_limits;

struct BoundingBox
{
private:
    float MinX, MinY, MinZ;
    float MaxX, MaxY, MaxZ;
public:
    BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
    BoundingBox(const BoundingBox& other) = default;
    BoundingBox& operator=(const BoundingBox& other) = default;
    BoundingBox(BoundingBox&& other) = default;
    BoundingBox& operator=(BoundingBox&& other) = default;
    ~BoundingBox() = default;
    [[nodiscard]] float GetMinX() const;
    [[nodiscard]] float GetMinY() const;
    [[nodiscard]] float GetMinZ() const;
    [[nodiscard]] float GetMaxX() const;
    [[nodiscard]] float GetMaxY() const;
    [[nodiscard]] float GetMaxZ() const;
    void Expand(float x, float y, float z);
    void Grow(float x, float y, float z);
    void Move(float x, float y, float z);
    [[nodiscard]] bool IsPointInside(float x, float y, float z) const;
    [[nodiscard]] bool IsIntersecting(const BoundingBox& other) const;
    [[nodiscard]] bool IsIntersectingX(const BoundingBox& other) const;
    [[nodiscard]] bool IsIntersectingY(const BoundingBox& other) const;
    [[nodiscard]] bool IsIntersectingZ(const BoundingBox& other) const;
    [[nodiscard]] float ClipCollisionX(const BoundingBox& other, float x) const;
    [[nodiscard]] float ClipCollisionY(const BoundingBox& other, float y) const;
    [[nodiscard]] float ClipCollisionZ(const BoundingBox& other, float z) const;
};