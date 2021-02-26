#include "../graphics_temp.h"
#include "malloc.h"
#include "mmu.h"

#define NULL 0

extern "C" void* alloc(unsigned int size);

blockMalloc m(0x1000000, 0x1FFFFFF);

void alloc_init(unsigned int base, unsigned int limit)
{
    
}

void* alloc(unsigned int size)
{

}





























































/*
struct header
{
    struct block* next;
};

struct block //13 bytes
{
    unsigned int* mem;

    struct block* next;

    unsigned int size;
    char is_free; //Free = 0, used > 0
};

#define RAM_16_MB 0x1000000
#define RAM_32_MB 0x2000000

unsigned char* heapPos = (unsigned char*)RAM_16_MB;
unsigned char* stackPos = (unsigned char*)RAM_32_MB;

struct header alloc_start;
struct block* list_end;

struct block firstElement;

struct block* alloc_look_for_free_block(unsigned int size);

void alloc_init(unsigned int memStart, unsigned int limit)
{
    heapPos = (unsigned int*)memStart;
    stackPos = (unsigned int*)limit;

    //Alloc some mem to start to list
    //Make it 4 bytes
    firstElement.mem = (unsigned int*)heapPos;
    sbrk(4);

    alloc_start.next = &firstElement;
    list_end = alloc_start.next;
}

void* malloc(unsigned int bytes)
{
    //Checks for free mem
    struct block* temp = alloc_look_for_free_block(bytes);
    if(temp == NULL) //Is mem found?
    {
        //No free mem
        //Create block
        temp = alloc_malloc_block();
        //Get some mem
        temp->mem = alloc_get_mem(bytes);
        //All block to end of list
        list_end->next = temp;
        list_end = temp;
    }
    else
    {
        //Free mem found
        return temp->mem;
    }
}

struct block* alloc_malloc_block()
{
    unsigned char* temp = heapPos;
    sbrk(sizeof(struct block));
    ((struct block*)temp)->next = NULL;
    return (struct block*)temp;
}

void* alloc_get_mem(unsigned int size)
{
    unsigned char* temp = heapPos;
    sbrk(size);
    return (void*)temp;
}

void sbrk(unsigned int size)
{
    heapPos += size;
}

struct block* alloc_look_for_free_block(unsigned int size)
{
    struct block* transverse = alloc_start.next;
    //Look though blocks
    while(transverse->next != NULL)
    {
        if(transverse->is_free && size <= transverse->size)
        {
            //Shrink current block if it is 32bytes bigger than desired
            if((transverse->size - size) > 32)
            {
                //Shrink

                //Add new block to define extra
            }
            else
            {
                //Return mem adress of block
                return transverse;
            }
        }
        transverse = transverse->next;
    }
    //No blocks were found
    return NULL;
}
*/