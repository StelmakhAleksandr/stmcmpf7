#include "SdRam.h"

#include "config/Sdram.h"

namespace stmcmp {

SdRam* SdRam::instance()
{
    static SdRam instance;
    return &instance;
}

void SdRam::setup(const SdramConfig& config)
{
    m_fmc.setup(config);
    m_blockMap.resize(SDRAM_NUM_BLOCKS, true);
}

void* SdRam::allocate(size_t size)
{
    printf("allocate sz = %d \r\n", size);
    size_t blocksNeeded = (size + SDRAM_BLOCK_SIZE - 1) / SDRAM_BLOCK_SIZE;
    size_t sequence = 0;
    for (size_t i = 0; i < SDRAM_NUM_BLOCKS; ++i) {
        if (m_blockMap[i]) {
            sequence++;
            if (sequence == blocksNeeded) {
                size_t startBlock = i - blocksNeeded + 1;
                for (size_t j = startBlock; j <= i; ++j) {
                    m_blockMap[j] = false;
                }
                auto addr = SDRAM_START + startBlock * SDRAM_BLOCK_SIZE;
                return reinterpret_cast<void*>(addr);
            }
        } else {
            sequence = 0;
        }
    }
    return nullptr;
}

void SdRam::deallocate(void* ptr, size_t size)
{
    uintptr_t startBlock = (reinterpret_cast<uintptr_t>(ptr) - SDRAM_START) / SDRAM_BLOCK_SIZE;
    size_t blocksToFree = (size + SDRAM_BLOCK_SIZE - 1) / SDRAM_BLOCK_SIZE;
    for (size_t i = startBlock; i < startBlock + blocksToFree; ++i) {
        m_blockMap[i] = true;
    }
}

bool SdRam::testMemory()
{
    int* sdram = reinterpret_cast<int*>(SDRAM_START);
    size_t elements = SDRAM_SIZE / sizeof(int);
    for (size_t i = 0; i < elements; i++) {
        sdram[i] = static_cast<int>(i);
    }
    for (size_t i = 0; i < elements; i++) {
        if (sdram[i] != static_cast<int>(i)) {
            return false;
        }
    }
    return true;
}

};