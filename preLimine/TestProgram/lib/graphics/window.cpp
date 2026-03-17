#include "window.h"
#include "../syscall.h"
#include "../memory.h"

void intercept_callback(uint32 type, uint32 param1, uint32 param2);

void (*window_callback)(uint32, uint32, uint32);

void createWindow(void (*callback)(uint32, uint32, uint32), WindowInfo* desiredWindow)
{
    window_callback = callback;

    unsigned int* args = malloc_temp(20);
    //System Call ID
    args[0] = 0x10;
    args[1] = (uint32)desiredWindow->windowName; //Name
    args[2] = (uint32)desiredWindow->width; //Width
    args[3] = (uint32)desiredWindow->height; //Height
    args[4] = (uint32)intercept_callback; //Callback

    sys1(5, args);
}

void intercept_callback(uint32 type, uint32 param1, uint32 param2)
{
    window_callback(type, param1, param2);
}