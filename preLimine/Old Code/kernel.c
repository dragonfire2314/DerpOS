/*
	DerpOS Kernel V 0.28
	By Tanner Kern, Coltyn Sheland
*/
//Function definitions

#include "testFeatures.h"
#include "mmu.h"
#include "floppy.h"
#include "vbe.h"
#include "graphics.h"
#include "desktop.h"

//Externs
extern void testFunc();
extern void longJump();
extern void v86RealMode();

extern void write_port(unsigned short port, unsigned char data);
extern char read_port(unsigned short port);

//Definitions
#define SCREENSIZE 4000 // 80 * 25 * 2

//Variables
char *screen = (char*)0xb8000;
char *vgaMem = (char*)0xa0000;
unsigned int* realModeProg = (unsigned int*)0x00007E00;
unsigned char* linearFrameBuffer = (unsigned char*)0x3D000000;

void cls(unsigned short color)
{
	int j = 0;
	while (j < SCREENSIZE) {
		/* blank character */
		screen[j] = ' ';
		/* attribute-byte */
		screen[j+1] = color;
		j = j + 2;
	}
}

void putc(char letter, int x, int y, unsigned short color)
{
	screen[((80 * y) + x) * 2] = letter; //Places the charater
	screen[(((80 * y) + x) * 2) + 1] = color; //Selects the color of the character
}

unsigned char symbol_map[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

void putd(int data, char x, char y, unsigned short color)
{
    char c1 = (data & 0xFF000000) >> 24;
    char c2 = (data & 0x00FF0000) >> 16;
    char c3 = (data & 0x0000FF00) >> 8;
    char c4 = data & 0x000000FF;

    putc(symbol_map[((c1 & 0xF0) >> 4)], x+2, y, color);
    putc(symbol_map[(c1 & 0x0F)], x+3, y, color);

    putc(symbol_map[((c2 & 0xF0) >> 4)], x+4, y, color);
    putc(symbol_map[(c2 & 0x0F)], x+5, y, color);

    putc(symbol_map[((c3 & 0xF0) >> 4)], x+6, y, color);
    putc(symbol_map[(c3 & 0x0F)], x+7, y, color);

    putc(symbol_map[((c4 & 0xF0) >> 4)], x+8, y, color);
    putc(symbol_map[(c4 & 0x0F)], x+9, y, color);
}

void NewP()
{
	while(1)
	{
		cls(0x77);
	}
}

void high_half_kmain()
{
	//asm("cli");
	//idt_init();
	graphic_init();

	asm("cli");
	//Load Real Mode program into mem, then jump to real mode program to set up graphics
	
	//readToLoc(2, realModeProg);
	//asm("int $36");


	cls(0x11);

	scheduler_Setup();
	//AddProcess((unsigned long)NewP);

	//Test image loading
	struct Image img;
	graphics_cls(100, 100, 100);
	img.width = 500;
	img.height = 500;
	img.Mem = readSector(2);
	for(int i = 3; i < 2000; i++)
	{
		readSector(i);
	}

	graphics_blit_image(&img, 100, 100, 500, 500);

	graphics_present();

	int testX = 0;
	int testY = 0;

	//Sound blaster 16 test code
	//init sound card
	write_port(0x226, 1);
	//Wait 3 micro seconds... im just going to make the code do somthing intensive
	graphics_present();
	graphics_present();
	graphics_present();
	graphics_present();
	graphics_present();
	graphics_blit_image(&img, 0, 0, 500, 500);
	graphics_blit_image(&img, 0, 0, 500, 500);
	graphics_blit_image(&img, 0, 0, 500, 500);
	//That should be enough slow down
	write_port(0x226, 0);
	if(read_port(0x22A) == 0xAA)
	{
		graphics_blit_image(&img, 300, 300, 500, 500);
		graphics_present();
	};

	while(1)
	{

	}

	/*
	desktop_init();

	unsigned int* windowLoc = k_malloc_large(500*500*3);

	int windowID = desktop_CreateWindow(100, 100, 500, 500, windowLoc, 0, 0, 0);
	desktop_SetWindowUpdate(windowID);

	for(int i = 0; i < 1464; i++)
	{
		readToLoc(i, windowLoc + (i*512));
	}

	while(1)
	{
		desktop_Update();
		graphics_present();
	}*/
}

void kmain(void)
{
	//Enable vbe and set a resolution
	vbe_init();

	//Make copy of frame buffer that is scaled to work with the kernels ds value of 0xC0000000
	linearFrameBuffer = (unsigned char*)frameBuffer - 0xC0000000;

	//Create the kernels page table
	unsigned long adr1 = 0;
	unsigned long adr2 = 0;
	unsigned long adr3 = (unsigned long)frameBuffer;

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

	//Enable paging
	write_cr3(paging_GetDirectory(0));
	write_cr0(read_cr0() | 0x80000000);
	//Long jump to high half kernel
	longJump();

	return;
}