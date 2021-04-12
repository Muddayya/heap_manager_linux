#ifndef SIZECLASSES_H
#define SIZECLASSES_H

#include "mm.h"

#define MAX_PAGES 5
#define CLASS_SIZE_LIMIT 1024
#define NUM_OF_CLASSES 42
#define TOTAL_BINS_IN_CLASS(no_of_pages,class_size)  (no_of_pages*SYSTEM_PAGE_SIZE)/(METABLOCK_SIZE+class_size)


typedef struct page_list {

    meta_data head;
    int free_bins;
    
}page_list;

extern int classSizeArray[NUM_OF_CLASSES];
// extern int isInit;
static page_list pageList[MAX_PAGES];
extern page_list sizeClassList[NUM_OF_CLASSES][MAX_PAGES];


void create_size_class_binlist (meta_data *head,int binSize, int no_of_pages);
meta_data get_free_page(int sizeclass);
void allocate_sizeClass_page(int sizeclass,int offset);
void SizeClassList_init ();




#endif