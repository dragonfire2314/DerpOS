/*
*   File name: memory.h
*   Usage: Memory management for derpOS userspace applications
*   Written By: Tanner Kern
*/

//Allocates memory and returns an adress to the memory
unsigned int* malloc(unsigned int bytes);
unsigned int* malloc_temp(unsigned int bytes);