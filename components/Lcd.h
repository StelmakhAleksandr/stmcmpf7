#pragma once

#include "../drivers/Ltdc.h"

namespace stmcmp {

class Lcd
{
public:
    static Lcd* instance();
    void setup(const LcdConfig& config);
    void on();
    void off();
    void backLightOn();
    void backLightOff();
    void* workBuffer(int layerIndex);
    void swapBuffers(int layerIndex);

private:
    Lcd() = default;
    GpioPin m_backlight = GpioBuilder::none();
    GpioPin m_button = GpioBuilder::none();
    Ltdc m_ltdc;
};

};