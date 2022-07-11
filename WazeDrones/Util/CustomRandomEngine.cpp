#include "CustomRandomEngine.h"

uint_fast64_t CustomRandomEngine::Seed{static_cast<uint_fast64_t>(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1))};

CustomRandomEngine::CustomRandomEngine() = default;

uint_fast64_t CustomRandomEngine::GetNext()
{
    Seed = (Seed * A + C) % M;
    return Seed;
}