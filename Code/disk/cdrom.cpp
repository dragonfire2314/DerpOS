#include "../global/io.h"
#include "../io/serial.h"
#include "../core/mmu.h"

extern "C" void ata_disk_wait();
extern "C" void ata_drq_wait();
extern "C" void ata_getData(int* memLoc);
extern "C" void ata_writeData();
extern "C" void write_port(unsigned short port, unsigned char data);

void cdrom_init() {
    serial_write_byte(read_port(0x172));
    serial_write_byte(read_port(0x173));
    serial_write_byte(read_port(0x174));
    serial_write_byte(read_port(0x175));
}

unsigned int* drive_readSector(int sector)
{
    serial_write_string("Here\r\n");

    ata_disk_wait(); // wait BSY to 0 and RDY to 1

    serial_write_string("Here1\r\n");

    write_port(0x1F6, 0 >> 24 | 0xE0);// Primary slave drive
    write_port(0x1F2, 1); // Read one sector
    write_port(0x1F3, sector); // 2 is the sector value
    write_port(0x1F4, sector >> 8);
    write_port(0x1F5, sector >> 16);
    // Make a read call
    write_port(0x1F7, 0x20);

    serial_write_string("Here2\r\n");

    ata_disk_wait();

    serial_write_string("Here3\r\n");

    ata_drq_wait();

    serial_write_string("Here4\r\n");

    unsigned int* readMem = (unsigned int*)k_create_512_block();

    ata_getData((int*)readMem);

    //print as debug
    for (int i = 0; i < 512; i++) 
    {
        serial_write_byte(((unsigned char*)readMem)[i]);
    }

    return readMem;
}