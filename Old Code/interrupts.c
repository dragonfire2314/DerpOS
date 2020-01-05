#include "testFeatures.h"

#define PROGRAMS_START 0x01000000

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE_RING_0 0x8e
#define TRAP_GATE_ALL 0xef
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
extern void timer_time(void);
extern void keyboard_handler(void);
extern void put_pixel(void);
extern void asm_scheduler(void);
extern void syscall_int(void);
extern void defualt_ISR(void);
extern void v86RealMode(void);

extern void Pic(void);

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	for(int i = 0; i < 0xDF; i++)
	{
		//INT 32
		unsigned long default_ISR = (unsigned long)defualt_ISR;
		IDT[0x20+i].offset_lowerbits = default_ISR & 0xffff;
		IDT[0x20+i].selector = 0x28;
		IDT[0x20+i].zero = 0;
		IDT[0x20+i].type_attr = INTERRUPT_GATE_RING_0;
		IDT[0x20+i].offset_higherbits = (default_ISR & 0xffff0000) >> 16;
	}

	//INT 32
	unsigned long timer_address = (unsigned long)asm_scheduler;
	IDT[0x20].offset_lowerbits = timer_address & 0xffff;
	IDT[0x20].selector = 0x28;
	IDT[0x20].zero = 0;
	IDT[0x20].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x20].offset_higherbits = (timer_address & 0xffff0000) >> 16;

	//INT 33
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

	//INT 34
	unsigned long pixel_address = (unsigned long)put_pixel;
	IDT[0x22].offset_lowerbits = pixel_address & 0xffff;
	IDT[0x22].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x22].zero = 0;
	IDT[0x22].type_attr = TRAP_GATE_ALL;
	IDT[0x22].offset_higherbits = (pixel_address & 0xffff0000) >> 16;

	//INT 35
	unsigned long syscall_adres = (unsigned long)syscall_int;
	IDT[0x23].offset_lowerbits = syscall_adres & 0xffff;
	IDT[0x23].selector = 0x28;
	IDT[0x23].zero = 0;
	IDT[0x23].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x23].offset_higherbits = (syscall_adres & 0xffff0000) >> 16;

	//INT 36
	unsigned long realMode = (unsigned long)v86RealMode;
	IDT[0x24].offset_lowerbits = realMode & 0xffff;
	IDT[0x24].selector = 0x28;
	IDT[0x24].zero = 0;
	IDT[0x24].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x24].offset_higherbits = (realMode & 0xffff0000) >> 16;

    //write_port(0x20 , 0x11);
	//write_port(0xA0 , 0x11);

	//write_port(0x21 , 0x20);
	//write_port(0xA1 , 0x28);

	//write_port(0x21 , 0x00);
	//write_port(0xA1 , 0x00);

	//write_port(0x21 , 0x01);
	//write_port(0xA1 , 0x01);

	//write_port(0x21 , 0xff);
	//write_port(0xA1 , 0xff);

	write_port(0x20, 0x11);
  	write_port(0xA0, 0x11);
  	write_port(0x21, 0x20);
  	write_port(0xA1, 0x28);
  	write_port(0x21, 0x04);
  	write_port(0xA1, 0x02);
  	write_port(0x21, 0x01);
  	write_port(0xA1, 0x01);

	write_port(0x43, 0x34);

	write_port(0x40, 0x00);
	write_port(0x40, 0x01);


    idt_address = (unsigned long)IDT+0xC0000000;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void Debug()
{
	//This is a second proccess
	cls(0x55);
	while(1);
}

char count = 0;

void Debug2()
{
	write_port(0x20, 0x20);

	if(count == 256){ count = 0; }
	putc(count, 0, 0, 0x49);	
	count++;
}