#include "window.h"
#include "../graphics/screen.h"
#include "../graphics/draw/image.h"
#include "../io/serial.h"
#include "../core/mmu.h"
#include "../global/constants.h"

Image* w_fontSheet;

void Init_Windows(Image* AsciiFontSheet) 
{
    w_fontSheet = AsciiFontSheet;
}

Window window_createWindow() 
{
    Window w;
    w.location.x = 0; 
    w.location.y = 0;

    w.size.x = info.width / 2;
    w.size.y = info.height / 2;

    w.frameBuffer.frame = (uint8*)malloc_page_aligned(w.size.x * w.size.y * (info.bpp / 8));
    w.frameBuffer.size.x = w.size.x;
    w.frameBuffer.size.y = w.size.y;

    return w;
}

void window_draw_character(Window* window, char c, Vector2 pointOnWindow) 
{
    View_rect rec;
    rec.height = CHAR_PIXEL_DIM; //Size af characters
    rec.width = CHAR_PIXEL_DIM; //Size af characters
    rec.x = (c % 16) * CHAR_PIXEL_DIM;
    rec.y = (c / 16) * CHAR_PIXEL_DIM;

    serial_write_string("X AND Y FOR DRAW CHAR: \r\n");
    serial_write_int(rec.x);
    serial_write_string("\r\n");
    serial_write_int(rec.y);
    serial_write_string("\r\n");

    //display a letter
    image_view_blit(w_fontSheet, rec, window->frameBuffer, 0, 0, CHAR_PIXEL_DIM, CHAR_PIXEL_DIM);
}