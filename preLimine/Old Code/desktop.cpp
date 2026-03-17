#include "desktopData.h"














/*
#define MAX_WINDOW_COUNT 32

extern "C" void desktop_init();
extern "C" void desktop_DrawScreen();
extern "C" void desktop_Update();
extern "C" int desktop_CreateWindow(int x, int y, int height, int width, unsigned int* windowMem, 
    char scale, char render, char stretch);
extern "C" void desktop_SetWindowUpdate(int windowID);

//Functions to call from this class
extern "C" void graphics_rect(int x, int y, int width, int height, char r, char g, char b);
extern "C" void graphics_cls(char r, char g, char b);
extern "C" void graphics_blit_rect(int x, int y, int width, int height, unsigned int* mem);

Desktop kernel_desktop;
Window window[MAX_WINDOW_COUNT];
int windowCount = -1;

void desktop_init()
{
    desktop_DrawScreen();
}

void desktop_DrawScreen()
{
    //Draw background
    graphics_cls(100, 30, 150);
    //Draw ToolBar
    graphics_rect(0, 728, 1024, 40, 0, 0, 0);
    //Draw Icons

    //Draw Windows
    for(int i = 0; i < windowCount; i++)
    {
        //Copy windows screen mem to the window using its settings
    }
}

void desktop_Update()
{
    //Check all windows for update flag
    for(int i = 0; i < windowCount+1; i++)
    {
        if(window[i].isUpdate)
        {
            //Copy windows screen mem to the window using its settings
            graphics_blit_rect(window[i].posX, window[i].posY, window[i].width, window[i].height, window[i].windowMemLoc);
            window[i].isUpdate = false;
        }
    }
    //Check to see if the desktop itself must be updated
    if(kernel_desktop.Update)
    {
        //Redraw the entire screen
        desktop_DrawScreen();
    }
}

int desktop_CreateWindow(int x, int y, int height, int width, unsigned int* windowMem, 
    char scale, char render, char stretch)
{
    windowCount++;
    if(windowCount < 32){
        window[windowCount].posX = x;
        window[windowCount].posY = y;
        window[windowCount].width = width;
        window[windowCount].height = height;
        
        window[windowCount].windowMemLoc = windowMem;

        window[windowCount].ScalingTpye = scale;
        window[windowCount].StretchType = stretch;
        window[windowCount].RenderType = render;

        //graphics_blit_rect(window[windowCount].posX, window[windowCount].posY, window[windowCount].width, window[windowCount].height, window[windowCount].windowMemLoc);

        return windowCount;
    }
    return -1;
}

void desktop_SetWindowUpdate(int windowID)
{
    window[windowID].isUpdate = true;
}*/