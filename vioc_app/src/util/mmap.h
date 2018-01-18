#ifndef __MMAP_H__
#define __MMAP_H__

#include <vioc.h>

addr_t *vioc_mmap(addr_t, unsigned int);
void vioc_munmap(addr_t *);

#endif
