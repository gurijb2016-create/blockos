#ifndef STDIO_H
#define STDIO_H

#include "../kernel/types.h"

/* Print functions */
void printf(const char* format, ...);
void putchar(char c);
void puts(const char* str);
void putd(int num);
void puth(uint32_t num);

/* Scan functions */
int scanf(const char* format, ...);
int getchar(void);
char* gets(char* buffer, int size);

/* File operations */
typedef struct {
    int fd;
    uint32_t position;
    uint32_t size;
} FILE;

FILE* fopen(const char* filename, const char* mode);
int fclose(FILE* file);
int fread(void* buffer, uint32_t size, uint32_t count, FILE* file);
int fwrite(const void* buffer, uint32_t size, uint32_t count, FILE* file);
int fseek(FILE* file, int offset, int origin);

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#endif
