#pragma once
#include "common.h"
#include "channel.h"
class DMA{
    u32 control;
    bool irq_en;
    u8 channel_irq_en;
    u8 channel_irq_flags;
    bool force_irq;
    u8 irq_dummy;
    Channel channels[7];
    public:
        DMA();
        u32 get_control();
        void set_control(u32 value);
        bool irq();
        u32 get_interrupt();
        void set_interrupt(u32 value);
        Channel* channel(Port port);

};