#include "ata.h"
#include "../global/io.h"
#include "../io/serial.h"
#include "../core/mmu.h"
#include "../global/types.h"
#include "drivers/atapi_driver.h"

extern "C" void ata_disk_wait();
extern "C" void ata_drq_wait();
extern "C" void ata_getData(int* memLoc);
extern "C" void ata_writeData();

void detect_devtype (bool isMaster, uint16 base);
void detect_drive_type (bool isMaster, uint16 base, Drive_info* drive);
void detect_non_ata_drive(bool isMaster, uint16 base, Drive_info* drive);
void getDriveInfo (bool isMaster, uint16 base, Drive_info* drive);
void init_file_system(uint8 slavebit, uint32 bus, Drive_info* drive);

//Variables
Drive_info drives[4];

uint8* drive_init_buffer;

uint8 temp_sector_buffer[2048];

void enumerate_drives()
{
    //Init buffer
    drive_init_buffer = (uint8*)k_create_512_block();

    //Populate the drives
    detect_drive_type(true,  0x1F0, &(drives[0]));
    detect_drive_type(false, 0x1F0, &(drives[1]));
    detect_drive_type(true,  0x170, &(drives[2]));
    detect_drive_type(false, 0x170, &(drives[3]));

    //Init the file systems
    init_file_system(0, 0x1F0, &(drives[0]));
    init_file_system(1, 0x1F0, &(drives[1]));
    init_file_system(0, 0x170, &(drives[2]));
    init_file_system(1, 0x170, &(drives[3]));

    //Init virtual file system
}

File_info readFile(char driveLetter, char* file)
{
    return atapi_read_file(file, 0x1F0, 0);
}

void init_file_system(uint8 slavebit, uint32 bus, Drive_info* drive)
{
    if (drive->drive_type == ATAPI) {
        serial_write_string("ATAPI \r\n");
        serial_write_int(bus);
        serial_write_byte(slavebit);
        serial_write_string("\r\n");

        atapi_detect_file_system(bus, slavebit);
        
        drive->driveLetter = 'C';
    }
    else if (drive->drive_type == ATA){ serial_write_string("ATA \r\n"); }
    else { serial_write_string("NONE \r\n"); }
}

void detect_drive_type (bool isMaster, uint16 base, Drive_info* drive)
{
	if (isMaster)
        write_port(base + 6, 0xA0);
    else
        write_port(base + 6, 0xB0);
    write_port(base + 2, 0);
    write_port(base + 3, 0);
    write_port(base + 4, 0);
    write_port(base + 5, 0);

    write_port(base + 7, 0xEC); //Identify command

    uint8 response = read_port(base + 7);
    if (response == 0) { //No drive
        if (isMaster)
            serial_write_string("No master drive found at: ");
        else
            serial_write_string("No slave drive found at: ");

        serial_write_int(base);
        serial_write_string(" \r\n");
        drive->drive_type = NONE;
    }
    else if ((response & 0x01) == 0x01) {
        serial_write_string("Drive found but its not ata, at: ");
        if (isMaster)
            serial_write_string("master: ");
        else
            serial_write_string("slave: ");

        serial_write_int(base);
        serial_write_string(" \r\n");

        detect_non_ata_drive(isMaster, base, drive);
    }
    else {
        serial_write_string("Ata Drive found: ");
        serial_write_int(base);
        serial_write_string(" \r\n");

        drive->drive_type = ATA;
        getDriveInfo(isMaster, base, drive);
    }

    //Set the driver functions for each drive type
    
}

void detect_non_ata_drive(bool isMaster, uint16 base, Drive_info* drive)
{
    if (isMaster)
        write_port(base + 6, 0xA0);
    else
        write_port(base + 6, 0xB0);
    write_port(base + 2, 0);
    write_port(base + 3, 0);
    write_port(base + 4, 0);
    write_port(base + 5, 0);

    write_port(base + 7, 0xA1); //Identify packet device command

    uint8 response = read_port(base + 7);
    if (response == 0) { //No drive
        serial_write_string("Well... This should never happen... disk was found for ATA but not for ATAPI \r\n");
    }
    else if ((response & 0x01) == 0x01) {
        serial_write_string("Drive ATAPI responded with error \r\n");
    }
    else {
        serial_write_string("Drive ATAPI responded with: ");
        serial_write_byte(response);
        serial_write_string(" \r\n");

        drive->drive_type = ATAPI;
        getDriveInfo(isMaster, base, drive);
    }
}

void getDriveInfo (bool isMaster, uint16 base, Drive_info* drive) 
{
    //ata_disk_wait();
    //ata_drq_wait();

    ata_getData((int*)drive_init_buffer);

    //TODO - Use the grabbed buffer to populate drive info

    serial_write_string("Grabbed data \r\n");

    //serial_write_data((char*)drive_init_buffer, 512);

    serial_write_string("\r\n");

    /* serial_write_byte(((uint16*)drive_init_buffer)[100]);
    serial_write_byte(((uint16*)drive_init_buffer)[101]);
    serial_write_byte(((uint16*)drive_init_buffer)[102]);
    serial_write_byte(((uint16*)drive_init_buffer)[103]); */
}