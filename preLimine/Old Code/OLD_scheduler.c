/*
;
;   Schedualer for DerpOS
;   Written by Tanner Kern
;
*/

#include "testFeatures.h"
#include "paging.h"
//#include "linkedList.h"

//Kernel start at 0x10000
//Proccess start at 0x1000000

/*
1mb stack size
0x10000 - Kernel Stack Top (Kinda)
0x20000 - Kernel Stack Bottom (Kinda)

16mb Space
0x100000 - Proccess Code Start
0x2FFFFF - Proccess Stack Bottom

*/

#define PROCESS_CODE_ADRESS 0x100000
#define PROCESS_OFFSET 0xFFFFB

struct Process {
    int esp;
    int ebp;

    unsigned long* directory;
};

struct Process kernel;
struct Process app1;
struct Process processes[PROCESS_LIMIT];
int processCount = 0;

int processRunning; //0 is the kernel 1-31 is the rest of the applications

//char *ProcessLocation = (char*)PROCESS_CODE_ADRESS + PROCESS_OFFSET_FROM_KERNEL;
//unsigned long *ProcessStackTop = (unsigned long*)PROCESS_EBP_OFFSET + PROCESS_OFFSET_FROM_KERNEL;
char *ProcessLocation = (char*)0x100000;

int temp_esp;
int temp_ebp;

char firstRun = 1;

void setKernelDirectory(unsigned long* dir){ processes[0].directory = dir; }

void scheduler()
{
    //Pull the register push to the stack due to calling a function, a non iret from this function will crash the OS
    asm("pop %ebp");
    asm("pop %eax");

    //Store Current esp and ebp
    asm("mov %%esp, %0"
    :"=r"(temp_esp)
    );
    asm("mov %%ebp, %0"
    :"=r"(temp_ebp)
    );

    if(firstRun == 1){
        //Came from kernel
        processes[0].esp = temp_esp;
        processes[0].ebp = temp_ebp;

        processRunning = 0;
        //Disable first run check
        firstRun = 0;
    }else
    {
        //Store esp and ebp
        processes[processRunning].esp = temp_esp;
        processes[processRunning].ebp = temp_ebp;

        //run next thing
        if(processRunning == processCount){ processRunning = 0; }
        else { processRunning++; }

        asm("mov %0, %%esp"
            :
            :"r"(processes[processRunning].esp)
            );
        asm("mov  %0, %%ebp"
            :
            :"r"(processes[processRunning].ebp)
            );

        //write_cr3(processes[0].directory);
    }

    //asm("out $32, $32");
    asm("popal");
    asm("iret");
}

void AddProcess(unsigned long func)
{
    if(processCount < 31){
        processCount++;

        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET    ] = 0x02;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET + 1] = 0x02;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 4] = 0x08;

        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 8] = (func & 0xFF);
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 7] = (func & 0xFF00) >> 8;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 6] = (func & 0xFF0000) >> 16;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 5] = (func & 0xFF000000) >> 24;

        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 12] = 0;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 16] = 0;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 20] = 0;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 24] = 0;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 28] = 0;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 32] = 0;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 36] = 0;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 40] = 0;

        processes[processCount].esp = PROCESS_CODE_ADRESS + (PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 40;
        processes[processCount].ebp = PROCESS_CODE_ADRESS + (PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET;
    }

    //Create tables
    paging_CreateDirectory(processCount);
    paging_CreatePageTable(processCount, 0);

    unsigned long address = 0;

    for(int i = 0; i < 1024; i++)
    {
        paging_AddPage(processCount, 0, i, address, 3);
        address = address + 4096;
    }

    processes[processCount].directory = paging_GetDirectory(processCount);
}