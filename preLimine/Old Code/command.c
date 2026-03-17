#include "system.h"
#include "ascii.h"

void rect(int x, int y, int width, int height);
void arrow(int x, int y);
void GetKeySettings();
void SettingsLinePutc(char letter, int x, int y);
void SettingsLinePuts(char *str, int x, int y);
int ColorPicker();

char BOARDER_COLOR = 0x90;
char BG_COLOR = 0x00;
char FG_COLOR = 0x0F;

char *start = (char*)0x0;

char *h = "help";
char *t = "test";
char *Help = "Help Menu - v1.0";

char *HEX_EDITOR = "Hex Editor -- v1.0";
char *HEX_EDITOR_TOP_BAR = "         00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F";
char *loc = "loc = 0x";

char listFunc[4][32] = {
    //Command 0
    {'h','e','l','p',' ','[','c','o','m','m','a','n','d',']', 0x30},
    //Command 1
    {'h','e','x',' ','[','l','o','c','a','t','i','o','n',']', 0x30},
    {'r','u','n',' ','[','p','r','o','g','r','a','m',']', 0x30},
    {'s','e','t','t','i','n','g','s',0x30}
};

char listProg[4][32] = {
    //Command 0
    {'b','l','a','c','k','j','a','c','k',0x30},
    {'t','i','c','t','a','c',0x30}
};

extern unsigned char ascii[128];
extern int test(void);

unsigned char hexPos[8];
int posCounter = 0;
int x = 8;

void help()
{
    for(int c = 0; c < commandCount; c++)
    {
        for(int i = 0; i < 256; i++)
        {
            if(listFunc[c][i] == 0x30){break;}
            //putc(list[c][i], i+2, c+1, 0x0F);
            CommandLinePutc(listFunc[c][i], i+5, c+1+cursorY);
        }
    }
    cursorY += commandCount;
}

void hex(char *addr)
{
    for(int i = 0; i < 8; i++)
    {
        CommandLinePutc(addr[i], i+5, 1+cursorY);
    }
    cursorY += 1;
}

void run(char *addr)
{
    for(int c = 0; c < commandCount; c++)
    {
        for(int i = 0; i < 256; i++)
        {
            if(listProg[c][i] == 0x30)
            {
                switch(c)
                {
                case 0:
                    Coltyn2Main();
                    break;
                case 1:
                    TicTacMain();
                    break;
                }
            }
            if(addr[i] != listProg[c][i])
            {
                break;
            }
        }
    }
}

void loadHex()
{
    
    
    /*double number;
    int counter = 0;
    for(int i = 8; i > 0; i--)
    {
        number += hexPos[i] * pow(16, counter);
        counter++;
    }
    puts(HEX_EDITOR_TOP_BAR, 1, 1, 0x0F);
    for(int i = 0; i < 4; i++)
    {
        putc(hexPos[i], 1, 1, 0x0F);
    }*/
}

int pow(int base, int power)
{
    double number2 = 1;
    for(int i = 1; i <= power; i++)
    {
        number2 = base * number2;
    }
    return number2;
}

char SettingsWindow[23][78][2];

void rect(int x, int y, int width, int height)
{
    for(int i = 0; i < height; i++)
    {
        for(int h = 0; h < width; h++)
        {
            SettingsWindow[y+i][h+x][0] = ' ';
            SettingsWindow[y+i][h+x][1] = BOARDER_COLOR;
            //putc(' ', x + h, y + i, 0xD0);
        }
    }
}

void rectC(int x, int y, int width, int height, char color)
{
    for(int i = 0; i < height; i++)
    {
        for(int h = 0; h < width; h++)
        {
            SettingsWindow[y+i][h+x][0] = ' ';
            SettingsWindow[y+i][h+x][1] = color;
            //putc(' ', x + h, y + i, 0xD0);
        }
    }
}

void rectNom(int x, int y, int width, int height)
{
    for(int i = 0; i < height; i++)
    {
        for(int h = 0; h < width; h++)
        {
            putc(' ', x + h, y + i, 0x30);
        }
    }
}

int arrowLocX = 0;
int arrowLocY = 0;

char ColorNameList[16][13] = {
    {'B','l','a','c','k',' ',' ',' ',' ',' ',' ',' ',0x30},
    {'B','l','u','e',' ',' ',' ',' ',' ',' ',' ',' ',0x30},
    {'G','r','e','e','n',' ',' ',' ',' ',' ',' ',' ',0x30},
    {'C','y','a','n',' ',' ',' ',' ',' ',' ',' ',' ',0x30},
    {'R','e','d',' ',' ',' ',' ',' ',' ',' ',' ',' ',0x30},
    {'M','a','g','e','n','t','a',' ',' ',' ',' ',' ',0x30},
    {'B','r','o','w','n',' ',' ',' ',' ',' ',' ',' ',0x30},
    {'L',' ','G','r','a','y',' ',' ',' ',' ',' ',' ',0x30},
    {'D',' ','G','r','a','y',' ',' ',' ',' ',' ',' ',0x30},
    {'L',' ','B','l','u','e',' ',' ',' ',' ',' ',' ',0x30},
    {'L',' ','G','r','e','e','n',' ',' ',' ',' ',' ',0x30},
    {'L',' ','C','y','a','n',' ',' ',' ',' ',' ',' ',0x30},
    {'L',' ','R','e','d',' ',' ',' ',' ',' ',' ',' ',0x30},
    {'L',' ','M','a','g','e','n','t','a',' ',' ',' ',0x30},
    {'Y','e','l','l','o','w',' ',' ',' ',' ',' ',' ',0x30},
    {'W','h','i','t','e',' ',' ',' ',' ',' ',' ',' ',0x30}
};

char *BG_NAME = "5555555555555";
int bg_selction = 0;
char *FG_NAME = "4444444444444";
int fg_selction = 15;
char *BD_NAME = "3333333333333";
int bd_selction = 9;

char Quit = 0;

void settings()
{
    arrowLocX = 0;
    arrowLocY = 0;
    Quit = 0;
    
    for(int i = 0; i < 13; i++)
    {
        BG_NAME[i] = ' ';
        FG_NAME[i] = ' ';
        BD_NAME[i] = ' ';
    }
    for(int i = 0; i < 13; i++)
    {
        if(ColorNameList[bg_selction][i] == 0x30) break;
        BG_NAME[i] = ColorNameList[bg_selction][i];
    }
    for(int i = 0; i < 13; i++)
    {
        if(ColorNameList[fg_selction][i] == 0x30) break;
        FG_NAME[i] = ColorNameList[fg_selction][i];
    }
    for(int i = 0; i < 13; i++)
    {
        if(ColorNameList[bd_selction][i] == 0x30) break;
        BD_NAME[i] = ColorNameList[bd_selction][i];
    }

    cls(BG_COLOR);
    boarder("Settings", 8);

    while(Quit == 0)
    {
        rectC(0, 0, 78, 23, BG_COLOR);

        rect(4, 3, 32, 7);
        rect(4, 14, 32, 7);
        rect(42, 3, 32, 7);
        rect(42, 14, 32, 7);

        GetKeySettings();

        arrow(5 + (arrowLocX * 38), 4 + (arrowLocY * 11));

        SettingsLinePuts("Backgroung Color", 14, 4);
        SettingsLinePuts("Foreground Color", 14, 15);
        SettingsLinePuts("Boarder Color", 52, 4);
        SettingsLinePuts("Exit", 52, 15);

        SettingsLinePuts(BG_NAME, 14, 7);
        SettingsLinePuts(FG_NAME, 14, 18);
        SettingsLinePuts(BD_NAME, 52, 7);

        for(int y = 0; y < 23; y++)
        {
            for(int x = 0; x < 78; x++)
            {
                putc(SettingsWindow[y][x][0], x+1, y+1, SettingsWindow[y][x][1]);
            }
        }
    };
}

void GetKeySettings()
{
    //Enter
    if(text[0] == 2)
    {
        //Determin Button

        //top left
        if(arrowLocX == 0 && arrowLocY == 0)
        {
            text[0] = 0;
            int Sel = ColorPicker();
            for(int i = 0; i < 13; i++)
            {
                if(ColorNameList[Sel][i] == 0x30) break;
                BG_NAME[i] = ColorNameList[Sel][i];
            }
            BG_COLOR = (char)Sel << 4;
            bg_selction = Sel;
        }
        //bottom left
        else if(arrowLocX == 0 && arrowLocY == 1)
        {
            text[0] = 0;
            int Sel = ColorPicker();
            for(int i = 0; i < 13; i++)
            {
                if(ColorNameList[Sel][i] == 0x30) break;
                FG_NAME[i] = ColorNameList[Sel][i];
            }
            FG_COLOR = (char)Sel;
            fg_selction = Sel;
        }
        //top right
        else if(arrowLocX == 1 && arrowLocY == 0)
        {
            text[0] = 0;
            int Sel = ColorPicker();
            for(int i = 0; i < 13; i++)
            {
                if(ColorNameList[Sel][i] == 0x30) break;
                BD_NAME[i] = ColorNameList[Sel][i];
            }
            BOARDER_COLOR = (char)Sel << 4;
            bd_selction = Sel;
        }
        //bottom right
        else if(arrowLocX == 1 && arrowLocY == 1)
        {
            Quit = 1;
            return;
        }
    }
    //normal
    else if(text[0] == 3)
    {
        //Backspace
        if(text[1] == 0x08)
        {
            
        }
        //Do nothing
        else if(text[1] == 12 || text[1] == 11){}
        //Normal
        else{
            if(text[1] == 'w' && arrowLocY == 1)
            {
                arrowLocY = 0;
            }
            if(text[1] == 's' && arrowLocY == 0)
            {
                arrowLocY = 1;
            }
            if(text[1] == 'a' && arrowLocX == 1)
            {
                arrowLocX = 0;
            }
            if(text[1] == 'd' && arrowLocX == 0)
            {
                arrowLocX = 1;
            }
        }
    }
    text[0] = 0;
}

int ColorPicker()
{
    rectNom(20, 3, 40, 20);
    puts("Color Picker", 34, 3, 0x3F);

    int Selection = 0;

    while(1)
    {
        //Draws list
        for(int y = 0; y < 16; y++)
        {
            for(int x = 0; x < 13; x++)
            {   
                if(ColorNameList[y][x] == 0x30){break;}
                if(Selection == y) { putc(ColorNameList[y][x], 36+x, 5+y, 0x0F); }
                else { putc(ColorNameList[y][x], 36+x, 5+y, 0x3F); }
            }
        }
        //Selector
        if(text[0] == 2)
        {
            return Selection;
        }
        else if(text[0] == 3)
        {
            if(text[1] == 12 || text[1] == 11){}
            else{
                if(text[1] == 'w' && Selection <= 15)
                {
                    Selection--;
                }
                if(text[1] == 's' && Selection >= 0)
                {
                    Selection++;
                }
                if(Selection > 15) Selection = 15;
                if(Selection < 0) Selection = 0;
            }
        }
        text[0] = 0;
    }
}

char arrowMap[5][6] = {
    {0xDB,0xDB,' ',' ',' ',' '},
    {0xDB,0xDB,0xDB,0xDB,' ',' '},
    {0xDB,0xDB,0xDB,0xDB,0xDB,0xDB},
    {0xDB,0xDB,0xDB,0xDB,' ',' '},
    {0xDB,0xDB,' ',' ',' ',' '}
};

void arrow(int x, int y)
{
    for(int i = 0; i < 5; i++)
    {
        for(int h = 0; h < 6; h++)
        {
            SettingsWindow[y+i][h+x][0] = arrowMap[i][h];
            SettingsWindow[y+i][h+x][1] = 0x0F;
            if(arrowMap[i][h] == ' ') SettingsWindow[y+i][h+x][1] = BOARDER_COLOR;
        }
    }
}

void SettingsLinePutc(char letter, int x, int y)
{
    SettingsWindow[y][x][0] = letter;
}

void SettingsLinePuts(char *str, int x, int y)
{
    int j = 0;
	int i = x;
	while (str[j] != '\0') {
		SettingsWindow[y][i][0] = str[j];
		SettingsWindow[y][i][1] = (BOARDER_COLOR) | (FG_COLOR & 0x0F);
		++j;
		i++;
	}
}