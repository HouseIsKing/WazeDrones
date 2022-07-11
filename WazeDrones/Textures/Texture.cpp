#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

using std::runtime_error;
using std::piecewise_construct;
using std::forward_as_tuple;

Texture* Texture::LoadTexture(const string& path)
{
    if (TexturesCache.contains(path))
    {
        return TexturesCache.at(path).get();
    }
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
    if (pixels == nullptr)
    {
        throw runtime_error("Failed to load texture");
    }
    GLuint textureId = 0;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
    const GLsizei numMipmaps = static_cast<GLsizei>(log2(std::max(width, height))) + 1;
    glTextureStorage2D(textureId, numMipmaps, GL_RGBA8, width, height);
    glTextureSubImage2D(textureId, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateTextureMipmap(textureId);
    stbi_image_free(pixels);
    TexturesCache.emplace(piecewise_construct, forward_as_tuple(path),
                          forward_as_tuple(new Texture(glGetTextureHandleARB(textureId), textureId)));
    return TexturesCache.at(path).get();
}

Texture::Texture(const GLuint64 handle, const GLuint textureId) : Handle(handle), TextureId(textureId)
{
}

unordered_map<string, unique_ptr<Texture>> Texture::TexturesCache;

GLuint64 Texture::GetHandle() const
{
    return Handle;
}

void Texture::Resident() const
{
    glMakeTextureHandleResidentARB(Handle);
}

void Texture::NonResident() const
{
    glMakeTextureHandleNonResidentARB(Handle);
}

bool Texture::operator==(const Texture& other) const
{
    return Handle == other.Handle;
}

Texture::~Texture()
{
    NonResident();
    glDeleteTextures(1, &TextureId);
}