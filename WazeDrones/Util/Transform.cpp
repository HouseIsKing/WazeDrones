#include "Transform.h"

Transform::Transform() : Position(vec3(0.0F)), Rotation(vec3(0.0F)), Scale(vec3(1.0F)), TransformMatrix(1.0F), IsDirty(true)
{
}

void Transform::CalculateTransformMatrix()
{
    TransformMatrix = translate(Position);
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.x), vec3(1.0F, 0.0F, 0.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.y), vec3(0.0F, -1.0F, 0.0F));
    TransformMatrix = rotate(TransformMatrix, glm::radians(Rotation.z), vec3(0.0F, 0.0F, 1.0F));
    TransformMatrix = scale(TransformMatrix, Scale);
    IsDirty = false;
}

vec3 Transform::GetPosition() const
{
    return Position;
}

vec3 Transform::GetRotation() const
{
    return Rotation;
}

void Transform::Rotate(const float x, const float y, const float z)
{
    Rotation.x += x;
    Rotation.y += y;
    Rotation.z += z;
    IsDirty = true;
}

void Transform::SetRotation(const float x, const float y, const float z)
{
    Rotation.x = x;
    Rotation.y = y;
    Rotation.z = z;
    IsDirty = true;
}

void Transform::Move(const float x, const float y, const float z)
{
    Position.x += x;
    Position.y += y;
    Position.z += z;
    IsDirty = true;
}

void Transform::SetPosition(const float x, const float y, const float z)
{
    Position.x = x;
    Position.y = y;
    Position.z = z;
    IsDirty = true;
}

void Transform::Grow(const float x, const float y, const float z)
{
    Scale.x += x;
    Scale.y += y;
    Scale.z += z;
    IsDirty = true;
}

void Transform::SetScale(const float x, const float y, const float z)
{
    Scale.x = x;
    Scale.y = y;
    Scale.z = z;
    IsDirty = true;
}

mat4x4 Transform::GetTransformMatrix()
{
    if (IsDirty)
    {
        CalculateTransformMatrix();
    }
    return TransformMatrix;
}