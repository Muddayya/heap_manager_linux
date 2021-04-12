#include <stdio.h>
#include <stdlib.h>
#include "sizeclass.h"



page_list sizeClassList[NUM_OF_CLASSES][MAX_PAGES];


int classSizeArray[NUM_OF_CLASSES] = {

    4,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,160,176,184,192,208,224,240,256,272,288,312,336,368,408,448,480,512,576,640,704,768,896,1024
};


void SizeClassList_init () {


    for(int i=0;i<NUM_OF_CLASSES;i++) {
        for(int j=0;j<MAX_PAGES;j++) {
            sizeClassList[i][j].head = NULL;
            sizeClassList[i][j].free_bins = -1;
        }
    }
    return;

}

void allocate_sizeClass_page(int sizeclass,int offset){

    if(sizeClassList[sizeclass][offset].head == NULL && sizeClassList[sizeclass][offset].free_bins == -1){

        if(sizeclass != 41)
        {
            sizeClassList[sizeclass][offset].head = (meta_data)mm_getnewvm(1);
            sizeClassList[sizeclass][offset].free_bins = TOTAL_BINS_IN_CLASS(1,classSizeArray[sizeclass]);
            create_size_class_binlist(&(sizeClassList[sizeclass][offset].head),classSizeArray[sizeclass],1);
        }
        else if(sizeclass == 41){
            sizeClassList[sizeclass][offset].head = (meta_data)mm_getnewvm(2);
            sizeClassList[sizeclass][offset].free_bins = TOTAL_BINS_IN_CLASS(2,classSizeArray[sizeclass]);
            create_size_class_binlist(&(sizeClassList[sizeclass][offset].head),classSizeArray[sizeclass],2);
        }
    }

    return;
}




void create_size_class_binlist (meta_data *head,int binSize, int no_of_pages){

    if(head == NULL)
        return;
    
    int binNum;
    binNum = no_of_pages*SYSTEM_PAGE_SIZE/(binSize+METABLOCK_SIZE);

    meta_data a, prev= NULL;
    a = *head;
    for(int i =0; i<binNum-1; i++){
        a->prev = prev;
        a->blockSize = binSize;
        a->head = *head;
        a->isFree = TRUE;
        a->next =(meta_data)(((char *)a)+METABLOCK_SIZE+binSize);
        prev = a;
        a= a->next;
    }
    a->prev = prev;
    a->blockSize = binSize;
    a->isFree = TRUE;
    a->next = NULL;
    return;
}

meta_data get_free_page(int sizeclass){
    int i =0;
    if(sizeClassList[sizeclass][0].head == NULL && sizeClassList[sizeclass][0].free_bins == -1){
        allocate_sizeClass_page(sizeclass,0);
        sizeClassList[sizeclass][0].free_bins -= 1;
        return sizeClassList[sizeclass][0].head;
    }

    while(i<MAX_PAGES && sizeClassList[sizeclass][i].free_bins<1 && sizeClassList[sizeclass][i].free_bins != -1){
        i++;
    }
    if(i>= MAX_PAGES)
        return NULL;
    
    else if (sizeClassList[sizeclass][i].free_bins >1){
        sizeClassList[sizeclass][i].free_bins -= 1;
        return sizeClassList[sizeclass][i].head;
    }

    else if (sizeClassList[sizeclass][i].free_bins == -1){
        allocate_sizeClass_page(sizeclass, i);
        sizeClassList[sizeclass][i].free_bins -= 1;
        return sizeClassList[sizeclass][i].head;
    }

    return NULL;
}