/*
	DerpOS Kernel V 0.28
	By Tanner Kern, Coltyn Sheland
*/
//Function definitions

#include "testFeatures.h"
#include "mmu.h"

//Externs
extern void testFunc();

//Definitions
#define SCREENSIZE 4000 // 80 * 25 * 2

//Variables
char *screen = (char*)0xb8000;

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
		//cls(0x66);
	}
}

void NewP2()
{
	while(1)
	{
		cls(0x99);
	}
}

void kmain(void)
{
	cls(0x00);

	//while(1);

	//cls(0x80);
	
	
	//kb_init();

	AddProcess((unsigned long)NewP);
	//AddProcess((unsigned long)NewP2);

	unsigned long numpag = 0;

	paging_CreateDirectory(0);
	paging_CreatePageTable(0, 0);
	paging_CreatePageTable(0, 1);
	paging_CreatePageTable(0, 2);
	paging_CreatePageTable(0, 3);
	paging_CreatePageTable(0, 4);
	paging_CreatePageTable(0, 5);
	for(int t=0; t<1024; t++)
	{
		paging_AddPage(0, 0, t, numpag, 2);
		numpag += 4096;
	};
	for(int t=0; t<1024; t++)
	{
		paging_AddPage(0, 1, t, numpag, 2);
		numpag += 4096;
	};
	for(int t=0; t<1024; t++)
	{
		paging_AddPage(0, 2, t, numpag, 2);
		numpag += 4096;
	};
	for(int t=0; t<1024; t++)
	{
		paging_AddPage(0, 3, t, numpag, 2);
		numpag += 4096;
	};
	for(int t=0; t<1024; t++)
	{
		paging_AddPage(0, 4, t, numpag, 2);
		numpag += 4096;
	};
	for(int t=0; t<1024; t++)
	{
		paging_AddPage(0, 5, t, numpag, 2);
		numpag += 4096;
	};

	write_cr3(paging_GetDirectory(0));
	write_cr0(read_cr0() | 0x80000000);

	//setKernelDirectory(page_directory);

	idt_init();

	scheduler_Setup();

	while (1) 
	{
		//vga16_update_buffer();
		//cls(0x88);

		//unsigned long *testPointer = (unsigned long *) 0x300000;
		//testPointer[0] = 100;
	}

	//boot a schedualer
	//have schedualer start a new program
	//make the schedualer interrupt and switch proccess

	return;
}

// Leagacy Code

/*

//set up pagging
	unsigned long *page_directory = (unsigned long*)0x200000;
	unsigned long *page_table = (unsigned long*)0x201000;

	unsigned long address = 0; // holds the physical address of where a page is
	unsigned int i;

    // map the first 4MB of memory
	for(i=0; i<1024; i++)
	{
		page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
		address = address + 4096; // 4096 = 4kb
	};

	page_directory[0] = (unsigned long)page_table; // attribute set to: supervisor level, read/write, present(011 in binary)
	page_directory[0] = page_directory[0] | 3;

	for(i=1; i<1024; i++)
	{
		page_directory[i] = 0 | 2; // attribute set to: supervisor level, read/write, not present(010 in binary)
	};

*/