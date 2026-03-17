#pragma once

#include "../global/types.h"
#include "vector.h"

class string
{
private:
    vector<char> data;
public:
    string(char* str);
    string();

    void free();

    uint32 size();

    char operator[](uint32 index);
    void operator=(string &s);
    void operator=(char* s);
};