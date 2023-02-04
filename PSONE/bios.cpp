#include "bios.h"
BIOS::BIOS(std::string path) : buffer(512 * 1024)
    {
        std::ifstream newFile(path, std::ios::binary | std::ios::ate);
        newFile.seekg(0, std::ios::beg);
        newFile.read(this->buffer.data(), 512 * 1024);
    }
u8 BIOS::load8(u32 offset)
    {
        return this->buffer[offset] & 0xff;
    }
u32 BIOS::load32(u32 offset)
    {
        return (this->buffer[offset + 0] & 0xff) | ((this->buffer[offset + 1] & 0xff) << 8) | ((this->buffer[offset + 2] & 0xff) << 16) | ((this->buffer[offset + 3] & 0xff) << 24);
    }