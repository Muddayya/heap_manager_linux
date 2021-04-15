#include "free_size_list.h"

#include<stdio.h>


free_size_list freeSizeClassList[NUM_OF_CLASSES];

void init_free_size_list(){

    for(int i =0; i<NUM_OF_CLASSES; i++){

        for(int j=0; j<free_size_list_size; j++){

            freeSizeClassList[i].B[j].blockadress = NULL;
            freeSizeClassList[i].B[j].blocksize = 0;
            freeSizeClassList[i].B[j].offset = -1;

        }

        freeSizeClassList[i].rear = -1;
    }
}

int is_freeSizeClassList_empty(int sizeclass) {

    for(int j=0; j< free_size_list_size; j++){

        if(freeSizeClassList[sizeclass].B[j].blockadress != NULL && freeSizeClassList[sizeclass].B[j].blocksize!=0){
            return FALSE;
        }
    }
    
    return TRUE;
}

void addBlocktoFreeSizeCLassList(meta_data ptr, int size, int offset){

    int i=0;
    while(size> classSizeArray[i]){
        i++;
    }
    if(freeSizeClassList[i].rear >= free_size_list_size){
        //printf("free list is full")
        return;
    }
    freeSizeClassList[i].rear++;
    freeSizeClassList[i].B[freeSizeClassList[i].rear].blockadress = ptr;
    freeSizeClassList[i].B[freeSizeClassList[i].rear].blocksize = size;
    freeSizeClassList[i].B[freeSizeClassList[i].rear].offset = offset;
    return;
}

void removeblockfromFreeSizeCLassList(meta_data ptr, int size){
    int i=0;
    while(size>classSizeArray[i]){
        i++;
    }
    int j=0;

    while(j<=freeSizeClassList[i].rear && ptr != freeSizeClassList[i].B[j].blockadress){
        j++;

    }

    if(j> freeSizeClassList[i].rear){
        //printf("no block available with given pointer")
        return;
    }

    _free_size_list temp;

    temp = freeSizeClassList[i].B[j];
    freeSizeClassList[i].B[j] = freeSizeClassList[i].B[freeSizeClassList[i].rear];
    freeSizeClassList[i].B[freeSizeClassList[i].rear] = temp;
    freeSizeClassList[i].B[freeSizeClassList[i].rear].blockadress =NULL;
    freeSizeClassList[i].B[freeSizeClassList[i].rear].blocksize = 0;
    freeSizeClassList[i].rear -= 1;  

    return;
}


void removeAllfreeBlocksFromOffset(int sizeclass, int offset){

    for(int i =0; i<= freeSizeClassList[sizeclass].rear; i++){

        if(freeSizeClassList[sizeclass].B[i].offset == offset){

            _free_size_list temp;
            temp = freeSizeClassList[sizeclass].B[i];
            freeSizeClassList[sizeclass].B[i] = freeSizeClassList[sizeclass].B[freeSizeClassList[sizeclass].rear];
            freeSizeClassList[sizeclass].B[freeSizeClassList[sizeclass].rear] = temp;
            freeSizeClassList[sizeclass].B[freeSizeClassList[sizeclass].rear].blockadress =NULL;
            freeSizeClassList[sizeclass].B[freeSizeClassList[sizeclass].rear].blocksize = 0;
            freeSizeClassList[sizeclass].B[freeSizeClassList[sizeclass].rear].offset = -1;

            freeSizeClassList[sizeclass].rear -= 1;
        }

    }
}

meta_data getfreeBlockfromFreelist(size_t bytes){
    int i =0;
    while(i<=NUM_OF_CLASSES && bytes>classSizeArray[i]){
        i++;
    }
    meta_data ptr;

    ptr = freeSizeClassList[i].B[0].blockadress;

    removeblockfromFreeSizeCLassList(freeSizeClassList[i].B[0].blockadress, freeSizeClassList[i].B[0].blocksize);
    return ptr;
}