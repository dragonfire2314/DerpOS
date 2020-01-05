/*#include "system.h"
// http://www.independent-software.com/writing-your-own-bootloader-for-a-toy-operating-system-7/
// Helpful link

extern void load_gdt(unsigned char *idt_ptr);

#define GDT_SIZE 256

char *GDT = (char*)0x800;

//char *tree = "tree";

unsigned char gdt_ptr[6];

void gdt_init()
{
    GDT[0] = 0x00;
    GDT[1] = 0x00;

    GDT[2] = 0x00;
    GDT[3] = 0x00;

    GDT[4] = 0x00;
    
    GDT[5] = 0x00;

    GDT[6] = 0x00;

    GDT[7] = 0x00;
    //second one
    GDT[8] = 0xff;
    GDT[9] = 0xff;

    GDT[10] = 0x00;
    GDT[11] = 0x00;

    GDT[12] = 0x00;
    
    GDT[13] = 0x9a;

    GDT[14] = 0xcf;

    GDT[15] = 0x00;
    //third one
    GDT[16] = 0xff;
    GDT[17] = 0xff;

    GDT[18] = 0x00;
    GDT[19] = 0x00;

    GDT[20] = 0x00;
    
    GDT[21] = 0x92;

    GDT[22] = 0xcf;

    GDT[23] = 0x00;

    gdt_ptr[0] = 0x00;
    gdt_ptr[1] = 0x17;

    gdt_ptr[2] = 0x00;
    gdt_ptr[3] = 0x00;
    gdt_ptr[4] = 0x08;
    gdt_ptr[5] = 0x00;

    load_gdt(gdt_ptr);
}







/*
extern void load_gdt();

struct GDT_ENTRY{
    //2bytes
    unsigned short lower_limit;
    //2bytes
    unsigned short lower_base;
    //1bytes
    unsigned char middle_base;
    //1bytes
    unsigned char access_bits;
    //1byte
    unsigned char upper_limit_flags;
    //1bytes
    unsigned char upper_base;
};

//struct GDT_ENTRY* GDT = (struct GDT_ENTRY*)0x800;

struct GDT_ENTRY GDT[GDT_SIZE];

void gdt_init()
{
	unsigned long gdt_address;
	unsigned long gdt_ptr[2];

    //NULL GDT page
    GDT[0].lower_limit = 0;
    GDT[0].lower_base = 0;
    GDT[0].middle_base = 0;
    GDT[0].access_bits = 0;
    GDT[0].upper_limit_flags = 0;
    GDT[0].upper_base = 0;
    //Code segment
    GDT[1].lower_limit = 0xFFFF;
    GDT[1].lower_base = 0x0000;
    GDT[1].middle_base = 0x00;
    GDT[1].access_bits = 0x9A;
    GDT[1].upper_limit_flags = 0xCF;
    GDT[1].upper_base = 0x00;
    //Data segment
    GDT[2].lower_limit = 0xFFFF;
    GDT[2].lower_base = 0x0000;
    GDT[2].middle_base = 0x00;
    GDT[2].access_bits = 0x92;
    GDT[2].upper_limit_flags = 0xCF;
    GDT[2].upper_base = 0x00;

    gdt_address = (unsigned long)GDT;
	gdt_ptr[0] = (sizeof (struct GDT_ENTRY) * GDT_SIZE) + ((gdt_address & 0xffff) << 16);
	gdt_ptr[1] = gdt_address >> 16 ;

	load_gdt(gdt_ptr);
}*/