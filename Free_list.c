#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "Free_list.h"
#include <limits.h>
#include <error.h>


free_list freeList;

void init_freelist(free_list *l1){
    l1->size = FREE_LIST_SIZE;
    l1->rear = -1;
    for(int i = 0; i< FREE_LIST_SIZE;i++){
        l1->List[i].blockptr = NULL;
        l1->List[i].blocksize = 0;
    }

    return;
}


void SwapListElements(free_list *l1, int i, int j){
    _free_list temp;
    temp = l1->List[j];
    l1->List[j] = l1->List[i];
    l1->List[i] = temp;
    return;

}

int addBlockTofreeList(free_list *l1, meta_data ptr){

    if(l1->rear == l1->size-1){
        return NULL;
    }

    l1->rear++;
    l1->List[l1->rear].blockptr = ptr;
    l1->List[l1->rear].blocksize = ptr->blockSize;

    int i= l1->rear;
    while(i>0 && ptr->blockSize > l1->List[PARENT(i)].blocksize ){
        SwapListElements(l1, i, PARENT(i));
        i = PARENT(i);
    }

    return 1;
}



void heapifyFreeList() {


    for(int i = 0; i< freeList.rear; i++){

        if(freeList.List[i].blocksize > freeList.List[PARENT(i)].blocksize){
            int j=i;
            while(freeList.List[j].blocksize > freeList.List[PARENT(j)].blocksize){
                SwapListElements(&freeList, j, PARENT(j));
                j= PARENT(j);

            }


        }
    }
}

void RemoveBlockFromFreeList(free_list *l1){
    if(l1->rear==-1)
        return;
    
    _free_list x = l1->List[0];
    l1->List[0]=l1->List[l1->rear];
    l1->List[l1->rear] = x;
    l1->rear--;

    int i=0;
    int max=0;
    while(i<l1->rear){
        int left = LCHILD(i);
        int right = RCHILD(i);
        if(left>l1->rear)
            return;

        if(right>l1->rear){
            if(l1->List[i].blocksize< l1->List[left].blocksize){
                SwapListElements(l1, i, left);
                return;

            }

        }
        if(l1->List[left].blocksize > l1->List[right].blocksize)
            max = left;
        else
            max = right;

        SwapListElements(l1,i, max);
        i = max;      
    }

    return;
}

int  deleteBlockfromFreeList( free_list *l1, int size, meta_data m1){
    if(!l1|| l1->List == NULL){
        return INT_MIN;
    }
    int i=0;
    while(i< FREE_LIST_SIZE && l1->List[i].blocksize != size && l1->List[i].blockptr != m1){
        i++;
    }
    if(i>=FREE_LIST_SIZE){
        return INT_MIN;
    }
    l1->List[i] = l1->List[l1->rear];
    l1->List[l1->rear].blockptr = NULL;
    l1->List[l1->rear].blocksize = 0;
    l1->rear--;
    heapifyFreeList();
    return 1;
}

int sortFreeList(free_list *l1) {

    heapifyFreeList();
    for(int i = l1->rear;i>0;i++) {
        swapListElements(l1,0,i);

        int j = 0,index;

        do {
            index = LCHILD(j);
            if(l1->List[index].blocksize < l1->List[RCHILD(j)].blocksize && index < (i-1))
                i++;
            if(l1->List[j].blocksize < l1->List[index].blocksize && index < i)
                swapListElements(l1,j,index);
            j = index;
        }while(index < i);
    }
    return 1;
}
