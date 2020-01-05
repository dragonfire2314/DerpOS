#include "system.h"

//tetrisScreen[25][80][0] = Text;
//tetrisScreen[25][80][1] = Color;
char snakeScreen[25][80][2];

int tetrisMain()
{
    while(1)
    {
        tetrisRenderer();
        tetrisKeyboard();
    }
}

void tetrisKeyboard()
{
    if(text[0] == 2){}
    else if(text[0] == 3)
    {
        if(text[1] == 'w')
        {
            tetrisLinePutc('w', 0, 0, 0x0F);
        }
        else if(text[1] == 's')
        {
            tetrisLinePutc('s', 0, 0, 0x0F);
        }
        else if(text[1] == 'a')
        {
            tetrisLinePutc('a', 0, 0, 0x0F);
        }
        else if(text[1] == 'd')
        {
            tetrisLinePutc('d', 0, 0, 0x0F);
        }
        else if(text[1] == ' ')
        {
            tetrisLinePutc(' ', 0, 0, 0x0F);
        }
    }
    text[0] = 0;
}

void tetrisRenderer()
{
    //Renders Screen
    for(int y = 0; y < 25; y++)
    {
        for(int x = 0; x < 80; x++)
        {
            putc(snakeScreen[y][x][0], x, y, snakeScreen[y][x][1]);
        }
    }
}

void tetrisLinePutc(char letter, int x, int y, char color)
{
    snakeScreen[y][x][0] = letter;
    snakeScreen[y][x][1] = color;
}