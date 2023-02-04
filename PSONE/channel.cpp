#include "channel.h"
Channel::Channel(){
        this->enable = false;
        this->direction = Direction::ToRam;
        this->step = Step::Increment;
        this->sync = Sync::Manual;
        this->trigger = false;
        this->chop = false;
        this->chop_dma_sz = 0;
        this->chop_cpu_sz = 0;
        this->dummy = 0;
        this->base = 0;
        this->block_size = 0;
        this->block_count = 0;
    }
u32 Channel::get_control(){
        return (((u32)this->direction << 0) | ((u32)this->step << 1) | ((u32)this->chop << 8) | ((u32)this->sync << 9) | ((u32)this->chop_dma_sz << 16) | ((u32)this->chop_cpu_sz << 20) | ((u32)this->enable << 24) | ((u32)this->trigger << 28) | ((u32)this->dummy << 29));
    }
void Channel::set_control(u32 value){
        this->direction = (value & 1) != 0 ? Direction::FromRam : Direction::ToRam;
        this->step = ((value >> 1) & 1) !=0 ? Step::Decrement : Step::Increment;
        this->chop = ((value >> 8) & 1) !=0;
        switch ((value >> 9) & 3)
        {
        case 0:
            this->sync = Sync::Manual;
            break;
        case 1:
            this->sync = Sync::Request;
            break;
        case 2:
            this->sync = Sync::LinkedList;
            break;
        default:
            std::cout << "Unknown DMA sync mode "<< ((value >> 9) & 3) <<std::endl;
            exit(0);
        }
        this->chop_dma_sz = (u8)((value >> 16) & 7);
        this->chop_cpu_sz = (u8)((value >> 20) & 7);
        this->enable = (value >> 24) & 1 !=0;
        this->trigger = (value >> 28) & 1 !=0;
        this->dummy = (u8)((value >> 29) & 3);
    }
u32 Channel::get_base(){
        return this->base;
    }
void Channel::set_base(u32 value){
        this->base = value & 0xffffff;
    }
u32 Channel::get_block_control(){
        return (((u32)this->block_count << 16) | (u32)this->block_size);
    }
void Channel::set_block_control(u32 value){
        this->block_size = (u16)value;
        this->block_count = (u16)(value >> 16);
    }
bool Channel::active(){
    return this->enable && (this->sync == Sync::Manual ? this->trigger : true);
}
u32 Channel::transfer_size(){
    return this->sync == Sync::Manual ? ((u32)this->block_size) : ((u32)this->block_size * (u32)this->block_count);
}
void Channel::done(){
    this->enable = false;
    this->trigger = false;
}