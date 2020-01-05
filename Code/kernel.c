/*
	DerpOS Kernel V 0.28
	By Tanner Kern
*/
#include "paging.h"
#include "graphics_temp.h"
#include "scheduler.h"
#include "interrupts.h"
#include "graphics/vbe.h"

//Function definitions

//C externals
extern void kmain(void);
extern void high_half_kmain();

//Externs
extern void testFunc();
extern void longJump();
extern void v86RealMode();

extern void write_cr3(unsigned long*);
extern void write_cr0(unsigned long);
extern unsigned long read_cr0();

extern void write_port(unsigned short port, unsigned char data);
extern char read_port(unsigned short port);


//Definitions
#define SCREENSIZE 4000 // 80 * 25 * 2

//Variables
char *vgaMem = (char*)0xa0000;
unsigned int* realModeProg = (unsigned int*)0x00007E00;
unsigned char* linearFrameBuffer = (unsigned char*)0x3D000000;

void temp()
{
	cls(0x99);
}

//Kernel that is run with the scheduler
void kernel_ultimate()
{
	cls(0x99);

	putc('p', 19, 5, 0x5F);

	scheduler_addProcess('t');

	putc('V', 20, 5, 0x5F);

	while(1)
	{
		putc('Q', 21, 5, 0x5F);
	}
}

void high_half_kmain()
{
	//Init tinyAlloc
	//ta_init((void*)0x3000000, (void*)0x2000000, 256, 64, 4);

	//Turn off interupts
	asm("cli");
	//Init the interrupts
	idt_init();
	//Add kernel to scheduler
	scheduler_addKernel((unsigned int*)kernel_ultimate);
	//Setup the scheduler
	scheduler_setup();
	//Turn on interupts
	asm("sti");
	//Loop till scheduler takes control on next interupt
	while(1);
}

void kmain(void)
{
	//vbe_init();

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

	//Frame buffer stuff
	//Will revisit after dynamic ram allocation is added

	/*
	//8mb at framebuffer Location (Normally between 3-4gb)
	int directoryMapping_frameBuffer = (((int)frameBuffer & 0xFFC00000) >> 22);
	for(int g = 0; g < 2; g++){
		paging_CreatePageTable(0, directoryMapping_frameBuffer+g);
		for(int i = 0; i < 1024; i++)
		{
			paging_AddPage(0, directoryMapping_frameBuffer+g, i, adr3, 3);
			adr3+=4096;
		}
	}
	*/

	//Enable paging
	write_cr3(paging_GetDirectory(0));
	write_cr0(read_cr0() | 0x80000000);
	//Long jump to high half kernel
	longJump();

	return;
}