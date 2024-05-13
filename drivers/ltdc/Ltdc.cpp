#include "Ltdc.h"

#include "../../components/SdRam.h"
#include "../../components/Terminal.h"
#include <vector>

namespace stmcmp {

void Ltdc::setup(const LcdConfig& config)
{
    for (GpioBuilder pin : config.ltdcPins) {
        pin.build();
    }
    setupPllSai();
    RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
    uint32_t horizontalTotalPorch = config.horizontalSync + config.horizontalBackPorch;
    uint32_t verticalTotalPorch = config.verticalSync + config.verticalBackPorch;
    uint32_t totalWidth = horizontalTotalPorch + config.activeWidth + config.horizontalFrontPorch;
    uint32_t totalHeight = verticalTotalPorch + config.activeHeight + config.verticalFrontPorch;
    // auto lcdClk = totalWidth * totalHeight * config.frameRate;
    LTDC->SSCR = ((config.horizontalSync - 1) << LTDC_SSCR_HSW_Pos) | ((config.verticalSync - 1) << LTDC_SSCR_VSH_Pos);
    LTDC->BPCR = ((horizontalTotalPorch - 1) << LTDC_BPCR_AHBP_Pos) | ((verticalTotalPorch - 1) << LTDC_BPCR_AVBP_Pos);
    LTDC->AWCR = ((horizontalTotalPorch + config.activeWidth) << LTDC_AWCR_AAW_Pos) | ((verticalTotalPorch + config.activeHeight) << LTDC_AWCR_AAH_Pos);
    LTDC->TWCR = ((totalWidth) << LTDC_TWCR_TOTALW_Pos) | ((totalHeight) << LTDC_TWCR_TOTALH_Pos);
    LTDC->GCR = (config.horizontalSyncPolarity ? LTDC_GCR_HSPOL : 0) | (config.verticalSyncPolarity ? LTDC_GCR_VSPOL : 0) | (config.noDataEnablePolarity ? LTDC_GCR_DEPOL : 0) | (config.pixelClockPolarity ? LTDC_GCR_PCPOL : 0);
    LTDC->BCCR = 0xFF0000FF;
    reload();
    LTDC->GCR |= LTDC_GCR_LTDCEN | LTDC_GCR_DEN;
    reload();

    for (auto& layerConfig : config.layers) {
        Layer* layer = new Layer();
        layer->setup(horizontalTotalPorch, verticalTotalPorch, config.activeWidth, config.activeHeight, layerConfig);
        m_layers.push_back(layer);
    }
    reload();
}

void Ltdc::setupPllSai()
{
    RCC->PLLSAICFGR = (108 << RCC_PLLSAICFGR_PLLSAIN_Pos | 2 << RCC_PLLSAICFGR_PLLSAIP_Pos | 2 << RCC_PLLSAICFGR_PLLSAIQ_Pos | 7 << RCC_PLLSAICFGR_PLLSAIR_Pos);
    RCC->DCKCFGR1 &= ~RCC_DCKCFGR1_PLLSAIDIVR;
    // RCC->DCKCFGR1 |= (4 << RCC_DCKCFGR1_PLLSAIDIVR_Pos);
    RCC->CR |= RCC_CR_PLLSAION;
    while ((RCC->CR & RCC_CR_PLLSAIRDY) == 0) {
    }
}

Layer* Ltdc::layer(int index)
{
    return m_layers[index];
}

void Ltdc::reload()
{
    LTDC->SRCR = LTDC_SRCR_IMR;
}

}