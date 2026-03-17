#include "serial.h"
#include "../global/io.h"

#define PORT 0x3f8 /* COM1 */

unsigned char symbol_map[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

void serial_init() {
   write_port(PORT + 1, 0x00);    // Disable all interrupts
   write_port(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   write_port(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   write_port(PORT + 1, 0x00);    //                  (hi byte)
   write_port(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   write_port(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   write_port(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   return;
}

int is_transmit_empty() {
   return read_port(PORT + 5) & 0x20;
}
 
void serial_write_char(char a) {
   while (is_transmit_empty() == 0);
 
   write_port(PORT,a);
   return;
}

//Output null terminated string
void serial_write_string(char* str) {
    int count = 0;
    while (str[count] != 0) {
        serial_write_char(str[count]);
        count++;
    }
    return;
}

//Output data with a size
void serial_write_data(char* str, int size) {
    for (int i = 0; i < size; i++)
        serial_write_char(str[i]);
    return;
}

//Output 4 bytes to serial port
void serial_write_int(int data)
{
    char c1 = (data & 0xFF000000) >> 24;
    char c2 = (data & 0x00FF0000) >> 16;
    char c3 = (data & 0x0000FF00) >> 8;
    char c4 = data & 0x000000FF;

    serial_write_char(symbol_map[((c1 & 0xF0) >> 4)]);
    serial_write_char(symbol_map[(c1 & 0x0F)]);

    serial_write_char(symbol_map[((c2 & 0xF0) >> 4)]);
    serial_write_char(symbol_map[(c2 & 0x0F)]);

    serial_write_char(symbol_map[((c3 & 0xF0) >> 4)]);
    serial_write_char(symbol_map[(c3 & 0x0F)]);

    serial_write_char(symbol_map[((c4 & 0xF0) >> 4)]);
    serial_write_char(symbol_map[(c4 & 0x0F)]);

    serial_write_char('\r');
    serial_write_char('\n');
}

//Output 1 byte to serial port
void serial_write_byte(char data)
{
    serial_write_char('0');
    serial_write_char('x');
    serial_write_char(symbol_map[((data & 0xF0) >> 4)]);
    serial_write_char(symbol_map[(data & 0x0F)]);

    serial_write_char('\r');
    serial_write_char('\n');
}

void serial_write_debug(char* str, int data)
{
    serial_write_string(str);
    serial_write_int(data);
}