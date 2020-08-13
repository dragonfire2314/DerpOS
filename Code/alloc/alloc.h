/*
	File: alloc.h
	Purpose: A simple and small malloc inplimentaion for derp OS
	Author: Tanner Kern
*/

#pragma once

#include "../global/types.h"

/*
Usage
    -Create class
    -Call malloc to create memory and free to free memory
    -if malloc returns NULL
        -Call getError to check the error type
*/

struct Malloc_Memory
{
    uint32 size;
    unsigned char free; //1 - free, 0 - used
    Malloc_Memory* next;
}__attribute__ ((packed));

enum Alloc_Error_Type {
    NONE,
    OUT_OF_MEMORY
};

class Alloc
{
private:
    //Start node
    Malloc_Memory* head_node = 0;
    Malloc_Memory* current_node;
    //Size
    uint8* heapPointer = 0x0;
    uint8* heapBase = 0x0;
    uint32 maxHeapSize = 0;
    uint32 minMemSize = 16;

    uint8* location;

    Alloc_Error_Type error = NONE;

    int init_malloc(uint8* heapStart, uint32 heapSize, uint32 minAllocSize);
    Malloc_Memory* addNode(uint32 size);
    void removeNode(uint32* address);
    Malloc_Memory* malloc_node();
    uint8* alloc_heap(uint32 size);
    void cleanMemoryList();
    void expandMemory(uint32 amount);
public:
    Alloc();
    ~Alloc();

    void init(uint32 heapSize, uint32 minAllocSize);
    
    void* malloc(uint32 size);
    void free(uint8* loc);

    Alloc_Error_Type getError();
};