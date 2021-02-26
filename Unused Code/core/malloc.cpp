#include "malloc.h"

#define CHECK_NULL(x) if(x == NULL) { return NULL; }

blockMalloc::blockMalloc(unsigned int memStart, unsigned int limit)
{
    heapPos = (unsigned char*)memStart;
    stackPos = (unsigned char*)limit;

    mem_start = (unsigned char*)memStart;
    mem_limit = (unsigned char*)limit;
}

void* blockMalloc::malloc(unsigned int size)
{
    //Checks for free mem
    struct block* temp = look_for_free_block(size);
    if(temp == NULL) //Is mem found?
    {
        //No free mem
        //Create block
        temp = malloc_block();
        CHECK_NULL(temp);
        //Get some mem
        temp->mem = (unsigned char*)malloc_mem(size);
        CHECK_NULL(temp->mem);
        //All block to end of list
        end->next = temp;
        end = temp;

        return temp->mem;
    }
    else
    {
        //Free mem found
        return temp->mem;
    }
}

block* blockMalloc::look_for_free_block(unsigned int size)
{
    block* transverse = start;
    //Look though blocks
    while(transverse->next != NULL)
    {
        if(transverse->isFree && size <= transverse->size)
        {
            //Shrink current block if it is 32bytes bigger than desired
            if((transverse->size - size) > 32)
            {
                //Shrink
                block* b = malloc_block();
                b->mem = transverse->mem + size;
                block* temp = transverse->next;
                b->next = temp;
                transverse->next = b;

                transverse->isFree = 0;
                return transverse;
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

block* blockMalloc::malloc_block()
{
    block* temp = (block*)heapPos;
    lbrk(sizeof(block));
    return temp;
}

void* blockMalloc::malloc_mem(unsigned int size)
{
    unsigned char* temp = heapPos;
    sbrk(size);
    return (void*)temp;
}

int blockMalloc::sbrk(unsigned int size)
{
    if(heapPos + size > stackPos)
    {
        return NULL;
    }

    heapPos += size;
}

int blockMalloc::lbrk(unsigned int size)
{
    if(stackPos - size < heapPos)
    {
        return NULL;
    }

    stackPos -= size;
}