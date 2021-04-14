#ifndef free_size_list_H
#define free_size_list_H

#include "sizeclass.h"

#define free_size_list_size 100

typedef struct  _free_size_list{

    meta_data blockadress;
    int blocksize;
    int offset;

} _free_size_list;

typedef struct free_size_list{

    _free_size_list B[free_size_list_size];
    int rear;

} free_size_list;


extern free_size_list freeSizeClassList[NUM_OF_CLASSES];

void init_free_size_list();
int is_freeSizeClassList_empty(int sizeclass);
void removeblockfromFreeSizeCLassList(meta_data ptr, int size);
void removeAllfreeBlocksFromOffset(int sizeclass, int offset);
meta_data getfreeBlockfromFreelist(size_t bytes)

#endif