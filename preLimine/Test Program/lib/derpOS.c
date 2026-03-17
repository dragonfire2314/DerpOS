/*
*   File name: derpOS.c
*   Usage: syscall implementation for usermode processes
*   Written By: Tanner Kern
*/

#include "derpOS.h"
#include "syscall.h"
#include "memory.h"

void putc(unsigned char character, unsigned char xPos, unsigned char yPos, unsigned char color)
{
    unsigned int* args = malloc_temp(20);
    //System Call ID
    args[0] = 1;
    args[1] = (unsigned int)character;
    args[2] = (unsigned int)xPos;
    args[3] = (unsigned int)yPos;
    args[4] = (unsigned int)color; 

    sys1(5, args);
}

void graphics_init(unsigned int mem, int width, int hieght, int process_id)
{
    unsigned int* args = malloc_temp(20);

    args[0] = 0x10;
    args[1] = (unsigned int)mem;
    args[2] = (unsigned int)width;
    args[3] = (unsigned int)hieght;
    args[4] = (unsigned int)process_id;

    sys1(5, args);
}

void draw_rect(int x, int y, int width, int height, unsigned int* mem)
{
    for(int i = 0; i < height; i++)
    {
        //Get offset for row
        int offset = (640*(y+i)*3)+(x*3);
        //Draw row
        for(int t = 0; t < width*3; t+=3)
        {
            mem[offset+t] = 0x00;
            mem[offset+t+1] = 0x00;
            mem[offset+t+2] = 0xFF;
        }
    }
}