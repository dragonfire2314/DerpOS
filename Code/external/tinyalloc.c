#include "tinyalloc.h"
//#include <stdint.h>

//typedef unsigned int unsigned int;

#ifdef TA_DEBUG
extern void print_s(char *);
extern void print_i(unsigned int);
#else
#define print_s(X)
#define print_i(X)
#endif

typedef struct Block Block;

struct Block {
    void *addr;
    Block *next;
    unsigned int size;
};

typedef struct {
    Block *free;   // first free block
    Block *used;   // first used block
    Block *fresh;  // first available blank block
    unsigned int top;    // top free addr
    Block *blocks;
} Heap;

static Heap *heap = NULL;
static void *heap_limit = NULL;
static unsigned int heap_split_thresh;
static unsigned int heap_alignment;
static unsigned int heap_max_blocks;

/**
 * If compaction is enabled, inserts block
 * into free list, sorted by addr.
 * If disabled, add block has new head of
 * the free list.
 */
static void insert_block(Block *block) {
#ifndef TA_DISABLE_COMPACT
    Block *ptr  = heap->free;
    Block *prev = NULL;
    while (ptr != NULL) {
        if ((unsigned int)block->addr <= (unsigned int)ptr->addr) {
            print_s("insert");
            print_i((unsigned int)ptr);
            break;
        }
        prev = ptr;
        ptr  = ptr->next;
    }
    if (prev != NULL) {
        if (ptr == NULL) {
            print_s("new tail");
        }
        prev->next = block;
    } else {
        print_s("new head");
        heap->free = block;
    }
    block->next = ptr;
#else
    block->next = heap->free;
    heap->free  = block;
#endif
}

#ifndef TA_DISABLE_COMPACT
static void release_blocks(Block *scan, Block *to) {
    Block *scan_next;
    while (scan != to) {
        print_s("release");
        print_i((unsigned int)scan);
        scan_next   = scan->next;
        scan->next  = heap->fresh;
        heap->fresh = scan;
        scan->addr  = 0;
        scan->size  = 0;
        scan        = scan_next;
    }
}

static void compact() {
    Block *ptr = heap->free;
    Block *prev;
    Block *scan;
    while (ptr != NULL) {
        prev = ptr;
        scan = ptr->next;
        while (scan != NULL &&
               (unsigned int)prev->addr + prev->size == (unsigned int)scan->addr) {
            print_s("merge");
            print_i((unsigned int)scan);
            prev = scan;
            scan = scan->next;
        }
        if (prev != ptr) {
            unsigned int new_size =
                (unsigned int)prev->addr - (unsigned int)ptr->addr + prev->size;
            print_s("new size");
            print_i(new_size);
            ptr->size   = new_size;
            Block *next = prev->next;
            // make merged blocks available
            release_blocks(ptr->next, prev->next);
            // relink
            ptr->next = next;
        }
        ptr = ptr->next;
    }
}
#endif

bool ta_init(void *base, void *limit, const unsigned int heap_blocks, const unsigned int split_thresh, const unsigned int alignment) {
    heap = (Heap *)base;
    heap_limit = limit;
    heap_split_thresh = split_thresh;
    heap_alignment = alignment;
    heap_max_blocks = heap_blocks;

    heap->free   = NULL;
    heap->used   = NULL;
    heap->fresh  = heap->blocks;
    heap->top    = (unsigned int)base + sizeof(Heap) + heap_blocks * sizeof(Block);
    heap->blocks = base + sizeof(Heap);

    Block *block = heap->blocks;
    unsigned int i     = heap_max_blocks - 1;
    while (i--) {
        block->next = block + 1;
        block++;
    }
    block->next = NULL;
    return true;
}

bool ta_free(void *free) {
    Block *block = heap->used;
    Block *prev  = NULL;
    while (block != NULL) {
        if (free == block->addr) {
            if (prev) {
                prev->next = block->next;
            } else {
                heap->used = block->next;
            }
            insert_block(block);
#ifndef TA_DISABLE_COMPACT
            compact();
#endif
            return true;
        }
        prev  = block;
        block = block->next;
    }
    return false;
}

static Block *alloc_block(unsigned int num) {
    Block *ptr  = heap->free;
    Block *prev = NULL;
    unsigned int top  = heap->top;
    num         = (num + heap_alignment - 1) & -heap_alignment;
    while (ptr != NULL) {
        const int is_top = ((unsigned int)ptr->addr + ptr->size >= top) && ((unsigned int)ptr->addr + num <= (unsigned int)heap_limit);
        if (is_top || ptr->size >= num) {
            if (prev != NULL) {
                prev->next = ptr->next;
            } else {
                heap->free = ptr->next;
            }
            ptr->next  = heap->used;
            heap->used = ptr;
            if (is_top) {
                print_s("resize top block");
                ptr->size = num;
                heap->top = (unsigned int)ptr->addr + num;
#ifndef TA_DISABLE_SPLIT
            } else if (heap->fresh != NULL) {
                unsigned int excess = ptr->size - num;
                if (excess >= heap_split_thresh) {
                    ptr->size    = num;
                    Block *split = heap->fresh;
                    heap->fresh  = split->next;
                    split->addr  = (void *)((unsigned int)ptr->addr + num);
                    print_s("split");
                    print_i((unsigned int)split->addr);
                    split->size = excess;
                    insert_block(split);
#ifndef TA_DISABLE_COMPACT
                    compact();
#endif
                }
#endif
            }
            return ptr;
        }
        prev = ptr;
        ptr  = ptr->next;
    }
    // no matching free blocks
    // see if any other blocks available
    unsigned int new_top = top + num;
    if (heap->fresh != NULL && new_top <= (unsigned int)heap_limit) {
        ptr         = heap->fresh;
        heap->fresh = ptr->next;
        ptr->addr   = (void *)top;
        ptr->next   = heap->used;
        ptr->size   = num;
        heap->used  = ptr;
        heap->top   = new_top;
        return ptr;
    }
    return NULL;
}

void *ta_alloc(unsigned int num) {
    Block *block = alloc_block(num);
    if (block != NULL) {
        return block->addr;
    }
    return NULL;
}

static void memclear(void *ptr, unsigned int num) {
    unsigned int *ptrw = (unsigned int *)ptr;
    unsigned int numw  = (num & -sizeof(unsigned int)) / sizeof(unsigned int);
    while (numw--) {
        *ptrw++ = 0;
    }
    num &= (sizeof(unsigned int) - 1);
    unsigned char *ptrb = (unsigned char *)ptrw;
    while (num--) {
        *ptrb++ = 0;
    }
}

void *ta_calloc(unsigned int num, unsigned int size) {
    num *= size;
    Block *block = alloc_block(num);
    if (block != NULL) {
        memclear(block->addr, num);
        return block->addr;
    }
    return NULL;
}

static unsigned int count_blocks(Block *ptr) {
    unsigned int num = 0;
    while (ptr != NULL) {
        num++;
        ptr = ptr->next;
    }
    return num;
}

unsigned int ta_num_free() {
    return count_blocks(heap->free);
}

unsigned int ta_num_used() {
    return count_blocks(heap->used);
}

unsigned int ta_num_fresh() {
    return count_blocks(heap->fresh);
}

bool ta_check() {
    return heap_max_blocks == ta_num_free() + ta_num_used() + ta_num_fresh();
}
