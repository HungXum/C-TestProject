#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

void printAddrData1Byte(void* startAddr, void* endAddr);
void printMCB(void* ptr);
void cur_chunk_data(void* mem);
void pre_chunk_data(void* mem);
void next_chunk_data(void* mem);
void malloc_memory()
{
    int *p = (int*)malloc(20);
    memset(p, 0x01, 24); //编译并没有报错，但是windows中malloc和memset空间不一样会运行错误，即使开辟了1个字节空间，但是可以memset后面很多空间，但是到了一定的大小时，free会出错(核心已转移)
    printf("p = %#x\n", p);

    int *p1 = (int*)malloc(0);
    memset(p, 0x02, 24);
    printf("p1 = %#x\n", p1);

    size_t size = malloc_usable_size(p);//该函数回返回指针p指向的能用的内存，但是由于对齐或者最小size的原因，返回的值可能比开的空间大，空间能用，但是是不好的编程习惯，最好不要越界使用
    printf("%zu\n", size);
    char* pCh = (char*)p;
    printf("----printf data of p\n");
    for(size_t i = 0; i < size; i++)
        printf("%x ", pCh[i]);
    printf("\n");


    //尝试去理解是否malloc指针指向的前两个字节保存关于size的信息，结果是当大于size <= 24, pSize[0] = 0x21; size > 24时，以16递增，故不知道？？？？
    // int* pSize = (int*)(pCh - 8);
    // printf("pSize[0] = %#x\n", *(pSize + 0));
    // printf("pSize[1] = %#x\n", *(pSize + 1));

    int *p2 = (int*)malloc(3);
    memset(p2, 0x03, 3);
    printf("p2 = %#x\n", p2);

    free(p);
    free(p1);
    // free(p2);
}

typedef size_t SIZE_SZ;

struct malloc_block
{
    SIZE_SZ pre_size;
    SIZE_SZ size;
    struct malloc_chunk* fd;         /* double links -- used only if free. */
    struct malloc_chunk* bk;

    /* Only used for large blocks: pointer to next larger size.  */
    struct malloc_chunk* fd_nextsize; /* double links -- used only if free. */
    struct malloc_chunk* bk_nextsize;

};

typedef struct malloc_block* mchunkptr;

#define PREV_INUSE 0x1
#define IS_MMAPPED 0x2
#define NON_MAIN_ARENA 0x4

#define SIZE_BIT (PREV_INUSE | IS_MMAPPED | NON_MAIN_ARENA)

#define chunksize(p) ((p)->size & ~(SIZE_BIT))
#define next_chunk(p) ((mchunkptr)((char*)(p) + ((p)->size & SIZE_BIT)))
#define pre_chunk(p) ((mchunkptr)((char*)(p) - ((p)->pre_size)))
#define mem2chunk(mem) ((mchunkptr)((char*)mem - 2 * sizeof(SIZE_SZ)))

#define MIN_CHUNK_SIZE        (offsetof(struct malloc_chunk, fd_nextsize))

#define MALLOC_ALIGNMENT       (2 *SIZE_SZ)
#define MALLOC_ALIGN_MASK      (MALLOC_ALIGNMENT - 1)

#define MINSIZE  \
    (unsigned long)(((MIN_CHUNK_SIZE+MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK))

#define request2size(req)                                  \
    (((req) + SIZE_SZ + MALLOC_ALIGN_MASK < MINSIZE)  ?    \        
    MINSIZE :                                              \       
    ((req) + SIZE_SZ + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK)

void malloc_mem_control_block()
{
    printf("sizeof(size_t) = %d\n", sizeof(size_t)); //8字节
    printf("sizeof(struct malloc_block) = %d\n", sizeof(struct malloc_block)); //16字节

    //最起始的堆内存地址
    void* curBrk = sbrk(0);
    printf("curBrk = %#x\n", curBrk);

    printf("-----------------------------\n");
    void* ptr1 = malloc(20);
    memset(ptr1, 0x01, 20);
    printf("ptr1 = %#x\n", ptr1);
    printf("ptr1 malloc_usable_size = %zu\n", malloc_usable_size(ptr1));
    cur_chunk_data(ptr1);
    printf("-----------------------------\n");

    //前面的mem_control_block或者是malloc_block应该是8个字节空间,但是输出的第一个字节是 size(开辟的size+sizeof(mem_control_block)) + isFree;例如size = 104,第一个字节输出是0x71，104 + 对齐8的字节 = 0x70;对齐规则:(size + sizeof(mem_control_block) + 对其的字节)%16 == 0(64位系统)
    

    //问题:而且很奇怪的一个点是block的header只有size = 8字节,并没有glibc中的pre_size??????
    //答案:当一个chunk为空闲时，至少要有prev_size、size、fd和bk四个参数，因此MINSIZE就代表了这四个参数需要占用的内存大小；而当一个chunk被使用时，prev_size可能会被前一个chunk用来存储，fd和bk也会被当作内存存储数据，因此当chunk被使用时，只剩下了size参数需要设置，request2size中的SIZE_SZ就是INTERNAL_SIZE_T类型的大小，因此至少需要req+SIZE_SZ的内存大小。MALLOC_ALIGN_MASK用来对齐，因此request2size就计算出了所需的chunk的大小
    //从request2size(req)可以看出整个block的大小是(size + size_t + 补齐16字节)
    //--------------------
    //.     .            .
    //.     .            .
    //--------------------
    void* ptr2 = malloc(112);
    memset(ptr2, 0x02, 112);
    printf("ptr2 = %#x\n", ptr2);
    printf("ptr2 malloc_usable_size = %zu\n", malloc_usable_size(ptr2));
    cur_chunk_data(ptr2);
    printAddrData1Byte(ptr1, ptr2);
    printf("-----------------------------\n");

    void* ptr3 = malloc(121);
    printf("ptr3 = %#x\n", ptr3);
    memset(ptr3, 0x03, 121);
    printf("ptr3 malloc_usable_size = %zu\n", malloc_usable_size(ptr3));
    cur_chunk_data(ptr3);
    printf("-----------------------------\n");

    //貌似超过130KB还是在堆中生成,而不是在mmap内存映射文件;但是试了200KB就是内存地址不一样了.
    void* ptr4 = malloc(200 * 1024);
    printf("ptr4 = %#x\n", ptr4);
    memset(ptr4, 0x04, 200 * 1024);
    cur_chunk_data(ptr4);
    printf("-----------------------------\n");

    // 第一个curBrk和最后的curBrk相差0x21000, 33 * 4KB,则malloc申请内存时,系统会一次行映射33个内存页
    curBrk = sbrk(0);
    printf("curBrk = %#x\n", curBrk);

    free(ptr1);
    free(ptr2);
    free(ptr3);
    free(ptr4);
    // printAddrData1Byte((int*)ptr4 - 2, ptr4); //printf:0xffffffa0 0 0 0 0 0 0 0

    //进程结束后,页表被删除了,即被映射的虚拟内存和物理内存的关系不存在了,所有的资源也被回收了,内存泄露的问题也就不存在了,所以内存泄露最大的担心点是出现在长时间运行程序,比如守护进程,服务等.

    // printf("-------------------free----------------\n");
    // cur_chunk_data(ptr1);
    // cur_chunk_data(ptr2);
    // cur_chunk_data(ptr3);
}

void printAddrData1Byte(void* startAddr, void* endAddr)
{
    printf("printf startAddr = %#x to endAddr = %#x data\n", startAddr, endAddr);
    char* pMove = (char*)startAddr;
    int i = 0;
    while(pMove < endAddr)
    {
        printf("%#x\040", *pMove);
        pMove += 1;
        i++;
        if(!(i % 8))
            printf("\n");
    }
}

void printMCB(void* ptr)
{
    // struct mem_control_block* mcb;
    // mcb = (struct mem_control_block*)((char*)ptr - sizeof(struct mem_control_block));
    // printf("ptr = %#x; mcb->is_available = %d; mcb->size = %d\n", ptr, mcb->is_available, mcb->size);
}

void cur_chunk_data(void* mem)
{
    mchunkptr pchunk = mem2chunk(mem);
    printf("mem = %#x, pchunk = %#x,size before chunksize = %d, size = %zu,, pre_size = %d\n", mem, pchunk, pchunk->size, chunksize(pchunk), pchunk->pre_size);
}

void pre_chunk_data(void* mem)
{
    mchunkptr pchunk = mem2chunk(mem);
    //If prev_inuse is set for any given chunk, then you CANNOT determine the size of the previous chunk, and might even get a memory addressing fault when trying to do so.
    //那官方的malloc_block结构的各个字段什么时候发挥作用呢???(在free之后空闲链表中发挥作用)
    mchunkptr prechunk = pre_chunk(pchunk);
    printf("prechunk = %#x, prechunk->size = %zu\n", prechunk, chunksize(prechunk));
}

void next_chunk_data(void* mem)
{

}

void access_free_data() {
    int *p = (int*)malloc(4 * 4);
    for(int i = 0; i < 4; i++){
        p[i] = i + 1;
    }
    free(p);
    printf("p[2] = %d\n", p[2]); //输出p[2] = 3,实验证明，即使释放了那个块还是存在的，只是将free设置成1，所以为什么malloc之后需要记得初始化，再访问，防止出现上个块使用后遗留的脏数据
}
