#define PROCESS_LIMIT 32

void idt_init(void);

void putd(int data, char x, char y, unsigned short color);

extern void write_cr3(unsigned long*);
extern void write_cr0(unsigned long);
extern unsigned long read_cr0();

void cls(unsigned short color);
void putc(char letter, int x, int y, unsigned short color);

void scheduler_Setup();
void AddProcess(unsigned long func);

unsigned int* read(int sector);
void readToLoc(int sector, unsigned int* loc);
unsigned int* readSector(int sector);

//vga.c
char vga_Start(char mode);
void vga_draw_text(char *str, short x, short y, char front_col, char back_col);
void vga_set_color(char color, char red, char green, char blue);
void vga_draw_line(short x1, short y1, short x2, short y2, char col);
void vga_draw_circle(short xc, short yc, short r, short col);
void vga_cls(char col);
void vga_draw_triangle(short x1, short y1, short x2, short y2, short x3, short y3, char col);
void vga_update_buffer();
void vga_buffer_pixel(short x, short y, short c);
void vga16_buffer_pixel(short x, short y, short c);
void vga16_update_buffer();
void draw_x(void);

void paging_CreateDirectory(int process_ID);
unsigned long* paging_GetDirectory(int process_ID);
void paging_CreatePageTable(int process_ID, int pageDirectorySlot);
void paging_AddPage(int process_ID, int pageDirectorySlot, int pageTableSlot, unsigned long adress, unsigned long flags);
struct Derp_Pagging** paging_GetInfo();
unsigned long paging_GetPageAdress(int process_ID, int pageDirectorySlot, int pageTableSlot);

struct Derp_Pagging
{
    struct pageDirectory* dir;
    struct pageTable* table_Loc[1024];
};

struct pageDirectory
{
    unsigned long D_entries[1024];
};

struct pageTable
{
    unsigned long T_entries[1024];
};