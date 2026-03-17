#pragma once
#include "global/types.h"

void paging_CreateDirectory(int process_ID);
unsigned long* paging_GetDirectory(int process_ID);
void paging_CreatePageTable(int process_ID, int pageDirectorySlot);
void paging_AddPage(int process_ID, int pageDirectorySlot, int pageTableSlot, unsigned long adress, unsigned long flags);
struct Derp_Pagging** paging_GetInfo();
unsigned long paging_GetPageAdress(int process_ID, int pageDirectorySlot, int pageTableSlot);

void pagecpy(uint32 dest_processID, void* dest_process_addr, void* src);