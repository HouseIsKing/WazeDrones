#pragma once
#include <chrono>

class CustomRandomEngine
{
    static constexpr uint_fast64_t A{0x5DEECE66DULL};
    static constexpr uint_fast64_t C{0xBULL};
    static uint_fast64_t Seed;
public:
    static constexpr uint_fast64_t M{(1ULL << 48) - 1};
    CustomRandomEngine();
    static uint_fast64_t GetNext();
};