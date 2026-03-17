/*
;
;   Scheduler for DerpOS
;   Written by Tanner Kern
;
*/
#include "scheduler.h"
#include "paging.h"
#include "core/mmu.h"
#include "disk/ata.h"
#include "io/serial.h"
#include "util/elf/elf.h"

#define SCHEDULER_MEMORY_NO_OFFSET 0x110023
#define SCHEDULER_MEMORY_SIZE 16384
#define SCHEDULER_OFFSET 0xC0000000
#define SCHEDULER_MEMORY 0xC0110023
#define SCHEDULER_PROCESSES_OFFSET 0x20

unsigned int* s_mem = (unsigned int*)SCHEDULER_MEMORY;
unsigned int* scheduler_memory = (unsigned int*)SCHEDULER_MEMORY_NO_OFFSET;
unsigned int* scheduler_memory_no_ofset = (unsigned int*)SCHEDULER_MEMORY_NO_OFFSET;
unsigned int* scheduler_memory_base_no_offset = (unsigned int*)(SCHEDULER_MEMORY_NO_OFFSET + SCHEDULER_PROCESSES_OFFSET);
unsigned int* scheduler_processes = (unsigned int*)(SCHEDULER_MEMORY_NO_OFFSET + SCHEDULER_PROCESSES_OFFSET);

char s_hold = 1;

//Temp index into mem
unsigned int* mem = (unsigned int*)0x123fCA;

unsigned int* memStart = (unsigned int*)0x1025F04;



//Scheduler memory map
//First 32 bytes reseved to data storage
//Int 1 - Current Proccess Running
//Int 2 - Current Proccess Count
//Byte 31 and on are a stack of proccesses and there information stored in order
//16bytes per entry
//  -Int 0- esp
//  -Int 1- ebp
//  -Int 2- cr3
//  -Int 3- ds value

//This function switches the proccess to the next one in a round robin method
//Functions can not be called from this function, all code must be written here and can not use functions
//doing so with break the stack, trust me
extern "C"
void scheduler()
{
    //cls(0xFF);
    //Save esp and ebp values
    //esp
    unsigned int esp;
    asm("mov %%esp, %0"
    :"=r"(esp)
    );
    scheduler_processes[scheduler_memory[0]*4] = esp+(6*4);
    //ebp
    unsigned int ebp;
    asm("mov %%esp, %0"
    :"=r"(ebp)
    );
    scheduler_processes[(scheduler_memory[0]*4)+1] = ebp+(6*4);
    //Set next process as current one
    if(scheduler_memory[0] >= scheduler_memory[1])
    {
        scheduler_memory[0] = 0; 
    }
    else
    { 
        scheduler_memory[0] = scheduler_memory[0]+1;
    }

    //Isolate first run
    if(s_hold == 1){ s_hold = 0; 
        //Load kernel data
        scheduler_memory_base_no_offset[0] = 0x123faa;
        scheduler_memory_base_no_offset[1] = 0x123faa;
        scheduler_memory_base_no_offset[2] = 0x1000000; //These values are oddly hardcoded, i dont know why since they get assigned before thie function is called.
        scheduler_memory_base_no_offset[3] = 0x30; 
        //Load esp and ebp values
        asm("mov  %0, %%esp"
        :
        :"r"(scheduler_processes[((scheduler_memory[0])*4)])
        );
        asm("mov  %0, %%ebp"
        :
        :"r"(scheduler_processes[((scheduler_memory[0])*4)+1])
        );
    }

    //Load esp and ebp values
    asm("mov  %0, %%esp"
    :
    :"r"(scheduler_processes[((scheduler_memory[0])*4)])
    );
    asm("mov  %0, %%ebp"
    :
    :"r"(scheduler_processes[((scheduler_memory[0])*4)+1])
    );
    //Load a new cr3
    asm("mov  %0, %%cr3"
    :
    :"r"(scheduler_processes[((scheduler_memory[0])*4)+2])
    );
    //Load ds and segement registers
    asm("mov  %0, %%eax"
    :
    :"r"(scheduler_processes[((scheduler_memory[0])*4)+3])
    );
    asm("mov %eax, %ds");
    asm("mov %eax, %ss");

    //Return to process
    asm("popal");
    asm("iret");
}

void scheduler_addKernel(unsigned int* func, uint32 kernel_page_directory)
{
    //Set up stack for kernel
    scheduler_memory_base_no_offset[0] = 0x123faa;
    scheduler_memory_base_no_offset[1] = 0x123faa;
    //scheduler_memory_base_no_offset[2] = 0x1000000;
    scheduler_memory_base_no_offset[2] = kernel_page_directory;
    scheduler_memory_base_no_offset[3] = 0x30;

    mem[0] = (unsigned int)func;
    mem[1] = 0x28;
    mem[2] = 0x202;
}

void scheduler_setup()
{
    //Setup 
    //Int 1 - Current Proccess Running
    //Int 2 - Current Proccess Count
    scheduler_memory_no_ofset[0] = 0;
    scheduler_memory_no_ofset[0] = 0;

    scheduler_memory_base_no_offset[4] = 0x12345;
    scheduler_memory_base_no_offset[5] = 0xabc45;
    scheduler_memory_base_no_offset[6] = 0xab54735;
    scheduler_memory_base_no_offset[7] = 0x100;
}

void scheduler_addProcess(uint8* elfData)
{
    serial_write_string("PASS 1 \r\n");
    //Disable interupts
    asm("cli");
    //Increase Process count
    scheduler_memory_no_ofset[1] = scheduler_memory_no_ofset[1] + 1;
    //Get next process location
    unsigned int porcessLoc = (scheduler_memory_no_ofset[1]) * 4;
    //Make cr3
    int process_ID = scheduler_memory_no_ofset[1];

    //Set table to identity
    paging_CreateDirectory(process_ID);
    paging_CreatePageTable(process_ID, 0);
    //0-4k - (future me thinks its 0-1mb)
    for(int i = 0; i < 1024; i++){
        paging_AddPage(process_ID, 0, i, (unsigned long)k_create_4k_block(), 3);
    }
    //0xBFFFF000 - 0xBFFFFFFF; I think
    paging_CreatePageTable(process_ID, 0x2FF);
    paging_AddPage(process_ID, 0x2FF, 0x3FF, (unsigned long)k_create_4k_block(), 3);
    //0xC0000000 - 0xC0400000 4MB of kernel   TODO: Move schdualer and syscalls lower in kernel space so that the mapping can be smaller here
    unsigned long adr1p = 0;
	for(int g = 0; g < 5; g++){
		paging_CreatePageTable(process_ID, 0x300+g);
		for(int i = 0; i < 1024; i++)
		{
			paging_AddPage(process_ID, 0x300+g, i, adr1p, 3);
			adr1p+=4096;
		}
	}

    serial_write_string("PASS 2 \r\n");

    //Get cr3 for the page directory
    int process_CR3 = (int)paging_GetDirectory(process_ID);
    //Set up paging for process
    unsigned long block = paging_GetPageAdress(1, 0, 0);
    block = block & 0xFFFFF000;
    unsigned char* location = (unsigned char*)block;
    //Load program to memory from disk
    //unsigned int* prog = drive_load_file(fileName);
    
    elf_copy_program(elfData, location + 0xff, process_ID);
    //uint8* progLoc = efl_get_code(elfData);
    //Copy program to the page for execution
    //unsigned char* progCharLoc = (unsigned char*)prog;
    //for(int i = 0; i < 0x1000; i++)
    //{
    //    //location[i+0xff] = progCharLoc[i];
    //    location[i + 0xFF] = progLoc[i];
    //}

    serial_write_string("PASS 3 \r\n");

    //Setup stack
    unsigned char* stackLoc = (unsigned char*)paging_GetPageAdress(process_ID, 0x2FF, 0x3FF);
    unsigned int* stackLoc2 = (unsigned int*)(stackLoc+0xF01);
    stackLoc2[0] = 0x00;
    stackLoc2[1] = 0x8;
    stackLoc2[2] = 0x202;

    serial_write_string("PASS 4 \r\n");

    asm("mov $0x10, %eax");
    asm("mov %eax, %ds");
    //memStart[0] = 0xFF;
    //memStart[1] = 0x8;
    //memStart[2] = 0x202;
    //asm("mov $0x30 %eax");
    //asm("mov %eax, %ds");

    serial_write_string("PASS 5 \r\n");

    scheduler_memory_base_no_offset[porcessLoc]   = 0xBFFFFF0C - 40;
    scheduler_memory_base_no_offset[porcessLoc+1] = 0xBFFFFF0C - 40;
    scheduler_memory_base_no_offset[porcessLoc+2] = process_CR3;
    scheduler_memory_base_no_offset[porcessLoc+3] = 0x10;

    serial_write_string("PASS 6 \r\n");

    asm("sti");
}

//Returns the kernels process id
//Im not sure why this is here, so im not touching it
int getProcess_id()
{
    return s_mem[0];
}

/*



extern void scheduler();

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
struct Process processes[32];
int processCount = 0;

int processRunning;

char *ProcessLocation = (char*)0x100000;

int temp_esp;
int temp_ebp;

char firstRun = 1;

struct Derp_Pagging** pagingInfo;
unsigned long* info[32];

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
    asm("popal");
    asm("iret");
}

//char arr[]

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
    //unsigned int* prog = read(1);
    //unsigned char* progCharLoc = (unsigned char*)prog;
    //for(int i = 0; i < 512; i++)
    //{
    //    location[i+0xff] = progCharLoc[i];
    //}
    location[0xff] = 0xEB;
    location[0xff+1] = 0xFE;
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
        
    }

    

}

*/