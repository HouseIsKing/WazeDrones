#include "Camera.h"

bool Frustum::CubeInFrustum(const float minX, const float minY, const float minZ, const float maxX, const float maxY,
                            const float maxZ) const
{
    // Basically, what is going on is, that we are given the size of the cube. Then we're checking each point
    // in the cube and seeing if it is inside the frustum.  If a point is found in front
    // of a side, then we skip to the next side.  If we get to a plane that does NOT have
    // a point in front of it, then it will return false.

    // *Note* - This will sometimes say that a cube is inside the frustum when it isn't.
    // This happens when all the corners of the bounding box are not behind any one plane.
    // This is rare and shouldn't affect the overall rendering speed.

    for (size_t i = 0; i < 6; i++)
    {
        if (Planes[i].x * minX + Planes[i].y * minY + Planes[i].z * minZ + Planes[i].w > 0)
        {
            continue;
        }
        if (Planes[i].x * maxX + Planes[i].y * minY + Planes[i].z * minZ + Planes[i].w > 0)
        {
            continue;
        }
        if (Planes[i].x * minX + Planes[i].y * maxY + Planes[i].z * minZ + Planes[i].w > 0)
        {
            continue;
        }
        if (Planes[i].x * maxX + Planes[i].y * maxY + Planes[i].z * minZ + Planes[i].w > 0)
        {
            continue;
        }
        if (Planes[i].x * minX + Planes[i].y * minY + Planes[i].z * maxZ + Planes[i].w > 0)
        {
            continue;
        }
        if (Planes[i].x * maxX + Planes[i].y * minY + Planes[i].z * maxZ + Planes[i].w > 0)
        {
            continue;
        }
        if (Planes[i].x * minX + Planes[i].y * maxY + Planes[i].z * maxZ + Planes[i].w > 0)
        {
            continue;
        }
        if (Planes[i].x * maxX + Planes[i].y * maxY + Planes[i].z * maxZ + Planes[i].w > 0)
        {
            continue;
        }
        // If we get here, it isn't in the frustum
        return false;
    }

    return true;
}


bool Frustum::CubeInFrustum(const BoundingBox& box) const
{
    return CubeInFrustum(box.GetMinX(), box.GetMinY(), box.GetMinZ(), box.GetMaxX(), box.GetMaxY(), box.GetMaxZ());
}

Frustum::Frustum(const mat4x4& viewMatrix, const mat4x4& projectionMatrix) : Planes()
{
    const mat4x4& v = viewMatrix;
    const mat4x4& p = projectionMatrix;
    mat4x4 clipMatrix;
    clipMatrix[0][0] = v[0][0] * p[0][0] + v[0][1] * p[1][0] + v[0][2] * p[2][0] + v[0][3] * p[3][0];
    clipMatrix[1][0] = v[0][0] * p[0][1] + v[0][1] * p[1][1] + v[0][2] * p[2][1] + v[0][3] * p[3][1];
    clipMatrix[2][0] = v[0][0] * p[0][2] + v[0][1] * p[1][2] + v[0][2] * p[2][2] + v[0][3] * p[3][2];
    clipMatrix[3][0] = v[0][0] * p[0][3] + v[0][1] * p[1][3] + v[0][2] * p[2][3] + v[0][3] * p[3][3];
    clipMatrix[0][1] = v[1][0] * p[0][0] + v[1][1] * p[1][0] + v[1][2] * p[2][0] + v[1][3] * p[3][0];
    clipMatrix[1][1] = v[1][0] * p[0][1] + v[1][1] * p[1][1] + v[1][2] * p[2][1] + v[1][3] * p[3][1];
    clipMatrix[2][1] = v[1][0] * p[0][2] + v[1][1] * p[1][2] + v[1][2] * p[2][2] + v[1][3] * p[3][2];
    clipMatrix[3][1] = v[1][0] * p[0][3] + v[1][1] * p[1][3] + v[1][2] * p[2][3] + v[1][3] * p[3][3];
    clipMatrix[0][2] = v[2][0] * p[0][0] + v[2][1] * p[1][0] + v[2][2] * p[2][0] + v[2][3] * p[3][0];
    clipMatrix[1][2] = v[2][0] * p[0][1] + v[2][1] * p[1][1] + v[2][2] * p[2][1] + v[2][3] * p[3][1];
    clipMatrix[2][2] = v[2][0] * p[0][2] + v[2][1] * p[1][2] + v[2][2] * p[2][2] + v[2][3] * p[3][2];
    clipMatrix[3][2] = v[2][0] * p[0][3] + v[2][1] * p[1][3] + v[2][2] * p[2][3] + v[2][3] * p[3][3];
    clipMatrix[0][3] = v[3][0] * p[0][0] + v[3][1] * p[1][0] + v[3][2] * p[2][0] + v[3][3] * p[3][0];
    clipMatrix[1][3] = v[3][0] * p[0][1] + v[3][1] * p[1][1] + v[3][2] * p[2][1] + v[3][3] * p[3][1];
    clipMatrix[2][3] = v[3][0] * p[0][2] + v[3][1] * p[1][2] + v[3][2] * p[2][2] + v[3][3] * p[3][2];
    clipMatrix[3][3] = v[3][0] * p[0][3] + v[3][1] * p[1][3] + v[3][2] * p[2][3] + v[3][3] * p[3][3];

    Planes[PlaneRight] = clipMatrix[3] - clipMatrix[0];

    Planes[PlaneLeft] = clipMatrix[3] + clipMatrix[0];

    Planes[PlaneBottom] = clipMatrix[3] + clipMatrix[1];

    Planes[PlaneTop] = clipMatrix[3] - clipMatrix[1];

    Planes[PlaneBack] = clipMatrix[3] - clipMatrix[2];

    Planes[PlaneFront] = clipMatrix[3] + clipMatrix[2];

    for (size_t i = 0; i < 6; i++)
    {
        Planes[i] = normalize(Planes[i]);
    }
}

void Camera::UpdateVectors()
{
    Front.x = cos(radians(Pitch)) * cos(radians(Yaw));
    Front.y = sin(radians(Pitch));
    Front.z = cos(radians(Pitch)) * sin(radians(Yaw));
    Front = normalize(Front);
    Right = normalize(cross(Front, vec3(0, 1, 0)));
    Up = normalize(cross(Right, Front));
}

Frustum Camera::GetFrustum()
{
    return {ViewMatrix, ProjectionMatrix};
}

Camera::Camera(const vec3 position, const float aspectRatio) : Front(0, 0, -1), Up(0, 1, 0), Right(1, 0, 0),
                                                               Fov(70), AspectRatio(aspectRatio), ViewMatrix(),
                                                               ProjectionMatrix(),
                                                               IsDirtyProjectionMatrix(true),
                                                               PrevYaw(0), ZNear(0.05F), ZFar(1000.0F), PrevPitch(0),
                                                               Position(position), Pitch(0), Yaw(0)
{
    UpdateVectors();
}


void Camera::SetFov(const float newFov)
{
    Fov = newFov;
    IsDirtyProjectionMatrix = true;
}

vec3 Camera::GetFrontVector() const
{
    return Front;
}

vec3 Camera::GetRightVector() const
{
    return Right;
}

mat4x4 Camera::GetViewMatrix()
{
    RecalculateViewMatrix();
    return ViewMatrix;
}

mat4x4 Camera::GetProjectionMatrix()
{
    if (IsDirtyProjectionMatrix)
    {
        RecalculateProjectionMatrix();
    }
    return ProjectionMatrix;
}

void Camera::RecalculateProjectionMatrix()
{
    ProjectionMatrix = perspective(radians(Fov), AspectRatio, ZNear, ZFar);
    IsDirtyProjectionMatrix = false;
}

void Camera::SetAspectRatio(const float newAspectRatio)
{
    AspectRatio = newAspectRatio;
    IsDirtyProjectionMatrix = true;
    RecalculateProjectionMatrix();
}

void Camera::RecalculateViewMatrix()
{
    PrevYaw = Yaw;
    PrevPitch = Pitch;
    UpdateVectors();
    ViewMatrix = lookAt(Position, Position + Front, Up);
}