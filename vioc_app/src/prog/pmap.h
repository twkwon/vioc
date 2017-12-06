#ifndef __PMAP_H__
#define __PMAP_H__

typedef struct {
    unsigned int base;
    unsigned int size;
} pmap_t;

extern int test_pmap(void);
extern int pmap_get_info(const char *name, pmap_t *mem);

#endif
