#pragma once

#include "../drivers/Clocks.h"
#include <chrono>

extern "C" void SysTick_Handler();

namespace stmcmp {

class SystemClock
{
    friend void ::SysTick_Handler();

public:
    static SystemClock* instance();
    void setup(const Clocks& clocks);
    void delay_ms(uint32_t us);

    template <typename Rep, typename Period>
    void delay(std::chrono::duration<Rep, Period> duration)
    {
        auto durationInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        delay_ms(durationInMilliseconds);
    }
    uint32_t ticks();
    static uint32_t millisecondsElapsed();

private:
    SystemClock() = default;
    volatile uint32_t m_tickCount;
};

};