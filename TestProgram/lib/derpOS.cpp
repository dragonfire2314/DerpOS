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

void puts(char* str, unsigned char xPos, unsigned char yPos, unsigned char color)
{
    int i = 0;
    while (str[i] != '\0') {
        putc(str[i], xPos + i, yPos, color);
        i++;
    }
}