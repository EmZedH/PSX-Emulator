#pragma once
#include "common.h"
#include "definedclasses.h"
class Channel{
    bool enable;
    Direction direction;
    Step step;
    Sync sync;
    bool trigger;
    bool chop;
    u8 chop_dma_sz;
    u8 chop_cpu_sz;
    u8 dummy;
    u32 base;
    u16 block_size;
    u16 block_count;
    public:
    Channel();
    u32 get_control();
    void set_control(u32 value);
    u32 get_base();
    void set_base(u32 value);
    u32 get_block_control();
    void set_block_control(u32 value);
    bool active();
    Direction _direction() const {return this->direction;}
    Step _step() const {return this->step;}
    Sync _sync() const {return this->sync;}
    u32 transfer_size();
    void done();
};