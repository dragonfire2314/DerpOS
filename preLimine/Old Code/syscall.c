#include "testFeatures.h"

void syscall(int blank, int syscallType, int arg1, int arg2)
{
    switch(syscallType)
    {
    //Calls putd on arg1
    case 0:
        putd(arg1, 50, 23, 0x09);
        break;    
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    default:
        break;
    }

    //putd(blank, 0, 20, 0x04);
    //putd(syscallType, 0, 10, 0x0F);
    //putd(arg1, 0, 12, 0x0F);
    //putd(arg2, 0, 13, 0x0F);

    return;
}