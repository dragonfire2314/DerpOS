#pragma once

#include "../../global/types.h"
#include "../vbe.h"

struct Image {
    const uint8* data;
    uint32 width;
    uint32 height;
    uint8 bpp;
    uint8 bitDepth;
};

struct View_rect {
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
};

void image_blit(const Image* img, int x, int y, int width, int height);
void image_view_blit(const Image* img, View_rect rect, FrameBuffer targetBuffer, int x, int y, int width, int height);