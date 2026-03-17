void desktop_init();
void desktop_DrawScreen();
void desktop_Update();
int desktop_CreateWindow(int x, int y, int height, int width, unsigned int* windowMem, 
    char scale, char render, char stretch);
void desktop_SetWindowUpdate(int windowID);