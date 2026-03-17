#define PROGRAMS_START 0x01000000

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE_RING_0 0x8e
#define TRAP_GATE_ALL 0xef
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

//Bring in io
#include "global/io.h"
#include "io/serial.h"

//External Functions
extern "C" void load_idt(unsigned long *idt_ptr);
extern "C" void timer_time(void);
extern "C" void keyboard_handler(void);
extern "C" void put_pixel(void);
extern "C" void asm_scheduler(void);
extern "C" void syscall_int(void);
extern "C" void defualt_ISR(void);
extern "C" void v86RealMode(void);
extern "C" void breakpoint(void);

extern "C" void Pic(void);

extern "C" void irq14_handler(void);
extern "C" void irq14_handler(void);

//IDT entry structure
struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

//IDT entries
struct IDT_entry IDT[IDT_SIZE];

unsigned long irq14_address;
extern "C" void irq14(void);

//Intiates the interrupts
void idt_init(void)
{
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	//Set all interupts to a default ISR
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

	//INT 32 - Scheduler
	unsigned long timer_address = (unsigned long)asm_scheduler;
	IDT[0x20].offset_lowerbits = timer_address & 0xffff;
	IDT[0x20].selector = 0x28;
	IDT[0x20].zero = 0;
	IDT[0x20].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x20].offset_higherbits = (timer_address & 0xffff0000) >> 16;

	//INT 33 - Keyboard
	unsigned long keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = timer_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x21].offset_higherbits = (timer_address & 0xffff0000) >> 16;

	//INT 35 - System call
	unsigned long syscall_adres = (unsigned long)syscall_int;
	IDT[0x23].offset_lowerbits = syscall_adres & 0xffff;
	IDT[0x23].selector = 0x28;
	IDT[0x23].zero = 0;
	IDT[0x23].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x23].offset_higherbits = (syscall_adres & 0xffff0000) >> 16;

	//INT 3 - Breakpoint
	unsigned long breakpoint_address = (unsigned long)breakpoint;
	IDT[0x3].offset_lowerbits = breakpoint_address & 0xffff;
	IDT[0x3].selector = 0x28;
	IDT[0x3].zero = 0;
	IDT[0x3].type_attr = INTERRUPT_GATE_RING_0;
	IDT[0x3].offset_higherbits = (breakpoint_address & 0xffff0000) >> 16;

	//Setup IRQ hardware
	write_port(0x20, 0x11);
  	write_port(0xA0, 0x11);
  	write_port(0x21, 0x20);
  	write_port(0xA1, 0x28);
  	write_port(0x21, 0x04);
  	write_port(0xA1, 0x02);
  	write_port(0x21, 0x01);
  	write_port(0xA1, 0x01);

	write_port(0x43, 0x34);

	write_port(0x40, 0xFE);
	write_port(0x40, 0xFE);

	//Set up irq14
	irq14_address = (unsigned long)irq14; 
	IDT[46].offset_lowerbits = irq14_address & 0xffff;
	IDT[46].selector = 0x28;
	IDT[46].zero = 0;
	IDT[46].type_attr = INTERRUPT_GATE_RING_0;
	IDT[46].offset_higherbits = (irq14_address & 0xffff0000) >> 16;

	//Set IDT ptr up
    idt_address = (unsigned long)IDT+0xC0000000;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	//Call asm to laod idt into system
	load_idt(idt_ptr);
}

bool isIRQ14 = false;

void irq14_handler() {
	isIRQ14 = true;

    write_port(0xA0, 0x20);
    write_port(0x20, 0x20);

	serial_write_string("IRQ 14 Called \r\n");
}

void prepare_for_irq14() {
	asm("cli");
	isIRQ14 = false;
	asm("sti");
}

void wait_for_irq14() {
	while (true) {
		asm("cli");
		bool temp = isIRQ14;
		asm("sti");

		if (isIRQ14)
			break;
	}
}

extern "C" void breakpoint_handler()
{
	while (1)
	{

	}
}