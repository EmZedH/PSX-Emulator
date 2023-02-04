#include "gpu.h"
GPU::GPU(){
    this->page_base_x = 0;
    this->page_base_y = 0;
    this->semi_transparency = 0;
    this->texture_depth = TextureDepth::T4Bits;
    this->dithering = false;
    this->draw_to_display = false;
    this->force_set_mask_bit = false;
    this->preserve_masked_pixels = false;
    this->field = Field::Top;
    this->texture_disable = false;
    this->hres = HorizontalRes(0,0);
    this->vres = VerticalRes::Y240Lines;
    this->vmode = VMode::Ntsc;
    this->display_depth = DisplayDepth::D15Bits;
    this->interlaced = false;
    this->display_disabled = false;
    this->interrupt = false;
    this->dma_direction = DMADirection::Off;

    this->gp0_command = CommandBuffer();
    this->gp0_words_remaining = 0;

    this->gp0_mode = GP0Mode::Command;
}
u32 GPU::status(){
    u32 r = 0;
    r |= ((u32)this->page_base_x) << 0;
    r |= ((u32)this->page_base_y) << 4;
    r |= ((u32)this->semi_transparency) << 5;
    r |= ((u32)this->texture_depth) << 7;
    r |= ((u32)this->dithering) << 9;
    r |= ((u32)this->draw_to_display) << 10;
    r |= ((u32)this->force_set_mask_bit) << 11;
    r |= ((u32)this->preserve_masked_pixels) << 12;
    r |= ((u32)this->field) << 13;
    r |= ((u32)this->texture_depth) << 15;
    r |= ((u32)this->hres.into_status());
    r |= ((u32)this->vres) << 19;
    r |= ((u32)this->vmode) << 20;
    r |= ((u32)this->display_depth) << 21;
    r |= ((u32)this->interlaced) << 22;
    r |= ((u32)this->display_disabled) << 23;
    r |= ((u32)this->interrupt) << 24;
    r |= 1 << 26;
    r |= 1 << 27;
    r |= 1 << 28;
    r |= ((u32)this->dma_direction) << 29;
    r |= 0 << 31;
    u32 dma_request;
    switch (this->dma_direction){
        case DMADirection::Off:
        dma_request = 0;
        break;
        case DMADirection::Fifo:
        dma_request = 1;
        break;
        case DMADirection::CpuToGp0:
        dma_request = (r >> 28) & 1;
        break;
        case DMADirection::VRamToCpu:
        dma_request = (r >> 27) & 1;
        break;
    }
    r |= dma_request << 25;
    return r;
}
u32 GPU::read(){
    return 0;
}
void GPU::gp0_func(GP0Command command){
    switch(command){
        case GP0Command::GP0_nop:
        this->gp0_nop();
        break;
        case GP0Command::GP0_quad_mono_opaque:
        this->gp0_quad_mono_opaque();
        break;
        case GP0Command::GP0_image_load:
        this->gp0_image_load();
        break;
        case GP0Command::GP0_clear_cache:
        this->gp0_clear_cache();
        break;
        case GP0Command::GP0_draw_mode:
        this->gp0_draw_mode();
        break;
        case GP0Command::GP0_texture_window:
        this->gp0_texture_window();
        break;
        case GP0Command::GP0_drawing_area_top_left:
        this->gp0_drawing_area_top_left();
        break;
        case GP0Command::GP0_drawing_area_bottom_right:
        this->gp0_drawing_area_bottom_right();
        break;
        case GP0Command::GP0_drawing_offset:
        this->gp0_drawing_offset();
        break;
        case GP0Command::GP0_mask_bit_setting:
        this->gp0_mask_bit_setting();
        break;
    }
}
void GPU::gp0(u32 value){
    if(this->gp0_words_remaining==0){
        u32 opcode = (value >> 24) & 0xff;
        GP0Command command;
        u32 len;
        switch (opcode)
        {
            case 0:
            len = 1;
            command = GP0Command::GP0_nop;
            break;
            case 0x01:
            len = 1;
            command = GP0Command::GP0_clear_cache;
            break;
            case 0x28:
            len = 5;
            command = GP0Command::GP0_quad_mono_opaque;
            break;
            case 0xa0:
            len = 3;
            command = GP0Command::GP0_image_load;
            break;
            case 0xe1:
            len = 1;
            command = GP0Command::GP0_draw_mode;
            break;
            case 0xe2:
            len = 1;
            command = GP0Command::GP0_texture_window;
            break;
            case 0xe3:
            len = 1;
            command = GP0Command::GP0_drawing_area_top_left;
            break;
            case 0xe4:
            len = 1;
            command = GP0Command::GP0_drawing_area_bottom_right;
            break;
            case 0xe5:
            len = 1;
            command = GP0Command::GP0_drawing_offset;
            break;
            case 0xe6:
            len = 1;
            command = GP0Command::GP0_mask_bit_setting;
            break;
            default:
                std::cout<<"Unhandled GP0 command "<<value<<std::endl;
                exit(0);
    }
    this->gp0_words_remaining = len;
    this->gp0_command_method = command;
    this->gp0_command.clear();
    }
    // this->gp0_command.push_word(value);
    this->gp0_words_remaining--;
    // if(this->gp0_words_remaining == 0){
    //     this->gp0_func(this->gp0_command_method);
    // } 
    switch (this->gp0_mode)
    {
    case GP0Mode::Command:
    this->gp0_command.push_word(value);
    if(this->gp0_words_remaining == 0){
        this->gp0_func(this->gp0_command_method);
    } 
        break;
    
    case GP0Mode::ImageLoad:
    if(this->gp0_words_remaining == 0){
        this->gp0_mode = GP0Mode::Command;
    } 
        break;
    }
}
void GPU::gp0_draw_mode(){
    u32 value = this->gp0_command.index(0);
    this->page_base_x = (u8)(value & 0xf);
    this->page_base_y = (u8)((value >> 4) & 1);
    this->semi_transparency = (u8)((value >> 5) & 3);
    switch ((value >> 7) & 3)
    {
    case 0:
        this->texture_depth = TextureDepth::T4Bits;
        break;
    case 1:
        this->texture_depth = TextureDepth::T8Bits;
        break;
    case 2:
        this->texture_depth = TextureDepth::T15Bits;
        break;
    default:
        std::cout<<"Unhandled texture depth "<<((value >> 7) & 3)<<std::endl;
        break;
    }
    this->dithering = ((value >> 9) & 1) != 0;
    this->draw_to_display = ((value >> 10) & 1) != 0;
    this->texture_disable = ((value >> 11) & 1) != 0;
    this->rectangle_texture_x_flip = ((value >> 12) & 1) != 0; 
    this->rectangle_texture_y_flip = ((value >> 13) & 1) != 0; 
}

void GPU::gp0_drawing_area_top_left(){
    u32 value = this->gp0_command.index(0);
    this->drawing_area_top = (u16)((value >> 10) & 0x3ff);
    this->drawing_area_left = (u16)(value & 0x3ff);
}
void GPU::gp0_drawing_area_bottom_right(){
    u32 value = this->gp0_command.index(0);
    this->drawing_area_bottom = (u16)((value >> 10) & 0x3ff);
    this->drawing_area_right = (u16)(value & 0x3ff);
}
void GPU::gp0_drawing_offset(){
    u32 value = this->gp0_command.index(0);
    u16 x = (u16)(value & 0x7ff);
    u16 y = (u16)((value >> 11) & 0x7ff);
    this->drawing_x_offset = ((s16)(x << 5)) >> 5;
    this->drawing_y_offset = ((s16)(y << 5)) >> 5;
}
void GPU::gp0_texture_window(){
    u32 value = this->gp0_command.index(0);
    this->texture_window_x_mask = (u8)(value & 0x1f);
    this->texture_window_y_mask = (u8)((value >> 5) & 0x1f);
    this->texture_window_x_offset = (u8)((value >> 10) & 0x1f);
    this->texture_window_y_offset = (u8)((value >> 15) & 0x1f);
}
void GPU::gp0_mask_bit_setting(){
    u32 value = this->gp0_command.index(0);
    this->force_set_mask_bit = (value & 1) !=0;
    this->preserve_masked_pixels = (value & 2) !=0;
}
void GPU::gp0_quad_mono_opaque(){
    std::cout<<"Draw Quad "<<std::endl;
}
void GPU::gp0_clear_cache(){
    std::cout<<"Clear Cache "<<std::endl;
}
void GPU::gp0_image_load(){
    std::cout<<"Image Load "<<std::endl;
    u32 res = this->gp0_command.index(2);
    u32 width = res & 0xffff;
    u32 height = res >> 16;
    u32 imgsize = width * height;
    imgsize = (imgsize + 1) & !1;
    this->gp0_words_remaining = imgsize/2;
    this->gp0_mode = GP0Mode::ImageLoad;
}
void GPU::gp1(u32 value){
    switch((value >> 24) & 0xff){
        case 0x00:
        this->gp1_reset();
        break;
        case 0x03:
        this->gp1_display_enable(value);
        break;
        case 0x04:
        this->gp1_dma_direction(value);
        break;
        case 0x05:
        this->gp1_display_vram_start(value);
        break;
        case 0x06:
        this->gp1_display_horizontal_range(value);
        break;
        case 0x07:
        this->gp1_display_vertical_range(value);
        break;
        case 0x08:
        this->gp1_display_mode(value);
        break;
        default:
        std::cout<<"Unhandled GP1 command "<<value<<std::endl;
        exit(0);
    }
}
void GPU::gp1_reset(){
    this->interrupt = false;
    this->page_base_x = 0;
    this->page_base_y = 0;
    this->semi_transparency = 0;
    this->texture_depth = TextureDepth::T4Bits;
    this->texture_window_x_mask = 0;
    this->texture_window_y_mask = 0;
    this->texture_window_x_offset = 0;
    this->texture_window_y_offset = 0;
    this->dithering = false;
    this->draw_to_display = false;
    this->texture_disable = false;
    this->rectangle_texture_x_flip = false;
    this->rectangle_texture_y_flip = false;
    this->drawing_area_left = 0;
    this->drawing_area_top = 0;
    this->drawing_area_right = 0;
    this->drawing_area_bottom = 0;
    this->drawing_x_offset = 0;
    this->drawing_y_offset = 0;
    this->force_set_mask_bit = false;
    this->preserve_masked_pixels = false;

    this->dma_direction = DMADirection::Off;

    this->display_disabled = true;
    this->display_vram_x_start = 0;
    this->display_vram_y_start = 0;
    this->hres = HorizontalRes(0,0);
    this->vres = VerticalRes::Y240Lines;

    this->vmode = VMode::Ntsc;
    this->interlaced = true;
    this->display_horiz_start = 0x200;
    this->display_horiz_end = 0xc00;
    this->display_line_start = 0x10;
    this->display_line_end = 0x100;
    this->display_depth = DisplayDepth::D15Bits;
}
void GPU::gp1_display_mode(u32 value){
    u8 hr1 = (u8)(value & 3);
    u8 hr2 = (u8)((value >> 6) & 1);
    this->hres = HorizontalRes(hr1,hr2);
    this->vres = ((value & 0x4) !=0) ? VerticalRes::Y480Lines : VerticalRes::Y240Lines;
    this->vmode = ((value & 0x8) !=0) ? VMode::Pal : VMode::Ntsc;
    this->display_depth = ((value & 0x10) !=0) ? DisplayDepth::D15Bits : DisplayDepth::D24Bits;
    this->interlaced = (value & 0x20) !=0;
    if((value & 0x80) !=0){
        std::cout<<"Unsupported display mode "<<value<<std::endl;
        exit(0);
    }
}

void GPU::gp1_dma_direction(u32 value){
    switch(value & 3){
        case 0:
        this->dma_direction = DMADirection::Off;
        break;
        case 1:
        this->dma_direction = DMADirection::Fifo;
        break;
        case 2:
        this->dma_direction = DMADirection::CpuToGp0;
        break;
        case 3:
        this->dma_direction = DMADirection::VRamToCpu;
        break;
        default:
        std::cout<<"Unreachable command "<<value<<std::endl;
        exit(0);
    }
}
void GPU::gp1_display_vram_start(u32 value){
    this->display_vram_x_start = (u16)(value & 0x3fe);
    this->display_vram_y_start = (u16)((value >> 10) & 0x1ff);
}
void GPU::gp1_display_horizontal_range(u32 value){
    this->display_horiz_start = (u16)(value & 0xfff);
    this->display_horiz_end = (u16)((value >> 12) & 0xfff);
}
void GPU::gp1_display_vertical_range(u32 value){
    this->display_line_start = (u16)(value & 0x3ff);
    this->display_line_end = (u16)((value >> 10) & 0x3ff);
}
void GPU::gp1_display_enable(u32 value){
    this->display_disabled = value & 1 !=0;
}