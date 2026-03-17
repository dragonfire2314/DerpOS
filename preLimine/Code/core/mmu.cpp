//Memory Manager for DerpOS
//By Tanner Kern

/*
* The MMU's job is to allocate processes memory and handle their pagging as well
* 
*/
#include "mmu.h"
#include "../io/serial.h"

//16mb
#define RAM_16_MB 0x1000000
#define NULL 0
#define MAX_ENTRIES 341 // (0x1000 / 12) - 1
#define PAGE_SIZE_UINT32 0x400

#define ADD_NODE_LOC_SECURE(x) if(!isNodeLocFull()) addNodeLoc(x); else return

/*
    Dynamic memory map layout
        -A linked list of page entries is used to lable memory
        -The linked list must be within the bounds of a page
            -Every page defined for the linked list may only be used my the linked list
*/

struct PageEntry
{
    //Defines a 4k page, bit 0 determines if the page is free
	uint32* location = NULL;
    //Defines the number of pages used
    uint32 size = 0;
    //Next page in linked list
    PageEntry* next = NULL;
} __attribute__ ((packed));

void* alloc_page();
void* malloc_page_aligned(uint32 size);
void* alloc_page_count(uint32 count);
void initNodeLocStatck();
bool isNodeLocEmpty();
uint32* popNodeLoc();

unsigned long* heapPos = (unsigned long*)0x1000000;

PageEntry* head;
PageEntry* current;

uint8* pageEntryStart;
uint32 pageEntriesCount = 0;

uint32* nodeLocStackTop;
uint32* nodeLocStack;

void memory_init()
{
    //Grab a page for allocations list
    pageEntryStart = (uint8*)alloc_page();
    pageEntriesCount = 0;
    //Setup the head pointer to NULL
    head->next = NULL;
    head->location = NULL;
    current = head;

    initNodeLocStatck();
}

PageEntry* prepareNodeLoc()
{
    //Check node list first
    if (!isNodeLocEmpty()) {
        return (PageEntry*)popNodeLoc();
    }

    //Prepare a spot for the node to be added to
    if (pageEntriesCount + 1 > MAX_ENTRIES) { //I think its possible to remove the +1, at worst not doing so is a small memory used increase.
        //Get a new page for adding this entry
        pageEntryStart = (uint8*)alloc_page();
        pageEntriesCount = 0;
    }

    PageEntry* temp = (PageEntry*)(pageEntryStart + (sizeof(PageEntry) * (pageEntriesCount)));

    pageEntriesCount++;

    return temp;
}

void initNodeLocStatck()
{
    nodeLocStack = (uint32*)alloc_page();
    nodeLocStackTop = nodeLocStack;
}

bool isNodeLocEmpty()
{
    if (nodeLocStack == nodeLocStackTop)
        return true;
    else 
        return false;
}

bool isNodeLocFull()
{
    if (nodeLocStack >= nodeLocStackTop + PAGE_SIZE_UINT32)
        return true;
    else 
        return false;
}

void printNodeLocStackSize()
{
    serial_write_debug("Stack Size: ", (uint32)(nodeLocStack - nodeLocStackTop));
}

uint32* popNodeLoc()
{
    nodeLocStack--;
    return nodeLocStack;
}

void addNodeLoc(uint32* loc)
{
    nodeLocStack[0] = (uint32)loc;
    nodeLocStack++;
}

void shrinkList()
{
    //This function combines near by free nodes with one anouther
    PageEntry* search = head;
    while (search->next != NULL) {
        if ((uint32)(search->location) & 0x1) { //Free node check
            //Check if next node is free
            PageEntry* nextNode = search->next;
            if ((uint32)(nextNode->location) & 0x1) { //Free node check
                //Combine the nodes
                search->next = nextNode->next;
                search->size = search->size + nextNode->size;

                ADD_NODE_LOC_SECURE((uint32*)nextNode);
            }
        }
        search = search->next;
    }
}

void addNode (uint32* location, uint32 pageCount)
{
    PageEntry* newPageEntryLoc = prepareNodeLoc();

    if (head->next == NULL) {
        head->next = newPageEntryLoc;

        PageEntry* newNode = head->next;
        newNode->location = location;
        newNode->size = pageCount;
        newNode->next = NULL;
        return;
    }

    //Find spot to place the node
    PageEntry *search = head;
    PageEntry *old = NULL;
    while ((uint32)(search->location) < (uint32)location && search->next != NULL) {
        old = search;
        search = search->next;
    }
    if ((uint32)(search->location) < (uint32)location) { //Greater than
        search->next = newPageEntryLoc;
        PageEntry* newNode = search->next;
        newNode->location = location;
        newNode->size = pageCount;
        newNode->next = NULL;
    }
    else { //Less than
        old->next = newPageEntryLoc;
        PageEntry* newNode = old->next;
        newNode->location = location;
        newNode->size = pageCount;
        newNode->next = search;
    }
}

void splitNode(PageEntry* node, uint32 size1)
{
    serial_write_debug("Split node: ", (uint32)node);

    if (node->size == size1)
        return;
    //Split the node
    PageEntry* newPageEntryLoc = prepareNodeLoc();

    newPageEntryLoc->next = node->next;
    newPageEntryLoc->size = node->size - size1;
    newPageEntryLoc->location = (uint32*)((uint8*)(node->location) + PAGE_SIZE * size1);

    node->next = newPageEntryLoc;
    node->size = size1;
}

void* malloc_page_aligned(uint32 size)
{    
    uint32* temp;

    uint32 pageCount = (size / 0x1000) + 1;
    //Check for unused memory
    PageEntry *search = head;
    while (search->next != NULL) {
        if ((uint32)(search->location) & 0x1) { //Free page found
            if (search->size >= pageCount) { //Size matches
                splitNode(search, pageCount);
                search->location = (uint32*)((uint32)search->location & 0xFFFFFFFE);
                temp = search->location;
                return temp;
            }
        }
        search = search->next;
    }
    if ((uint32)(search->location) & 0x1) { //Free page found
        if (search->size >= pageCount) { //Size matches
            splitNode(search, pageCount);
            search->location = (uint32*)((uint32)search->location & 0xFFFFFFFE);
            temp = search->location;
            return temp;
        }
    }
    search = search->next;
    //Alloc mem
    temp = (uint32*)alloc_page_count(pageCount);
    addNode(temp, pageCount);


    serial_write_string("ALLOCATING: ");  
    serial_write_int((uint32)temp);
    serial_write_string("\r\n");


    return temp;
}

void free_page_aligned(void* mem)
{
    //Find entry in list
    PageEntry* search = head;
    PageEntry* old = NULL;
    while (search->location != mem && search->next != NULL) {
        old = search;
        search = search->next;
    }
    if (search->next == NULL) {
        //Last node
        if (search->location == mem)
            search->location = (uint32*)((uint32)search->location | 0x1);
        else
            serial_write_debug("Free called on no allocated memory: ", (uint32)mem);
        return;
    }
    search->location = (uint32*)((uint32)search->location | 0x1);
}

void* alloc_page_count(uint32 count)
{
    uint32* ret = (uint32*)alloc_page();
    for (int i = 1; i < count; i++)
    {
        alloc_page();
    }
    return ret;
}

void* alloc_page() 
{
    unsigned long* temp = heapPos;
    heapPos = heapPos + 1024;
    return temp;
}

void* malloc_pages(uint32 pageCount)
{
    return malloc_page_aligned(pageCount * 0x1000 - 1);
}

void printLinkedList()
{
    serial_write_string("Printing List: \r\n");
    PageEntry* search = head;
    while(search->next != NULL) {
        serial_write_debug("Node data: ", (uint32)search);

        search = search->next;
    }

    serial_write_debug("Node data: ", (uint32)search);
}

//This test suite should not be used with any other memory commands
void MEMORY_ALLOC_TEST_SUITE () 
{
    memory_init();

    uint32 *m0, *m1, *m2, *m3, *m4, *m5, *m6, *m7, *m8,*m9;

    m0 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m1 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m2 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m3 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m4 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m5 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m6 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m7 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m8 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m9 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);

    serial_write_debug("M0: " , (uint32)m0);
    serial_write_debug("M1: " , (uint32)m1);
    serial_write_debug("M2: " , (uint32)m2);
    serial_write_debug("M3: " , (uint32)m3);
    serial_write_debug("M4: " , (uint32)m4);
    serial_write_debug("M5: " , (uint32)m5);
    serial_write_debug("M6: " , (uint32)m6);
    serial_write_debug("M7: " , (uint32)m7);
    serial_write_debug("M8: " , (uint32)m8);
    serial_write_debug("M9: " , (uint32)m9);

    printLinkedList();

    free_page_aligned(m0);
    free_page_aligned(m1);
    free_page_aligned(m2);
    free_page_aligned(m3);
    free_page_aligned(m4);
    free_page_aligned(m5);
    free_page_aligned(m6);
    free_page_aligned(m7);
    free_page_aligned(m8);
    free_page_aligned(m9);

    printLinkedList();

    m0 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m1 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m2 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m3 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m4 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m5 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m6 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m7 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m8 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);
    m9 = (uint32*)malloc_page_aligned(5 * PAGE_SIZE);

    serial_write_debug("M0: " , (uint32)m0);
    serial_write_debug("M1: " , (uint32)m1);
    serial_write_debug("M2: " , (uint32)m2);
    serial_write_debug("M3: " , (uint32)m3);
    serial_write_debug("M4: " , (uint32)m4);
    serial_write_debug("M5: " , (uint32)m5);
    serial_write_debug("M6: " , (uint32)m6);
    serial_write_debug("M7: " , (uint32)m7);
    serial_write_debug("M8: " , (uint32)m8);
    serial_write_debug("M9: " , (uint32)m9);

    free_page_aligned(m0);
    free_page_aligned(m1);
    free_page_aligned(m2);
    free_page_aligned(m3);
    free_page_aligned(m4);
    free_page_aligned(m5);
    free_page_aligned(m6);
    free_page_aligned(m7);
    free_page_aligned(m8);
    free_page_aligned(m9);

    printLinkedList();

    serial_write_string("Entry Spilting Test \r\n");

    printLinkedList();

    m0 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m1 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m2 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m3 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m4 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m5 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m6 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m7 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m8 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m9 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);

    printLinkedList();

    serial_write_debug("M0: " , (uint32)m0);
    serial_write_debug("M1: " , (uint32)m1);
    serial_write_debug("M2: " , (uint32)m2);
    serial_write_debug("M3: " , (uint32)m3);
    serial_write_debug("M4: " , (uint32)m4);
    serial_write_debug("M5: " , (uint32)m5);
    serial_write_debug("M6: " , (uint32)m6);
    serial_write_debug("M7: " , (uint32)m7);
    serial_write_debug("M8: " , (uint32)m8);
    serial_write_debug("M9: " , (uint32)m9);

    uint32* m10;

    m10 = (uint32*)malloc_page_aligned(1 * PAGE_SIZE);

    serial_write_debug("M10: " , (uint32)m10);

    printLinkedList();

    serial_write_string("Entry Comblining test \r\n");

    free_page_aligned(m0);
    free_page_aligned(m1);
    free_page_aligned(m4);
    free_page_aligned(m8);
    free_page_aligned(m9);

    printLinkedList();

    shrinkList();
    printNodeLocStackSize();

    printLinkedList();

    m0 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m1 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);
    m2 = (uint32*)malloc_page_aligned(3 * PAGE_SIZE);

    serial_write_debug("M0: " , (uint32)m0);
    serial_write_debug("M1: " , (uint32)m1);
    serial_write_debug("M2: " , (uint32)m2);

    printLinkedList();
    printNodeLocStackSize();

    while (1);
}






















unsigned int* k_malloc_large(unsigned int sizeInBytes);
//Large Allocator
//TODO - Make it (currently just gives out memory(I am requiring calls to large free for future use))
void* alloc_large_memory(uint32 size)
{
    return malloc_page_aligned(size);
}

void free_large(void* memory)
{
    //TODO - Free the memory
    //free_page_aligned(memory);
}


unsigned long* k_create_4k_block()
{
    return (unsigned long*)alloc_page();
}























//Should work
int blockCounter512 = 0;
unsigned char* blockPointer512;

unsigned int* k_create_512_block()
{
    if(blockCounter512 <= 0)
    {
        blockPointer512 = (unsigned char*)k_create_4k_block();
        blockCounter512 = 8;
    }

    blockCounter512--;
    unsigned int* temp = (unsigned int*)blockPointer512;
    blockPointer512+=512;

    return temp;
}

//2k block generator
int blockCounter2k = 0;
unsigned char* blockPointer2k;

unsigned int* k_create_2k_block()
{
    if(blockCounter2k <= 0)
    {
        blockPointer2k = (unsigned char*)k_create_4k_block();
        blockCounter2k = 2;
    }

    blockCounter2k--;
    unsigned int* temp = (unsigned int*)blockPointer2k;
    blockPointer2k+=2048;

    return temp;
}

unsigned int* k_malloc_large(unsigned int sizeInBytes)
{
    unsigned long* retAdress = k_create_4k_block();
    for(int i = 0; i < ((sizeInBytes/4096)*4)+1; i++)
    {
        unsigned long* hold = k_create_4k_block();
    }
    return (unsigned int*)retAdress;
}

//Memset
void memset(void* loc, char val, unsigned int size)
{
    for(int i = 0; i < size; i++)
	{
		((char*)loc)[i] = val;
	}
}

//Memcopy
void memcpy(void* dest, const void* src, uint32 size)
{
    for (int i = 0; i < size; i++)
    {
        ((uint8*)dest)[i] = ((uint8*)src)[i];
    }
}