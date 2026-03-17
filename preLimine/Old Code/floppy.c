#include "floppy.h"
#include "testFeatures.h"

extern void write_port(unsigned short port, unsigned char data);
extern unsigned char read_port(unsigned port);

void floppy_Init()
{
    unsigned char c;
    write_port(0x70, 0x10);
    c = read_port(0x71);

    int t = c;
    putd(t, 0, 21, 0x0F);
}

void floppy_CopyProgram()
{

}