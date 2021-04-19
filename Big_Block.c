#include <stdio.h>
#include "Big_Block.h"
#include <limits.h>
#include <error.h>

big_block_list bigBlockList;

void BigBlockList_init()
{

    for (int i = 0; i < MAX_SIZE_FOR_BIG_BLOCK; i++)
    {

        bigBlockList.bigBlock[i].blockptr = NULL;
        bigBlockList.bigBlock[i].availablesize = 0;
        bigBlockList.bigBlock[i].offset = 0;
    }
    return;
}

int splitBigBlock(meta_data mptr, size_t bytes)
{

    if (mptr->blockSize <= bytes)
    {
        return INT_MIN;
    }

    meta_data temp = NULL, next = NULL;
    size_t size = mptr->blockSize;
    temp = mptr;
    temp->blockSize = bytes;

    temp->next = (meta_data)(((char *)temp) + METABLOCK_SIZE + bytes);

    next = temp->next;
    next->prev = temp;
    next->next = temp->next->next;
    next->blockSize = (size - (METABLOCK_SIZE + bytes));
    next->isFree = TRUE;
    next->head = temp->head;

    return 1;
}

int mergeBigBlock(meta_data B1)
{

    if (!B1 || B1->next == NULL || B1->isFree == FALSE || B1->next->isFree == FALSE)
        return INT_MIN;

    meta_data m;
    m = B1->next;
    size_t nextsize = m->blockSize;
    B1->next = m->next;
    m->next->prev = B1;
    B1->blockSize += METABLOCK_SIZE + nextsize;
    return 1;
}

void *ReturnBigBlock(size_t bytes)
{

    int i = 0, requirepages;
    while (i <= bigBlockList.count && bytes <= bigBlockList.bigBlock[i].availablesize)
    {
        i++;
    }

    if (i >= bigBlockList.count && bigBlockList.bigBlock[0].blockptr == NULL && bigBlockList.bigBlock[0].availablesize == 0 && bigBlockList.bigBlock[0].offset == 0)
    {
        requirepages = 4 > ((bytes + METABLOCK_SIZE) / SYSTEM_PAGE_SIZE) ? 4 : ((bytes + METABLOCK_SIZE) / SYSTEM_PAGE_SIZE) + 1;
        bigBlockList.bigBlock[0].blockptr = (meta_data)mm_getnewvm(requirepages);
        bigBlockList.bigBlock[0].offset = 0;
        bigBlockList.bigBlock[0].availablesize = bytes;
        bigBlockList.count = 0;
        meta_data temp;
        temp = bigBlockList.bigBlock[0].blockptr;
        temp->blockSize = bytes;
        temp->prev = NULL;
        temp->next = NULL;
        temp->isFree = TRUE;
        temp->head = temp;
        i = 0;
    }

    if (i <= bigBlockList.count)
    {

        meta_data ptr;
        ptr = bigBlockList.bigBlock[i].blockptr;
        while (ptr->blockSize < bytes)
        {
            ptr = ptr->next;
        }
        ptr->head = bigBlockList.bigBlock[i].blockptr;
        int buf = ptr->blockSize % bytes;
        if (buf < (METABLOCK_SIZE + 1024))
        {
            return (void *)(ptr + 1);
        }
        else
        {
            if (splitBigBlock(ptr, bytes) == INT_MIN)
            {
                return NULL;
            }
            else
            {
                return (void *)(ptr + 1);
            }
        }
    }

    else
    {

        bigBlockList.count += 1;
        if (bigBlockList.count >= MAX_SIZE_FOR_BIG_BLOCK)
        {
            return NULL;
        }
        requirepages = 4 > ((bytes + METABLOCK_SIZE) / SYSTEM_PAGE_SIZE) ? 4 : ((bytes + METABLOCK_SIZE) / SYSTEM_PAGE_SIZE) + 1;
        bigBlockList.bigBlock[bigBlockList.count].blockptr = (meta_data)mm_getnewvm(requirepages);
        bigBlockList.bigBlock[bigBlockList.count].offset = 0;
        bigBlockList.bigBlock[bigBlockList.count].availablesize = requirepages * SYSTEM_PAGE_SIZE - METABLOCK_SIZE;
        meta_data temp, a;
        temp = bigBlockList.bigBlock[bigBlockList.count].blockptr;
        temp->blockSize = bytes;
        temp->prev = NULL;
        temp->next = (meta_data)((char *)temp + bytes + METABLOCK_SIZE);
        temp->isFree = FALSE;
        temp->head = temp;

        bigBlockList.bigBlock[bigBlockList.count].availablesize -= (bytes + METABLOCK_SIZE);
        bigBlockList.bigBlock[bigBlockList.count].offset = METABLOCK_SIZE + bytes;
        a = temp->next;
        a->prev = temp;
        a->next = NULL;
        a->blockSize = bigBlockList.bigBlock[bigBlockList.count].availablesize;
        a->isFree = TRUE;
        a->head = temp;
        return (void *)(temp + 1);
    }
}

int isBigBlockPageEmpty(meta_data head)
{

    meta_data block = head;
    while (block != NULL)
    {
        if (block->isFree == FALSE)
        {
            return FALSE;
        }
        block = block->next;
    }
    return TRUE;
}

void RemoveBigBlockPage(meta_data head)
{
    int i = 0;
    while (i < MAX_SIZE_FOR_BIG_BLOCK && head != bigBlockList.bigBlock[i].blockptr)
    {
        i++;
    }

    if (i == MAX_SIZE_FOR_BIG_BLOCK)
        return;

    big_block temp = bigBlockList.bigBlock[i];
    bigBlockList.bigBlock[i] = bigBlockList.bigBlock[bigBlockList.count];
    bigBlockList.bigBlock[bigBlockList.count].offset = 0;
    bigBlockList.bigBlock[bigBlockList.count].availablesize = 0;
    bigBlockList.bigBlock[bigBlockList.count].blockptr = NULL;
    bigBlockList.count -= 1;
    mm_returnvm(head, 4);
    return;
}
