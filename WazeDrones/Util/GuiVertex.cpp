#include "GuiVertex.h"

GuiVertex::GuiVertex(const float x, const float y, const float z, const float u, const float v, const float r, const float g, const float b, const float a,
                     const uint16_t indexTexture) : GuiVertex(x, y, z, u, v, r, g, b, a, indexTexture, 0)
{
}

GuiVertex::GuiVertex(const glm::vec3& pos, const float u, const float v, const float r, const float g, const float b, const float a, const uint16_t indexTexture) : GuiVertex(pos.x, pos.y, pos.z, u, v, r, g, b, a, indexTexture)
{
}

GuiVertex::GuiVertex(const float x, const float y, const float z, const float u, const float v, const float r, const float g, const float b, const float a,
                     const uint16_t indexTexture, const unsigned specialFactor) : Position(x, y, z), TexCoords(u, v), Color(r, g, b, a),
    IndexTexture(indexTexture),
    SpecialFactor(specialFactor)
{
}

GuiVertex::GuiVertex(const glm::vec3& pos, const float u, const float v, const float r, const float g, const float b, const float a, const uint16_t indexTexture,
                     const unsigned specialFactor) : GuiVertex(pos.x, pos.y, pos.z, u, v, r, g, b, a, indexTexture, specialFactor)
{
}
