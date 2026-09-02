#include "string.h"

void *memset(void *dest, int value, uint32_t n)
{
    uint8_t *d = (uint8_t *)dest;

    while (n--)
        *d++ = (uint8_t)value;

    return dest;
}

void *memcpy(void *dest, const void *src, uint32_t n)
{
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    while (n--)
        *d++ = *s++;

    return dest;
}
