#include "windowManager.h"
#include "draw/shapes.h"
#include "../io/serial.h"

#define MENU_SPACE 50
#define TOP_MENU MENU_SPACE / 2

struct WindowInfo {
    uint16 width = 0;
    uint16 height = 0;
    uint8 bbp = 0;

    char windowName;

    void (*callback)(uint32, uint32, uint32);

    uint32 window_ID;
} __attribute__ ((packed));

void renderWindow (uint32 windowID);

WindowInfo windows[64]; //Max 64 windows for now, will change once dynamic allocation in added for small amounts
uint32 windowCounter = 0;

void createWindow(char name, uint32 width, uint32 height, void (*callback)(uint32, uint32, uint32))
{
    serial_write_debug("Window width: ", width);
    serial_write_debug("Window height: ", height);
    serial_write_debug("Window name: ", (uint32)name);

    //Create window
    WindowInfo* window = windows + windowCounter;
    windowCounter++;
    //Load window params
    window->width = width;
    window->height = height;
    window->windowName = name;
    window->callback = callback;
    window->window_ID = windowCounter - 1;

    //Render window
    renderWindow(window->window_ID);

    //Present the window
    userPresentScreen();
}

void renderWindow(uint32 windowID)
{
    draw_rect(0, TOP_MENU, info.width, info.height - MENU_SPACE, 0x000065);
}