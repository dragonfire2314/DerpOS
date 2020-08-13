#pragma once

#include "../global/types.h"

struct Video_Info {
    uint32 width;
    uint32 height;
    uint8* kenel_frameBuffer;
    uint8* frameBuffer;
} __attribute__ ((packed));

extern Video_Info info;
extern uint8* frame_double_buffer;

Video_Info* init_graphics();
void set_up_framebuffer();

void userPresentScreen();
void presentScreen();
void biltBuffer();