#include "Gpio.h"

namespace {
constexpr uint32_t ModerBits = 2;
constexpr uint32_t SpeedBits = 2;
constexpr uint32_t TyperBits = 1;
constexpr uint32_t PupdrBits = 2;
constexpr uint32_t AFRBits = 4;
constexpr uint32_t RegisterHMinPinNumber = 8;
};

namespace stmcmp {

GpioPin::GpioPin(GPIO_TypeDef* port, uint8_t pin)
    : m_port(port)
    , m_pin(pin)
{
}

void GpioPin::toggle()
{
    toggleValue(&m_port->ODR, 1);
}

void GpioPin::setModer(PinModer value)
{
    setValue(&m_port->MODER, ModerBits, (uint32_t)value);
}

void GpioPin::setSpeed(PinSpeed value)
{
    setValue(&m_port->OSPEEDR, SpeedBits, (uint32_t)value);
}

void GpioPin::setType(PinType value)
{
    setValue(&m_port->OTYPER, TyperBits, (uint32_t)value);
}

void GpioPin::setPull(PinPull value)
{
    setValue(&m_port->PUPDR, PupdrBits, (uint32_t)value);
}

void GpioPin::setAlternateFunction(uint8_t afValue)
{
    setModer(PinModer::AF);
    if (m_pin < RegisterHMinPinNumber) {
        setValue(&m_port->AFR[0], AFRBits, afValue);
    } else {
        setValue(&m_port->AFR[1], AFRBits, afValue, RegisterHMinPinNumber);
    }
}

void GpioPin::setValue(__IO uint32_t* reg, uint32_t bitsPerValue, uint32_t value, uint8_t offsetPin)
{
    *reg &= ~(((1UL << bitsPerValue) - 1) << (m_pin * bitsPerValue));
    *reg |= (value << ((m_pin - offsetPin) * bitsPerValue));
}

void GpioPin::toggleValue(__IO uint32_t* reg, uint32_t bitsPerValue, uint8_t offsetPin)
{
    *reg ^= (1UL << ((m_pin - offsetPin) * bitsPerValue));
}

GpioBuilder::GpioBuilder(uint32_t port, uint8_t pin)
    : m_port(port)
    , m_pin(pin)
{
}

GpioBuilder& GpioBuilder::setModer(PinModer value)
{
    m_moder = value;
    return *this;
}

GpioBuilder& GpioBuilder::setType(PinType value)
{
    m_type = value;
    return *this;
}

GpioBuilder& GpioBuilder::setSpeed(PinSpeed value)
{
    m_speed = value;
    return *this;
}

GpioBuilder& GpioBuilder::setPull(PinPull value)
{
    m_pull = value;
    return *this;
}

GpioBuilder& GpioBuilder::setAlternateFunction(uint8_t value)
{
    m_af = value;
    return *this;
}

GpioPin GpioBuilder::build()
{
    auto index = ((uint32_t)m_port - AHB1PERIPH_BASE) / (GPIOB_BASE - AHB1PERIPH_BASE);
    RCC->AHB1ENR |= (1UL << index);
    GpioPin pin((GPIO_TypeDef*)m_port, m_pin);
    pin.setModer(m_moder);
    pin.setSpeed(m_speed);
    pin.setPull(m_pull);
    pin.setType(m_type);
    if (m_af > 0) {
        pin.setAlternateFunction(m_af);
    }
    return pin;
}

};