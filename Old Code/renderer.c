#include "system.h"

#define CURSORX_HOME 2

extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);

//Title Text
char *str = "DerpOS Version 0.28--Ready";
char *bot = "                                                                                ";
char *top = "                           ";

unsigned char adddr[8];

unsigned char num[10] = 
{
    '0','1','2','3','4','5','6','7','8','9'
};

int cursorX = CURSORX_HOME;
int cursorY = 1;

char *text = (char*)0x7E00;
unsigned char command[256];
int commandCounter = 0; 

int commandCount = 4;
char list[4][16] = {
    //Command 0
    {'h','e','l','p', 0x30},
    //Command 1
    {'h','e','x', 0x30},
    {'r','u','n', 0x30},
    {'s','e','t','t','i','n','g','s', 0x30}
};

char CommandLine[256][78];
int ListStart = 0;

//Functions
void keyCheck();
int commandCheck();
void Seconds();

void RenderScreenKernel()
{

    boarder(str, 26);

    //Cursor
    CommandLine[cursorY][1] = '>';

    //Renders Screen
    for(int y = ListStart; y < 23 + ListStart; y++)
    {
        for(int x = 0; x < 78; x++)
        {
            putc(CommandLine[y][x], x + 1, y + 1 - ListStart, (BG_COLOR) | (FG_COLOR & 0x0F));
        }
    }

    //Key Check and place
    keyCheck();

    //Seconds
    Seconds();    

    //Needs to be linked to the array that represents the screen otherwise it disapears...
    //Move the cursor far away from the screen for now
    cursormove(cursorY+1, cursorX+1);
}

void Seconds()
{
    write_port(0x70, (read_port(0x70)&0x7F << 7) | (0x00));
    int test = read_port(0x71);

    CommandLinePutc(num[test % 16], 77, 0+ListStart);
    CommandLinePutc(num[test / 16], 76, 0+ListStart);

    write_port(0x70, (read_port(0x70)&0x7F << 7) | (0x02));
    int test2 = read_port(0x71);

    CommandLinePutc(':', 75, 0+ListStart);
    CommandLinePutc(num[test2 % 16], 74, 0+ListStart);
    CommandLinePutc(num[test2 / 16], 73, 0+ListStart);

    write_port(0x70, (read_port(0x70)&0x7F << 7) | (0x04));
    int test3 = read_port(0x71);

    CommandLinePutc(':', 72, 0+ListStart);
    CommandLinePutc(num[test3 % 16], 71, 0+ListStart);
    CommandLinePutc(num[test3 / 16], 70, 0+ListStart);
}

/*
write_port(0x70, (read_port(0x70)&0x7F << 7) | (0x00));
    char seconds = read_port(0x71);
    write_port(0x70, (read_port(0x70)&0x7F << 7) | (0x02));
    char minutes = read_port(0x71);
    write_port(0x70, (read_port(0x70)&0x7F << 7) | (0x04));
    char hours = read_port(0x71);

    putc(num[hours % 16], 72, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    putc(num[hours / 16], 73, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    putc(':', 74, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    putc(num[minutes % 16], 75, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    putc(num[minutes / 16], 76, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    putc(':', 77, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    putc(num[seconds % 16], 78, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    putc(num[seconds / 16], 79, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
*/

void cursormove(int row, int col)
{
    if(cursorY > 20)
    {
        row = 21;
    }
    unsigned short location=(row*80) + col;/* Short is a 16bit type , the formula is used here*/

    //Cursor Low port
    write_port(0x3D4, 0x0F);//Sending the cursor low byte to the VGA Controller
    write_port(0x3D5, (unsigned char)(location&0xFF));

    //Cursor High port
    write_port(0x3D4, 0x0E);//Sending the cursor high byte to the VGA Controller
    write_port(0x3D5, (unsigned char )((location>>8)&0xFF)); //Char is a 8bit type
}

void keyCheck()
{
    //Enter
    if(text[0] == 2)
    {
        text[0] = 0;
        cursorX = CURSORX_HOME;
        
        commandCheck();
        //Scrolls Screen
        thing();
    }
    //normal
    else if(text[0] == 3)
    {
        //Backspace
        if(text[1] == 0x08)
        {
            cursorX--;
            //putc(' ', cursorX, cursorY, 0x0F);
            CommandLine[cursorY][cursorX] = ' ';
            commandCounter--;
            command[commandCounter] = 0;
        }
        //Do nothing
        else if(text[1] == 12 || text[1] == 11){}
        //Tab
        else if(text[1] == 0x09){ if (ListStart < 256)ListStart++; }
        else if(text[1] == 0x60){ if (ListStart > 0)ListStart--; }
        //Normal
        else {
            //Get letter into command
            command[commandCounter] = text[1];
            commandCounter++;
            //Place letter on screen
            CommandLine[cursorY][cursorX] = text[1];
            //putc(text[1], cursorX, cursorY, 0x0F);
            cursorX++;
        }
    }
    text[0] = 0;
}

void thing()
{
    if(cursorY > 20){ ListStart = cursorY - 20; }
    for(int i = 0; i < 256; i++)
    {
        command[i] = 0;
    }
    commandCounter = 0;
}

int commandCheck()
{
    for(int c = 0; c < commandCount; c++)
    {
        for(int i = 0; i < 256; i++)
        {
            if(list[c][i] == 0x30)
            {
                switch(c)
                {
                //help
                case 0:
                    help();
                    cursorY++;
                    break;
                //hex
                case 1:
                    for(int k = 0; k < 8; k++)
                    {
                        adddr[k] = command[k+i+1];
                    }
                    cursormove(cursorY+100, cursorX+100);
                    cursorY++;
                    hex(adddr);
                    
                    break;
                //run
                case 2:
                    for(int k = 0; k < 32; k++)
                    {
                        adddr[k] = command[k+i+1];
                    }
                    cursormove(cursorY+100, cursorX+100);
                    cursorY++;
                    run(adddr);
                    break;
                case 3:
                    cursormove(cursorY+100, cursorX+100);
                    cursorY++;
                    settings();
                    break;
                }
            }
            if(command[i] != list[c][i])
            {
                break;
            }
        }
    }
}

void CommandLinePutc(char str, int x, int y)
{
    CommandLine[y][x] = str;
}

void boarder(char *string, int length)
{
    int pos = (80 - length) / 2;
    //Fill Top with logo
    puts(string, pos, 0, (BOARDER_COLOR << 4) | (FG_COLOR & 0x0F));
    //Boarder Stuff
    puts(bot, 0, 24, BOARDER_COLOR);
    for(int i = 0; i < pos; i++)
    {
        putc(' ', i, 0, BOARDER_COLOR);
        putc(' ', (pos + length) + i, 0, BOARDER_COLOR);
    }
    for(int i = 0; i < 25; i++)
    {
        putc(' ', 0, i, BOARDER_COLOR);
        putc(' ', 79, i, BOARDER_COLOR);
    }
}
