//
// Created by amit on 4/21/2022.
//

#include "Vertex.h"

vec4 Vertex::GetColor() const
{
    return Color;
}

vec2 Vertex::GetTexCoords() const
{
    return TexCoords;
}

vec3 Vertex::GetPosition() const
{
    return Position;
}

Vertex::Vertex(const float x, const float y, const float z, const float u, const float v, const float r, const float g,
               const float b, const float a, const uint16_t indexTexture) : Position(x, y, z), TexCoords(u, v), Color(r, g, b, a),
                                                                            IndexTexture(indexTexture)
{
}

Vertex::Vertex(const vec3& pos, const float u, const float v, const float r, const float g, const float b, const float a,
               const uint16_t indexTexture) : Vertex(pos.x, pos.y, pos.z, u, v, r, g, b, a, indexTexture)
{
}