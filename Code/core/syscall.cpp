/*
* File Name: syscall.c
* Usage: functions that can be called from userspace for the kernel mode to handle and execute
* Wrtten By: Tanner Kern
*/

#include "../scheduler.h"
#include "../graphics/vgaText.h"
#include "../io/serial.h"
#include "../global/types.h"
#include "../graphics/windowManager.h"

//Syscall Handler
//Takes 5 control integers
extern "C" void syscall()
{
    unsigned int arg1;
    unsigned int* arg2;
    //Gets args from registers
    asm("mov %%ebx, %0"
    :"=r"(arg1)
    );
    asm("mov %%ecx, %0"
    :"=r"(arg2)
    );

    //Change data segment to user space
    asm("mov $0x10, %eax");
    asm("mov %eax, %ds");

    //Load control integers from stack
    unsigned int a0 = (unsigned int)arg2[0];
    unsigned int a1 = (unsigned int)arg2[1];
    unsigned int a2 = (unsigned int)arg2[2];
    unsigned int a3 = (unsigned int)arg2[3];
    unsigned int a4 = (unsigned int)arg2[4];

    //Return data segment to kernel
    asm("mov $0x30, %eax");
    asm("mov %eax, %ds");

    //Determine syscall to use
    switch(a0)
    {
    case 1:
        putc((char)a1, a2, a3, (unsigned short)a4);
        break;
    case 0x10:
        serial_write_string("Create window syscall was made \r\n");
        
        createWindow((char)a1, a2, a3, (void (*)(uint32, uint32, uint32))a4);
        break;
    }
}