#include "string.h"

string::string(char* str)
{
    int count = 0;
    while (str[count] != 0) {
        data.push_back(str[count]);
        count++;
    }
    data.push_back(str[count]);
}

string::string()
{
}

void string::free()
{
    data.free();
}

uint32 string::size()
{
    return data.size();
}

char string::operator[](uint32 index)
{
    return data[index];
}

void string::operator=(string &s)
{
    for (uint32 i = 0; i < s.size(); i++)
        data.push_back(s[i]);
}

void string::operator=(char* s)
{
    int count = 0;
    while (s[count] != 0) {
        data.push_back(s[count]);
        count++;
    }
    data.push_back(s[count]);
}