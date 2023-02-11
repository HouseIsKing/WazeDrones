#include "FontManager.h"
#include "EngineDefaults.h"
#include <stb_image.h>

FontManager::FontManager(const std::string& texture) : CharacterWidths()
{
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    stbi_set_flip_vertically_on_load(0);
    unsigned char* pixels = stbi_load(texture.c_str(), &width, &height, &nrChannels, 4);
    const size_t maxCharWidth = static_cast<size_t>(width) / 16;
    for (size_t i = 0; i < 128; i++)
    {
        const size_t xCharPos = i % 16;
        const size_t yCharPos = i / 16;
        size_t x = 0;
        for (size_t emptyColumn = 0; x < maxCharWidth && emptyColumn < maxCharWidth / 8; x++)
        {
            emptyColumn++;
            const size_t indexX = (xCharPos * maxCharWidth + x) * 4;
            for (size_t y = 0; y < maxCharWidth; y++)
            {
                if (const size_t indexY = (yCharPos * maxCharWidth + y) * static_cast<size_t>(width) * 4; pixels[indexX + indexY + 3] > 128)
                {
                    emptyColumn = 0;
                    break;
                }
            }
        }
        if (i == 32)
        {
            x = 4 * maxCharWidth / 8;
        }
        if (maxCharWidth > 0)
        {
            CharacterWidths[i] = static_cast<char>((x * 8 + 8) / maxCharWidth);
        }
    }
    stbi_image_free(pixels);
}

char FontManager::GetCharacterWidth(const char c) const
{
    return CharacterWidths[static_cast<unsigned char>(c)];
}
