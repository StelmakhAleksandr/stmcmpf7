#include "Fmc.h"

#include "config/Sdram.h"

namespace stmcmp {

namespace {
constexpr uint32_t FMC_PINS_AF = 12;
}

void FmcSdram::setup(const SdramConfig& config)
{
    for (auto pin : config.fmcPins) {
        pin.setModer(PinModer::Output)
            .setType(PinType::PushPull)
            .setSpeed(PinSpeed::VeryHigh)
            .setPull(PinPull::Up)
            .setAlternateFunction(FMC_PINS_AF)
            .build();
    }
    RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
    FMC_Bank5_6->SDCR[0] &= ~(FMC_SDCR1_NC
        | FMC_SDCR1_NR
        | FMC_SDCR1_MWID
        | FMC_SDCR1_NB
        | FMC_SDCR1_CAS
        | FMC_SDCR1_WP
        | FMC_SDCR1_SDCLK
        | FMC_SDCR1_RBURST
        | FMC_SDCR1_RPIPE);
    FMC_Bank5_6->SDTR[0] &= ~(FMC_SDTR1_TMRD
        | FMC_SDTR1_TXSR
        | FMC_SDTR1_TRAS
        | FMC_SDTR1_TRC
        | FMC_SDTR1_TWR
        | FMC_SDTR1_TRP
        | FMC_SDTR1_TRCD);
    FMC_Bank5_6->SDCR[0] |= config.control.value();
    FMC_Bank5_6->SDRTR = config.refreshTime;
    FMC_Bank5_6->SDTR[0] |= config.timing.value();
    setMode(SdramCommandMode::ClockConfigurationEnable);
    setMode(SdramCommandMode::AllBankPrecharge);
    setMode(SdramCommandMode::AutoRefresh, (uint32_t)config.autoRefresh << FMC_SDCMR_NRFS_Pos);
    setMode(SdramCommandMode::LoadModeRegister, config.mrd.value() << FMC_SDCMR_MRD_Pos);
    setMode(SdramCommandMode::Normal);
}

void FmcSdram::setMode(SdramCommandMode mode, uint32_t additional)
{
    FMC_Bank5_6->SDCMR = ((uint32_t)mode << FMC_SDCMR_MODE_Pos) | FMC_SDCMR_CTB1 | additional;
    while ((FMC_Bank5_6->SDSR & FMC_SDSR_BUSY) != 0) {
    }
}

};