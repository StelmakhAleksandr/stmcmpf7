#pragma once

#include "../../components/SdRam.h"
#include "Common.h"
#include <vector>

namespace stmcmp {

class Pixel;
class ARGB8888Pixel;

template <LayerFormat format>
struct PixelVector;

template <>
struct PixelVector<LayerFormat::ARGB8888>
{
    using pixel = ARGB8888Pixel;
    using type = std::vector<pixel, SdramAllocator<pixel>>;
};

// template <>
// struct PixelVector<LayerFormat::RGB888>
// {
//     using type = std::vector<RGB888Pixel, SdramAllocator<RGB888Pixel>>;
// };

class Pixel
{
};

class TestPixel
{
public:
    ~TestPixel()
    {
        printf("remove TestPixel\r\n");
    }
};

class ARGB8888Pixel : public Pixel
{
public:
    ARGB8888Pixel() = default;
    ARGB8888Pixel(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
    {
        setColor(alpha, red, green, blue);
    }
    void setAlpha(uint8_t alpha);
    void setRed(uint8_t red);
    void setGreen(uint8_t green);
    void setBlue(uint8_t blue);
    uint8_t alpha() const;
    uint8_t red() const;
    uint8_t green() const;
    uint8_t blue() const;
    void setColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);
    uint32_t color() const;

private:
    uint8_t m_blue = 0;
    uint8_t m_green = 0;
    uint8_t m_red = 255;
    uint8_t m_alpha = 255;
};

}