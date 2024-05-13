#pragma once

#include "../drivers/Pins.h"

namespace stmcmp {

struct I2cMem
{
    uint8_t* addr;
    size_t len;
};

class I2c
{
public:
    I2c() = default;
    void setup(I2C_TypeDef* i2c);
    void writeRead(uint32_t address, I2cMem send, I2cMem read);

private:
    I2C_TypeDef* m_i2c;
    void start(uint32_t address, uint8_t direction, uint8_t bytes);
    void stop();
    void write(uint8_t data);
    uint8_t read();
};

}