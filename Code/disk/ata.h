#ifndef DRIVE_H
#define DRIVE_H

#include "../global/types.h"

enum Drive_type {
    NONE,
    ATA,
    ATAPI
};

struct Drive_info {
    Drive_type drive_type;

    char driveLetter;
    //char* (*read_file)(char*);
};

struct File_info {
    uint8* data;

    uint32 fileSize;
};

void enumerate_drives();
File_info readFile(char driveLetter, char* file);

#endif