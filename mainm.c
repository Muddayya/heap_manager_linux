
#include<stdio.h>
#include<unistd.h>
#include"mm.h"

typedef struct test{
    int a, b;
    char c;

}test;




int main(int argc, char **argv) {
    // mm_init();
    // size_t sz= getpagesize(); // gets the size of virtual page
    // printf(" VM PAGE SIZE = %zu \n",sz);
    // void *addr1 = mm_getnewvm(3); // virtual page is allocated 
    // void *addr2 = mm_getnewvm(1);
    // printf("page 1 = %p, page 2 = %p\n", addr1 ,addr2);
    test *temp,*tmp1, *tmp2;
    temp = (test *)Xmalloc(sizeof(test)); 
    tmp1 =  (test *)Xmalloc(sizeof(test));
    tmp2 =  (test *)Xmalloc(sizeof(test));

    tmp1->a = 4;
    tmp2->b = 13;
    printf("%d and %d", tmp1->a, tmp2->b);
    return 0;
}