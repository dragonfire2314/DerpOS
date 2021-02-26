#pragma once

#include "../graphics/draw/image.h"
#include "../global/types.h"
#include "../disk/ata.h"

struct Window 
{
    Vector2 location;
    Vector2 size;

    FrameBuffer frameBuffer;

    uint8* name;
};

void Init_Windows(Image* AsciiFontSheet);

Window window_createWindow();

void window_draw_character(Window* window, char c, Vector2 pointOnWindow);