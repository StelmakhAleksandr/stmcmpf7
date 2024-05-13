#pragma once

#include "../drivers/Fmc.h"
#include "Terminal.h"
#include <cstdint>

namespace stmcmp {

template <typename T>
class SdramAllocator;

class SdRam
{
public:
    static SdRam* instance();
    void setup(const SdramConfig& config);

    void* allocate(size_t bytes);
    void deallocate(void* ptr, size_t size);
    bool testMemory();

private:
    SdRam() = default;
    FmcSdram m_fmc;
    std::vector<bool> m_blockMap;
};

template <typename T>
class SdramAllocator : public std::allocator<T>
{
public:
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using value_type = T;

    template <typename U>
    struct rebind
    {
        using other = SdramAllocator<U>;
    };
    SdramAllocator() = default;
    template <typename U>
    SdramAllocator(const SdramAllocator<U>&)
    {
    }
    pointer allocate(size_type n, const void* hint = 0)
    {
        return reinterpret_cast<pointer>(SdRam::instance()->allocate(sizeof(T) * n));
    }
    void deallocate(pointer p, size_type n)
    {
        SdRam::instance()->deallocate(p, sizeof(T) * n);
    }
};

};