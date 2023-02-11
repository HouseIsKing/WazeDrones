#pragma once
#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"
#include "CustomRandomEngine.h"
#include "FontManager.h"
#include <array>

using std::array;
using std::unique_ptr;

class EngineDefaults
{
    static unique_ptr<Shader> TheShader;
    static std::unordered_map<Texture*, uint16_t> TextureList;
    static GLuint UboTextures;
    static bool HasBuiltTextureUbo;
    static bool HasInit;
    static void Init();
    static CustomRandomEngine Engine;
    static FontManager MainFont;

public:
    static Shader* GetShader();
    template <typename T>
    static T GetNext(T maxValue);
    template <typename T>
    static T GetNext(T minValue, T maxValue);
    static float GetNextFloat();
    static int GetNextInt(int min, int max);
    static int GetNextInt(int max);
    static uint16_t RegisterTexture(Texture* texture);
    static void BuildTextureUbo();
    static void ResetTextures();
};

template <typename T>
T EngineDefaults::GetNext(T maxValue)
{
    return GetNext<T>(0, maxValue);
}

template <typename T>
T EngineDefaults::GetNext(T minValue, T maxValue)
{
    T modulo = maxValue - minValue;
    T next = Engine.GetNext() % modulo;
    return next + minValue;
}
