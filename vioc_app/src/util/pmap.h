#ifndef __PMAP_H__
#define __PMAP_H__

typedef struct {
    unsigned int base;
    unsigned int size;
} pmap_t;

int test_pmap(void);

#endif
