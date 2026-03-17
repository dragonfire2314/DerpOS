/*
*   File Name:graphics_driver.c
*   Written By: Tanner Kern
*   Purpose: Simple Graphics driver for derpOS, supports full screen pixel mode and emulated text mode
*/

#include "../core/mmu.h"

struct Screen
{
    unsigned char* mem;
    int width;
    int height;
    int process_id;
};

struct Screen* screens;
unsigned int screenCount = 0;

void graphic_driver_init()
{
    //Set Up VBE
    //Create mem_bank for screen mem
    //Set up screens mem
    screens = k_create_4k_block();
}

void graphics_add_screen(unsigned int* mem, int width, int height, int process_id)
{
    screenCount++;
    screens[screenCount].mem = (unsigned char*)mem;
    screens[screenCount].width = width;
    screens[screenCount].height = height;
    screens[screenCount].process_id = process_id;
}

