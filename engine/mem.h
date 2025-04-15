#ifndef __bse_mem__
#define __bse_mem__

// definitions so I can change the allocator if I so choose
#define create(T) (T*)malloc(sizeof(T))
#define createArr(T, size) (T*)malloc(sizeof(T) * (size))
#define createSized(size) malloc(size)
#define resizeArr(T, ptr, size) (T*)realloc(ptr, sizeof(T) * (size))
#define destroy(ptr) free(ptr)

#endif
