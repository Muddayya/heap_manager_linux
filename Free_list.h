#ifndef FREE_LIST_H
#define FREE_LIST_H

#include "Big_Block.h"
#define FREE_LIST_SIZE 100
#define LCHILD(i) (2 * i + 1)
#define RCHILD(i) (2 * i + 2)
#define PARENT(i) ((i - 1) / 2)

typedef struct _free_list
{
    meta_data blockptr;
    int blocksize;
} _free_list;

typedef struct free_list
{
    _free_list List[FREE_LIST_SIZE];
    int rear;
    int size;
} free_list;

extern free_list freeList;

void init_freelist(free_list *l1);
void SwapListElements(free_list *l1, int i, int j);
int addBlockTofreeList(free_list *l1, meta_data ptr);

void heapifyFreeList();
void RemoveBlockFromFreeList(free_list *l1);
int deleteBlockfromFreeList(free_list *l1, int size, meta_data m1);

int sortFreeList(free_list *l1);

#endif