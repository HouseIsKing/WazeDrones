#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct GuiVertex
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec4 Color;
    int IndexTexture;
    unsigned int SpecialFactor;
    GuiVertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, uint16_t indexTexture);
    GuiVertex(const glm::vec3& pos, float u, float v, float r, float g, float b, float a, uint16_t indexTexture);
    GuiVertex(float x, float y, float z, float u, float v, float r, float g, float b, float a, uint16_t indexTexture, unsigned int specialFactor);
    GuiVertex(const glm::vec3& pos, float u, float v, float r, float g, float b, float a, uint16_t indexTexture, unsigned int specialFactor);
};
