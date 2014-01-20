#include "mem.h"

static inline
size_t min(size_t a, size_t b) {
    return (a<b)?a:b;
}

static
void init() {
    static int first=1;
    if (first) {
        //dprintf("init\n");
        mem_init(MEMORY_SIZE);
        first = 0;
    }
}

void *malloc(size_t s) {
    void *result;

    init();
    //dprintf("Allocation de %lu octets\n", (unsigned long) s);
    result = mem_alloc(s);
    if (!result)
        PutString(" Alloc FAILED !!\n");
    return result;
}

void *calloc(size_t count, size_t size) {
    int i;
    char *p;
    size_t s = count*size;

    init();
    //dprintf("Allocation de %zu octets\n", s);
    p = mem_alloc(s);
    if (!p)
        PutString(" Alloc FAILED !!\n");
    if (p)
        for (i=0; i<s; i++)
            p[i] = 0;
    return p;
}

void *realloc(void *ptr, size_t size) {
    size_t s;
    char *result;

    init();
    //dprintf("\nReallocation de la zone en %lx\n", (unsigned long) ptr);
    if (!ptr) {
        //dprintf(" Realloc of NULL pointer\n");
        return mem_alloc(size);
    }
    if (mem_get_size(ptr) >= size) {
        //dprintf(" Useless realloc\n");
        return ptr;
    }
    result = mem_alloc(size);
    if (!result) {
        PutString(" Realloc FAILED !!\n");
        return NULL;
    }
    for (s = 0; s<mem_get_size(ptr); s++)
        result[s] = ((char *) ptr)[s];
    mem_free(ptr);
    //dprintf(" Realloc ok\n");
    return result;
}

void free(void *ptr) {
    init();
    if (ptr) {
        //dprintf("Liberation de la zone en %lx\n", (unsigned long) ptr);
        mem_free(ptr);
    } else {
        //dprintf("Liberation de la zone NULL\n");
    }
}
