#ifndef TYPES_H
#define TYPES_H

/* Unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/* Signed integer types */
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* Boolean type */
typedef int bool;
#define true 1
#define false 0

/* Null pointer */
#define NULL ((void*)0)

/* Size type */
typedef unsigned int size_t;

/* Pointer-sized types */
typedef int ptrdiff_t;
typedef unsigned int uintptr_t;
typedef signed int intptr_t;

#endif
