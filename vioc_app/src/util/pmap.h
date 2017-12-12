#ifndef __PMAP_H__
#define __PMAP_H__

struct pmap_t {
    unsigned int base;
    unsigned int size;
};

int get_pmap(char *, struct pmap_t *);
int test_pmap(void);

#endif
