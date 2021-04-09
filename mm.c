#include<stdio.h>
#include<memory.h>
#include<unistd.h>// for getpagesize
#include<sys/mman.h> // for mmap() and munmap() function calls


static size_t SYSTEM_PAGE_SIZE = 0;

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
    return ( void *)vm_page;
}
//function to return page to kernel
 void mm_returnvm(void *vm_page, int units) {
   if(munmap(vm_page, units * SYSTEM_PAGE_SIZE))
      printf("Error : COuld not rlease VM Page");
}