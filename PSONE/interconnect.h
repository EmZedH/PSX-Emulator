#pragma once
#include "bios.h"
#include "ram.h"
#include "dma.h"
#include "gpu.h"
#include "memorymap.h"
class Interconnect
{
    Map map;
    BIOS bios;
    RAM ram;
    DMA dma;
    GPU gpu;
public:
    Interconnect() = default;
    Interconnect(BIOS bios);
    Port from_index_port(u32 index);
    u8 load8(u32 address);
    u32 load16(u32 address);
    u32 load32(u32 address);
    void store8(u32 address, u8 value);
    void store16(u32 address, u16 value);
    void store32(u32 address, u32 value);
    //from here
    u32 get_dma_reg(u32 offset);
    void set_dma_reg(u32 offset, u32 value);
    void do_dma(Port port);
    void do_dma_block(Port port);
    void do_dma_linked_list(Port port);

    //extra
    void printRam(u32 instruction,u32 pc,RegisterIndex source,u32 sourceVal){
        if(this->ram.load32(0x74ddc) != 0xcacacaca){
        std::cout<<"Instruction :"<<instruction<<" PC: "<<pc<<" RAM value at 0x74ddc: "<<this->ram.load32(0x74ddc)<<" Source: "<<source.index<<", "<<sourceVal<<std::endl;
        }
        if(this->ram.load32(0x74ddc) == 0x21000000){
        std::cout<<"Instruction :"<<instruction<<" PC: "<<pc<<" RAM value at 0x74ddc: "<<this->ram.load32(0x74ddc)<<std::endl;exit(0);
        }
    }
    //
};