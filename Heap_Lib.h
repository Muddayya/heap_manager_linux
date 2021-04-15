#ifndef MALLOC_H
#define MALLOC_H

#include <stdint.h>
#include <stddef.h>

void *Xmalloc(size_t bytes);

void *XCalloc(size_t num, size_t size);

void *Xrealloc(void *ptr, size_t size);

void Xfree(void *ptr);

#endif