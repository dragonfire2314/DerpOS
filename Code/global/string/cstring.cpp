#include "cstring.h"

bool strcmp(char* a, char* b) 
{
    int c = 0;
 
    while (a[c] == b[c]) {
        if (a[c] == '\0' || b[c] == '\0')
            break;
        c++;
    }
   
     if (a[c] == '\0' && b[c] == '\0')
        return true;
     else
        return false;
}

bool strcmp_with_size(char* a, char* b, uint16 size)
{
    uint16 count = 0;

    while (a[count] == b[count] && count < size - 1) {
        count++;

        if (a[count] == '\0' || b[count] == '\0')
            return false;
    }

    if (a[count] == b[count])
        return true;
    else
        return false;
}

uint32 strlen(const char* str)
{
    uint32 i = 0;
    while (str[i] != '\0') {
        i++;
    }

    return i;
}