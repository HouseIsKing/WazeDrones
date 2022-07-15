//
// Created by amit on 4/22/2022.
//

#pragma once
#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Util/BoundingBox.h"

using glm::vec3;
using glm::mat4x4;
using glm::cos;
using glm::sin;
using glm::radians;
using glm::normalize;
using glm::lookAt;
using glm::perspective;

struct Frustum
{
    enum Plane { PlaneBack, PlaneFront, PlaneLeft, PlaneRight, PlaneTop, PlaneBottom };

    Frustum(const mat4x4& viewMatrix, const mat4x4& projectionMatrix);
    [[nodiscard]] bool CubeInFrustum(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) const;
    [[nodiscard]] bool CubeInFrustum(const BoundingBox& box) const;

    std::array<glm::vec4, 6> Planes;
};

class Camera
{
    vec3 Front;
    vec3 Up;
    vec3 Right;
    float Fov;
    float AspectRatio;
    mat4x4 ViewMatrix;
    mat4x4 ProjectionMatrix;
    bool IsDirtyProjectionMatrix;
    float PrevYaw;
    float ZNear;
    float ZFar;
    float PrevPitch;
    void UpdateVectors();
public:
    vec3 Position;
    float Pitch;
    float Yaw;
    Frustum GetFrustum();
    Camera(vec3 position, float aspectRatio);
    void SetFov(float newFov);
    [[nodiscard]] vec3 GetFrontVector() const;
    [[nodiscard]] vec3 GetRightVector() const;
    void SetAspectRatio(float newAspectRatio);
    void RecalculateProjectionMatrix();
    void RecalculateViewMatrix();
    mat4x4 GetViewMatrix();
    mat4x4 GetProjectionMatrix();
};
