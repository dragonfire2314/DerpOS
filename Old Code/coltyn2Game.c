#include "system.h"

// Use char 26 for arrow putc(26,4,6,0x0F);
char* C2controls = " E=Render Board   Space=Choose Card   W/S/A/D=Up/Down/Left/Right  C=Check Match ";

void eCursor();
    int exPos = 0;
    int eyPos = 0;

int card1 = -1;
int card2 = -2;


void eRender();

void checkMatch();

char eCard2[4][4] = {
    {' ',' ',' ',' '},
    {' ',' ',' ',' '},
    {' ',' ',' ',' '},
    {' ',' ',' ',' '}
};
char eCard[4][4] = {
    {'1','2','3','4'},
    {'5','6','7','8'},
    {'1','2','3','4'},
    {'5','6','7','8'}
};
int eChecked[4][4] = {
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
};
int checkedCounter = 0;




int boardRendered = 0;



void Coltyn2Main()
{
cls(0x0F);
puts(C2controls, 0,24,0x1F);
    while(boardRendered == 0)
    {
        if(text[0] == 3)
        {
            if(text[1] == 'e')
                {
                    eRender();
                }
        text[0] = 0;
        }
    
    }
    while(1)
    {
        while(1)
        {
        eCursor();
        checkMatch();
        }
        
    }

}

void eRender()
{
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            putc(eCard2[y][x], 31 + (x * 4), 6 + (y * 2), 0xF0);
        }
    }
    boardRendered = 1;
    putc(26,30+(exPos*4),6+(eyPos*2),0x0F);
}


void eCursor()
{
    if(text[0] == 3)
    {
        if(text[1] == 'w' && eyPos > 0)
        {
            putc(26,30+(exPos*4),6+(eyPos*2), 0x00);
            eyPos--;
            putc(26,30+(exPos*4),6+(eyPos*2), 0x0F);
        }
        else if(text[1] == 's' && eyPos < 3)
        {
            putc(26,30+(exPos*4),6+(eyPos*2), 0x00);
            eyPos++;
            putc(26,30+(exPos*4),6+(eyPos*2), 0x0F);
        }
        else if(text[1] == 'a' && exPos > 0)
        {
            putc(26,30+(exPos*4),6+(eyPos*2), 0x00);
            exPos--;
            putc(26,30+(exPos*4),6+(eyPos*2),0x0F);
        }
        else if(text[1] == 'd' && exPos < 3)
        {
            putc(26,30+(exPos*4),6+(eyPos*2),0x00);
            exPos++;
            putc(26,30+(exPos*4),6+(eyPos*2), 0x0F);
        }
        else if(text[1] == ' ')
        {
            if(eChecked[eyPos][exPos] == 0 && checkedCounter < 2)
            {
                putc(eCard[eyPos][exPos], 31+(exPos*4), 6+(eyPos*2), 0x0F);
                eChecked[eyPos][exPos] = 1;
                if(checkedCounter == 0)
                {
                    card1 = eCard[eyPos][exPos];
                }
                else if(checkedCounter == 1)
                {
                    card2 = eCard[eyPos][exPos];
                }
                checkedCounter++;
            }
            else if(eChecked[eyPos][exPos] == 1)
            {
                
                putc(' ', 31+(exPos*4), 6+(eyPos*2), 0xF0);
                eChecked[eyPos][exPos] = 0;
                if(checkedCounter == 2)
                {
                    card2 = -2;
                }
                else if(checkedCounter == 1)
                {
                    card1 = -1;
                }
                checkedCounter--;
            }
        }
    
        text[0] = 0;
    }

}



char* matchL1 = "                         THATS A MATCH!   ";
char* matchL2 = "                       THATS NOT A MATCH! ";

void checkMatch()
{
    if(text[0] == 3)
    {
        if(text[1] == 'c' && checkedCounter == 2)
        {
            if(card1 == card2)
            {
                puts(matchL1, 0, 23, 0x0F);
            }
            else if (card1 != card2)
            {
                puts(matchL2, 0, 23, 0x0F);
            }
            
        }
    }
}