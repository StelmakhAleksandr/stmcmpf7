#include "Layer.h"

namespace stmcmp {

void Layer::setup(uint32_t horizontalTotalPorch, uint32_t verticalTotalPorch, uint32_t activeWidth, uint32_t activeHeight, const LayerConfig& config)
{
    m_dma2d.setup();
    uint32_t bytesPerPixel = [](LayerFormat format) {
        switch (format) {
        case LayerFormat::ARGB8888:
            return 4;
        case LayerFormat::RGB888:
            return 3;
        case LayerFormat::RGB565:
        case LayerFormat::ARGB1555:
        case LayerFormat::ARGB4444:
            return 2;
        default:
            return 4;
        }
    }(config.format);
    m_workBuffer = SdRam::instance()->allocate(bytesPerPixel * activeHeight * activeWidth + activeHeight);
    m_renderBuffer = SdRam::instance()->allocate(bytesPerPixel * activeHeight * activeWidth + activeHeight);
    m_layer = config.layer;
    m_layer->WHPCR = ((horizontalTotalPorch - 1) << LTDC_LxWHPCR_WHSTPOS_Pos) | ((activeWidth + horizontalTotalPorch - 1) << LTDC_LxWHPCR_WHSPPOS_Pos);
    m_layer->WVPCR = ((verticalTotalPorch - 1) << LTDC_LxWVPCR_WVSTPOS_Pos) | ((activeHeight + verticalTotalPorch - 1) << LTDC_LxWVPCR_WVSPPOS_Pos);
    m_layer->PFCR = (uint32_t)config.format;
    m_layer->CACR = 0xFF;
    m_layer->DCCR = 0xFF0000FF;
    m_layer->CFBAR = (uint32_t)m_renderBuffer;
    uint32_t lineLength = activeWidth * bytesPerPixel + 4;
    uint32_t pitch = activeWidth * bytesPerPixel;
    m_layer->CFBLR = ((lineLength << LTDC_LxCFBLR_CFBLL_Pos) & LTDC_LxCFBLR_CFBLL_Msk) | ((pitch << LTDC_LxCFBLR_CFBP_Pos) & LTDC_LxCFBLR_CFBP_Msk);
    m_layer->CFBLNR = activeHeight;
    m_layer->BFCR = ((uint32_t)LayerBF1::CompositeAlpha << LTDC_LxBFCR_BF1_Pos) | ((uint32_t)LayerBF2::InverseCompositeAlpha << LTDC_LxBFCR_BF2_Pos);
    m_layer->CR &= ~LTDC_LxCR_CLUTEN;
    m_layer->CR |= LTDC_LxCR_LEN;
    m_nlr = (activeWidth << 16) | (activeHeight & 0xFFFF);
    printf("Layer RAM address = %x sizeof = %d\r\n", (uint32_t)m_renderBuffer, sizeof(ARGB8888Pixel));
}

void Layer::swapBuffers()
{
    m_dma2d.copyMemToMemNlr((uint32_t)m_workBuffer, (uint32_t)m_renderBuffer, m_nlr);
    // std::swap(m_workBuffer, m_renderBuffer);
    // m_layer->CFBAR = (uint32_t)m_renderBuffer;
}

void* Layer::workBuffer() const
{
    return m_workBuffer;
}

}