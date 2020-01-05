//Memory Manager for DerpOS
//By Tanner Kern

/*
* The MMU's job is to allocate processes memory and handle their pagging as well
* 
*/
//16mb
#define RAM_16_MB 0x1000000

unsigned long* heapPos = (unsigned long*)RAM_16_MB;

unsigned long* k_create_4k_block()
{
    unsigned long* temp = heapPos;
    heapPos = heapPos + 1024;
    return temp;
}

//Should work
int blockCounter = 0;
unsigned char* blockPointer;

unsigned int* k_create_512_block()
{
    if(blockCounter <= 0)
    {
        blockPointer = (unsigned char*)k_create_4k_block();
        blockCounter = 8;
    }

    blockCounter--;
    unsigned int* temp = (unsigned int*)blockPointer;
    blockPointer+=512;

    return temp;

    /*if(blockPos < 512)
    {
        current_512_block = k_create_4k_block();
    }
    unsigned long* temp = current_512_block;
    current_512_block += 512;
    return temp;*/
}

unsigned int* k_malloc_large(unsigned int sizeInBytes)
{
    unsigned long* retAdress = k_create_4k_block();
    for(int i = 0; i < ((sizeInBytes/4096)*4)+1; i++)
    {
        unsigned long* hold = k_create_4k_block();
    }
    return (unsigned int*)retAdress;
    /*
    //float temp = (float)sizeInBytes / 4096.0f;
    int temp = sizeInBytes / 4096;
    temp++;
    unsigned long* retAdress = k_create_4k_block();
    for(int i = 0; i < temp-1; i++)
    {
        unsigned long* hold = k_create_4k_block();
    }
    return (unsigned int*)retAdress;
    */
}

//Memset
void memset(unsigned char* loc, char val, unsigned int size)
{
    for(int i = 0; i < size; i++)
	{
		loc[i] = val;
	}
}