#pragma once

#include "Dma2d.h"
#include "Pixel.h"

namespace stmcmp {

struct LayerConfig
{
    LTDC_Layer_TypeDef* layer;
    LayerFormat format;
    AllocatorType allocator;
    bool useSwapBuffers;
};

class Layer
{
public:
    void setup(uint32_t horizontalTotalPorch, uint32_t verticalTotalPorch, uint32_t activeWidth, uint32_t activeHeight, const LayerConfig& config);
    void swapBuffers();
    void* workBuffer() const;

private:
    void* m_workBuffer;
    void* m_renderBuffer;
    LTDC_Layer_TypeDef* m_layer;
    Dma2d m_dma2d;
    uint32_t m_nlr;
};

}
