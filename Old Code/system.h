//command.c
void help();
void hex(char *addr);
void loadHex();
int pow(int base, int power);
void run(char *addr);
void settings();

extern char BG_COLOR;
extern char FG_COLOR;
extern char BOARDER_COLOR;

//kernel.asm
//extern short read_word(unsigned short port);

//kernel.c
void kmain(void);

//gdt.c
void gdt_init();

//Screen.c
extern char *screen;
void putc(char letter, int x, int y, unsigned short color);
void puts(char *str, int x, int y, unsigned short color);
void putd(char data, char x, char y, unsigned short color);
void cls(unsigned short color);
unsigned short MakeColor(unsigned char bg, unsigned char fg);

//Renderer.c
void RenderScreenKernel();
void keyCheck();
int commandCheck();
void boarder(char *string, int length);
void CommandLinePutc(char str, int x, int y);
void cursormove(int row, int col);
void thing();

extern char *text;
extern char list[4][16];
extern int cursorX;
extern int cursorY;
extern int commandCount;

//io.c
char* textBankAdress();

void idt_init(void);
void kb_init(void);
void init_keyboard();
void keyboard_handler_main(void);
void timer_set_main();
//bool keypress

//interrupts.c


//kernel.c
extern char current_program_ID;

//paging.c
void InitPaging();

//drive.c
void drive();

//debug.c
void debug();


//draw.c
void d_render();
void d_cls(unsigned short color);
void d_puts(char *str, char x, char y, unsigned short color);
void d_putc(char letter, char x, char y, unsigned short color);
void d_square(char x, char y, char w, char h, char color);

//data.c
extern unsigned char symbol_map[16];

//coltynGame.c
void ColtynMain();
void Coltyn2Main();

//tictac.c
void TicTacMain();

typedef enum { false, true } bool;
