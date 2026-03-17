#include "kernelLog.h"
#include "graphics/vgaText.h"
#include "core/mmu.h"
#include "global/string/cstring.h"

#define ROW_LENGTH 0xA0
#define videoMem 0xB8000

unsigned char kernelLog_symbol_map[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

void kernelLog_string(char* str)
{
    //Move everthing up one row on the screen
    for (int i = 0; i < 24; i++)
    {
        uint8* src = (uint8*)(videoMem + ((i + 1) * ROW_LENGTH));
        uint8* dest = (uint8*)(videoMem + (i * ROW_LENGTH));

        memcpy(dest, src, ROW_LENGTH);
    }
    //Print new row
    uint32 count = 0;
    uint8* dest = (uint8*)(videoMem + (24 * ROW_LENGTH));
    puts("                                                                       ", 0, 24, 0x00); //Clear the row
    puts(str, 0, 24, 0x0F);
}

void kernelLog_string_int(char* str, uint32 data)
{
    kernelLog_string(str);

    uint32 str_size = strlen(str);

    char c1 = (data & 0xFF000000) >> 24;
    char c2 = (data & 0x00FF0000) >> 16;
    char c3 = (data & 0x0000FF00) >> 8;
    char c4 = data & 0x000000FF;

    putc(kernelLog_symbol_map[((c1 & 0xF0) >> 4)], str_size    , 24, 0x0F);
    putc(kernelLog_symbol_map[(c1 & 0x0F)       ], str_size + 1, 24, 0x0F);

    putc(kernelLog_symbol_map[((c2 & 0xF0) >> 4)], str_size + 2, 24, 0x0F);
    putc(kernelLog_symbol_map[(c2 & 0x0F)       ], str_size + 3, 24, 0x0F);

    putc(kernelLog_symbol_map[((c3 & 0xF0) >> 4)], str_size + 4, 24, 0x0F);
    putc(kernelLog_symbol_map[(c3 & 0x0F)       ], str_size + 5, 24, 0x0F);

    putc(kernelLog_symbol_map[((c4 & 0xF0) >> 4)], str_size + 6, 24, 0x0F);
    putc(kernelLog_symbol_map[(c4 & 0x0F)       ], str_size + 7, 24, 0x0F);
}