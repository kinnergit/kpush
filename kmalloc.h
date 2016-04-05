#ifndef KPUSH_KMALLOC_H
#define KPUSH_KMALLOC_H

#include <stdlib.h>

void * kmalloc(size_t sz);
void kfree(void * data);

#endif //KPUSH_KMALLOC_H
