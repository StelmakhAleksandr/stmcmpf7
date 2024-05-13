#include "Lcd.h"

namespace stmcmp {

Lcd* Lcd::instance()
{
    static Lcd instance;
    return &instance;
}

void Lcd::setup(const LcdConfig& config)
{
    m_button = GpioBuilder(config.displayButton).setModer(PinModer::Output).setSpeed(PinSpeed::VeryHigh).setType(PinType::PushPull).build();
    m_backlight = GpioBuilder(config.backLight).setModer(PinModer::Output).setSpeed(PinSpeed::VeryHigh).setType(PinType::PushPull).build();
    on();
    backLightOn();
    m_ltdc.setup(config);
}

void Lcd::on()
{
    m_button.on();
}

void Lcd::off()
{
    m_button.off();
}

void Lcd::backLightOn()
{
    m_backlight.on();
}

void Lcd::backLightOff()
{
    m_backlight.off();
}

void* Lcd::workBuffer(int layerIndex)
{
    return m_ltdc.layer(layerIndex)->workBuffer();
}

void Lcd::swapBuffers(int layerIndex)
{
    m_ltdc.layer(layerIndex)->swapBuffers();
    // m_ltdc.reload();
}

};