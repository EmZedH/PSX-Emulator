#include "interconnect.h"   
Interconnect::Interconnect(BIOS bios)
    {
        this->bios = bios;
        this->dma = DMA();
        this->gpu = GPU();
    }
Port Interconnect::from_index_port(u32 index){
        switch (index)
        {
        case 0:
            return Port::MdecIn;
            break;
        case 1:
            return Port::MdecOut;
            break;
        case 2:
            return Port::Gpu;
            break;
        case 3:
            return Port::CdRom;
            break;
        case 4:
            return Port::Spu;
            break;
        case 5:
            return Port::Pio;
            break;
        case 6:
            return Port::Otc;
            break;
        default:
            std::cout<<"Invalid port "<<index<<std::endl;
            exit(0);
        }
    }
u8 Interconnect::load8(u32 address)
    {
        u32 addr = map.maskRegion(address);
        if ((map.contains(&BIOS_SIZE, addr)))
        {
            return this->bios.load8(addr - BIOS_SIZE.start);
        }
        else if ((map.contains(&EXPANSION_1, addr)))
        {
            return 0xff;
        }
        else if ((map.contains(&RAM_SIZE, addr)))
        {
            return this->ram.load8(addr - RAM_SIZE.start);
        }
        else
        {
            std::cout << "unhandled load8 from address "
                 << std::hex << addr
                 << "\n";
            exit(0);
        }
    }
u32 Interconnect::load16(u32 address){
        u32 addr = map.maskRegion(address);
        if(map.contains(&RAM_SIZE,addr)){
            return this->ram.load16(addr-RAM_SIZE.start);
        }
        else if(map.contains(&SPU,addr)){
            std::cout<<"Unhandled read from SPU register "<<addr<<std::endl;
            return 0;
        }
        else if(map.contains(&IRQ_CONTROL,addr)){
            std::cout<<"Unhandled read from IRQ_CONTROL register "<<addr<<std::endl;
            return 0;
        }
        else
        {
            std::cout << "unhandled load16 from address "
                 << std::hex << addr
                 << "\n";
            exit(0);
        }
    }
u32 Interconnect::load32(u32 address)
    {
        u32 addr = map.maskRegion(address);
        if ((map.contains(&BIOS_SIZE, addr)))
        {
            return this->bios.load32(addr - BIOS_SIZE.start);
        }
        else if ((map.contains(&RAM_SIZE, addr)))
        {
            return this->ram.load32(addr - RAM_SIZE.start);
        }
        else if ((map.contains(&EXPANSION_1, addr)))
        {
            return 0xff; // 0xff
        }
        else if ((map.contains(&IRQ_CONTROL, addr)))
        {
            std::cout << "Unhandled IRQ Control read " << addr - IRQ_CONTROL.start << std::endl;
            return 0;
        }
        else if ((map.contains(&DMA_SIZE, addr)))
        {
            return this->get_dma_reg(addr - DMA_SIZE.start);
        }
        else if ((map.contains(&GPU_SIZE, addr)))
        {
            std::cout << "GPU_SIZE read " << addr - GPU_SIZE.start << std::endl;
            return ((addr - GPU_SIZE.start) == 4 ? 0x1c000000 : 0);
        }
        //TODO
        else if((map.contains(&TIMERS,addr))){
            std::cout << "Unhandled TIMER read " << addr - TIMERS.start << std::endl;
            return 0;
        }
        //
        else
        {
            std::cout << "unhandled load32 from address "
                 << std::hex << addr
                 << "\n";
            exit(0);
        }
    }

void Interconnect::store8(u32 address, u8 value)
    {
        u32 addr = map.maskRegion(address);
        if ((map.contains(&EXPANSION_2, addr)))
        {
            std::cout << "Unhandled write to EXPANSION 2 register " << std::hex << addr - EXPANSION_2.start << std::endl;
            return;
        }
        else if ((map.contains(&RAM_SIZE, addr)))
        {
            this->ram.store8(addr - RAM_SIZE.start, value);
        }
        else
        {
            std::cout << "Unhandled store8 into address " << std::hex << addr << std::endl;
            exit(0);
        }
    }

void Interconnect::store16(u32 address, u16 value)
    {
        u32 addr = map.maskRegion(address);
        if ((map.contains(&SPU, addr)))
        {
            std::cout << "Unhandled write to SPU register " << std::hex << addr - SPU.start<< std::endl;
            return;
        }
        else if ((map.contains(&TIMERS, addr)))
        {
            std::cout << "Unhandled write to TIMER register " << addr - TIMERS.start << std::endl;
            return;
        }
        else if ((map.contains(&RAM_SIZE, addr)))
        {
            this->ram.store16(addr - RAM_SIZE.start,value);
        }
        else if ((map.contains(&IRQ_CONTROL, addr)))
        {
            std::cout << "Unhandled write to IRQ CONTROL register " << addr - IRQ_CONTROL.start << std::endl;
            return;
        }
        else
        {
            std::cout << "Unhandled store16 into address " << std::hex << addr << std::endl; //
            exit(0);
        }
    }

void Interconnect::store32(u32 address, u32 value)
    {
        u32 addr = map.maskRegion(address);
        if (map.contains(&SYS_CONTROL, addr))
        {
            switch (addr - SYS_CONTROL.start)
            {
            case 0:
                if (value != 0x1f000000)
                {
                    std::cout << "Bad expansion 1 base address for value " << value << std::endl;
                    exit(0);
                }
                break;
            case 4:
                if (value != 0x1f802000)
                {
                    std::cout << "Bad expansion 2 base address for value " << value << std::endl;
                    exit(0);
                }
                break;
            default:
                std::cout << "Unhandled write to MEMCONTROL register " << std::hex << addr << std::endl;
                // exit(0);
            }
        }
        else if (map.contains(&RAM_SIZE, addr))
        {
            this->ram.store32(addr - RAM_SIZE.start, value);
        }
        else if (map.contains(&CACHE_CONTROL, addr))
        {
            std::cout << "Unhandled write to CACHE CONTROL register " << std::hex << addr << std::endl;
            return;
        }
        else if (map.contains(&DMA_SIZE, addr))
        {
            this->set_dma_reg(addr - DMA_SIZE.start, value);
        }
        else if (map.contains(&TIMERS, addr))
        {
            std::cout << "Unhandled write to TIMER register " << std::hex << addr << std::endl;
            return;
        }
        else if (map.contains(&RAM_REG_SIZE, addr))
        {
            std::cout << "Unhandled write to RAM register " << addr << std::endl;
            return;
        }
        else if (map.contains(&GPU_SIZE, addr))
        {
            switch (addr - GPU_SIZE.start){
                case 0:
                this->gpu.gp0(value);
                break;
                case 4:
                this->gpu.gp1(value);
                break;
                default:
                std::cout << "Unhandled write to GPU_SIZE register " << addr << std::endl;
                exit(0);
            // return;
            }
        }
        else if ((map.contains(&IRQ_CONTROL, addr)))
        {
            std::cout << "IRQ control " << addr - IRQ_CONTROL.start << " " << value << std::endl;
            return;
        }
        else
        {
            std::cout << "Unhandled store32 into address " << std::hex << addr << std::endl;
            exit(0);
        }
    }

    //TODO
u32 Interconnect::get_dma_reg(u32 offset){
        u32 major = (offset & 0x70) >> 4;
        u32 minor = offset & 0xf;
        if(major >= 0 && major <= 6){
            Channel* channel = this->dma.channel(from_index_port(major));
            if(minor == 0){
                return channel->get_base();
            }
            else if(minor == 4){
                return channel->get_block_control();
            }
            else if(minor == 8){
                return channel->get_control();
            }
            else{
                std::cout<<"Unhandled DMA read at "<<offset <<" "<<major<<" "<<minor<<std::endl;
                exit(0);
            }
        }
        else if(major== 7){
            if(minor == 0){
                return this->dma.get_control();
            }
            else if(minor == 4){
                return this->dma.get_interrupt();
            }
            else{
                std::cout <<"Unhandled DMA read at "<<offset<<" "<<major<<" "<<minor<<std::endl;
                exit(0);
            }
        }
        else{
            std::cout <<"Unhandled DMA read at "<<offset<<std::endl;
            exit(0);
        }
    }
void Interconnect::set_dma_reg(u32 offset, u32 value){
        u32 major = (offset & 0x70) >> 4;
        u32 minor = offset & 0xf;
        if(major >= 0 && major <= 6){
            if(minor == 0){
                this->dma.channel(from_index_port(major))->set_base(value);
            }
            else if(minor == 4){
                this->dma.channel(from_index_port(major))->set_block_control(value);
            }
            else if(minor == 8){
                this->dma.channel(from_index_port(major))->set_control(value);
            }
            else{
                std::cout<<"Unhandled DMA write at "<<offset<<" "<<value<<" "<<major<<" "<<minor<<std::endl;
                exit(0);
            }
            if(this->dma.channel(from_index_port(major))->active()){
                this->do_dma(from_index_port(major));
                return;
            }
        }
        else if(major== 7){
            if(minor == 0){
                this->dma.set_control(value);
            }
            else if(minor == 4){
                this->dma.set_interrupt(value);
            }
            else{
                std::cout <<"Unhandled DMA write at "<<offset<<" "<<value<<" "<<major<<" "<<minor<<std::endl;
                exit(0);
            }
        }
        else{
            std::cout <<"Unhandled DMA write at "<<offset<<" "<<value<<std::endl;
            exit(0);
        }
    }
    void Interconnect::do_dma(Port port){
        if(this->dma.channel(port)->_sync() == Sync::LinkedList){
            this->do_dma_linked_list(port);
            return;
        }
        this->do_dma_block(port);
    }
    void Interconnect::do_dma_block(Port port){
        Channel* channel = this->dma.channel(port);
        u32 increment = channel->_step() == Step::Increment ? (u32)4 : (u32)(s32)(-4);
        u32 addr = channel->get_base();
        if(channel->_sync() == Sync::LinkedList){
            std::cout << "Couldn't figure out DMA block size"<<std::endl;
            exit(0);
        }
        u32 remsz = channel->transfer_size();
        while(remsz > 0){
            u32 cur_addr = addr & 0x1ffffc;
            if(channel->_direction() == Direction::FromRam){
                u32 src_word = this->ram.load32(cur_addr);
                if(port == Port::Gpu){
                    // std::cout<<"GPU read "<<src_word<<" at address: "<<cur_addr<<std::endl;
                    this->gpu.gp0(src_word);
                }
                else{
                    std::cout<<"Unhandled DMA destination port "<<(u8)port<<std::endl;
                    exit(0);
                }
            }
            else if(channel->_direction() == Direction::ToRam){
                if(port != Port::Otc){
                    std::cout<<"Unhandled DMA source port "<<(u8)port<<std::endl;
                    exit(0);
                }
                u32 src_word = (remsz == 1) ? 0xffffff : ((addr - 4) & 0x1fffff);
                // std::cout<<"do_dma_block RAM store at: "<<cur_addr<<" value: "<<src_word<<std::endl;
            this->ram.store32(cur_addr,src_word);
            }
            addr += increment;
            remsz--;
        }
        channel->done();
    }
    void Interconnect::do_dma_linked_list(Port port){
        Channel* channel = this->dma.channel(port);
        u32 addr = channel->get_base() & 0x1ffffc;
        if(channel->_direction() == Direction::ToRam){
            std::cout<<"Invalid DMA direction for linked list mode"<<std::endl;
            exit(0);
        }
        if(port != Port::Gpu){
            std::cout<<"Attempted Linked List DMA on port "<<(u8)port<<std::endl;
            exit(0);
        }
        while(true){
            u32 header = this->ram.load32(addr);
            u32 remsz = header >> 24;
            while(remsz > 0){
                addr = (addr + 4) & 0x1ffffc;
                u32 command = this->ram.load32(addr);
                this->gpu.gp0(command);
                std::cout<<"GPU command "<<command<<std::endl;
                remsz--;
            }
            if(header & 0x800000 != 0){
                break;
            }
            addr = header & 0x1ffffc;
        }
        channel->done();
    }
    //
