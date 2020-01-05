#include "system.h"

int TTboard[3][3] = 
{
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

char* TTO1 = "------------"; 
char* TTO2 = "------------"; 
char* TTO3 = "------------"; 
char* TTO4 = "------------"; 

char* TTX1 = "------------"; 
char* TTX2 = "------------"; 
char* TTX3 = "------------"; 
char* TTX4 = "------------"; 

char* TTline1 = "|------------| |-------| |------| |------------| |--------| |------|";
char* TTline2 = "|            | |       | |      | |            | |  |  |  | |      |";
char* TTline3 = "|---|    |---|  -|   |-  |   |--| |---|    |---| |  |__|  | |   |--|";
char* TTline4 = "    |    |       |   |   |   |        |    |     |        | |   |   ";
char* TTline5 = "    |    |      -|   |-  |   |--|     |    |     |  |--|  | |   |--|";
char* TTline6 = "    |    |     |       | |      |     |    |     |  |  |  | |      |";
char* TTline7 = "    |----|     |-------| |------|     |----|     |--|  |--| |------|";
char* TTline8 = "-----------------------------Press  Space-----------------------------";
char* TTline9 = "                                 P1/P2                                ";
char* TTline10 ="    |------------------------------------------------------------|";
char* TTline11 ="    |     Q      ------------      W     ------------     E      |";
char* TTline12 ="    |     /      ------------      /     ------------     /      |";
char* TTline13 ="    |     I      ------------      O     ------------     P      |";
char* TTline14 ="    |------------------------------------------------------------|";
char* TTline15 ="    |     A      ------------      S     ------------     D      |";
char* TTline16 ="    |     /      ------------      /     ------------     /      |";
char* TTline17 ="    |     J      ------------      K     ------------     L      |";
char* TTline18 ="    |------------------------------------------------------------|";
char* TTline19 ="    |     Z      ------------      X     ------------     C      |";
char* TTline20 ="    |     /      ------------      /     ------------     /      |";
char* TTline21 ="    |     N      ------------      M     ------------     ,      |";
char* TTline22 ="    |------------------------------------------------------------|";

char* TTgame17 = "                            Tic-Tac                           ";
char* TTgame0 =  "|------------------------------------------------------------|";
char* TTgame1 =  "|            ------------            ------------            |";
char* TTgame2 =  "|            ------------            ------------            |";
char* TTgame3 =  "|            ------------            ------------            |";
char* TTgame4 =  "|            ------------            ------------            |";
char* TTgame5 =  "|------------------------------------------------------------|";
char* TTgame6 =  "|            ------------            ------------            |";
char* TTgame7 =  "|            ------------            ------------            |";
char* TTgame8 =  "|            ------------            ------------            |";
char* TTgame9 =  "|            ------------            ------------            |";
char* TTgame10 = "|------------------------------------------------------------|";
char* TTgame11 = "|            ------------            ------------            |";
char* TTgame12 = "|            ------------            ------------            |";
char* TTgame13 = "|            ------------            ------------            |";
char* TTgame14 = "|            ------------            ------------            |";
char* TTgame15 = "|------------------------------------------------------------|";
char* TTgame16 = "            T = Exit Game          Space = New Game           ";

int player = 0;
int update = 0;

int isRunning = 1;

void TTKeyboard();
void TTclearboard();

void TicTacMain()
{
    isRunning = 1;
    TTclearboard();

    //Test line to see if it works
    cls(0x0F);

    puts(TTline1, 5, 2, 0x02);
    puts(TTline2, 5, 3, 0x02);
    puts(TTline3, 5, 4, 0x02);
    puts(TTline4, 5, 5, 0x02);
    puts(TTline5, 5, 6, 0x02);
    puts(TTline6, 5, 7, 0x02);
    puts(TTline7, 5, 8, 0x02);
    puts(TTline8, 5, 10, 0x02);
    puts(TTline9, 5, 11, 0x02);
    puts(TTline10, 5, 12, 0x02);
    puts(TTline11, 5, 13, 0x02);
    puts(TTline12, 5, 14, 0x02);
    puts(TTline13, 5, 15, 0x02);
    puts(TTline14, 5, 16, 0x02);
    puts(TTline15, 5, 17, 0x02);
    puts(TTline16, 5, 18, 0x02);
    puts(TTline17, 5, 19, 0x02);
    puts(TTline18, 5, 20, 0x02);
    puts(TTline19, 5, 21, 0x02);
    puts(TTline20, 5, 22, 0x02);
    puts(TTline21, 5, 23, 0x02);
    puts(TTline22, 5, 24, 0x02);

    while(1)
    {
        if(text[0] == 3)
        {
            if(text[1] == ' ')
            {
                text[0] = 0; 
                break;
            }
        }
        text[0] = 0;
    }

    cls(0x02);

    puts(TTgame17, 9, 2, 0x04);
    puts(TTgame0, 9, 4, 0x02);
    puts(TTgame1, 9, 5, 0x02);
    puts(TTgame2, 9, 6, 0x02);
    puts(TTgame3, 9, 7, 0x02);
    puts(TTgame4, 9, 8, 0x02);
    puts(TTgame5, 9, 9, 0x02);
    puts(TTgame6, 9, 10, 0x02);
    puts(TTgame7, 9, 11, 0x02);
    puts(TTgame8, 9, 12, 0x02);
    puts(TTgame9, 9, 13, 0x02);
    puts(TTgame10, 9, 14, 0x02);
    puts(TTgame11, 9, 15, 0x02);
    puts(TTgame12, 9, 16, 0x02);
    puts(TTgame13, 9, 17, 0x02);
    puts(TTgame14, 9, 18, 0x02);
    puts(TTgame15, 9, 19, 0x02);
    puts(TTgame16, 9, 21, 0x04);

    while(isRunning)
    {
        TTKeyboard();
        if(update == 1)
        {
            //draw screen
            for(int y = 0; y < 3; y++)
            {
                for(int x = 0; x < 3; x++)
                {
                    if(TTboard[y][x] == 1)
                    {
                        puts(TTX1, 10 + (12 * 2*x), 5 + (5 * y), 0x09);
                        puts(TTX2, 10 + (12 * 2*x), 6 + (5 * y), 0x09);
                        puts(TTX3, 10 + (12 * 2*x), 7 + (5 * y), 0x09);
                        puts(TTX4, 10 + (12 * 2*x), 8 + (5 * y), 0x09);
                    }
                    if(TTboard[y][x] == 2)
                    {
                        puts(TTO1, 10 + (12 * 2*x), 5 + (5 * y), 0x0C);
                        puts(TTO2, 10 + (12 * 2*x), 6 + (5 * y), 0x0C);
                        puts(TTO3, 10 + (12 * 2*x), 7 + (5 * y), 0x0C);
                        puts(TTO4, 10 + (12 * 2*x), 8 + (5 * y), 0x0C);
                    }
                }
            }
        }
    };
}

void TTKeyboard()
{
        //This is when enter is pressed
        if(text[0] == 2)
        {

        }
        //All other keys
        else if(text[0] == 3)
        {
            if(player == 0)
            {
                if(text[1] == 't')
                {
                    isRunning = 0;
                }
                else if(text[1] == 'q')
                {
                    if(TTboard[0][0] == 0)
                    {
                        TTboard[0][0] = 1;
                    }
                }
                else if(text[1] == 'w')
                {
                    if(TTboard[0][1] == 0)
                    {
                        TTboard[0][1] = 1;
                    }
                }
                else if(text[1] == 'e')
                {
                    if(TTboard[0][2] == 0)
                    {
                        TTboard[0][2] = 1;
                    }
                }
                else if(text[1] == 'a')
                {
                    if(TTboard[1][0] == 0)
                    {
                        TTboard[1][0] = 1;
                    }
                }
                else if(text[1] == 's')
                {
                    if(TTboard[1][1] == 0)
                    {
                        TTboard[1][1] = 1;
                    }
                }
                else if(text[1] == 'd')
                {
                    if(TTboard[1][2] == 0)
                    {
                        TTboard[1][2] = 1;
                    }
                }
                else if(text[1] == 'z')
                {
                    if(TTboard[2][0] == 0)
                    {
                        TTboard[2][0] = 1;
                    }
                }
                else if(text[1] == 'x')
                {
                    if(TTboard[2][1] == 0)
                    {
                        TTboard[2][1] = 1;
                    }
                }
                else if(text[1] == 'c')
                {
                    if(TTboard[2][2] == 0)
                    {
                        TTboard[2][2] = 1;
                    }
                }
                else if(text[1] == ' ')
                {
                    TTclearboard();
                }
                update = 1;
            }
            if(player == 0)
            {
                if(text[1] == 'i')
                {
                    if(TTboard[0][0] == 0)
                    {
                        TTboard[0][0] = 2;
                    }
                }
                else if(text[1] == 'o')
                {
                    if(TTboard[0][1] == 0)
                    {
                        TTboard[0][1] = 2;
                    }
                }
                else if(text[1] == 'p')
                {
                    if(TTboard[0][2] == 0)
                    {
                        TTboard[0][2] = 2;
                    }
                }
                else if(text[1] == 'j')
                {
                    if(TTboard[1][0] == 0)
                    {
                        TTboard[1][0] = 2;
                    }
                }
                else if(text[1] == 'k')
                {
                    if(TTboard[1][1] == 0)
                    {
                        TTboard[1][1] = 2;
                    }
                }
                else if(text[1] == 'l')
                {
                    if(TTboard[1][2] == 0)
                    {
                        TTboard[1][2] = 2;
                    }
                }
                else if(text[1] == 'n')
                {
                    if(TTboard[2][0] == 0)
                    {
                        TTboard[2][0] = 2;
                    }
                }
                else if(text[1] == 'm')
                {
                    if(TTboard[2][1] == 0)
                    {
                        TTboard[2][1] = 2;
                    }
                }
                else if(text[1] == ',')
                {
                    if(TTboard[2][2] == 0)
                    {
                        TTboard[2][2] = 2;
                    }
                }
                update = 1;
            }
        }
        text[0] = 0;
}

void TTclearboard()
{
    for(int y = 0; y < 3; y++)
                    {
                        for(int x = 0; x < 3; x++)
                        {
                            TTboard[y][x] = 0;
                                if(TTboard[y][x] == 0)
                                {
                                    puts(TTX1, 10 + (12 * 2*x), 5 + (5 * y), 0x00);
                                    puts(TTX2, 10 + (12 * 2*x), 6 + (5 * y), 0x00);
                                    puts(TTX3, 10 + (12 * 2*x), 7 + (5 * y), 0x00);
                                    puts(TTX4, 10 + (12 * 2*x), 8 + (5 * y), 0x00);
                                }
                        }
                    }
}