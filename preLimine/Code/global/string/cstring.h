#ifndef STRING_H
#define STRING_H

#include "../types.h"

bool strcmp(char* a, char* b);
bool strcmp_with_size(char* a, char* b, uint16 size);
uint32 strlen(const char* str);

#endif