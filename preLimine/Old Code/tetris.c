#include "system.h"

//tetrisScreen[25][80][0] = Text;
//tetrisScreen[25][80][1] = Color;
char tetrisScreen[25][80][2];

char a[4][4] = 
{
    {0x0B,0,0,0},
    {0x0B,0,0,0},
    {0x0B,0,0,0},
    {0x0B,0,0,0}
};
char b[4][4] = 
{
    {0x0E,0x0E,0,0},
    {0x0E,0x0E,0,0},
    {0,0,0,0},
    {0,0,0,0}
};
char c[4][4] = 
{
    {0,0x0D,0,0},
    {0x0D,0x0D,0x0D,0},
    {0,0,0,0},
    {0,0,0,0}
};
char d[4][4] = 
{
    {0,0x09,0,0},
    {0,0x09,0,0},
    {0,0x09,0,0},
    {0x09,0x09,0,0}
};
char e[4][4] = 
{
    {0x06,0,0,0},
    {0x06,0,0,0},
    {0x06,0,0,0},
    {0x06,0x06,0,0}
};
char f[4][4] = 
{
    {0,0,0,0},
    {0x0C,0x0C,0,0},
    {0,0,0x0C,0x0C},
    {0,0,0,0}
};
char g[4][4] = 
{
    {0,0,0,0},
    {0,0,0x0A,0x0A},
    {0x0A,0x0A,0,0},
    {0,0,0,0}
};


char tetrisMap[20][10] = {
    {0xF0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
    {0xF0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0},
};

void tetrisBoardPutc(char color, int x, int y);
void tetrisLinePuts(char *str, int x, int y);
void tetrisLinePutc(char letter, int x, int y, char color);
void 55placetetrominoe(char letter, int x, int y);
void tetrisKeyboard();
void tetrisRenderer();

int tetrisMain()
{
    placetetrominoe('a', 1, 1);
    placetetrominoe('b', 5, 1);
    placetetrominoe('c', 1, 5);
    placetetrominoe('d', 5, 5);
    placetetrominoe('e', 1, 10);
    placetetrominoe('f', 5, 10);
    placetetrominoe('g', 1, 15);

    while(1)
    {
        tetrisRenderer();
        tetrisKeyboard();
    }
}

void placetetrominoe(char letter, int x, int y)
{
    for(int h = 0; h < 4; h++)
    {
        for(int w = 0; w < 4; w++)
        {
            switch(letter)
            {
            case 'a':
                tetrisMap[y+h][x+w] = a[y][x];
                break;
            case 'b':
                tetrisMap[y+h][x+w] = b[y][x];
                break;
            case 'c':
                tetrisMap[y+h][x+w] = c[y][x];
                break; 
            case 'd':
                tetrisMap[y+h][x+w] = d[y][x];
                break; 
            case 'e':
                tetrisMap[y+h][x+w] = e[y][x];
                break; 
            case 'f':
                tetrisMap[y+h][x+w] = f[y][x];
                break;
            case 'g':
                tetrisMap[y+h][x+w] = g[y][x];
                break;  
            }
        }
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

    for(int y = 0; y < 20; y++)
        {
            for(int x = 0; x < 20; x+=2)
            {
                tetrisScreen[y+5][x+30][0] = ' ';
                tetrisScreen[y+5][x+31][0] = ' ';
                tetrisScreen[y+5][x+30][1] = tetrisMap[y][x/2];
                tetrisScreen[y+5][x+31][1] = tetrisMap[y][x/2];
            }
        }

        //Renders Screen
        for(int y = 0; y < 25; y++)
        {
            for(int x = 0; x < 80; x++)
            {
                putc(tetrisScreen[y][x][0], x, y, tetrisScreen[y][x][1]);
            }
        }
}

void tetrisLinePutc(char letter, int x, int y, char color)
{
    tetrisScreen[y][x][0] = letter;
    tetrisScreen[y][x][1] = color;
}

void tetrisLinePuts(char *str, int x, int y)
{
    int j = 0;
	int i = x;
	while (str[j] != '\0') {
		tetrisScreen[y][i][0] = str[j];
		tetrisScreen[y][i][1] = (BOARDER_COLOR) | (FG_COLOR & 0x0F);
		++j;
		i++;
	}
}

void tetrisBoardPutc(char color, int x, int y)
{
    tetrisMap[y][x] = color;
}