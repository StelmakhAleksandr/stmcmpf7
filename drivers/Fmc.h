
#pragma once

#include "Pins.h"
#include "stm32f746xx.h"
#include <vector>

namespace stmcmp {

#define MRD_CAS_LATENCY_POS (4U)
#define MRD_BURST_LENGTH_POS (0U)
#define MRD_BURST_TYPE_POS (3U)
#define MRD_WRITE_MODE_POS (9U)

enum class ColumnBits : uint8_t
{
    Bits8 = 0b00,
    Bits9 = 0b01,
    Bits10 = 0b10,
    Bits11 = 0b11
};

enum class DataWidth : uint8_t
{
    Bits8 = 0b00,
    Bits16 = 0b01,
    Bits32 = 0b10
};

enum class BankCount : uint8_t
{
    Two = 0b0,
    Four = 0b1
};

enum class CasLatency : uint8_t
{
    Latency1 = 0b01,
    Latency2 = 0b10,
    Latency3 = 0b11
};

enum class SdramClock : uint8_t
{
    Disabled = 0b00,
    TwoHCLK = 0b10,
    ThreeHCLK = 0b11
};

enum class BurstRead : uint8_t
{
    Disabled = 0,
    Enabled = 1
};

enum class ReadPipe : uint8_t
{
    NoDelay = 0b00,
    OneCycleDelay = 0b01,
    TwoCycleDelay = 0b10
};

enum class SdramCycles : uint8_t
{
    Cycles1 = 0,
    Cycles2 = 1,
    Cycles3 = 2,
    Cycles4 = 3,
    Cycles5 = 4,
    Cycles6 = 5,
    Cycles7 = 6,
    Cycles8 = 7,
    Cycles9 = 8,
    Cycles10 = 9,
    Cycles11 = 10,
    Cycles12 = 11,
    Cycles13 = 12,
    Cycles14 = 13,
    Cycles15 = 14,
    Cycles16 = 15
};

enum class SdramCommandMode : uint8_t
{
    Normal = 0b000,
    ClockConfigurationEnable = 0b001,
    AllBankPrecharge = 0b010,
    AutoRefresh = 0b011,
    LoadModeRegister = 0b100,
    SelfRefresh = 0b101,
    PowerDown = 0b110,
    Reserved = 0b111
};

enum class BurstLength : uint8_t
{
    One = 0b000,
    Two = 0b001,
    Four = 0b010,
    Eight = 0b011,
    FullPage = 0b111
};

enum class BurstType : uint8_t
{
    Sequential = 0b0,
    Interleaved = 0b1
};

enum class WriteMode : uint8_t
{
    ProgrammedBurstLength = 0b0,
    SingleLocationAccess = 0b1
};

struct SdcrConfiguration
{
    ColumnBits column;
    DataWidth dataWidth;
    BankCount bankCount;
    CasLatency casLatency;
    SdramClock sdramClock;
    BurstRead burstRead;
    ReadPipe readPipe;

    uint32_t value() const
    {
        return (static_cast<uint32_t>(column) << FMC_SDCR1_NR_Pos)
            | (static_cast<uint32_t>(dataWidth) << FMC_SDCR1_MWID_Pos)
            | (static_cast<uint32_t>(bankCount) << FMC_SDCR1_NB_Pos)
            | (static_cast<uint32_t>(casLatency) << FMC_SDCR1_CAS_Pos)
            | (static_cast<uint32_t>(sdramClock) << FMC_SDCR1_SDCLK_Pos)
            | (static_cast<uint32_t>(burstRead) << FMC_SDCR1_RBURST_Pos)
            | (static_cast<uint32_t>(readPipe) << FMC_SDCR1_RPIPE_Pos);
    }
};

struct SdtrConfiguration
{
    SdramCycles modeRegToActive;
    SdramCycles exitSelfRefresh;
    SdramCycles activeToPrecharge;
    SdramCycles rowCycle;
    SdramCycles writeRecovery;
    SdramCycles rowPrecharge;
    SdramCycles rowToColumn;

    uint32_t value() const
    {
        return (static_cast<uint32_t>(modeRegToActive) << FMC_SDTR1_TMRD_Pos)
            | (static_cast<uint32_t>(exitSelfRefresh) << FMC_SDTR1_TXSR_Pos)
            | (static_cast<uint32_t>(activeToPrecharge) << FMC_SDTR1_TRAS_Pos)
            | (static_cast<uint32_t>(rowCycle) << FMC_SDTR1_TRC_Pos)
            | (static_cast<uint32_t>(writeRecovery) << FMC_SDTR1_TWR_Pos)
            | (static_cast<uint32_t>(rowPrecharge) << FMC_SDTR1_TRP_Pos)
            | (static_cast<uint32_t>(rowToColumn) << FMC_SDTR1_TRCD_Pos);
    }
};

struct MrdConfiguration
{
    CasLatency casLatency;
    BurstLength burstLength;
    BurstType burstType;
    WriteMode writeMode;

    uint32_t value() const
    {
        return (static_cast<uint32_t>(casLatency) << MRD_CAS_LATENCY_POS)
            | (static_cast<uint32_t>(burstLength) << MRD_BURST_LENGTH_POS)
            | (static_cast<uint32_t>(burstType) << MRD_BURST_TYPE_POS)
            | (static_cast<uint32_t>(writeMode) << MRD_WRITE_MODE_POS);
    }
};

struct SdramConfig
{
    SdcrConfiguration control;
    SdtrConfiguration timing;
    MrdConfiguration mrd;
    SdramCycles autoRefresh;
    uint32_t refreshTime;
    std::vector<GpioBuilder> fmcPins;
};

class FmcSdram
{
public:
    FmcSdram() = default;
    void setup(const SdramConfig& config);

private:
    void setMode(SdramCommandMode mode, uint32_t additional = 0);
};

};