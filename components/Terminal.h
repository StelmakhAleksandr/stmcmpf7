#pragma once

#include "../drivers/Clocks.h"
// #include <streambuf>
#include <string>

namespace stmcmp {

void printf(const char* format, ...);

struct TerminalConfig
{
    uint32_t baudrate;
};

class Terminal //: public std::streambuf
{
public:
    static Terminal* instance();
    void send(const std::string& msg);
    void setup(const TerminalConfig& config, const Clocks& clocks);

protected:
    // virtual int_type overflow(int_type c) override
    // {
    //     sendChar(static_cast<char>(c));
    //     return c;
    // }

private:
    Terminal() = default;
    void sendChar(char c);
};

};