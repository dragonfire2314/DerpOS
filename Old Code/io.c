#include "system.h"
#include "io.h"
#include "keyboard_map.h"

#define SCREENSIZE 4000

#define ENTER_KEY_CODE 0x1C
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

extern void put_pixel(void);
extern void timer_time(void);

//Thge text bank will not excide 2 charaters long
char *textBank = (char*)0x7E00;
unsigned int current_loc = 0;

	//     Ports
	//	     PIC1	PIC2
	//Command 0x20	0xA0
	//Data	 0x21	0xA1

void kb_init(void)
{
	write_port(0x21 , 0xFD);
}

void keyboard_handler_main(void)
{
	unsigned char status;
	char keycode;

	//Resets the IRQ chip so that it know that it can send more
	write_port(0x20, 0x20);
	//Reads the status of the keyboard that was pressed
	status = read_port(KEYBOARD_STATUS_PORT);

	if (status & 0x01) 
	{
		//Reads the key that was pressed
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0) //Checks for a real number
			return;
		for (int i = 0; i < 256; i++)
		{
			if(keycode == i){ keyNewState[i] = keycode; }

			keys[i].bPressed = 0;
			keys[i].bReleased = 0;

			if (keyNewState[i] != keyOldState[i])
			{
				if (keyNewState[i] & 0x8000)
				{
					keys[i].bPressed = !keys[i].bHeld;
					keys[i].bHeld = 1;
				}
				else
				{
					keys[i].bReleased = 1;
					keys[i].bHeld = 0;
				}
			}

			keyOldState[i] = keyNewState[i];
		}

		//Legacy Input so that the OS does not break
		if(keycode == ENTER_KEY_CODE) {
			textBank[0] = 2;
			return;
		}

		if(keyboard_map[(unsigned char) keycode] == 12)
		{
			thing();
			kmain();
		}

		if(keyboard_map[(unsigned char) keycode] == 31)
		{
			debug();
		}

		//putc(keyboard_map[(unsigned char) keycode], 5, 5, 0x56);

        //Shifts chars over one space in the array
        for(int i = 2; i <= 254; i++){ textBank[i-1] = textBank[i]; }
        textBank[1] = keyboard_map[(unsigned char) keycode];
        textBank[0] = 3;
	}
}

void timer_set_main()
{
	putc('y', 9, 9, 0x0F);
	return;
}

/*

unsigned char status;
	char keycode;

	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;

		if(keycode == ENTER_KEY_CODE) {
			textBank[0] = 2;
			return;
		}

		if(keyboard_map[(unsigned char) keycode] == 12)
		{
			thing();
			kmain();
		}

		if(keyboard_map[(unsigned char) keycode] == 31)
		{
			debug();
		}

		//putc(keyboard_map[(unsigned char) keycode], 5, 5, 0x56);

        //Shifts chars over one space in the array
        for(int i = 2; i <= 254; i++){ textBank[i-1] = textBank[i]; }
        textBank[1] = keyboard_map[(unsigned char) keycode];
        textBank[0] = 3;
	}

*/