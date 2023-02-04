#include "ram.h"
RAM::RAM() : data(2 * 1024 * 1024, 0xca){}
u8 RAM::load8(u32 offset)
    {
        return this->data[offset];
    }
u16 RAM::load16(u32 offset){
        return (this->data[offset + 0] & 0xff) | ((this->data[offset + 1] & 0xff) << 8);
    }
u32 RAM::load32(u32 offset)
    {
        return (this->data[offset + 0] & 0xff) | ((this->data[offset + 1] & 0xff) << 8) | ((this->data[offset + 2] & 0xff) << 16) | ((this->data[offset + 3] & 0xff) << 24);
    }
void RAM::store8(u32 offset, u8 value)
    {
        this->data[offset] = value;
    }
void RAM::store16(u32 offset, u16 value){
        this->data[offset + 0] = value & 0xff;
        this->data[offset + 1] = (value >> 8) & 0xff;
    }
void RAM::store32(u32 offset, u32 value)
    {
        this->data[offset + 0] = value & 0xff;
        this->data[offset + 1] = (value >> 8) & 0xff;
        this->data[offset + 2] = (value >> 16) & 0xff;
        this->data[offset + 3] = (value >> 24) & 0xff;
    }