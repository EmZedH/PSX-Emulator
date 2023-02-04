#pragma once
#include <vector>
#include "common.h"
class RAM
{
    std::vector<char> data;
public:
    RAM();
    u8 load8(u32 offset);
    u16 load16(u32 offset);
    u32 load32(u32 offset);
    void store8(u32 offset, u8 value);
    void store16(u32 offset, u16 value);
    void store32(u32 offset, u32 value);
};