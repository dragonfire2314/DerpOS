/*
* File Name: syscall.c
* Usage: functions that can be called from userspace for the kernel mode to handle and execute
* Wrtten By: Tanner Kern
*/

#include "../graphics_temp.h"
#include "../scheduler.h"
#include "../graphics/graphics_driver.h"

//Syscall Handler
//Takes 5 control integers
void syscall()
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

    //Change data segment to kernel
    asm("mov $0x10, %eax");
    asm("mov %eax, %ds");

    //Load control integers from stack
    unsigned int a0 = (unsigned int)arg2[0];
    unsigned char a1 = (unsigned char)arg2[1];
    unsigned int a2 = (unsigned char)arg2[2];
    unsigned int a3 = (unsigned char)arg2[3];
    unsigned short a4 = (unsigned char)arg2[4];

    //Return data segment
    asm("mov $0x30, %eax");
    asm("mov %eax, %ds");

    //Determine syscall to use
    switch(a0)
    {
    case 1:
        putc(a1, a2, a3, a4);
        break;
    case 0x10:
        //graphics_add_screen(a1, a2, a3, 1);
        break;
    }
}

//Basic print char
void syscall_putc()
{

}