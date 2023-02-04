#include "dma.h"
DMA::DMA(){
        this->control = 0x07654321;
        //everything from here new
        this->irq_en = false;
        this->channel_irq_en = 0;
        this->channel_irq_flags = 0;
        this->force_irq = false;
        this->irq_dummy = 0;
        this->channels[0] = Channel();
        this->channels[1] = Channel();
        this->channels[2] = Channel();
        this->channels[3] = Channel();
        this->channels[4] = Channel();
        this->channels[5] = Channel();
        this->channels[6] = Channel();
    }
u32 DMA::get_control(){
        return this->control;
    }
void DMA::set_control(u32 value){
        this->control = value;
    }
bool DMA::irq(){
    u32 channel_irq = this->channel_irq_flags & this->channel_irq_en;
    return this->force_irq || ((this->irq_en && channel_irq) !=0);
    }
u32 DMA::get_interrupt(){
        return (((u32)this->irq_dummy) | ((u32)this->force_irq << 15) | ((u32)this->channel_irq_en << 16) | ((u32)this->irq_en << 23) | ((u32)this->channel_irq_flags << 24) | ((u32)this->irq() << 31));
    }
void DMA::set_interrupt(u32 value){
        this->irq_dummy = (u8)(value & 0x3f);
        this->force_irq = ((value >> 15) & 1) !=0;
        this->channel_irq_en = (u8)((value >> 16) & 0x7f);
        this->irq_en = ((value >> 23) & 0x3f) !=0;
        this->channel_irq_flags &= !((u8)((value >> 24) & 0x3f));
    }
Channel* DMA::channel(Port port){
        return &this->channels[(u32)port];
    }