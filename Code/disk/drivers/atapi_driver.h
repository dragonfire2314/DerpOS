#include "../../global/types.h"
#include "../ata.h"

int atapi_drive_read_sector(uint32 bus, uint32 drive, uint32 lba, uint8 *buffer);
File_info atapi_read_file(char* fileDir, uint32 bus, uint8 slavebit);
void atapi_detect_file_system(uint32 bus, uint8 slavebit);