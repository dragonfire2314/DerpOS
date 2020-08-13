#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "global/types.h"

void scheduler_setup();
void scheduler_addKernel(unsigned int* func, uint32 kernel_page_directory);
void scheduler_addProcess(uint8* elfData);
int getProcess_id();

#endif