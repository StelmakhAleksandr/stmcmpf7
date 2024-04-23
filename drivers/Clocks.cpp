#include "Clocks.h"

#include "stm32f746xx.h"
#include "stmcmpf7/stmcmp.h"

namespace stmcmp {

namespace {
constexpr uint32_t HSI_FREQ = 16_MHz;
constexpr uint32_t PLLM_MAX = 63;
constexpr uint32_t PLLM_MIN = 2;
constexpr uint32_t PLLN_MIN = 50;
constexpr uint32_t PLLN_MAX = 432;
constexpr uint32_t VCO_MIN = 1_MHz;
constexpr uint32_t VCO_MAX = 2_MHz;
constexpr uint32_t VCOCLK_MIN = 50_MHz;
constexpr uint32_t VCOCLK_MAX = 432_MHz;
constexpr auto PLLP_VALUES = { 2, 4, 6, 8 };
};

unsigned long long Clocks::pclk2() const
{
    return m_pclk2;
}

unsigned long long Clocks::sysclk() const
{
    return m_sysclk;
}

void Clocks::runHse()
{
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0) {
    };
}

void Clocks::runPll()
{
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
    };
}

void Clocks::runPllclk()
{
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0) {
    };
}

void Clocks::setupPll(uint32_t m, uint32_t n, PLLP p)
{
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN);
    RCC->PLLCFGR |= m_hse.has_value() ? RCC_PLLCFGR_PLLSRC_HSE : RCC_PLLCFGR_PLLSRC_HSI;
    RCC->PLLCFGR |= (n << RCC_PLLCFGR_PLLN_Pos);
    RCC->PLLCFGR |= (m << RCC_PLLCFGR_PLLM_Pos);
    RCC->PLLCFGR |= ((uint32_t)p << RCC_PLLCFGR_PLLP_Pos);
}

void Clocks::setupBusPrescalers()
{
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
}

void Clocks::setupFlash()
{
    FLASH->ACR |= FLASH_ACR_LATENCY_7WS;
}

ClocksBuilder& ClocksBuilder::setHclk(unsigned long long value)
{
    m_clock.m_hclk = value;
    return *this;
}

ClocksBuilder& ClocksBuilder::setPclk1(unsigned long long value)
{
    m_clock.m_pclk1 = value;
    return *this;
}

ClocksBuilder& ClocksBuilder::setPclk2(unsigned long long value)
{
    m_clock.m_pclk2 = value;
    return *this;
}

ClocksBuilder& ClocksBuilder::setSysclk(unsigned long long value)
{
    m_clock.m_sysclk = value;
    return *this;
}

ClocksBuilder& ClocksBuilder::setTimclk1(unsigned long long value)
{
    m_clock.m_timclk1 = value;
    return *this;
}

ClocksBuilder& ClocksBuilder::setTimclk2(unsigned long long value)
{
    m_clock.m_timclk2 = value;
    return *this;
}

ClocksBuilder& ClocksBuilder::setHse(unsigned long long value)
{
    m_clock.m_hse = value;
    return *this;
}

Clocks ClocksBuilder::build()
{
    uint32_t m = PLLM_MIN;
    uint32_t n = PLLN_MAX;
    std::optional<PLLP> p = std::nullopt;
    calculatePllDivs(m, n, p);
    if (m_clock.m_hse.has_value()) {
        m_clock.runHse();
    }
    m_clock.setupPll(m, n, PLLP::Div2);
    m_clock.runPll();
    m_clock.setupBusPrescalers();
    m_clock.setupFlash();
    m_clock.runPllclk();

    return m_clock;
}

void ClocksBuilder::calculatePllDivs(uint32_t& m, uint32_t& n, std::optional<PLLP>& p)
{
    auto baseClock = m_clock.m_hse.has_value() ? m_clock.m_hse.value() : HSI_FREQ;
    while (true) {
        if (m > PLLM_MAX) {
            break;
        }
        auto vco = baseClock / m;
        if (vco < VCO_MIN) {
            break;
        }
        if (vco > VCO_MAX || n < PLLN_MIN) {
            m++;
            n = PLLN_MAX;
            continue;
        }
        auto vcoClk = baseClock * n / m;
        if (vcoClk < VCOCLK_MIN) {
            m++;
            n = PLLN_MAX;
            continue;
        }
        if (vcoClk > VCOCLK_MAX) {
            n--;
            continue;
        }
        for (auto divP : PLLP_VALUES) {
            auto pllClkOut = vcoClk / divP;
            if (pllClkOut >= (m_clock.m_sysclk - 1) && pllClkOut <= (m_clock.m_sysclk + 1)) {
                switch (divP) {
                case 2:
                    p = PLLP::Div2;
                    break;
                case 4:
                    p = PLLP::Div4;
                    break;
                case 6:
                    p = PLLP::Div6;
                    break;
                case 8:
                    p = PLLP::Div8;
                    break;
                default:
                    break;
                }
                break;
            }
        }
        if (p.has_value()) {
            break;
        } else {
            n -= 1;
            continue;
        }
    }
}

};