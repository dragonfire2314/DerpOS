#include "graphics_temp.h"
#include "core/mmu.h"

char *screen = (char*)0xb8000;
char **graphicsBuffers;

unsigned char symbol_map[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

void graphics_init()
{
    //graphicsBuffers = 
}

int graphics_create()
{

}




















void putc(char letter, int x, int y, unsigned short color)
{
	screen[((80 * y) + x) * 2] = letter; //Places the charater
	screen[(((80 * y) + x) * 2) + 1] = color; //Selects the color of the character
}

void cls(unsigned short color)
{
	int j = 0;
	while (j < 4000) {
		// blank character 
		screen[j] = ' ';
		// attribute-byte
		screen[j+1] = color;
		j = j + 2;
	}
}

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