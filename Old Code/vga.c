#include "system.h"
#include "math.h"
#include "vga.h"

#define VGA_RAM_OFFSET 0xA0000

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		    0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)


static void (*g_write_pixel)(unsigned x, unsigned y, unsigned c);
static unsigned g_wd, g_ht;

static void vpokeb(unsigned off, unsigned val);
static void write_pixel8(unsigned x, unsigned y, unsigned c);
void write_regs(unsigned char *regs);
static int get_fb_seg(void);
void outportb(unsigned short port, unsigned char data);
char inportb(unsigned short port);
static void set_plane(unsigned p);
static unsigned vpeekb(unsigned off);
static void write_pixel4p(unsigned x, unsigned y, unsigned c);
void set_text_mode();
//static void write_font(unsigned char *buf, unsigned font_height);

char *mem = (char*)0x0;

unsigned char g_320x200x256[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

unsigned char g_640x480x16[] =
{
/* MISC */
	0xE3,
/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

char* vga_mem_buffer = (char*)0x1010000;

char vga_Start(char mode)
{

	switch(mode)
	{
	case 0x01:
	/* 16-color */
		write_regs(g_640x480x16);
		g_wd = 640;
		g_ht = 480;
		g_write_pixel = write_pixel4p;
		break;
	case 0x02:
	/* 256-color */
		write_regs(g_320x200x256);
		g_wd = 320;
		g_ht = 200;
		g_write_pixel = write_pixel8;
		break;
	default:
		return 0x0;
	}
	return 0x1;
}

unsigned char vga_letters[27][5][5] = {
	{{0,1,1,0,0},{1,0,0,1,0},{1,1,1,1,0},{1,0,0,1,0},{1,0,0,1,0}},//A
	{{1,1,1,0,0},{1,0,0,1,0},{1,1,1,0,0},{1,0,0,1,0},{1,1,1,0,0}},//B
	{{0,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{0,1,1,1,0}},//C
	{{1,1,1,0,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,1,1,0,0}},//D
	{{1,1,1,1,0},{1,0,0,0,0},{1,1,1,0,0},{1,0,0,0,0},{1,1,1,1,0}},//E
	{{1,1,1,1,0},{1,0,0,0,0},{1,1,1,0,0},{1,0,0,0,0},{1,0,0,0,0}},//F
	{{1,1,1,1,0},{1,0,0,0,0},{1,0,1,1,0},{1,0,0,1,0},{1,1,1,1,0}},//G
	{{1,0,0,1,0},{1,0,0,1,0},{1,1,1,1,0},{1,0,0,1,0},{1,0,0,1,0}},//H
	{{0,1,1,1,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,1,1,1,0}},//I
	{{0,0,0,1,0},{0,0,0,1,0},{0,0,0,1,0},{1,0,0,1,0},{0,1,1,0,0}},//J
	{{1,0,0,1,0},{1,0,1,0,0},{1,1,0,0,0},{1,0,1,0,0},{1,0,0,1,0}},//K
	{{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,0}},//L
	{{1,0,0,0,1},{1,1,0,1,1},{1,0,1,0,1},{1,0,0,0,1},{1,0,0,0,1}},//M
	{{1,0,0,1,0},{1,1,0,1,0},{1,0,1,1,0},{1,0,0,1,0},{1,0,0,1,0}},//N
	{{0,1,1,0,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{0,1,1,0,0}},//O
	{{1,1,1,0,0},{1,0,0,1,0},{1,1,1,0,0},{1,0,0,0,0},{1,0,0,0,0}},//P
	{{0,1,1,0,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,1,0,0},{0,1,0,1,0}},//Q
	{{1,1,1,0,0},{1,0,0,1,0},{1,1,1,0,0},{1,0,1,0,0},{1,0,0,1,0}},//R
	{{0,1,1,1,0},{1,0,0,0,0},{0,1,1,0,0},{0,0,0,1,0},{1,1,1,0,0}},//S
	{{1,1,1,1,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,0,1,0,0}},//T
	{{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{1,0,0,1,0},{0,1,1,0,0}},//U
	{{1,0,0,0,1},{1,0,0,0,1},{0,1,0,1,0},{0,1,0,1,0},{0,0,1,0,0}},//V
	{{1,0,0,0,1},{1,0,1,0,1},{1,0,1,0,1},{0,1,0,1,0},{0,1,0,1,0}},//W
	{{1,0,0,1,0},{1,0,0,1,0},{0,1,1,0,0},{1,0,0,1,0},{1,0,0,1,0}},//X
	{{1,0,0,1,0},{1,0,0,1,0},{0,1,1,0,0},{0,0,1,0,0},{1,1,0,0,0}},//Y
	{{0,1,1,1,0},{0,0,0,1,0},{0,0,1,0,0},{0,1,0,0,0},{0,1,1,1,0}},//Z
	{{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}} //Space
};

unsigned char vga_text_lookup_table[128] = 
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	26,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 
	0,0,1,2,3,4,5,6, //NULL, A, B, C, D
	7,8,9,10,11,12,13,14,
	15,16,17,18,19,20,21,22,
	23,24,25,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

void vga_draw_text(char *str, short x, short y, char front_col, char back_col)
{
	int j = 0;
	while (str[j] != '\0')
	{
		for(int i = 0; i < 5; i++)
		{
			for(int k = 0; k < 5; k++)
			{
				if(vga_letters[vga_text_lookup_table[str[j]]][k][i] == 1){
					vga_buffer_pixel(x+i+(j*5), y+k, front_col);
				}else{
					vga_buffer_pixel(x+i+(j*5), y+k, back_col);
				}
			}
		}
		j++;
	}
}

void vga_set_color(char color, char red, char green, char blue)
{
	outportb(0x3C8, color);
	outportb(0x3C9, red);
	outportb(0x3C9, green);
	outportb(0x3C9, blue);
}

void vga_draw_line(short x1, short y1, short x2, short y2, char col)
{
	short x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			vga_buffer_pixel(x, y, col);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				vga_buffer_pixel(x, y, col);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			vga_buffer_pixel(x, y, col);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				vga_buffer_pixel(x, y, col);
			}
		}
}

void vga_draw_circle(short xc, short yc, short r, short col)
{
	short x = 0;
	short y = r;
	short p = 3 - 2 * r;
	if (!r) return;

	while (y >= x) // only formulate 1/8 of circle
	{
		vga_buffer_pixel(xc - x, yc - y, col);//upper left left
		vga_buffer_pixel(xc - y, yc - x, col);//upper upper left
		vga_buffer_pixel(xc + y, yc - x, col);//upper upper right
		vga_buffer_pixel(xc + x, yc - y, col);//upper right right
		vga_buffer_pixel(xc - x, yc + y, col);//lower left left
		vga_buffer_pixel(xc - y, yc + x, col);//lower lower left
		vga_buffer_pixel(xc + y, yc + x, col);//lower lower right
		vga_buffer_pixel(xc + x, yc + y, col);//lower right right
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void vga_cls(char col)
{
	for(int y = 0; y < g_ht; y++)
		for(int x = 0; x < g_wd; x++)
			vga_buffer_pixel(x, y, col);
}

void vga_draw_triangle(short x1, short y1, short x2, short y2, short x3, short y3, char col)
{
		vga_draw_line(x1, y1, x2, y2, col);
		vga_draw_line(x2, y2, x3, y3, col);
		vga_draw_line(x3, y3, x1, y1, col);
}

void vga_update_buffer()
{
	for(int i = 0; i < 320*200; i++)
	{
		mem[VGA_RAM_OFFSET + i] = vga_mem_buffer[i];
	}
}

void vga_buffer_pixel(short x, short y, short c)
{
	unsigned wd_in_bytes;
	unsigned off;

	wd_in_bytes = g_wd;
	off = wd_in_bytes * y + x;

	vga_mem_buffer[off] = c;
}

void vga16_buffer_pixel(short x, short y, short c)
{
	unsigned wd_in_bytes, off, mask, p, pmask;

	wd_in_bytes = g_wd / 8;
	off = wd_in_bytes * y + x / 8;
	x = (x & 7) * 1;
	mask = 0x80 >> x;
	pmask = 1;
	for(p = 0; p < 4; p++)
	{
		//Set Plane
		unsigned char pmask;
		p &= 3;
		pmask = 1 << p;
		outportb(VGA_GC_INDEX, 4);
		outportb(VGA_GC_DATA, p);

		outportb(VGA_SEQ_INDEX, 2);
		outportb(VGA_SEQ_DATA, pmask);
		//Poke bytes?
		if(pmask & c)
			vga_mem_buffer[off] = vga_mem_buffer[off] | mask;
		else
			vga_mem_buffer[off] = vga_mem_buffer[off] & ~mask;
		pmask <<= 1;
	}
}

void vga16_update_buffer()
{
	int t = get_fb_seg();

	for(int i = 0; i < 640*480; i++)
	{
		mem[i] = vga_mem_buffer[i];
	}
}

void draw_x(void)
{
	unsigned x, y;

/* clear screen */
	for(y = 0; y < g_ht; y++)
		for(x = 0; x < g_wd; x++)
			vga16_buffer_pixel(x, y, 0);
/* draw 2-color X */
	for(y = 0; y < g_ht; y++)
	{
		vga16_buffer_pixel((g_wd - g_ht) / 2 + y, y, 1);
		vga16_buffer_pixel((g_ht + g_wd) / 2 - y, y, 2);
	}
}

/******************************************************************************
*
*   DEPENDENCY'S
*
*******************************************************************************/

static void vpokeb(unsigned off, unsigned val)
{
	//pokeb(get_fb_seg(), off, val);
	mem[get_fb_seg()+off] = val;
}
static unsigned vpeekb(unsigned off)
{
	//return peekb(get_fb_seg(), off);
	return mem[get_fb_seg()+off];
}

static void write_pixel8(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes;
	unsigned off;

	wd_in_bytes = g_wd;
	off = wd_in_bytes * y + x;
	vpokeb(off, c);
}

static void write_pixel4p(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes, off, mask, p, pmask;

	wd_in_bytes = g_wd / 8;
	off = wd_in_bytes * y + x / 8;
	x = (x & 7) * 1;
	mask = 0x80 >> x;
	pmask = 1;
	for(p = 0; p < 4; p++)
	{
		set_plane(p);
		if(pmask & c)
			vpokeb(off, vpeekb(off) | mask);
		else
			vpokeb(off, vpeekb(off) & ~mask);
		pmask <<= 1;
	}
}

static void set_plane(unsigned p)
{
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
/* set read plane */
	outportb(VGA_GC_INDEX, 4);
	outportb(VGA_GC_DATA, p);
/* set write plane */
	outportb(VGA_SEQ_INDEX, 2);
	outportb(VGA_SEQ_DATA, pmask);
}

void write_regs(unsigned char *regs)
{
	unsigned i;

/* write MISCELLANEOUS reg */
	outportb(VGA_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outportb(VGA_SEQ_INDEX, i);
		outportb(VGA_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	outportb(VGA_CRTC_INDEX, 0x03);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) | 0x80);
	outportb(VGA_CRTC_INDEX, 0x11);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outportb(VGA_CRTC_INDEX, i);
		outportb(VGA_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outportb(VGA_GC_INDEX, i);
		outportb(VGA_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inportb(VGA_INSTAT_READ);
		outportb(VGA_AC_INDEX, i);
		outportb(VGA_AC_WRITE, *regs);
		regs++;
	}
/* lock 16-color palette and unblank display */
	(void)inportb(VGA_INSTAT_READ);
	outportb(VGA_AC_INDEX, 0x20);
}

static int get_fb_seg(void)
{
	unsigned seg;

	outportb(VGA_GC_INDEX, 6);
	seg = inportb(VGA_GC_DATA);
	seg >>= 2;
	seg &= 3;
	switch(seg)
	{
	case 0:
	case 1:
		seg = 0xA0000;
		break;
	case 2:
		seg = 0xB0000;
		break;
	case 3:
		seg = 0xB8000;
		break;
	}
	return seg;
}

extern void write_port(unsigned short port, unsigned char data);
extern char read_port(unsigned short port);

void outportb(unsigned short port, unsigned char data)
{
	write_port(port, data);
}

char inportb(unsigned short port) 
{
	 return read_port(port); 
}