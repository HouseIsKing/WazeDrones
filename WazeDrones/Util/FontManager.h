#pragma once
#include <array>
#include <string>

class FontManager
{
    std::array<char, 128> CharacterWidths;

public:
    explicit FontManager(const std::string& texture);
    [[nodiscard]] char GetCharacterWidth(char c) const;
};
