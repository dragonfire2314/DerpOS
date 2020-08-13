#pragma once

#include "../global/types.h"
#include "screen.h"

Video_Info vbe_init();
extern unsigned int* frameBuffer;
extern unsigned char* kenel_frameBuffer;

extern int vbe_width;
extern int vbe_height;