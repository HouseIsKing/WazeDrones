#pragma once
#include <vector>

#include "GuiVertex.h"
#include "TessellationInterface.h"

class GuiTessellation final : public TessellationInterface
{
    std::vector<GuiVertex> Vertices{};

public:
    GuiTessellation();
    uint16_t AddVertex(const GuiVertex& vertex);
    void AddTriangle(uint16_t triangle) override;
    void Reset() override;
    void FreeMemory() override;
    void Draw() override;
};
