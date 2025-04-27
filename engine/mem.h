#ifndef __engine_mem__
#define __engine_mem__

#include "include.h"

#define GrowCapacity(cap) ((cap) == 0 ? 8 : (cap) * 2)
#define GrowArray(T, ptr, e) ((T*)realloc(ptr, sizeof(T) * (e)))
#define GrowVoidArray(size, ptr, e) ((void*)realloc((size) * (e)))
#define CreateArray(T, e) ((T*)malloc(sizeof(T) * (e)))
#define CreateVoidArray(size, e) ((void*)malloc((size) * (e)))
#define Create(T) ((T*)malloc(sizeof(T)))
#define Destroy(ptr) free(ptr)

#endif
