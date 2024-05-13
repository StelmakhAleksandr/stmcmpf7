#pragma once

#include "Common.h"

namespace stmcmp {

class Dma2d
{
public:
    void setup();
    void copyMemToMem(uint32_t from, uint32_t to, uint32_t width, uint32_t height);
    void copyMemToMemRegion(uint32_t from, uint32_t to, uint32_t width, uint32_t height, uint32_t oor);
    void copyMemToMemNlr(uint32_t from, uint32_t to, uint32_t nlr);
    void fillColor(uint32_t to, uint32_t width, uint32_t height, uint32_t color);

private:
    void start();
};

}