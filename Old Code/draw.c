#include "system.h"

#define SCREENSIZE 4000

char *RenderScreen = (char*)0x10000;

void d_putc(char letter, char x, char y, unsigned short color)
{
    //Make sure pixel is on the screen
    if((x * y) > 2000) return;
	screen[((80 * y) + x) * 2] = letter; //Places the charater
	screen[(((80 * y) + x) * 2) + 1] = color; //Selects the color of the character
}

void d_puts(char *str, char x, char y, unsigned short color)
{

	int j = 0;
	int i = x;
	while (str[j] != '\0') {
		screen[((80 * y) + i) * 2] = str[j];
		screen[(((80 * y) + i) * 2) + 1] = color;
		++j;
		i++;
	}
}

//void putData(char data, char x, char y, unsigned short color)
//{
//    putc(symbol_map[((data & 0xF0) >> 4)], x+2, y, color);
//    putc(symbol_map[(data & 0x0F)], x+3, y, color);
//}

void d_cls(unsigned short color)
{
	int j = 0;
	while (j < SCREENSIZE) {
		/* blank character */
		screen[j] = ' ';
		/* attribute-byte */
		screen[j+1] = color;
		j = j + 2;
	}
}

void d_render()
{
    for(int i = 0; i < 4000; i++)
    {
        screen[i] = RenderScreen[i];
    }
}


void d_square(char x, char y, char w, char h, char color)
{
    for(int t = 0; t < h; t++)
    {    
        for(int i = 0; i < w; i++)
        {
            d_putc(219, x+i, y+t, color);
        }
    }
}