#include "I2c.h"

namespace stmcmp {

void I2c::setup(I2C_TypeDef* i2c)
{
    m_i2c = i2c;
    if (i2c == I2C1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    } else if (i2c == I2C2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    } else if (i2c == I2C3) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    } else if (i2c == I2C4) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C4EN;
    }
    m_i2c->CR1 &= ~I2C_CR1_PE;
    uint32_t timingr = (3 << 28) | // PRESC
        (3 << 20) | // SCLDEL
        (1 << 16) | // SDADEL
        (30 << 8) | // SCLH
        (40 << 0); // SCLL
    m_i2c->TIMINGR = timingr;
    m_i2c->OAR1 &= ~I2C_OAR1_OA1EN;
    m_i2c->OAR2 &= ~I2C_OAR2_OA2EN;
    m_i2c->CR1 |= I2C_CR1_PE;
}

void I2c::writeRead(uint32_t address, I2cMem send, I2cMem receive)
{
    start(address, 0, 1);
    write(*send.addr);
    stop();
    start(address, 1, receive.len);
    size_t curr = 0;
    auto addr = receive.addr;
    while (curr != receive.len) {
        *addr = read();
        addr++;
        curr++;
    }
    stop();
}

void I2c::start(uint32_t address, uint8_t direction, uint8_t bytes)
{
    while (m_i2c->ISR & I2C_ISR_BUSY) {
    }
    m_i2c->CR2 &= ~(I2C_CR2_SADD_Msk | I2C_CR2_RD_WRN_Msk | I2C_CR2_NBYTES_Msk | I2C_CR2_RELOAD_Msk | I2C_CR2_AUTOEND_Msk);
    m_i2c->CR2 |= ((address << 1) << I2C_CR2_SADD_Pos) & I2C_CR2_SADD_Msk;
    m_i2c->CR2 |= (direction << I2C_CR2_RD_WRN_Pos) & I2C_CR2_RD_WRN_Msk;
    m_i2c->CR2 |= (bytes << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES_Msk;
    m_i2c->CR2 |= I2C_CR2_START;
}

void I2c::stop()
{
    if (!(m_i2c->CR2 & I2C_CR2_AUTOEND)) {
        m_i2c->CR2 |= I2C_CR2_STOP;
    }
    while (!(m_i2c->ISR & I2C_ISR_STOPF)) {
    }
    m_i2c->ICR |= I2C_ICR_STOPCF;
}

void I2c::write(uint8_t data)
{
    while (!(m_i2c->ISR & I2C_ISR_TXIS)) {
    }
    m_i2c->TXDR = data;
    while (!(m_i2c->ISR & I2C_ISR_TC)) {
    }
}

uint8_t I2c::read()
{
    while (!(m_i2c->ISR & I2C_ISR_RXNE)) {
    }
    return m_i2c->RXDR;
}

}