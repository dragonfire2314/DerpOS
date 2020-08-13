#include "atapi_driver.h"
#include "../../global/io.h"
#include "../../global/string/cstring.h"
#include "../../io/serial.h"
#include "../../interrupts.h"
#include "../../core/mmu.h"

void atapi_iso9660_init(uint32 bus, uint8 slavebit);

struct iso9660_Directory {
	uint8 length;
	uint8 extended_attribute_record_length;
	uint8 lba[8];
	uint8 data_length[8];
	uint8 time_and_date[7];
	uint8 file_flags;
	uint8 file_unit_size;
	uint8 interleave_gap;
	uint8 volume_sequence_number[4];
	uint8 length_of_file_name;
	uint8 file_name;
} __attribute__ ((packed));

struct iso9660_PathTable
{
	uint8 length;
	uint8 extended_attribute_record_length;
	uint8 lba[4];
	uint8 parent_directory_number[2];
	uint8 extra;
} __attribute__ ((packed));

struct iso9660_PrimaryVolumeDescriptor {
	uint8 type_code;
	uint8 standard_identifier[5];
	uint8 version;
	uint8 unused;
	uint8 system_identifier[32];
	uint8 volume_identifier[32];
	uint8 unused2[8];
	uint8 volume_space_size[8];
	uint8 unused3[32];
	uint8 volume_set_size[4];
	uint8 volume_sequence_number[4];
	uint8 logical_block_size[4];
	uint8 path_table_size[8];
	uint8 path_table_type_L[4];
	uint8 path_table_type_L_optional[4];
	uint8 path_table_type_M[4];
	uint8 path_table_type_M_optional[4];
	iso9660_Directory root_directory;
	//TODO - Add the rest of the fields
} __attribute__ ((packed));

iso9660_PrimaryVolumeDescriptor* primaryVolumeDescriptor;
iso9660_PathTable* pathTable;
uint8* rootExtent; 

File_info atapi_read_file(char* fileDir, uint32 bus, uint8 slavebit)
{
	//TODO - break the fileDir donw into parts so folders can be used
	//As of now only files on the root can be accessed

	File_info file;
	file.fileSize = 0;
	file.data = 0;

	uint32 file_lba = 0;

	//Step 1 - check every file in root for a match
	uint32 offset = 0;

	while (1)
	{
		iso9660_Directory* dir = (iso9660_Directory*)(rootExtent + offset);

		uint8 entryLength = dir->length;
		if (entryLength == 0) { //All possible entries were viewed - in the root of the disk that is
			serial_write_string("entryLength was zero iso9660 drive\r\n");
			file.fileSize = 0;
			file.data = 0;
			return file;
		}
		if (strcmp_with_size(fileDir, (char*)&(dir->file_name), dir->length_of_file_name)) {
			serial_write_string("Match \r\n");

			file.fileSize = ((uint32*)dir->data_length)[0];
			file.data = 0;

			file_lba = ((uint32*)dir->lba)[0];
			break;
		}
		else {
			offset += entryLength;

			serial_write_string("Looked at: ");
			for (int i = 0; i < dir->length_of_file_name; i++) {
				serial_write_char((&(dir->file_name))[i]);
			}
			serial_write_string("\r\n");
		}
	}

	//Step 2 - Load file
	//Get space for file
	uint32* fileMem = k_malloc_large(file.fileSize);

	uint32 readCount = 0;
	if ((file.fileSize % 0x800) > 0)
		readCount = (file.fileSize / 0x800) + 1;
	else
		readCount = (file.fileSize / 0x800);

	for (int i = 0; i < readCount; i++) 
	{
		atapi_drive_read_sector(bus, slavebit, file_lba + i, ((uint8*)fileMem) + (0x800 * i));
	}

	file.data = (uint8*)fileMem;

	return file;
}

void atapi_detect_file_system(uint32 bus, uint8 slavebit)
{
	//For now assume iso9660
	atapi_iso9660_init(bus, slavebit);
}

void atapi_iso9660_init(uint32 bus, uint8 slavebit) 
{
	//Pull off the root Directory and keep for find files later

	//Step 0 - make a buffer for the data
	primaryVolumeDescriptor = (iso9660_PrimaryVolumeDescriptor*)k_create_2k_block();

	//Step 1 - Get the primary Volume Descriptor (should be at lba 0x10)
	atapi_drive_read_sector(bus, slavebit, 0x10, (uint8*)primaryVolumeDescriptor);
	//Step 2 - check if it is the primary Volume Descriptor
	if (primaryVolumeDescriptor->type_code != 1) serial_write_string("Kernel Panic: primary Volume Descriptor not found! \r\n");
	//Step 3 - get path table (assuming path table is only one sector big, as dynamic memory has not been added yet)
	pathTable = (iso9660_PathTable*)k_create_4k_block();
	uint32 pathTable_L_location = ((uint32*)primaryVolumeDescriptor->path_table_type_L)[0];
	atapi_drive_read_sector(bus, slavebit, pathTable_L_location, (uint8*)pathTable);
	//Step 4 - Grab root extent for faster future loading
	uint32 rootExtent_location = ((uint32*)pathTable->lba)[0];
	rootExtent = (uint8*)k_create_2k_block();
	atapi_drive_read_sector(bus, slavebit, rootExtent_location, rootExtent);
	//Done
	serial_write_string("ISO9660 Drive Init Finshed \r\n");
}

















/* The default and seemingly universal sector size for CD-ROMs. */
#define ATAPI_SECTOR_SIZE 2048
 
/* The default ISA IRQ numbers of the ATA controllers. */
#define ATA_IRQ_PRIMARY     0x0E
#define ATA_IRQ_SECONDARY   0x0F
 
/* The necessary I/O ports, indexed by "bus". */
#define ATA_DATA(x)         (x)
#define ATA_FEATURES(x)     (x+1)
#define ATA_SECTOR_COUNT(x) (x+2)
#define ATA_ADDRESS1(x)     (x+3)
#define ATA_ADDRESS2(x)     (x+4)
#define ATA_ADDRESS3(x)     (x+5)
#define ATA_DRIVE_SELECT(x) (x+6)
#define ATA_COMMAND(x)      (x+7)
#define ATA_DCR(x)          (x+0x206)   /* device control register */
 
/* valid values for "bus" */
#define ATA_BUS_PRIMARY     0x1F0
#define ATA_BUS_SECONDARY   0x170
/* valid values for "drive" */
#define ATA_DRIVE_MASTER    0xA0
#define ATA_DRIVE_SLAVE     0xB0
 
/* ATA specifies a 400ns delay after drive switching -- often
 * implemented as 4 Alternative Status queries. */
#define ATA_SELECT_DELAY(bus) \
  {read_port(ATA_DCR(bus));read_port(ATA_DCR(bus));read_port(ATA_DCR(bus));read_port(ATA_DCR(bus));}
 
/* Use the ATAPI protocol to read a single sector from the given
 * bus/drive into the buffer using logical block address lba. */
int atapi_drive_read_sector (uint32 bus, uint32 drive, uint32 lba, uint8 *buffer)
{
	/* 0xA8 is READ SECTORS command byte. */
	uint8 read_cmd[12] = { 0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8 status;
	int size;

    //serial_write_string("PASS 1 \r\n");

	/* Select drive (only the slave-bit is set) */
	write_port(ATA_DRIVE_SELECT (bus), drive << 4);      
	ATA_SELECT_DELAY(bus);       /* 400ns delay */
	write_port(ATA_FEATURES (bus), 0x0);       /* PIO mode */
	write_port(ATA_ADDRESS2 (bus), ATAPI_SECTOR_SIZE & 0xFF);
	write_port(ATA_ADDRESS3 (bus), ATAPI_SECTOR_SIZE >> 8);
	write_port(ATA_COMMAND (bus), 0xA0);       /* ATA PACKET command */

    //serial_write_string("PASS 2 \r\n");

	while ((status = read_port(ATA_COMMAND (bus))) & 0x80)     /* BUSY */
	//asm volatile ("pause");
	while (!((status = read_port(ATA_COMMAND (bus))) & 0x8) && !(status & 0x1))
	//asm volatile ("pause");
	/* DRQ or ERROR set */
	if (status & 0x1) {
	    size = -1;
		serial_write_string("FAIL 1 \r\n");
        return -1;
	}

    //serial_write_string("PASS 3 \r\n");

	prepare_for_irq14();

	read_cmd[9] = 1;              /* 1 sector */
	read_cmd[2] = (lba >> 0x18) & 0xFF;   /* most sig. byte of LBA */
	read_cmd[3] = (lba >> 0x10) & 0xFF;
	read_cmd[4] = (lba >> 0x08) & 0xFF;
	read_cmd[5] = (lba >> 0x00) & 0xFF;   /* least sig. byte of LBA */
	/* Send ATAPI/SCSI command */
	asm("rep outsw"::"c"(6), "d"(bus), "S"(read_cmd));
    //write_port_w(ATA_DATA(bus), ((uint16*)read_cmd)[0]);
    //write_port_w(ATA_DATA(bus), ((uint16*)read_cmd)[1]);
    //write_port_w(ATA_DATA(bus), ((uint16*)read_cmd)[2]);
    //write_port_w(ATA_DATA(bus), ((uint16*)read_cmd)[3]);
    //write_port_w(ATA_DATA(bus), ((uint16*)read_cmd)[4]);
    //write_port_w(ATA_DATA(bus), ((uint16*)read_cmd)[5]);

    //serial_write_string("PASS 4 \r\n");

    wait_for_irq14();

    //for (int i = 0; i < 2000000; i++) {} //Should be wait for irq, but some systems are getting the irq before the wait call
	//, will need to be addressed.

    //serial_write_string("PASS 5 \r\n");

    /* Read actual size */
	size =
	(((int) read_port(ATA_ADDRESS3 (bus))) << 8) |
	(int) (read_port(ATA_ADDRESS2 (bus)));

    if (size != ATAPI_SECTOR_SIZE) serial_write_string("Sector size wrong \r\n");

    //serial_write_string("PASS 6 \r\n");

	//asm("pushw %es");
 	//asm("mov %%ax, %%es"::"a"(selector));
 	//asm("rep insw"::"c"(words), "d"(bus), "D"(edi));// Receive Data.
 	//asm("popw %es"); 

	while ((status = read_port(ATA_COMMAND (bus))) & 0x08)

    for (int i = 0; i < 1024; i++)
        //serial_write_byte(read_port_w(ATA_DATA(bus)));
        ((uint16*)buffer)[i] = read_port_w(ATA_DATA(bus));

	while ((status = read_port(ATA_COMMAND (bus))) & 0x88)

    //serial_write_string("PASS 7 \r\n");

    //serial_write_data((char*)buffer, 2048);

	return 0;
}