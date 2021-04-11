#ifndef mm_H
#define mm_H


#include <stdint.h>
#include <stddef.h>
#define TRUE 1
#define FALSE 0
#define METABLOCK_SIZE sizeof(meta_data_block_)
#define NUMBER_OF_BINS_PER_PAGE  SYSTEM_PAGE_SIZE/(BIN_SIZE+METABLOCK_SIZE)

extern size_t SYSTEM_PAGE_SIZE ;

typedef struct meta_data_block_ {

    int isFree;
    uint32_t blockSize;
    struct meta_data_block_ *prevBlock;
    struct meta_data_block_ *nextBlock;
    struct meta_data_block_ *headPtr;
}meta_data_block_;

typedef meta_data_block_ * meta_data_block;
void mm_init();
void * mm_getnewvm (int units);
void mm_returnvm(void *vm_page, int units);
void * mymalloc(size_t bytes);
    
#endif