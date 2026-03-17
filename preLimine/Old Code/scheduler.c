/*
;
;   Schedualer for DerpOS
;   Written by Tanner Kern
;
*/

#include "testFeatures.h"
#include "paging.h"
#include "mmu.h"
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

extern void asm_scheduler_fin();

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

struct Derp_Pagging** pagingInfo;
unsigned long* info[PROCESS_LIMIT];

void scheduler_Setup()
{
    pagingInfo = paging_GetInfo();
    info[0] = paging_GetDirectory(0);
}

unsigned long *gf = (unsigned long*)0x200000;

unsigned int* testp = (unsigned int*)0xc0000000;

void scheduler()
{
    //Pull the register push to the stack due to calling a function, a non iret from this function will crash the OS
    asm("pop %eax");
    asm("pop %eax");
    asm("pop %ebp");
    asm("pop %eax");

    asm("mov $0x30, %eax");
    asm("mov %eax, %ds");
    asm("mov %eax, %ss");
    asm("mov %eax, %es");
    asm("mov %eax, %fs");
    asm("mov %eax, %gs");

    asm("mov  %0, %%eax"
    :
    :"r"(info[0])
    );
    asm("mov %eax, %cr3");    

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
        asm("mov  %0, %%eax"
            :
            :"r"(info[0])
            );
            asm("mov %eax, %cr3");

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


        
        //write_cr3(gf);

        if(processRunning == 0)
        {
            asm("mov  %0, %%eax"
            :
            :"r"(info[processRunning])
            );
            asm("mov %eax, %cr3");
            asm("popal");

            asm("iret");
        }else
        {
            asm("mov  %0, %%eax"
            :
            :"r"(info[processRunning])
            );
            asm("mov %eax, %cr3");

            asm("mov $0x10, %eax");
            asm("mov %eax, %ds");
            asm("mov %eax, %ss");
            asm("mov %eax, %es");
            asm("mov %eax, %fs");
            asm("mov %eax, %gs");

            asm_scheduler_fin();

            //asm("popal");
            
            //asm("ljmp $0x8,%0"
            //:
            ///:"r"(testp)
            //);
            //asm("popal");
        }
    }

    //asm("out $32, $32");
    asm("popal");
    asm("iret");
    //Manual IRET
    //pop eip
    //asm("mov %esp+4");
    //asm("pop %eflags");
    //asm("jmp (%esp)");
}

char program[48] = 
{
    0xe8,0x07,0x00,0x00,0x00,
    0xc3,
    0xcd, 0x23,
    0xc3,
    0x66, 0x90,
    0x90,
    0x55,
    0x89, 0xe5,
    0x83, 0xec, 0x18,
    0xc7,0x44,0x24,0x08,0x43,0x65,0x87,0x09,
    0xc7,0x44,0x24,0x04,0x30,0x40,0x50,0x60,
    0xc7,0x04,0x24,0x14,0x23,0x14,0x23,
    0xe8,0xd8,0xff,0xff,0xff,
    0xeb,0xfe
};

//char program[24] =
//{
//	0x55,
//	0x89, 0xE5, 
//	0x83, 0xEC, 0x10, 
//	0xC7, 0x45, 0xFC, 0x00, 0x01, 0x00, 0x00,
//	0x8B, 0x45, 0xFC, 
//	0xC7, 0x00, 0x12, 0xEF, 0xCD, 0xAB, 
//	0xEB, 0xFE
//};

void AddProcess(unsigned long func)
{
    processCount++;

    //Set table to identity
    paging_CreateDirectory(1);
    paging_CreatePageTable(1, 0);
    //0-4k
    paging_AddPage(1, 0, 0, (unsigned long)k_create_4k_block(), 3);
    //0xBFFFF000 - 0xBFFFFFFF; I think
    paging_CreatePageTable(1, 0x2FF);
    paging_AddPage(1, 0x2FF, 0x3FF, (unsigned long)k_create_4k_block(), 3);
    //0xC0000000 - 0xC0400000 4MB of kernel   TODO: Move schdualer and syscalls lower in kernel space so that the mapping can be smaller here
    unsigned long adr1p = 0;
	for(int g = 0; g < 5; g++){
		paging_CreatePageTable(1, 0x300+g);
		for(int i = 0; i < 1024; i++)
		{
			paging_AddPage(1, 0x300+g, i, adr1p, 3);
			adr1p+=4096;
		}
	}


    info[1] = paging_GetDirectory(1);

    putd((int)info[1], 0, 3, 0xE);

    //Move program to the 4k block it should run out of
    unsigned long block = paging_GetPageAdress(1, 0, 0);
    block = block & 0xFFFFF000;
    unsigned char* location = (unsigned char*)block;
    unsigned int* prog = read(1);
    unsigned char* progCharLoc = (unsigned char*)prog;
    for(int i = 0; i < 512; i++)
    {
        location[i+0xff] = progCharLoc[i];
    }
    putd((int)block, 0, 24, 0x0A);

    //Stack
    block = paging_GetPageAdress(1, 0x2FF, 0x3FF);
    block = block & 0xFFFFF000;
    unsigned char* stackLoc = (unsigned char*)block;
    putd((int)block, 0, 23, 0x0C);

    stackLoc[0xF0C] = 0x02;
    stackLoc[0xF0C+1] = 0x02;
    stackLoc[0xF0C-4] = 0x08;

    stackLoc[0xF0C-8] = 0xFF;
    stackLoc[0xF0C-7] = 0x00;
    stackLoc[0xF0C-6] = 0x00;
    stackLoc[0xF0C-5] = 0x00;

    stackLoc[0xF0C-12] = 0x00;
    stackLoc[0xF0C-16] = 0x00;
    stackLoc[0xF0C-20] = 0x00;
    stackLoc[0xF0C-24] = 0x00;
    stackLoc[0xF0C-28] = 0x00;
    stackLoc[0xF0C-32] = 0x00;
    stackLoc[0xF0C-36] = 0x00;
    stackLoc[0xF0C-40] = 0x00;

    processes[1].esp = 0xBFFFFF0C - 40;
    processes[1].ebp = 0xBFFFFF0C - 40;

    putd((int)processes[1].esp, 0, 5, 0x0F);
    putd((int)processes[1].ebp, 0, 6, 0x0F);

    //while(1);


    if(processCount < 31){
        //processCount++;

        //Stack Setup



        /*
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET    ] = 0x02;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET + 1] = 0x02;
        ProcessLocation[(PROCESS_CODE_ADRESS * processCount) + PROCESS_OFFSET - 4] = 0x08;
        //0x1007000

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
        */
    }

    

}