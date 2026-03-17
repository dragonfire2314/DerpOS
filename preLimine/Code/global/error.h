#pragma once

#include "../global/types.h"

struct ErrorInfo 
{
    char* message;

    char* data;
    uint32 dataLength;
};

//Warns the user that something has happened
void Error_Warning(char* message);
//Warns the user of a major problem but keeps going
void Error_Bad(char* message);
//Warns the user and crashes the OS
void Error_Critical(char* message);