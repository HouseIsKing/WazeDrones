#pragma once
#include "../Shaders/Shader.h"

class TessellationInterface
{
protected:
    GLuint Vbo;
    GLuint Vao;
    GLuint Ebo;
    std::vector<GLushort> TriangleIndices{};
    Shader* TheShader;
    bool HasInit;
    size_t TrianglesCount;

public:
    virtual ~TessellationInterface();
    TessellationInterface();
    TessellationInterface(const TessellationInterface&) = delete;
    TessellationInterface(TessellationInterface&&) = delete;
    TessellationInterface& operator=(const TessellationInterface&) = delete;
    TessellationInterface& operator=(TessellationInterface&&) = delete;
    [[nodiscard]] bool HasInitialized() const;
    [[nodiscard]] size_t GetCurrentTriangleCount() const;
    virtual void AddTriangle(uint16_t triangle) = 0;
    virtual void FreeMemory() = 0;
    virtual void Reset() = 0;
    virtual void Draw() = 0;
};
