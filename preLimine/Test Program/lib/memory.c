/*
*   File name: memory.c
*   Usage: Memory management for derpOS userspace applications
*   Written By: Tanner Kern
*/

//Memory manager starts at mem location 0x1000

unsigned int* mem = (unsigned int*)0xF00;

//Allocates memory and returns an adress to the memory
unsigned int* malloc(unsigned int bytes)
{
    unsigned int* temp = mem;
    mem += bytes;
    return temp; 
}

unsigned int* malloc_temp(unsigned int bytes)
{
    return (unsigned int*)0x1000; 
}