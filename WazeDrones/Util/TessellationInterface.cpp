#include "TessellationInterface.h"

TessellationInterface::~TessellationInterface()
{
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    glDeleteVertexArrays(1, &Vao);
}

TessellationInterface::TessellationInterface() : Vbo(0), Vao(0), Ebo(0), TheShader(nullptr), HasInit(false), TrianglesCount(0)
{
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
    glGenBuffers(1, &Ebo);
}

bool TessellationInterface::HasInitialized() const
{
    return HasInit;
}

size_t TessellationInterface::GetCurrentTriangleCount() const
{
    return TriangleIndices.size();
}
