#include "core/mmu.h"

struct Derp_Pagging
{
    struct pageDirectory* dir;
    struct pageTable* table_Loc[1024];
};

struct pageDirectory
{
    unsigned long D_entries[1024];
};

struct pageTable
{
    unsigned long T_entries[1024];
};

struct Derp_Pagging* derp_pagging[32];

void paging_CreateDirectory(int process_ID)
{
    derp_pagging[process_ID]->dir = (struct pageDirectory*)k_create_4k_block();
}

unsigned long* paging_GetDirectory(int process_ID)
{
    return (unsigned long*)derp_pagging[process_ID]->dir;
}

void paging_CreatePageTable(int process_ID, int pageDirectorySlot)
{
    derp_pagging[process_ID]->table_Loc[pageDirectorySlot] = (struct pageTable*)k_create_4k_block();
    derp_pagging[process_ID]->dir->D_entries[pageDirectorySlot] = (int)derp_pagging[process_ID]->table_Loc[pageDirectorySlot] | 3;
}

void paging_AddPage(int process_ID, int pageDirectorySlot, int pageTableSlot, unsigned long adress, unsigned long flags)
{
    derp_pagging[process_ID]->table_Loc[pageDirectorySlot]->T_entries[pageTableSlot] = adress | 3;
}

unsigned long paging_GetPageAdress(int process_ID, int pageDirectorySlot, int pageTableSlot)
{
    return derp_pagging[process_ID]->table_Loc[pageDirectorySlot]->T_entries[pageTableSlot];
}

struct Derp_Pagging** paging_GetInfo()
{
    return derp_pagging;
}