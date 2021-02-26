#pragma once

#include "../global/types.h"

struct Video_Info {
    uint32 width;
    uint32 height;
    uint8 bpp;
    uint8* kenel_frameBuffer;
    uint8* frameBuffer;
} __attribute__ ((packed));

struct FrameBuffer
{
    uint8* frame;

    Vector2 size;
};


extern Video_Info info;
extern FrameBuffer double_buffer;

Video_Info* init_graphics();
void set_up_framebuffer();

void userPresentScreen();
void presentScreen();
void blitFrameToFrame(FrameBuffer src, FrameBuffer dest, uint32 x, uint32 y, uint32 width, uint32 height);
void biltBuffer();