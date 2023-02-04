#pragma once
#include <vector>
#include <fstream>
#include "common.h"
class BIOS
{
    std::vector<char> buffer;
public:
    BIOS() = default;
    BIOS(std::string path);
    u8 load8(u32 offset);
    u32 load32(u32 offset);
};