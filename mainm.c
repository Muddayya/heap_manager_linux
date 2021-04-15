
#include <stdio.h>
#include <unistd.h>
#include "mm.h"

typedef struct test
{
    int a, b;
    char c;

} test;

int main(int argc, char **argv)
{
    test *temp, *tmp1, *tmp2;
    temp = (test *)Xmalloc(sizeof(test));
    tmp1 = (test *)Xmalloc(sizeof(test));
    tmp2 = (test *)Xmalloc(sizeof(test));

    tmp1->a = 4;
    tmp2->b = 13;
    printf("%d and %d", tmp1->a, tmp2->b);
    return 0;
}