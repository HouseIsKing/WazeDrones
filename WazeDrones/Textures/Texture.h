//
// Created by amit on 4/22/2022.
//
#pragma once
#include <glad/glad.h>
#include <memory>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::unique_ptr;

class Texture
{
    static unordered_map<string, unique_ptr<Texture>> TexturesCache;
    GLuint64 Handle;
    GLuint TextureId;
public:
    Texture(GLuint64 handle, GLuint textureId);
    static Texture* LoadTexture(const string& path);
    [[nodiscard]] GLuint64 GetHandle() const;
    void Resident() const;
    void NonResident() const;
    bool operator==(const Texture& other) const;
    ~Texture();
    Texture(const Texture& other) = default;
    Texture(Texture&& other) = default;
    Texture& operator =(const Texture& other) = default;
    Texture& operator =(Texture&& other) = default;
};

template <>
struct std::hash<Texture*>
{
    size_t operator()(const Texture* texture) const noexcept
    {
        return std::hash<GLuint64>()(texture->GetHandle());
    }
};