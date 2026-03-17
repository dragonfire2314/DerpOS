#pragma once

#include "../global/types.h"
#include "../core/mmu.h"

template <class T>
class vector 
{
private:
    uint8* data;
    uint32 dataCount;
    uint32 maxData;

    uint32 pageCount;

    bool hasInit = false;
public:
    vector();
    void free();
    void init();

    void push_back(T data);

    T operator[](uint32 i);

    uint32 size();
};

template <class T>
vector<T>::vector()
{
    init();
}

template <class T>
void vector<T>::free()
{
    free_page_aligned(data);
}

template <class T>
uint32 vector<T>::size()
{
    return dataCount;
}

template <class T>
T vector<T>::operator[](uint32 i)
{
    return ((T*)data)[i];
}

template <class T>
void vector<T>::init() 
{
    if (hasInit)
        return;

    hasInit = true;

    pageCount = 1;

    data = (uint8*)malloc_page_aligned(PAGE_TO_BYTE(pageCount));

    maxData = PAGE_SIZE / sizeof(T);
    dataCount = 0;
}

template <class T>
void vector<T>::push_back(T value) 
{
    //Check for space
    if (dataCount >= maxData) { //Not enough space
        //Double memory and copy current memory over
        uint8* oldData = data;
        uint32 oldPageCount = pageCount;

        pageCount *= 2;
        data = (uint8*)malloc_page_aligned(PAGE_TO_BYTE(pageCount));

        memcpy(oldData, data, oldPageCount * PAGE_SIZE);

        free_page_aligned(oldData);
    }
    else { //Space to add stuff
        ((T*)(data + (dataCount * sizeof(T))))[0] = value;

        dataCount++;
    }
}