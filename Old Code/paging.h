//Data structs and calls to pagging.c
void paging_CreateDirectory(int process_ID);
void paging_CreatePageTable(int process_ID, int pageDirectorySlot);
void paging_AddPage(int process_ID, int pageDirectorySlot, int pageTableSlot, unsigned long adress, unsigned long flags);
unsigned long* paging_GetDirectory(int process_ID);