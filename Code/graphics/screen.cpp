#include "screen.h"
#include "vbe.h"
#include "../core/mmu.h"
#include "../io/serial.h"

Video_Info info;
uint8* frame_double_buffer;

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
    frame_double_buffer = (uint8*)k_malloc_large((info.width * info.height *3));
    //Clear said buffer
    for(int i = 0; i < (info.width * info.height * 3); i++)
    {
        frame_double_buffer[i] = 0x50;
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

void biltBuffer()
{
    uint8* frame = (uint8*)info.frameBuffer - 0xC0000000;

    for(int i = 0; i < (info.width * info.height * 3); i++)
    {
        frame[i] = frame_double_buffer[i];
    }
}