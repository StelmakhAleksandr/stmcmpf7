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
    SysTick->LOAD = clocks.sysclk() / 1_kHz - 1;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SystemClock::delay_ms(uint32_t us)
{
    int start = m_tickCount;
    while ((m_tickCount - start) < us)
        ;
}

uint32_t SystemClock::ticks()
{
    return m_tickCount;
}

uint32_t SystemClock::millisecondsElapsed()
{
    return SystemClock::instance()->ticks(); /// 1000;
}

};
