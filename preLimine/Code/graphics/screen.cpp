#include "screen.h"
#include "vbe.h"
#include "../core/mmu.h"
#include "../io/serial.h"

Video_Info info;
FrameBuffer double_buffer;

bool b_shouldPresent = false;

Video_Info* init_graphics()
{
    //Choose Sceern render method, for now its only vbe
    info = vbe_init();

    return &info;
}

void set_up_framebuffer()
{
    //Generate a screen buffer
    double_buffer.frame = (uint8*)k_malloc_large((info.width * info.height *3));
    double_buffer.size.x = info.width;    
    double_buffer.size.y = info.height;    
    //Clear said buffer
    for(int i = 0; i < (info.width * info.height * 3); i++)
    {
        double_buffer.frame[i] = 0x50;
    }
}

void userPresentScreen()
{
    b_shouldPresent = true;
}

void presentScreen()
{
    if (b_shouldPresent) {
        b_shouldPresent = false;
        biltBuffer();
    }
}

void blitFrameToFrame(FrameBuffer src, FrameBuffer dest, uint32 x, uint32 y, uint32 width, uint32 height)
{
    uint32 screen_width = dest.size.x;

    for(int p = 0; p < height; p++){

        uint32 destRow = (screen_width * 3 * (p + y)) + x * 3;
        uint32 srcRow    = (src.size.x * 3 * (p)); 

        for(int i = 0; i < width * 3; i += 3)
        {
            dest.frame[destRow + i]   =   src.frame[srcRow + i    ];
            dest.frame[destRow + i + 1] = src.frame[srcRow + i + 1];
            dest.frame[destRow + i + 2] = src.frame[srcRow + i + 2];
        }
    }
}

void biltBuffer()
{
    uint8* frame = (uint8*)info.frameBuffer - 0xC0000000;

    for(int i = 0; i < (info.width * info.height * 3); i++)
    {
        frame[i] = double_buffer.frame[i];
    }
}