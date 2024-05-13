#include "Touch.h"

#include "Terminal.h"
#include <vector>

extern "C" void EXTI15_10_IRQHandler()
{
    stmcmp::Touch::instance()->interrupt();
    EXTI->PR = EXTI_PR_PR13;
}

namespace stmcmp {

Touch* Touch::instance()
{
    static Touch instance;
    return &instance;
}

void Touch::setup(const TouchConfig& config)
{
    m_config = config;
    m_button = GpioBuilder(config.button).setModer(PinModer::Input).setSpeed(PinSpeed::VeryHigh).setExti(true, false).build();
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    auto scl = PH7.setModer(PinModer::AF).setType(PinType::OpenDrain).setAlternateFunction(4).build();
    auto sda = PH8.setModer(PinModer::AF).setType(PinType::OpenDrain).setAlternateFunction(4).build();

    m_i2c.setup(I2C3);
}

bool Touch::hasTouch()
{
    return m_states.size() > 0;
}

void Touch::interrupt()
{
    auto touch = countTouch();
    // stmcmp::printf("touch = %d \r\n", touch);
    for (int i = 0; i < touch; i++) {
        m_states.push_back(state(i));
        // auto state = this->state(i);
        // stmcmp::printf("%d) x = %d y = %d w = %d misc = %d\r\n", i + 1, state.x, state.y, state.weight, state.misc);
    }
}

uint8_t Touch::countTouch()
{
    uint8_t reg = m_config.statusReg;
    uint8_t result;
    m_i2c.writeRead(m_config.address,
        {
            .addr = &reg,
            .len = 1,
        },
        {
            .addr = &result,
            .len = 1,
        });
    return result;
}

TouchState Touch::state(uint8_t touchNumber)
{
    uint8_t reg = m_config.firstXhReg + touchNumber * m_config.sizeStateReg;
    std::vector<uint8_t> buf(m_config.sizeStateReg);
    m_i2c.writeRead(m_config.address,
        {
            .addr = &reg,
            .len = 1,
        },
        {
            .addr = &buf[0],
            .len = m_config.sizeStateReg,
        });
    uint16_t y = ((m_config.yBitmask & buf[0]) << 8) | buf[1];
    uint16_t x = ((m_config.xBitmask & buf[2]) << 8) | buf[3];
    return {
        .x = x,
        .y = y,
        .weight = buf[4],
        .misc = buf[5],
    };
}

TouchState Touch::lastTouch()
{
    if (!hasTouch()) {
        return {};
    }
    auto result = m_states.front();
    m_states.pop_front();
    return result;
}

}