#include <stdlib.h>

void *fisrt_block = NULL;
typedef struct s_block* t_block;
const int BLOCK_SIZE = 40;//由于存在虚拟的data字段，sizeof不能正确计算meta长度，所以固定了大小
struct s_block {
    size_t size; /*数据区大小*/
    t_block next;/*指向下个块的指针*/
    t_block pre; /*指向上个块的指针，为了合并block*/
    int free;    /*是否是空闲块*/
    int padding; /*填充4字节，保证meta块长度为8的倍数*/
    void* ptr;   /*Magic pointer,指向data，为了校验是该进程malloc出来的*/
    char data[1];/*虚拟字段，表示数据块的第一个字节，长度不计入meta，所以meta的长度是24字节，但是整个结构体是32字节*/
};
//寻找合适的block
/*
1.First fit:从头开始，使用第一个数据区大小大于要求size的块作为此次分配的块
2.Best fit：从头开始，遍历所有的块，使用数据区大小大于size且差值最小的块作为此次分配的块
*/
/*First fit*/
t_block find_block(t_block* last, size_t size) {
    //更新last，是为了找不到可用内存是，开辟新的block
    t_block b = fisrt_block;
    while(b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return b;
}

//开辟新的block
/*如果现有的block都不能满足size的要求，则需要在链表最后开辟一个新的block，关键是使用sbrk()创建*/
t_block extend_heap(t_block last, size_t s) {
    t_block b;
    b = sbrk(0);
    if(sbrk(BLOCK_SIZE + s) == (void*)-1)
        return NULL;
    b->size = s;
    b->next = NULL;
    if(last)
        last->next = b;
    b->free = 0;
    return b;
}

//分裂block
/*First fit时，在malloc新的堆空间时，如果很小的size占用了很大的一个block，此时，为了提高payload，将其分裂为一个新的block*/
void split_block(t_block b, size_t s) {
    t_block new_block;
    new_block = b->data + s;
    new_block->size = b->size - s - BLOCK_SIZE;
    new_block->next = b->next;
    new_block->free = 1;
    b->size = s;
    b->next = new_block;
}

//8字节对齐
/*和0x07位与操作，如果不对齐，则先右移3位+1,实则直接将后三位直接进位，然后再左移3位，达到原来s的位数大小*/
size_t align8(size_t s) {
    if(s & 0x7 == 0)
        return s;
    return ((s >> 3) + 1) << 3;
}

//malloc的实现
void* malloc(size_t size) {
    t_block b, last;
    size_t s;
    s = align8(size);
    if(fisrt_block) {
        last = fisrt_block;
        b = find_block(&last, s);
        if(b) {
            if((b->size - s) >= (BLOCK_SIZE + 8)) {
                split_block(b, s);
            }
            b->free = 0;
        } else {
            b = extend_heap(last, s);
            if(!b) {
                return NULL;
            }
        }
    } else {
        b = extend_heap(NULL, s);
        if(!b)
            return NULL;
        fisrt_block = b;
    }
    return b->data;
}


//calloc实现，将数据初始化为0
void* calloc(size_t number, size_t size){
    size_t *newMem; // 用size_t*指针，每一步为8个字节
    size_t s8, i;
    newMem = malloc(number * size);
    if(newMem) {
        s8 = align8(number * size) >> 3;
        for(i = 0; i < s8; i++)
            newMem[i] = 0;
    }
    return newMem;
}

//检查地址合法性
t_block get_block(void* p){
    char* tmp;
    tmp = p;
    return (p = tmp -= BLOCK_SIZE);
}

int valid_addr(void* p) {
    if(fisrt_block) {
        if(p > fisrt_block && p < sbrk(0))
            return p == (get_block(p))->ptr;
    }
    return 0;
}

//合并block，如果发现它相邻的block也是free的，则将block和相邻的block合并
t_block fusion(t_block b) {
    if(b->next && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if(b->next)
            b->next->prev = b;
    }
    return b;
}

//free的实现
void free(void* p) {
    t_block b;
    if(valid_addr(p)) {
        b = get_block(p);
        b->free = 1;
        if(b->prev && p->prev->free) {
            b = fusion(b->prev);
        }

        if(b->next)
            fusion(b);
        else {
            if(b->prev)
                b->prev->prev = NULL;
            else
                fisrt_block = NULL;
            brk(b); //回退break，相当于释放物理内存，否则堆内存地址在内存映射表中一直处于使用中？
        }
    }
}

void sizeof_mem_block(){
    printf("sizeof s_block = %zu\n", sizeof(s_block));
}