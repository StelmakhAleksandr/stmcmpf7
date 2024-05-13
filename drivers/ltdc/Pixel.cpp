#include "Pixel.h"

namespace stmcmp {

void ARGB8888Pixel::setAlpha(uint8_t alpha)
{
    m_alpha = alpha;
}

void ARGB8888Pixel::setRed(uint8_t red)
{
    m_red = red;
}

void ARGB8888Pixel::setGreen(uint8_t green)
{
    m_green = green;
}

void ARGB8888Pixel::setBlue(uint8_t blue)
{
    m_blue = blue;
}

uint8_t ARGB8888Pixel::alpha() const
{
    return m_alpha;
}

uint8_t ARGB8888Pixel::red() const
{
    return m_red;
}

uint8_t ARGB8888Pixel::green() const
{
    return m_green;
}

uint8_t ARGB8888Pixel::blue() const
{
    return m_blue;
}

void ARGB8888Pixel::setColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
    m_alpha = alpha;
    m_red = red;
    m_green = green;
    m_blue = blue;
}

uint32_t ARGB8888Pixel::color() const
{
    return (static_cast<uint32_t>(m_alpha) << 24) | (static_cast<uint32_t>(m_red) << 16) | (static_cast<uint32_t>(m_green) << 8) | (static_cast<uint32_t>(m_blue));
}

}