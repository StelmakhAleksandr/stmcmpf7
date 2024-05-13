#pragma once

#include "../drivers/I2c.h"
#include "../drivers/Pins.h"

#include <deque>

extern "C" void EXTI15_10_IRQHandler();

namespace stmcmp {

struct TouchConfig
{
    GpioBuilder button = GpioBuilder(0, 0);
    uint8_t address;
    uint8_t statusReg;
    uint8_t firstXhReg;
    uint8_t sizeStateReg;
    uint8_t xBitmask;
    uint8_t yBitmask;
};

struct TouchState
{
    uint16_t x;
    uint16_t y;
    uint8_t weight;
    uint8_t misc;
};

class Touch
{
    friend void ::EXTI15_10_IRQHandler();

public:
    static Touch* instance();
    void setup(const TouchConfig& config);
    bool hasTouch();
    TouchState lastTouch();

private:
    Touch() = default;
    void interrupt();
    GpioPin m_button = GpioBuilder::none();
    I2c m_i2c;
    TouchConfig m_config;

    uint8_t countTouch();
    TouchState state(uint8_t touchNumber);

    std::deque<TouchState> m_states;
};

}