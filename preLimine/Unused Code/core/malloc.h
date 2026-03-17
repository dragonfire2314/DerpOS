#define NULL 0

struct block
{
    unsigned char* mem = NULL;

    unsigned int size = NULL;
    char isFree = false;

    block* next = NULL;
};

class blockMalloc
{
private:
    unsigned char* heapPos;
    unsigned char* stackPos;

    unsigned char* mem_start;
    unsigned char* mem_limit;

    block* start;
    block* end;
public:
    blockMalloc(unsigned int memStart, unsigned int limit);
    ~blockMalloc(){}

    void* malloc(unsigned int size);
    block* look_for_free_block(unsigned int size);

    block* malloc_block();
    void* malloc_mem(unsigned int size);
    int sbrk(unsigned int size);
    int lbrk(unsigned int size);
};