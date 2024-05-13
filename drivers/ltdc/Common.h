#pragma once

#include "stm32f746xx.h"

enum class LayerBF1 : uint8_t
{
    ConstantAlpha = 0b100,
    CompositeAlpha = 0b110,
};

enum class LayerBF2 : uint8_t
{
    InverseConstantAlpha = 0b101,
    InverseCompositeAlpha = 0b111,
};

enum class LayerFormat : uint8_t
{
    ARGB8888 = 0b000,
    RGB888 = 0b001,
    RGB565 = 0b010,
    ARGB1555 = 0b011,
    ARGB4444 = 0b100,
};

enum class AllocatorType : uint8_t
{
    SDRAM,
    Standard
};
