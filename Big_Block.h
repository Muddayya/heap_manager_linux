#ifndef Big_Block.H
#define Big_Block.H

#include "mm.h"

#define MAX_SIZE_FOR_BIG_BLOCK 100

typedef struct big_block{
    meta_data blockptr;
    int offset;
    int availablesize;

}big_block;



typedef struct big_block_list{
    big_block bigBlock[MAX_SIZE_FOR_BIG_BLOCK];
    int count;
}big_block_list;

extern big_block_list bigBlockList;

void BigBlockList_init();

int mergeBigBlock(meta_data B1);

void* ReturnBigBlock( size_t bytes);
int isBigBlockPageEmpty(meta_data head);
void RemoveBigBlockPage(meta_data head);




#endif