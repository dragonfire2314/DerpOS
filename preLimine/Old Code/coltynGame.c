#include "system.h"

/*
        AVALIBLE FUNCTIONS
----------------------------------------------------------------------
void putc(char letter, int x, int y, unsigned short color);
void puts(char *str, int x, int y, unsigned short color);
void cls(unsigned short color);
unsigned short MakeColor(unsigned char bg, unsigned char fg);

void boarder(char *string, int length);

        KEYBOARD
----------------------------------------------------------------------
For keyboard input
    //This is when enter is pressed
    if(text[0] == 2){}


    //All other keys
    else if(text[0] == 3)
    {
        Keyboard Input is in here  ----->  text[1]
    }
    text[0] = 0;

Set text[0] = 0 after the keypress has been accepted
*/

/*
        TO RUN THE PROGRAM
----------------------------------------------------------------------
        1.Compile.bat
        2.Run.bat
        3.Enter "run coltyn" in the command line
        4.Profit
*/
char* bJpLine1 = " Score:                     Player Hand                                         ";
char* bJpLine2 = "--------------------------------------------------------------------------------";
char* bJpLine3 = "                                                                                ";
char* bJpLine4 = "                                                                                ";

char* bJdLine1 = " Score:                     Dealer Hand                                         ";
char* bJdLine2 = "--------------------------------------------------------------------------------";
char* bJdLine3 = "                                                                                ";
char* bJdLine4 = "                                                                                ";

char* dbust = "DEALER BUSTS, PLAYER WINS";
char* pbust = "PLAYER BUSTS, DEALER WINS";

void bJKeyboard();
void bJdeal();

void bJPlayerCard();
void bJDealerCard();

void dealerAi();

char CintToChar();
char* intToString();

int bJPcards[6];
char bJPsym[6];
int bJPoffset = 0;
int bJPcardCount = 0;
int bJPscore = 0;
int pStay = 0;
int pbustb = 0;

void Pscore();
void renderP();

int bJDcards[6];
char bJDsym[6];
int bJDoffset = 0;
int bJDcardCount = 0;
int bJDscore = 0;
int dStay = 0;
int dbustb = 0;

void Dscore();
void renderD();

char bJsym[56] = {0x03,0x04,0x06,0x04,0x05,0x03,0x06,0x04,0x05,0x04,0x06,0x03,0x03,0x06,0x04,0x06,0x03,0x04,0x03,0x04,0x06,0x04,0x06,0x04,0x04,0x06,0x05,0x03,0x06,0x05,0x06,0x04,0x03,0x05,0x03,0x06,0x05,0x04,0x05,0x03,0x06,0x06,0x05,0x04,0x06,0x03,0x04,0x04,0x06,0x05,0x03,0x06,0x03,0x04,0x06,0x03};
int bJCard[56] = {0,1,3,2,0,12,7,5,1,2,4,8,2,4,1,6,12,10,3,9,7,8,11,10,10,9,0,12,11,5,8,2,12,3,4,11,6,2,8,1,5,7,6,4,7,9,4,10,0,9,3,11,5,1,8,6};
int bJcardCount = 0;

char* bJMenu = "        H=Hit Card          S=Stand           E=Exit          N=New Game        ";

char* bJt1 = " BLACK JACK ";
char* bJt2 = "            ";
char* bJt3 = "  P = Play  ";
char* bJt4 = "  E = Exit  ";

void ColtynMain()
{
    //Test line to see if it works
    cls(0x19);

    while(1)
    {
            puts(bJt1, 35, 8, 0x1C);
            puts(bJt2, 35, 9, 0x1C);
            puts(bJt3, 35, 10, 0x1C);
            puts(bJt4, 35, 11, 0x1C);

        if(text[0] == 3)
        {
                if (text[1] == 'p')
                {
                        text[0] = 0;
                       break; 
                }
                if (text[1] == 'e')
                {
                       text[0] = 0;
                        return;
                }
        }
        text[0] = 0;
    }
    cls(0x20);
        puts(bJdLine1, 0, 2, 0x20);
        puts(bJdLine2, 0, 3, 0x20);

        puts(bJpLine1, 0, 21, 0x20);
        puts(bJpLine2, 0, 22, 0x20);
        bJdeal();
    while(1)
    {
        bJKeyboard();
        puts(bJMenu, 0, 24, 0x0F);
        if(bJcardCount > 50)
        {
                bJcardCount = 0;
        }
        while(pStay == 1 && dStay == 0)
        {
                dealerAi();
                if(bJDscore > 21)
                {
                        puts(dbust, 30, 12, 0x20);
                        dStay = 1;
                        bJPscore = 0;
                        bJDscore = 0;
                        dbustb = 1;
                }
        }
        if(pStay == 1 && dStay ==1 && dbustb == 0 && pbustb ==0)
        {
                if(bJPscore > bJDscore)
                {
                        char* pwin = "PLAYER WINS";
                        puts(pwin, 33, 12, 0x20);
                }
                else 
                {
                        char* dwin = "DEALER WINS";
                        puts(dwin, 33, 12, 0x20);
                }
        }
    };
}

void bJKeyboard()
{
        //This is when enter is pressed
        if(text[0] == 2)
        {
                
        }
        //All other keys
        else if(text[0] == 3)
        {
                if(text[1] == 'w')
                {
                        putc(0x03, 5, 5, 0x0F);
                        putc(0x04, 6, 5, 0x0F);  
                        putc(0x05, 7, 5, 0x0F);
                        putc(0x06, 8, 5, 0x0F);
                }
                else if(text[1] == 'h')
                {
                        bJPlayerCard();
                        if(bJPscore > 21)
                        {
                                puts(pbust, 30, 12, 0x20);
                                bJDscore = 0;
                                bJPscore = 0;
                                pbustb = 1;
                        }
                }
                if (text[1] == 'e')
                {
                       text[0] = 0;
                        return;
                }
                if(text[1] == 's')
                {
                        pStay = 1;
                }
                if(text[1] == 'n')
                {
                        for(int f = 0; f < bJPcardCount; f++)
                        {
                                bJPcards[f] = 0;
                                bJPsym[f] = 0;
                        }
                        for (int g = 0; g < bJDcardCount; g++)
                        {
                                bJDcards[g] = 0;
                                bJDsym[g] = 0;
                        }

                                bJPoffset = 0;
                                bJPcardCount = 0;
                                bJPscore = 0;
                                pStay = 0;
                                pbustb = 0;

                                bJDoffset = 0;
                                bJDcardCount = 0;
                                bJDscore = 0;
                                dStay = 0;
                                dbustb = 0;

                                cls(0x20);
                                puts(bJdLine1, 0, 2, 0x20);
                                puts(bJdLine2, 0, 3, 0x20);

                                puts(bJpLine1, 0, 21, 0x20);
                                puts(bJpLine2, 0, 22, 0x20);
                                bJdeal();

                }
        }
        text[0] = 0;
}

void bJPlayerCard()
{
        bJPcards[bJPcardCount] = bJCard[bJcardCount];
        bJPsym[bJPcardCount] = bJsym[bJcardCount];
        bJcardCount++;
        bJPcardCount++;
        bJPoffset++;
        renderP();
}

void bJDealerCard()
{
        bJDcards[bJDcardCount] = bJCard[bJcardCount];
        bJDsym[bJDcardCount] = bJsym[bJcardCount];
        bJDcardCount++;
        bJcardCount++;
        bJDoffset++;
        renderD();
}

void bJdeal()
{
        bJPlayerCard();
        
        bJDealerCard();

        bJPlayerCard();

        bJDealerCard();
        
}

void renderP()
{
        for (int j = 0; j < bJPcardCount; j++)
        {
                if(bJPsym[j] == 0x03 || bJPsym[j] == 0x04)
                        {
                                
                                putc(bJPsym[j], 16 + (bJPoffset * 3), 23, 0xFC);
                                putc(CintToChar(bJPcards[j]), 17 + (bJPoffset * 3), 23, 0xFC);
                                
                        }
                else if(bJPsym[j] == 0x05 || bJPsym[j] == 0x06)
                        {
                                putc(bJPsym[j], 16 + (bJPoffset * 3), 23, 0xF0);
                                putc(CintToChar(bJPcards[j]), 17 + (bJPoffset * 3), 23, 0xF0);
                        }

        }
        Pscore();
        puts(intToString(bJPscore), 1, 23, 0x20);
}

void renderD()
{
        for (int k = 0; k < bJDcardCount; k++)
                {
                        if(bJDsym[k] == 0x03 || bJDsym[k] == 0x04)
                                {
                                        
                                        putc(bJDsym[k], 16 + (bJDoffset * 3), 4, 0xFC);
                                        putc(CintToChar(bJDcards[k]), 17 + (bJDoffset * 3), 4, 0xFC);
                                        
                                }
                        else if(bJDsym[k] == 0x05 || bJDsym[k] == 0x06)
                                {
                                        putc(bJDsym[k], 16 + (bJDoffset * 3), 4, 0xF0);
                                        putc(CintToChar(bJDcards[k]), 17 + (bJDoffset * 3), 4, 0xF0);
                                }

                }
        Dscore();
        puts(intToString(bJDscore), 1, 4, 0x20);
}

void dealerAi()
{
        if(bJDscore >= 16)
        {
                dStay = 1;
        }
        else if(bJDscore < 16)
        {
                bJDealerCard();
        }
}

char* intToString(int b)
{
        switch(b)
        {
                case 0:
                        return "0";
                        break;
                case 1:
                        return "1";
                        break;
                case 2:
                        return "2";
                        break;
                case 3:
                        return "3";
                        break;
                case 4:
                        return "4";
                        break;
                case 5:
                        return "5";
                        break;
                case 6:
                        return "6";
                        break;
                case 7:
                        return "7";
                        break;
                case 8:
                        return "8";
                        break;
                case 9:
                        return "9";
                        break;
                case 10:
                        return "10";
                        break;
                case 11:
                        return "11";
                        break;
                case 12:
                        return "12";
                        break;
                case 13:
                        return "13";
                        break;
                case 14:
                        return "14";
                        break;
                case 15:
                        return "15";
                        break;
                case 16:
                        return "16";
                        break;
                case 17:
                        return "17";
                        break;
                case 18:
                        return "18";
                        break;
                case 19:
                        return "19";
                        break;
                case 20:
                        return "20";
                        break;
                case 21:
                        return "21";
                        break;
                case 22:
                        return "22";
                        break;
                case 23:
                        return "23";
                        break;
                case 24:
                        return "24";
                        break;
                case 25:
                        return "25";
                        break;
                case 26:
                        return "26";
                        break;
                case 27:
                        return "27";
                        break;
                case 28:
                        return "28";
                        break;
                case 29:
                        return "29";
                        break;
                case 30:
                        return "30";
                        break;
                default:
                        return "E";
                        break;
                
        }
}

char CintToChar(int a)
{
        switch(a)
        {
                case 0:
                        return 'A';
                        break;
                case 1:
                        return '2';
                        break;
                case 2:
                        return '3';
                        break;
                case 3:
                        return '4';
                        break;
                case 4:
                        return '5';
                        break;
                case 5:
                        return '6';
                        break;
                case 6:
                        return '7';
                        break;
                case 7:
                        return '8';
                        break;
                case 8:
                        return '9';
                        break;
                case 9:
                        return '0';
                        break;
                case 10:
                        return 'J';
                        break;
                case 11:
                        return 'Q';
                        break;
                case 12:
                        return 'K';
                        break;
                default:
                        return 'e';
                        break;
        }
}

void Pscore()
{
        bJPscore = 0;
        for(int i = 0; i < bJPcardCount; i++)
        {
                if(bJPcards[i] == 1)
                {
                        bJPscore += 2;
                }
                else if(bJPcards[i] == 2)
                {
                        bJPscore += 3;
                }
                else if(bJPcards[i] == 3)
                {
                        bJPscore += 4;
                }
                else if(bJPcards[i] == 4)
                {
                        bJPscore += 5;
                }
                else if(bJPcards[i] == 5)
                {
                        bJPscore += 6;
                }
                else if(bJPcards[i] == 6)
                {
                        bJPscore += 7;
                }
                else if(bJPcards[i] == 7)
                {
                        bJPscore += 8;
                }
                else if(bJPcards[i] == 8)
                {
                        bJPscore += 9;
                }
                else if(bJPcards[i] == 9)
                {
                        bJPscore += 10;
                }
                else if(bJPcards[i] == 10)
                {
                        bJPscore += 10;
                }
                else if(bJPcards[i] == 11)
                {
                        bJPscore += 10;
                }
                else if(bJPcards[i] == 12)
                {
                        bJPscore += 10;
                }
                else if(bJPcards[i] == 0)
                {
                       if(bJPscore > 10)
                       { 
                               bJPscore += 1;
                       }
                       else if(bJPscore <= 10)
                       {
                               bJPscore += 11;
                       }
                }
        }
}

void Dscore()
{
        bJDscore = 0;
        for(int i = 0; i < bJDcardCount; i++)
        {
                if(bJDcards[i] == 1)
                {
                        bJDscore += 2;
                }
                else if(bJDcards[i] == 2)
                {
                        bJDscore += 3;
                }
                else if(bJDcards[i] == 3)
                {
                        bJDscore += 4;
                }
                else if(bJDcards[i] == 4)
                {
                        bJDscore += 5;
                }
                else if(bJDcards[i] == 5)
                {
                        bJDscore += 6;
                }
                else if(bJDcards[i] == 6)
                {
                        bJDscore += 7;
                }
                else if(bJDcards[i] == 7)
                {
                        bJDscore += 8;
                }
                else if(bJDcards[i] == 8)
                {
                        bJDscore += 9;
                }
                else if(bJDcards[i] == 9)
                {
                        bJDscore += 10;
                }
                else if(bJDcards[i] == 10)
                {
                        bJDscore += 10;
                }
                else if(bJDcards[i] == 11)
                {
                        bJDscore += 10;
                }
                else if(bJDcards[i] == 12)
                {
                        bJDscore += 10;
                }
                else if(bJDcards[i] == 0)
                {
                       if(bJDscore > 10)
                       { 
                               bJDscore += 1;
                       }
                       else if(bJDscore <= 10)
                       {
                               bJDscore += 11;
                       }
                }
        }
}