//
// Created by amit on 4/22/2022.
//

#include "EngineDefaults.h"
#include <memory>
#include <ranges>

bool EngineDefaults::HasInit = false;
bool EngineDefaults::HasBuiltTextureUbo = false;
unique_ptr<Shader> EngineDefaults::TheShader = {};
CustomRandomEngine EngineDefaults::Engine = {};
std::unordered_map<Texture*, uint16_t> EngineDefaults::TextureList = {};
GLuint EngineDefaults::UboTextures = 0;

Shader* EngineDefaults::GetShader()
{
    if (!HasInit)
    {
        Init();
        TheShader->Use();
    }
    return TheShader.get();
}

void EngineDefaults::Init()
{
    HasInit = true;
    TheShader = std::make_unique<Shader>("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    glGenBuffers(1, &UboTextures);
}

float EngineDefaults::ConvertLightLevelToAmbient(const int lightLevel)
{
    if (lightLevel == 0)
    {
        return 0.6F;
    }
    return 1.0F;
}

float EngineDefaults::GetNextFloat()
{
    return static_cast<float>(static_cast<double>(CustomRandomEngine::GetNext()) / CustomRandomEngine::M);
}

uint16_t EngineDefaults::RegisterTexture(Texture* texture)
{
    HasBuiltTextureUbo = false;
    if (!TextureList.emplace(texture, static_cast<uint16_t>(TextureList.size())).second)
    {
        return TextureList.at(texture);
    }
    texture->Resident();
    return static_cast<uint16_t>(TextureList.size() - 1);
}

void EngineDefaults::BuildTextureUbo()
{
    if (HasBuiltTextureUbo)
    {
        return;
    }
    HasBuiltTextureUbo = true;
    std::vector<GLuint64> helper;
    helper.reserve(TextureList.size());
    for (const Texture* texture : TextureList | std::ranges::views::keys)
    {
        helper.push_back(texture->GetHandle());
    }
    glBindBuffer(GL_UNIFORM_BUFFER, UboTextures);
    glBufferData(GL_UNIFORM_BUFFER, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()), helper.data(), GL_STATIC_COPY);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UboTextures, 0, static_cast<GLintptr>(sizeof(GLuint64) * helper.size()));
}

void EngineDefaults::ResetTextures()
{
    for (const auto& texture : TextureList | std::ranges::views::keys)
    {
        texture->NonResident();
    }
    TextureList.clear();
    HasBuiltTextureUbo = false;
}