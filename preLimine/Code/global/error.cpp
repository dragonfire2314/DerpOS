#include "error.h"
#include "../io/serial.h"

void Error_Warning(char* message) 
{
    serial_write_string("Warning: ");
    serial_write_string(message);
    serial_write_string("\r\n");
}

void Error_Bad(char* message)
{
    serial_write_string("Bad: ");
    serial_write_string(message);
    serial_write_string("\r\n");
}

void Error_Critical(char* message)
{
    serial_write_string("Critical: ");
    serial_write_string(message);
    serial_write_string("\r\n");

    while(1);
}