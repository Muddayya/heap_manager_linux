#include <stdio.h>
#include <memory.h>
#include <unistd.h>   // for getpagesize
#include <sys/mman.h> // for mmap() and munmap() function calls
#include <stdlib.h>
#include <limits.h>
#include "mm.h"
#include "sizeclass.h"
#include "free_size_list.h"
#include "Big_Block.h"
#include "Free_list.h"

size_t SYSTEM_PAGE_SIZE = 0;

void mm_init()
{

  SYSTEM_PAGE_SIZE = getpagesize();
}

// function to request VM page from kernel
void *mm_getnewvm(int units)
{
  char *vm_page = mmap(0, units * SYSTEM_PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, 0, 0);

  if (vm_page == MAP_FAILED)
  {
    printf("VM page allocation failed");
    return NULL;
  }
  memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
  return (void *)vm_page;
}
//function to return page to kernel
void mm_returnvm(void *vm_page, int units)
{

  if (munmap(vm_page, units * SYSTEM_PAGE_SIZE))
    printf("Error : COuld not rlease VM Page");

  return;
}

meta_data getFreeBin(meta_data head)
{
  meta_data a;
  a = head;
  while (a->isFree == FALSE && a->next != NULL)
  {
    a = a->next;
  }

  return a;
}

void *Xmalloc(size_t bytes)
{

  static int init = FALSE;

  if (init == FALSE)
  {
    mm_init();
    SizeClassList_init();
    init_free_size_list();
    init_freelist(&freeList);

    init = TRUE;
  }

  if (bytes <= 0)
    return NULL;

  if (bytes < CLASS_SIZE_LIMIT)
  {

    int i = 0;
    while (bytes > classSizeArray[i])
    {
      i++;
    }

    if (is_freeSizeClassList_empty(i) == FALSE)
    {
      return (void *)(getfreeBlockfromFreelist(bytes) + 1);
    }

    meta_data ptr, mptr;
    //mm_init();
    ptr = get_free_page(i);

    // int n = 0;
    // while (n < MAX_PAGES && sizeClassList[i][n].head != ptr)
    // {
    //   n++;
    // }

    if (ptr == NULL)
    {
      //printf("malloc allocation failed");
      return NULL;
    }

    mptr = getFreeBin(ptr);
    mptr->isFree = FALSE;
    mptr->head = ptr;

    return (void *)(mptr + 1);
  }

  else
  {

    sortFreeList(&freeList);
    int i = 0;
    while (i <= freeList.rear && bytes > freeList.List[i].blocksize)
      i++;
    if (i <= freeList.rear)
    {

      meta_data bptr;

      if (freeList.List[i].blocksize > (bytes + METABLOCK_SIZE + CLASS_SIZE_LIMIT))
      {
        if (splitBigBlock(freeList.List[i].blockptr, freeList.List[i].blocksize) == INT_MIN)
        {
          return NULL;
        }
        addBlockTofreeList(&freeList, freeList.List[i].blockptr->next);
        bptr = freeList.List[i].blockptr;
        deleteBlockfromFreeList(&freeList, freeList.List[i].blocksize, freeList.List[i].blockptr);
        return (void *)(bptr + 1);
      }
      bptr = freeList.List[i].blockptr;
      deleteBlockfromFreeList(&freeList, freeList.List[i].blocksize, freeList.List[i].blockptr);
      return (void *)(bptr + 1);
    }

    else
    {

      return ReturnBigBlock(bytes);
    }
  }
  return NULL;
}

void Xfree(void *ptr)
{
  meta_data mptr;
  mptr = ((meta_data)ptr) - 1;

  if (mptr == NULL)
  {
    //printf("cannot free null pointer ")
    return;
  }

  if (mptr->isFree == TRUE)
  {
    //printf("memory is free")
    return;
  }

  if (mptr->blockSize < CLASS_SIZE_LIMIT)
  {

    meta_data head;
    head = mptr->head;

    int i = 0;
    while (i < NUM_OF_CLASSES && mptr->blockSize > classSizeArray[i])
    {
      i++;
    }

    static int isinit = FALSE;

    if (isinit == FALSE)
    {
      init_free_size_list();
      isinit = TRUE;
    }

    int offset = 0;

    while (offset < MAX_PAGES && head != sizeClassList[i][offset].head)
    {
      offset++;
    }

    mptr->isFree = TRUE;
    if (isSizeClassPageEmpty(i, offset) == TRUE)
    {

      removeAllfreeBlocksFromOffset(i, offset); //remove all the blocks from the page which is empty
      RemoveEmptypage(i, offset);               // send the empty page back to kernel using munmap fuction call
      return;
    }
    else
      addBlocktoFreeSizeCLassList(mptr, mptr->blockSize, offset);
  }

  else
  {
    // Memory block belongs to large alloc
    meta_data next = mptr->next;
    meta_data prev = mptr->prev;
    meta_data head;
    // merging next and prev memory blocks if they are free
    if (next != NULL && next->isFree == TRUE)
    {
      mergeBigBlock(mptr);
    }
    if (prev != NULL && prev->isFree == TRUE)
    {
      mergeBigBlock(prev);
      if (isBigBlockPageEmpty(prev->head) == TRUE)
      {
        // return page to the kernel if all blocks are free
        head = prev->head;
        for (int i = 0; i <= freeList.rear; i++)
        {

          if (freeList.List[i].blockptr->head == head)
            deleteBlockfromFreeList(&freeList, freeList.List[i].blocksize, freeList.List[i].blockptr);
        }
        RemoveBigBlockPage(head);
      }
      else
      {
        addBlockTofreeList(&freeList, prev);
      }
    }
    else
    {

      if (isBigBlockPageEmpty(mptr->head) == TRUE)
      {
        head = mptr->head;
        for (int i = 0; i <= freeList.rear; i++)
        {
          if (freeList.List[i].blockptr->head == head)
            deleteBlockfromFreeList(&freeList, freeList.List[i].blocksize, freeList.List[i].blockptr);
        }
        RemoveBigBlockPage(head);
      }
      else
        addBlockTofreeList(&freeList, mptr);
    }
  }
  return;
}

void *XCalloc(size_t num, size_t size)
{

  void *ptr;

  ptr = Xmalloc(num * size);
  if (ptr == NULL)
  {
    return ptr;
  }
  // Initializing it to zero
  memset(ptr, 0, num * size);
  return ptr;
}

void *XRealloc(void *ptr, size_t size)
{

  meta_data mptr;
  if (ptr == NULL)
    return Xmalloc(size);
  mptr = ((meta_data)ptr) - 1;
  if (size < 0)
    return NULL;
  if (size == 0)
  {
    Xfree(ptr);
    return NULL;
  }

  void *newPtr;
  newPtr = Xmalloc(size);
  if (newPtr == NULL)
    return NULL;
  // Moving contents to the memory to the new allocated location
  if (mptr->blockSize < size)
    memmove(newPtr, ptr, mptr->blockSize);
  else
    memmove(newPtr, ptr, size);
  Xfree(ptr);
  return newPtr;
}
