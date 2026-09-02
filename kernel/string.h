#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include "types.h"

void *memcpy(void *dest, const void *src, uint32_t n);
void *memset(void *dest, int value, uint32_t n);

#endif
