#pragma once
#include "common.h"
const u32 REGION_MASK[8] = {
    // KUSEG: 2048MB
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    // KSEG0: 512MB
    0x7fffffff,
    // KSEG1: 512MB
    0x1fffffff,
    // KSEG2: 1024MB
    0xffffffff, 0xffffffff};
class RegisterIndex
{
public:
    u32 index;
    RegisterIndex() = default;
    RegisterIndex(u32 index)
    {
        this->index = index;
    }
};

class RegTuple
{
public:
    RegisterIndex index;
    u32 value;
    RegTuple() = default;
    RegTuple(RegisterIndex index, u32 value)
    {
        this->index = index;
        this->value = value;
    }
};

class Range
{
public:
    u32 start;
    u32 length;
    Range(u32 x, u32 y)
    {
        this->start = x;
        this->length = y;
    }
};
enum class ExceptionType{
    Syscall = 0x8,
    Overflow = 0xc,
    LoadAddressError = 0x4,
    StoreAddressError = 0x5,
    Break = 0x9,
    CoprocessorError = 0xb,
    IllegalInstruction = 0xa
};

//DMA
enum class Direction{
    ToRam = 0,
    FromRam = 1
};
enum class Step{
    Increment = 0,
    Decrement = 1
};
enum class Sync{
    Manual = 0,
    Request = 1,
    LinkedList = 2
};
enum class Port{
    MdecIn = 0,
    MdecOut = 1,
    Gpu = 2,
    CdRom = 3,
    Spu = 4,
    Pio = 5,
    Otc = 6
};
//

//GPU
enum class TextureDepth{
    T4Bits = 0,
    T8Bits = 1,
    T15Bits = 2
};
enum class Field{
    Top = 1,
    Bottom = 0
};
//TODO
class HorizontalRes{
    u8 hr;
    public:
    HorizontalRes() = default;
    HorizontalRes(u8 hr1, u8 hr2){
        this->hr = (hr2 & 1) | ((hr1 & 3) << 1);
    }
    // HorizontalRes from_fields(u8 hr1, u8 hr2){
    //     return HorizontalRes(hr1,hr2);
    // }
    u32 into_status(){
        return (u32)this->hr <<16;
    }
};
//
enum class VerticalRes{
    Y240Lines = 0,
    Y480Lines = 1
};
enum class VMode{
    Ntsc = 0,
    Pal = 1
};
enum class DisplayDepth{
    D15Bits = 0,
    D24Bits = 1
};
enum class DMADirection{
    Off = 0,
    Fifo = 1,
    CpuToGp0= 2,
    VRamToCpu = 3
};
class CommandBuffer{
    u32 buffer[12];
    u8 len;
    public:
    CommandBuffer() : buffer{0,0,0,0,0,0,0,0,0,0,0,0}{
        this->len = 0;
    }
    void clear(){
        this->len = 0;
    }
    void push_word(u32 word){
        this->buffer[this->len] = word;
        this->len++;
    }
    u32 index(u32 index){
        if(index >= this->len){
            std::cout<< "Command buffer index out of range "<<index<<" "<<this->len<<std::endl;
            exit(0);
        }
        return this->buffer[index];
    }
};
enum class GP0Command{
    GP0_nop = 0,
    GP0_clear_cache = 0x10,
    GP0_quad_mono_opaque = 0x28,
    GP0_image_load = 0xa0,
    GP0_draw_mode = 0xe1,
    GP0_texture_window = 0xe2,
    GP0_drawing_area_top_left = 0xe3,
    GP0_drawing_area_bottom_right = 0xe4,
    GP0_drawing_offset = 0xe5,
    GP0_mask_bit_setting = 0xe6
};
enum class GP0Mode{
    Command,
    ImageLoad
};
//