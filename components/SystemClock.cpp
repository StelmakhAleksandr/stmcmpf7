#include "SystemClock.h"

#include "stm32f746xx.h"

extern "C" void SysTick_Handler()
{
    stmcmp::SystemClock::instance()->m_tickCount += 1;
}

namespace stmcmp {

SystemClock* SystemClock::instance()
{
    static SystemClock instance;
    return &instance;
}

void SystemClock::setup(const Clocks& clocks)
{
    SysTick->LOAD = clocks.sysclk() / 1000 - 1;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SystemClock::delay_ms(uint32_t us)
{
    int start = m_tickCount;
    while ((m_tickCount - start) < us)
        ;
}

};
