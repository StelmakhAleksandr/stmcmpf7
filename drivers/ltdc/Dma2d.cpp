#include "Dma2d.h"

#include "../../components/Terminal.h"

namespace stmcmp {

void Dma2d::setup()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2DEN;
}

void Dma2d::copyMemToMem(uint32_t from, uint32_t to, uint32_t width, uint32_t height)
{
    DMA2D->CR = 0;
    DMA2D->OPFCCR = 0;
    DMA2D->OMAR = to;
    DMA2D->FGMAR = from;
    DMA2D->NLR = (width << 16) | (height & 0xFFFF);
    DMA2D->OOR = 0;
    start();
}

void Dma2d::copyMemToMemNlr(uint32_t from, uint32_t to, uint32_t nlr)
{
    DMA2D->CR = 0;
    DMA2D->OPFCCR = 0;
    DMA2D->OMAR = to;
    DMA2D->FGMAR = from;
    DMA2D->NLR = nlr;
    DMA2D->OOR = 0;
    start();
}

void Dma2d::copyMemToMemRegion(uint32_t from, uint32_t to, uint32_t width, uint32_t height, uint32_t oor)
{
    DMA2D->CR = 0;
    DMA2D->OPFCCR = 0;
    DMA2D->OMAR = to;
    DMA2D->FGMAR = from;
    DMA2D->NLR = (width << 16) | (height & 0xFFFF);
    DMA2D->OOR = oor;
    start();
}

void Dma2d::fillColor(uint32_t to, uint32_t width, uint32_t height, uint32_t color)
{
    DMA2D->CR = (0b11 << DMA2D_CR_MODE_Pos);
    DMA2D->OPFCCR = 0;
    DMA2D->OMAR = to;
    DMA2D->OCOLR = color;
    DMA2D->NLR = (width << 16) | (height & 0xFFFF);
    DMA2D->OOR = 0;
    start();
}

void Dma2d::start()
{
    DMA2D->IFCR = DMA2D_IFCR_CTCIF | DMA2D_IFCR_CTEIF;
    DMA2D->CR |= DMA2D_CR_START;
    while ((DMA2D->ISR & DMA2D_ISR_TCIF) == 0) {
    }
}

}