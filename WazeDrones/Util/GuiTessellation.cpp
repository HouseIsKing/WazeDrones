#include "GuiTessellation.h"
#include "EngineDefaults.h"

GuiTessellation::GuiTessellation()
{
    TheShader = EngineDefaults::GetGuiShader();
}

uint16_t GuiTessellation::AddVertex(const GuiVertex& vertex)
{
    const auto index = static_cast<uint16_t>(Vertices.size());
    Vertices.push_back(vertex);
    return index;
}

void GuiTessellation::AddTriangle(const uint16_t triangle)
{
    TriangleIndices.push_back(triangle);
}

void GuiTessellation::Reset()
{
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    glDeleteVertexArrays(1, &Vao);
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &Vbo);
    glGenBuffers(1, &Ebo);
    Vertices.clear();
    TriangleIndices.clear();
    HasInit = false;
}

void GuiTessellation::FreeMemory()
{
    Vertices.shrink_to_fit();
    TriangleIndices.shrink_to_fit();
}

void GuiTessellation::Draw()
{
    if (Vertices.empty() && !HasInit)
    {
        return;
    }
    TheShader->Use();
    glBindVertexArray(Vao);
    if (!HasInit)
    {
        HasInit = true;
        glBindBuffer(GL_ARRAY_BUFFER, Vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLintptr>(Vertices.size() * sizeof(GuiVertex)), Vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GuiVertex), nullptr);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GuiVertex), reinterpret_cast<void*>(offsetof(GuiVertex, TexCoords)));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GuiVertex), reinterpret_cast<void*>(offsetof(GuiVertex, Color)));
        glVertexAttribIPointer(3, 1, GL_INT, sizeof(GuiVertex), reinterpret_cast<void*>(offsetof(GuiVertex, IndexTexture)));
        glVertexAttribIPointer(4, 1, GL_UNSIGNED_INT, sizeof(GuiVertex), reinterpret_cast<void*>(offsetof(GuiVertex, SpecialFactor)));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(TriangleIndices.size() * sizeof(GLushort)), TriangleIndices.data(), GL_STATIC_DRAW);
        TrianglesCount = TriangleIndices.size();
        Vertices.clear();
        TriangleIndices.clear();
    }
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(TrianglesCount), GL_UNSIGNED_SHORT, nullptr);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}
