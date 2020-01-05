/*
TODO
-Stop user from laoding a sector that is too large
*/

//#include "system.h"
#include "drive.h"
#include "core/mmu.h"
#include "external/tinyalloc.h"
#include "graphics_temp.h"

extern void ata_disk_wait();
extern void ata_drq_wait();
extern void ata_getData(int* memLoc);
extern void ata_writeData();
extern void write_port(unsigned short port, unsigned char data);

unsigned int* drive_readSector(int sector);

void drive_init()
{

}

unsigned int* drive_load_file(char fileName)
{
    int fileSector, fileSectorCount;

    //Open sector 0, the FAT sector
    unsigned int* FAT = drive_readSector(0);
    //Look though file entries
    for(int i = 1; i < 128; i++)
    {
        //Load sector for file
        unsigned int* temp = drive_readSector(FAT[i]);
        //Compare with file name
        if(temp[0] == fileName)
        {
            fileSector = i+1;
            fileSectorCount = temp[65];

            putd(fileSectorCount, 0, 0, 0x0F);
            putd(fileSector, 0, 1, 0x0F);

            break;
        }
    }
    //Get ram block in 4kb
    unsigned int* drive_memLoc = (unsigned int*)k_create_4k_block();
    k_create_4k_block();
    k_create_4k_block();
    //Load all data from a file entry into a block of ram
    for(int i = 0; i < fileSectorCount; i++)
    {
        ata_disk_wait(); // wait BSY to 0 and RDY to 1

        write_port(0x1F6, 0 >> 24 | 0xF0);// Primary slave drive
        write_port(0x1F2, 1); // Read one sector
        write_port(0x1F3, fileSector+i); // 2 is the sector value
        write_port(0x1F4, fileSector+i >> 8);
        write_port(0x1F5, fileSector+i >> 16);
        // Make a read call
        write_port(0x1F7, 0x20);

        ata_disk_wait();

        ata_drq_wait();

        ata_getData((int*)drive_memLoc + (512 * i));
    }
    //Retrun ram adress
    return drive_memLoc;
}

unsigned int* drive_readSector(int sector)
{
    ata_disk_wait(); // wait BSY to 0 and RDY to 1

    write_port(0x1F6, 0 >> 24 | 0xF0);// Primary slave drive
    write_port(0x1F2, 1); // Read one sector
    write_port(0x1F3, sector); // 2 is the sector value
    write_port(0x1F4, sector >> 8);
    write_port(0x1F5, sector >> 16);
    // Make a read call
    write_port(0x1F7, 0x20);

    ata_disk_wait();

    ata_drq_wait();

    unsigned int* readMem = (unsigned int*)k_create_512_block();

    ata_getData((int*)readMem);

    return readMem;
}


char *sectorPointer = (char*)0x1000000;
char *writePointer = (char*)0x1000200;

void write(int sector);

void drive()
{
    //for(int i = 0; i < 512; i++)
    //{
    //    writePointer[i] = i;
    //}

    //write(3);
    //read(3);
}

void write(int sector)
{
    //WRITE
    ata_disk_wait();
    write_port(0x1F6, 0 >> 24 | 0xF0);// Secondary slave drive i think
    write_port(0x1F2, 1); // write one sector
    write_port(0x1F3, sector); // 2 is the sector value
    write_port(0x1F4, sector >> 8);
    write_port(0x1F5, sector >> 16);

    write_port(0x1F7, 0x30);

    ata_drq_wait();

    ata_writeData();
}

unsigned int* read(int sector)
{
    //READ
    ata_disk_wait(); // wait BSY to 0 and RDY to 1

    write_port(0x1F6, 0 >> 24 | 0xF0);// Secondary slave drive i think
    write_port(0x1F2, 1); // Read one sector
    write_port(0x1F3, sector); // 2 is the sector value
    write_port(0x1F4, sector >> 8);
    write_port(0x1F5, sector >> 16);
    // Make a read call
    write_port(0x1F7, 0x20);

    ata_disk_wait();

    ata_drq_wait();

    unsigned int* readMem = (unsigned int*)k_create_4k_block();

    ata_getData((int*)readMem);

    return readMem;

    //while(1);
}

unsigned int* readSector(int sector)
{
    //READ
    ata_disk_wait(); // wait BSY to 0 and RDY to 1

    write_port(0x1F6, 0 >> 24 | 0xF0);// Secondary slave drive i think
    write_port(0x1F2, 1); // Read one sector
    write_port(0x1F3, sector); // 2 is the sector value
    write_port(0x1F4, sector >> 8);
    write_port(0x1F5, sector >> 16);
    // Make a read call
    write_port(0x1F7, 0x20);

    ata_disk_wait();

    ata_drq_wait();

    unsigned int* readMem = (unsigned int*)k_create_512_block();

    ata_getData((int*)readMem);

    return readMem;
}

void readToLoc(int sector, unsigned int* loc)
{
    //READ
    ata_disk_wait(); // wait BSY to 0 and RDY to 1

    write_port(0x1F6, 0 >> 24 | 0xF0);// Secondary slave drive i think
    write_port(0x1F2, 1); // Read one sector
    write_port(0x1F3, sector); // 2 is the sector value
    write_port(0x1F4, sector >> 8);
    write_port(0x1F5, sector >> 16);
    // Make a read call
    write_port(0x1F7, 0x20);

    ata_disk_wait();

    ata_drq_wait();

    ata_getData((int*)loc);
}

