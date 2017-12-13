#ifndef __VIOC_H__
#define __VIOC_H__

#include <list.h>
#include <vioc_rdma.h>

#define REG_OFFSET(offset)	(offset / 4)

#define read_reg(reg)		(*(volatile addr_t *)(reg))
#define write_reg(reg, val)	(*(volatile addr_t *)(reg) = (val))

#define BITSET(X, MASK)				((X) |= (unsigned int)(MASK))
#define BITSCLR(X, SMASK, CMASK)	((X) = ((((unsigned int)(X)) | ((unsigned int)(SMASK))) & ~((unsigned int)(CMASK))) )
#define BITCSET(X, CMASK, SMASK)	((X) = ((((unsigned int)(X)) & ~((unsigned int)(CMASK))) | ((unsigned int)(SMASK))) )
#define BITCLR(X, MASK)				((X) &= ~((unsigned int)(MASK)) )
#define BITXOR(X, MASK)				((X) ^= (unsigned int)(MASK) )
#define ISZERO(X, MASK)				(!(((unsigned int)(X))&((unsigned int)(MASK))))
#define	ISSET(X, MASK)				((unsigned long)(X)&((unsigned long)(MASK)))

typedef unsigned int addr_t;
typedef unsigned int reg_t;

enum vioc_components {
	RDMA,
	WDMA,
	WMIX,
	SC,
	LUT,
	OUTCFG,
	INTRCFG,
};

struct vioc_component_t {
	int id;						// component id
	int addr_offset;			// offset from vioc base address
	unsigned int reset_bit;		// soft-reset bit

	/* parsed data */
	int reg[160];				// get register's value from data(.txt);
	int nr_regs;				// number of parsed regs
};

struct vioc_rdma_t {
	struct vioc_component_t info;
	VIOC_RDMA *addr;				// virtual address of physical register (by mmap)
	VIOC_RDMA reg;					// parsed register values
};

struct test_case_t {
	addr_t *vioc_base_addr;
	struct vioc_rdma_t rdma;
};

struct test_data_t {
	int test_no;
	char test_name[64];
	struct vioc_component_t rdma;
	struct vioc_component_t wdma;
	struct vioc_component_t wmix;
	struct vioc_component_t sc;
	struct vioc_component_t lut;
	struct vioc_component_t outcfg;
	struct vioc_component_t intrcfg;
	struct list_head list;
};

#endif /*__VIOC_H__*/
