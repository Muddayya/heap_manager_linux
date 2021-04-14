#include<stdio.h>
#include<memory.h>
#include<unistd.h>// for getpagesize
#include<sys/mman.h> // for mmap() and munmap() function calls
#include "mm.h"
#include"sizeclass.h"
#include"free_size_list.h"

size_t SYSTEM_PAGE_SIZE = 0;

void mm_init(){
  
  SYSTEM_PAGE_SIZE = getpagesize();
} 

// function to request VM page from kernel
void * mm_getnewvm (int units){
  char *vm_page = mmap(0, units*SYSTEM_PAGE_SIZE, PROT_READ|PROT_WRITE|PROT_EXEC,MAP_ANON|MAP_PRIVATE,0,0);
       

  if(vm_page == MAP_FAILED){
    printf("VM page allocation failed");
    return NULL;
  }
  memset(vm_page,0,units*SYSTEM_PAGE_SIZE);
  return ( void *)vm_page;
}
//function to return page to kernel
 void mm_returnvm(void *vm_page, int units) {

  if(munmap(vm_page, units * SYSTEM_PAGE_SIZE))
      printf("Error : COuld not rlease VM Page");
    
  return;
}


meta_data getFreeBin(meta_data head){
  meta_data a;
  a = head;
  while(a->isFree == FALSE && a->next != NULL){
    a = a->next;
  }

  return a;

}





void * Xmalloc(size_t bytes){

  static int init = FALSE;

  if(init == FALSE){
    mm_init();
    SizeClassList_init ();
    init_free_size_list();

    init = TRUE;
  
  }

  if(bytes<= 0)
    return NULL;
  

  if(bytes< CLASS_SIZE_LIMIT){

    int i = 0;
    while(bytes>classSizeArray[i]){
      i++;
    }

    if(is_freeSizeClassList_empty(i)==FALSE){
      return (void *)(getfreeBlockfromFreelist(bytes)+1);
    }



    meta_data ptr, mptr;
    //mm_init();
    ptr = get_free_page(i);

    int n=0;
    while(n<MAX_PAGES && sizeClassList[i][n].head != ptr){
      n++;
    }

    if(ptr == NULL){
      //printf("malloc allocation failed");
      return NULL;
    }
    

    mptr = getFreeBin(ptr);
    mptr->isFree = FALSE;
    mptr->head = ptr;

    return (void *) (mptr +1);

  }


}

void * Xfree(size_t bytes){
  
}