#ifndef MMU_H
#define MMU_H

#include "../global/types.h"

#define PAGE_SIZE 0x1000
#define PAGE_TO_BYTE(x) (x * 0x1000) - 1

//New Memory manager
void memory_init();
void* malloc_pages(uint32 pageCount);
void* malloc_page_aligned(uint32 size);
void free_page_aligned(void* mem);

void MEMORY_ALLOC_TEST_SUITE () ;

unsigned int* kmalloc(int size);
unsigned long* k_create_4k_block();
void memset(void* loc, char val, unsigned int size);
void memcpy(void* dest, const void* src, uint32 size);
unsigned int* k_malloc_large(unsigned int sizeInBytes);
unsigned int* k_create_512_block();
unsigned int* k_create_2k_block();

void* alloc_large_memory(uint32 size);
void free_large(void* memory);

#endif