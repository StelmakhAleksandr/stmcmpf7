#include "Terminal.h"

#include "../drivers/Pins.h"
#include "stm32f746xx.h"
#include <cstdarg>
#include <cstdio>

namespace stmcmp {

namespace {
constexpr uint32_t USART_PINS_AF = 7;
}

Terminal* Terminal::instance()
{
    static Terminal instance;
    return &instance;
}

void Terminal::send(const std::string& msg)
{
    for (char c : msg) {
        sendChar(c);
    }
}

void Terminal::setup(const TerminalConfig& config, const Clocks& clocks)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    PA9.setAlternateFunction(USART_PINS_AF)
        .setPull(PinPull::No)
        .setSpeed(PinSpeed::VeryHigh)
        .setType(PinType::PushPull)
        .build();
    USART1->CR1 &= ~USART_CR1_M;
    USART1->CR2 |= USART_CR2_RTOEN;
    USART1->BRR = clocks.pclk2() / config.baudrate;

    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 |= USART_CR1_TE;
    // std::cout.rdbuf(this);
}

void Terminal::sendChar(char c)
{
    USART1->ICR &= USART_ICR_TCCF;
    while (!(USART1->ISR & USART_ISR_TXE)) {
    };
    USART1->TDR = c;
    while (!(USART1->ISR & USART_ISR_TC)) {
    };
}

void printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Terminal::instance()->send(buffer);
}

};