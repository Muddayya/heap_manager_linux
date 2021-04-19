
#include <stdio.h>
#include <unistd.h>
#include "mm.h"

typedef struct temp
{
    int a, b, c, d;
} temp;

typedef struct largealloc
{

    char arr[1000];
} largealloc;

int main()
{

    largealloc *buf;
    buf = (largealloc *)Xmalloc(sizeof(largealloc));

    int *arr;
    arr = (int *)Xmalloc(sizeof(int) * 5);
    for (int i = 0; i < 5; i++)
    {
        arr[i] = i;
    }
    for (int j = 0; j < 5; j++)
    {
        printf("%d ", arr[j]);
    }
    printf("\n");
    arr = (int *)XRealloc(arr, sizeof(int) * 10);
    for (int i = 5; i < 10; i++)
    {
        arr[i] = i;
    }

    for (int j = 0; j < 10; j++)
    {
        printf("%d ", arr[j]);
    }
    printf("\n");

    Xfree(buf);
    printf("free1");
    Xfree(arr);
    int size;
    printf("enter size of array");
    scanf("%d", &size);
    int *a;
    a = (int *)Xmalloc(sizeof(int) * size);
    if (a == NULL)
    {
        printf("gandtay");
    }

    return 0;
}