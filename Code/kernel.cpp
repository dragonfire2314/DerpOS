/*
	DerpOS Kernel V 0.28
	By Tanner Kern
*/
#include "paging.h"
#include "interrupts.h"
#include "scheduler.h"
#include "disk/ata.h"
#include "global/io.h"
#include "global/types.h"
#include "core/mmu.h"
#include "io/serial.h"
#include "graphics/vgaText.h"
#include "kernelLog.h"
#include "graphics/screen.h"
#include "graphics/draw/shapes.h"
#include "desktop/desktop.h"

//Function definitions

//C externals
extern "C" void kmain(void);
extern "C" void high_half_kmain();

//Externs
extern "C" void testFunc();
extern "C" void longJump();
extern "C" void v86RealMode();

extern "C" void write_cr3(unsigned long*);
extern "C" void write_cr0(unsigned long);
extern "C" unsigned long read_cr0();

//Definitions
#define SCREENSIZE 4000 // 80 * 25 * 2

unsigned int* realModeProg = (unsigned int*)0x00007E00;
unsigned char* linearFrameBuffer = (unsigned char*)0x3D000000;

//Kernel that is run with the scheduler
void kernel_ultimate()
{
	kernelLog_string("Enumerating Drives");
	enumerate_drives();
	kernelLog_string("Drives Enumerated");

	kernelLog_string("Loading Terminal Process...");
	File_info info = readFile('C', "TERMINAL.ELF");
    if (info.data == 0) serial_write_string("File data not returned \r\n");
	// else scheduler_addProcess(info.data);
	else serial_write_string("File Found and returned\r\n");

	//This thread is being used for the application viewer
	start_desktop();

	while(1);
}

void high_half_kmain()
{
	//Turn off interupts
	asm("cli");

	kernelLog_string("Kernel Entered High Half");

	//Init memory manager
	memory_init();
	
	//Init the interrupts
	idt_init();
	//Add kernel to scheduler
	scheduler_addKernel((unsigned int*)kernel_ultimate, *paging_GetDirectory(0));

	//Setup the scheduler
	scheduler_setup();

	kernelLog_string("Scheduler Loaded");

	//Turn on interupts
	asm("sti");
	//Loop till scheduler takes control on next interupt

	while(1);
}

void kmain(void)
{
	//Init the kernel log
	kernelLog_string("Kernel Loaded");

	//Init VESA
	kernelLog_string("Setting Up VESA...");
	Video_Info* info = init_graphics();
	kernelLog_string("VESA Initiated");

	//draw_rect(0, 25, info->width, info->height - 50, 0xFFFFFF);
	//draw_rect(100, 100, 100, 100, 0xFFFFFF);
	//draw_rect(100, 100, 100, 100, 0xFFFFFF);

	//while(1);

	//Init serial output
	serial_init();

	//Test the memory allocator
	//MEMORY_ALLOC_TEST_SUITE();


	kernelLog_string("Serial Enabled");
	serial_write_string("\r\n\r\nSerial Start \r\n\r\n");

	
	serial_write_debug("Width: ", info->width);
	serial_write_debug("Height: ", info->height);
	serial_write_debug("BPP: ", info->bpp);
	serial_write_debug("Frame: ", (uint32)info->frameBuffer);
	serial_write_debug("KFrame: ", (uint32)info->kenel_frameBuffer);

	//Create the kernels page table
	unsigned long adr1 = 0;
	unsigned long adr2 = 0;
	//unsigned long adr3 = (unsigned long)frameBuffer;

	//Create pagging for the higher half kernal at 0xC0000000
	paging_CreateDirectory(0);
	//64mb at 0xC0000000
	for(int g = 0; g < 16; g++){
		paging_CreatePageTable(0, 0x300+g);
		for(int i = 0; i < 1024; i++)
		{
			paging_AddPage(0, 0x300+g, i, adr1, 3);
			adr1+=4096;
		}
	}
	//64mb at 0x0
	for(int g = 0; g < 16; g++){
		paging_CreatePageTable(0, g);
		for(int i = 0; i < 1024; i++)
		{
			paging_AddPage(0, g, i, adr2, 3);
			adr2+=4096;
		}
	}

	//Map vbe buffer into memory
	uint32 frameBufferLocation = (uint32)info->frameBuffer;
	uint32 bufferPageTable = (uint32)info->frameBuffer / 0x400000;
	serial_write_debug("bufferPageTable: ", bufferPageTable);
	paging_CreatePageTable(0, bufferPageTable);
	for(int i = 0; i < 1024; i++)
	{
		paging_AddPage(0, bufferPageTable, i, frameBufferLocation, 3);
		frameBufferLocation+=4096;
	}

	//Enable paging
	write_cr3(paging_GetDirectory(0));
	write_cr0(read_cr0() | 0x80000000);
	//Long jump to high half kernel

	set_up_framebuffer();

	longJump();

	return;
}