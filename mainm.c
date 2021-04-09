
#include<stdio.h>
#include<unistd.h>
#include"mm.h"


int main(int argc, char **argv) {
    mm_init();
    size_t sz= getpagesize(); // gets the size of virtual page
    printf(" VM PAGE SIZE = %zu \n",sz);
    void *addr1 = mm_getnewvm(3); // virtual page is allocated 
    void *addr2 = mm_getnewvm(1);
    printf("page 1 = %p, page 2 = %p\n", addr1 ,addr2);
    return 0;
}