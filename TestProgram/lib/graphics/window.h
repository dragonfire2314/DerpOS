#pragma once

#include "../globals/types.h"

/*

Create a window
    -

*/

struct WindowInfo {
    uint16 width = 0;
    uint16 height = 0;
    uint8 bbp = 0;

    char windowName;
} __attribute__ ((packed));

/*
callback - A function that is called upon window updates
desiredWindow - A list of the desired requirements for the window
return - The actual window created by the kernel
*/
void createWindow(void (*callback)(uint32, uint32, uint32), WindowInfo* desiredWindow);