/*
	File: alloc.cpp
	Purpose: A simple and small malloc inplimentaion for derp OS
	Author: Tanner Kern
*/

#include "alloc.h"
#include "../core/mmu.h"

#define HEADER_SIZE sizeof(Malloc_Memory)

Alloc::~Alloc()
{
    //Free the allocator
    free_large(location);
}

Alloc::Alloc() {}

void Alloc::init(uint32 heapSize, uint32 minAllocSize)
{
	location = (uint8*)k_malloc_large((heapSize / 0x1000) + 1);

    init_malloc(location, heapSize, minAllocSize);
}

int Alloc::init_malloc(uint8* heapStart, uint32 heapSize, uint32 minAllocSize)
{
	minMemSize = minAllocSize;
	heapPointer = heapStart;
	heapBase = heapStart;
	maxHeapSize = heapSize;
	if (heapSize < 1024) { return -1; } //Return if heap is below 1024 bytes in size (This can be disabled manually)

	//Create start node of size 0
	head_node = (Malloc_Memory*)heapStart;
	head_node->size = 0;
	head_node->free = 0;
	head_node->next = NULL;
	heapPointer += HEADER_SIZE;
}

Malloc_Memory* Alloc::addNode(uint32 size)
{
	current_node = head_node;
	//Find end of node list
	while (current_node->next != NULL)
	{
		current_node = (Malloc_Memory*)current_node->next;
	}
	//On the last node
	//Create a node
	Malloc_Memory* new_node = malloc_node();
	//Point current at new
	current_node->next = (struct Malloc_Memory*)new_node;
	//Set node size
	new_node->size = size;
	//Return new node
	return new_node;
}

void Alloc::removeNode(uint32* address)
{
	current_node = head_node;
	//Look for node
	while (current_node->next != (Malloc_Memory*)address)
	{
		current_node = current_node->next;
	}
	//node found
	Malloc_Memory* temp = current_node->next;
	current_node->next = temp->next;
	current_node->size += (temp->size + HEADER_SIZE);
}

Malloc_Memory* Alloc::malloc_node()
{
	Malloc_Memory* temp = (Malloc_Memory*)heapPointer;
	heapPointer += HEADER_SIZE;

	temp->free = 0;
	temp->next = 0;

	return temp;
}

uint8* Alloc::alloc_heap(uint32 size)
{
	uint8* temp = heapPointer;
	heapPointer += size;
	return temp;
}

//Cleans up the linked list by combining free ones that are next to one anouther
void Alloc::cleanMemoryList()
{
	current_node = head_node;
	//Loop till end of list is hit and return is called
	while (current_node->next != NULL)
	{
		//Shearch for freed memory
		while (current_node->free != 1 && current_node->next != NULL) {
			current_node = current_node->next;
		}
		//Found free memory
		//Check next node for free mem
		if (current_node->next != NULL) {
			Malloc_Memory* temp = current_node->next;
			if (temp->free == 1) {
				removeNode((uint32*)temp);
			}
			else {
				current_node = temp;
			}
		}
		else { //Hit end of list
			return;
		}
	}
}

void Alloc::expandMemory(uint32 amount)
{
	maxHeapSize += amount;
}

void* Alloc::malloc(uint32 size)
{
	//Clean the linked list (combine free nodes)
	cleanMemoryList();
	//Check if size is large enough, if not add to size
	if (size <= minMemSize) {
		size = minMemSize;
	}
	//Check for free memory
	current_node = head_node;
	while (current_node->next != NULL) {
		//Check for a free node
		if (current_node->free == 1) {
			//Check if node is big enough
			if (current_node->size > size) {
				//Check if node should be broken apart
				if ((HEADER_SIZE + minMemSize + size) < current_node->size) {
					//Alloc data
					int oldSize = current_node->size;
					Malloc_Memory* returnData = (Malloc_Memory*)((uint8*)current_node + HEADER_SIZE);
					current_node->size = size;
					current_node->free = 0;
					//Insert a node
					Malloc_Memory* temp = current_node->next;
					current_node->next = (Malloc_Memory*)((uint8*)current_node + HEADER_SIZE + size);
					current_node = current_node->next;
					current_node->next = temp;
					current_node->free = 1;
					current_node->size = (oldSize - HEADER_SIZE - size);
					//Return data
					return returnData;
				}
			}
		}
		current_node = current_node->next;
	}

	//Check if memory is avalible
	if (!((size + HEADER_SIZE) < ((heapBase + maxHeapSize) - heapPointer))) {
		//expandMemory();
        //Memory is not avalible, return zero to the user and post error to the class
        error = OUT_OF_MEMORY;
        return (void*)NULL;
	}
	//Create new node and data
	addNode(size);
	//Alloc data
	return alloc_heap(size);
}

void Alloc::free(uint8* loc)
{
	//Label memory as free
	Malloc_Memory* temp = (Malloc_Memory*)(loc - HEADER_SIZE);
	temp->free = 1;
}

Alloc_Error_Type Alloc::getError()
{
    return error;
}