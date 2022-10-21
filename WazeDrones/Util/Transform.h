#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::mat4x4;
using glm::translate;

class Transform
{
    vec3 Position;
    vec3 Rotation;
    vec3 Scale;
    mat4x4 TransformMatrix;
    bool IsDirty;
    void CalculateTransformMatrix();
public:
    Transform();
    [[nodiscard]] vec3 GetPosition() const;
    [[nodiscard]] vec3 GetRotation() const;
    void Rotate(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    void Move(float x, float y, float z);
    void SetPosition(float x, float y, float z);
    void SetPosition(const vec3& position);
    void Grow(float x, float y, float z);
    void SetScale(float x, float y, float z);
    mat4x4 GetTransformMatrix();
};