#include "kmalloc.h"

void * kmalloc(size_t sz)
{
    return malloc(sz);
}

void kfree(void * data)
{
    if (data != NULL) {
        free(data);
        data = NULL;
    }
}
