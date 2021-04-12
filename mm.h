#ifndef mm_H
#define mm_H


#include <stdint.h>
#include <stddef.h>
#define TRUE 1
#define FALSE 0
#define METABLOCK_SIZE sizeof(meta_data_)
#define NUMBER_OF_BINS_PER_PAGE  SYSTEM_PAGE_SIZE/(BIN_SIZE+METABLOCK_SIZE)

extern size_t SYSTEM_PAGE_SIZE ;

typedef struct meta_data_ {

    int isFree;
    uint32_t blockSize;
    struct meta_data_ *prev;
    struct meta_data_ *next;
    struct meta_data_ *head;
}meta_data_;

typedef meta_data_ * meta_data;
void mm_init();
void * mm_getnewvm (int units);
void mm_returnvm(void *vm_page, int units);
meta_data getFreeBin(meta_data head);
void * Xmalloc(size_t bytes);
    
#endif