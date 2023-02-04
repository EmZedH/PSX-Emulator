#pragma once
#include "common.h"
#include "definedclasses.h"
class GPU{
    u8 page_base_x;
    u8 page_base_y;
    u8 semi_transparency;
    TextureDepth texture_depth;
    bool dithering;
    bool draw_to_display;
    bool force_set_mask_bit;
    bool preserve_masked_pixels;
    Field field;
    bool texture_disable;
    HorizontalRes hres;
    VerticalRes vres;
    VMode vmode;
    DisplayDepth display_depth;
    bool interlaced;
    bool display_disabled;
    bool interrupt;
    DMADirection dma_direction;
    bool rectangle_texture_x_flip;
    bool rectangle_texture_y_flip;

    u8 texture_window_x_mask;
    u8 texture_window_y_mask;
    u8 texture_window_x_offset;
    u8 texture_window_y_offset;
    u16 drawing_area_left;
    u16 drawing_area_top;
    u16 drawing_area_right;
    u16 drawing_area_bottom;
    s16 drawing_x_offset;
    s16 drawing_y_offset;
    u16 display_vram_x_start;
    u16 display_vram_y_start;
    u16 display_horiz_start;
    u16 display_horiz_end;
    u16 display_line_start;
    u16 display_line_end;

    CommandBuffer gp0_command;
    u32 gp0_words_remaining;
    GP0Command gp0_command_method;

    GP0Mode gp0_mode;
    
    public:
    GPU();
    u32 status();
    u32 read();

    void gp0_func(GP0Command command);

    void gp0(u32 value);
    void gp0_nop(){};
    void gp0_draw_mode();
    void gp0_drawing_area_top_left();
    void gp0_drawing_area_bottom_right();
    void gp0_drawing_offset();
    void gp0_texture_window();
    void gp0_mask_bit_setting();
    void gp0_quad_mono_opaque();
    void gp0_clear_cache();
    void gp0_image_load();

    void gp1(u32 value);
    void gp1_reset();
    void gp1_display_mode(u32 value);
    void gp1_dma_direction(u32 value);
    void gp1_display_vram_start(u32 value);
    void gp1_display_horizontal_range(u32 value);
    void gp1_display_vertical_range(u32 value);
    void gp1_display_enable(u32 value);
};