//
// Created by amit on 4/22/2022.
//

#include "BoundingBox.h"

#include <valarray>

float BoundingBox::GetMinX() const
{
    return MinX;
}

float BoundingBox::GetMinY() const
{
    return MinY;
}

float BoundingBox::GetMinZ() const
{
    return MinZ;
}

float BoundingBox::GetMaxX() const
{
    return MaxX;
}

float BoundingBox::GetMaxY() const
{
    return MaxY;
}

float BoundingBox::GetMaxZ() const
{
    return MaxZ;
}

BoundingBox::BoundingBox(const float minX, const float minY, const float minZ, const float maxX, const float maxY, const float maxZ) :
    MinX(minX), MinY(minY), MinZ(minZ), MaxX(maxX), MaxY(maxY), MaxZ(maxZ)
{
}

void BoundingBox::Expand(const float x, const float y, const float z)
{
    if (x < 0)
    {
        MinX += x;
    }
    else
    {
        MaxX += x;
    }
    if (y < 0)
    {
        MinY += y;
    }
    else
    {
        MaxY += y;
    }
    if (z < 0)
    {
        MinZ += z;
    }
    else
    {
        MaxZ += z;
    }
}

void BoundingBox::Grow(const float x, const float y, const float z)
{
    MinX -= x;
    MinY -= y;
    MinZ -= z;
    MaxX += x;
    MaxY += y;
    MaxZ += z;
}

void BoundingBox::Move(const float x, const float y, const float z)
{
    MinX += x;
    MinY += y;
    MinZ += z;
    MaxX += x;
    MaxY += y;
    MaxZ += z;
}

bool BoundingBox::IsPointInside(const float x, const float y, const float z) const
{
    return x >= MinX && x <= MaxX && y >= MinY && y <= MaxY && z >= MinZ && z <= MaxZ;
}

bool BoundingBox::IsIntersecting(const BoundingBox& other) const
{
    return IsIntersectingX(other) && IsIntersectingY(other) && IsIntersectingZ(other);
}

bool BoundingBox::IsIntersectingX(const BoundingBox& other) const
{
    return MinX <= other.GetMaxX() && MaxX >= other.GetMinX();
}

bool BoundingBox::IsIntersectingY(const BoundingBox& other) const
{
    return MinY <= other.GetMaxY() && MaxY >= other.GetMinY();
}

bool BoundingBox::IsIntersectingZ(const BoundingBox& other) const
{
    return MinZ <= other.GetMaxZ() && MaxZ >= other.GetMinZ();
}

float BoundingBox::ClipCollisionX(const BoundingBox& other, float x) const
{
    if (IsIntersectingY(other) && IsIntersectingZ(other))
    {
        if (x < 0 && MinX >= other.GetMaxX())
        {
            if (const float max = other.GetMaxX() - MinX + 0.001F; max > x)
            {
                x = max;
            }
        }
        else if (x > 0 && MaxX <= other.GetMinX())
        {
            if (const float min = other.GetMinX() - MaxX - 0.001F; min < x)
            {
                x = min;
            }
        }
    }
    return x;
}

float BoundingBox::ClipCollisionY(const BoundingBox& other, float y) const
{
    if (IsIntersectingX(other) && IsIntersectingZ(other))
    {
        if (y < 0 && MinY >= other.GetMaxY())
        {
            if (const float max = other.GetMaxY() - MinY + 0.001F; max > y)
            {
                y = max;
            }
        }
        else if (y > 0 && MaxY <= other.GetMinY())
        {
            if (const float min = other.GetMinY() - MaxY - 0.001F; min < y)
            {
                y = min;
            }
        }
    }
    return y;
}

float BoundingBox::ClipCollisionZ(const BoundingBox& other, float z) const
{
    if (IsIntersectingX(other) && IsIntersectingY(other))
    {
        if (z < 0 && MinZ >= other.GetMaxZ())
        {
            if (const float max = other.GetMaxZ() - MinZ + 0.001F; max > z)
            {
                z = max;
            }
        }
        else if (z > 0 && MaxZ <= other.GetMinZ())
        {
            if (const float min = other.GetMinZ() - MaxZ - 0.001F; min < z)
            {
                z = min;
            }
        }
    }
    return z;
}

bool BoundingBox::IsNeighbor(const BoundingBox& other, const float tolerance) const
{
    return std::abs(MinX - other.MinX) < tolerance || std::abs(MinY - other.MinY) < tolerance ||
        std::abs(MinZ - other.MinZ) < tolerance || std::abs(MaxX - other.MaxX) < tolerance ||
        std::abs(MaxY - other.MaxY) < tolerance || std::abs(MaxZ - other.MaxZ) < tolerance;
}

float BoundingBox::GetVolume() const
{
    return (MaxX - MinX) * (MaxY - MinY) * (MaxZ - MinZ);
}
