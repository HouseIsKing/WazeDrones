//
// Created by amit on 4/21/2022.
//

#pragma once
#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"
#include "Transform.h"
#include "Vertex.h"
#include <vector>

using std::vector;

class TessellationHelper
{
    GLuint Vbo;
    GLuint Vao;
    GLuint Ebo;
    vector<Vertex> Vertices{};
    vector<GLushort> TriangleIndices{};
    vector<Transform> TessellationTransforms{};
    Shader* TheShader;
    bool HasInit;
    int PositionUniform;
    size_t TrianglesCount;
    mat4x4 GetTransformationMatrix(size_t id);
public:
    void Reset();
    ~TessellationHelper();
    TessellationHelper(const TessellationHelper&) = delete;
    TessellationHelper& operator=(const TessellationHelper&) = delete;
    TessellationHelper(TessellationHelper&&) = delete;
    TessellationHelper& operator=(TessellationHelper&&) = delete;
    [[nodiscard]] bool HasInitialized() const;
    explicit TessellationHelper(Shader* shader);
    TessellationHelper(Shader* shader, float x, float y, float z);
    [[nodiscard]] Transform& GetTransform(size_t id);
    uint16_t AddVertex(const Vertex& vertex);
    void AddTriangle(uint16_t triangle);
    void Draw();
    void FreeMemory();
    void Draw(size_t transformId, size_t startPos, size_t count);
    size_t AddTransform(Transform transform);
    [[nodiscard]] size_t GetCurrentTriangleCount() const;
};