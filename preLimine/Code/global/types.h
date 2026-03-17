#ifndef TYPES_H
#define TYPES_H

//Defines
#define NULL 0
#define INT_MAX 2147483647

//Typedefs
typedef unsigned char  ub;
typedef unsigned short uh;
typedef unsigned int   uw;

typedef char  sb;
typedef short sh;
typedef int   sw;

typedef unsigned char  ubyte;
typedef unsigned short uhalfword;
typedef unsigned int   uword;

typedef char  sbyte;
typedef short shalfword;
typedef int   sword;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

typedef char  sint8;
typedef short sint16;
typedef int   sint32;

//Structures
struct Vector2
{
    uint32 x;
    uint32 y;
};

#endif