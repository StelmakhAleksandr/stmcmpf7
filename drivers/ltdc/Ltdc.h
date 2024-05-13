#pragma once

#include "Layer.h"
#include <vector>

namespace stmcmp {

struct LcdConfig
{
    std::vector<GpioBuilder> ltdcPins;
    GpioBuilder displayButton;
    GpioBuilder backLight;
    uint32_t activeWidth;
    uint32_t activeHeight;
    uint32_t frameRate;
    uint32_t horizontalBackPorch;
    uint32_t horizontalFrontPorch;
    uint32_t verticalFrontPorch;
    uint32_t verticalBackPorch;
    uint32_t horizontalSync;
    uint32_t verticalSync;
    bool horizontalSyncPolarity;
    bool verticalSyncPolarity;
    bool noDataEnablePolarity;
    bool pixelClockPolarity;
    std::vector<LayerConfig> layers;
};

class Ltdc
{
public:
    void setup(const LcdConfig& config);
    Layer* layer(int index);
    void reload();

private:
    std::vector<Layer*> m_layers;
    void setupPllSai();
};

};