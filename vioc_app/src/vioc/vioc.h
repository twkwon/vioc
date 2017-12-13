#ifndef __VIOC_H__
#define __VIOC_H__

#include <list.h>
#include <vioc_rdma.h>
#include <vioc_wdma.h>
#include <vioc_wmix.h>
#include <vioc_scaler.h>
#include <vioc_lut.h>
#include <vioc_outcfg.h>
#include <vioc_ireq.h>

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

/* offset from vioc base address */
#define OFFSET_RDMA_FROM_VIOC_BASE		(HwVIOC_RDMA00 - BASE_ADDR_VIOC)
#define OFFSET_WDMA_FROM_VIOC_BASE		(HwVIOC_WDMA00 - BASE_ADDR_VIOC)
#define OFFSET_WMIX_FROM_VIOC_BASE		(HwVIOC_WMIX0 - BASE_ADDR_VIOC)
#define OFFSET_SC_FROM_VIOC_BASE		(HwVIOC_SC0 - BASE_ADDR_VIOC)
#define OFFSET_LUT_FROM_VIOC_BASE		(HwVIOC_LUT - BASE_ADDR_VIOC)
#define OFFSET_OUTCFG_FROM_VIOC_BASE	(HwVIOC_OUTCFG - BASE_ADDR_VIOC)
#define OFFSET_CONFIG_FROM_VIOC_BASE	(HwVIOC_CONFIG - BASE_ADDR_VIOC)


typedef unsigned int addr_t;
typedef unsigned int reg_t;

enum vioc_components {
	VC_START = 0,
	VC_RDMA = VC_START,
	VC_WDMA,
	VC_WMIX,
	VC_SC,
	VC_LUT,
	VC_OUTCFG,
	VC_CONFIG,
	VC_END = VC_CONFIG,
};

/*
 *-----------------------------------------------------------------------------
 * storage for vioc information & registers
 * main structure is test_cast_t
 *-----------------------------------------------------------------------------
 */

struct vioc_component_t {
	int id;						// component id
	int addr_offset;			// offset from vioc base address
	unsigned int reset_bit;		// soft-reset bit
};

struct vioc_rdma_t {
	struct vioc_component_t info;
	VIOC_RDMA *addr;			// virtual address of physical register (by mmap)
	VIOC_RDMA reg;				// parsed register values
};

struct vioc_wdma_t {
	struct vioc_component_t info;
	VIOC_WDMA *addr;
	VIOC_WDMA reg;
};

struct vioc_wmix_t {
	struct vioc_component_t info;
	VIOC_WMIX *addr;
	VIOC_WMIX reg;
};

struct vioc_sc_t {
	struct vioc_component_t info;
	VIOC_SC *addr;
	VIOC_SC reg;
};

struct vioc_lut_t {
	struct vioc_component_t info;
	//VIOC_LUT *addr;	//TODO:
	//VIOC_LUT reg;		//TODO:
};

struct vioc_outcfg_t {
	struct vioc_component_t info;
	VIOC_OUTCFG *addr;
	VIOC_OUTCFG reg;
};

struct vioc_config_t {
	struct vioc_component_t info;
	VIOC_IREQ_CONFIG *addr;
	VIOC_IREQ_CONFIG reg;
};

struct test_case_t {
	addr_t *vioc_base_addr;
	struct vioc_rdma_t rdma;
	struct vioc_wdma_t wdma;
	struct vioc_wmix_t wmix;
	struct vioc_sc_t sc;
	struct vioc_lut_t lut;
	struct vioc_outcfg_t outcfg;
	struct vioc_config_t config;
};


/*
 *-----------------------------------------------------------------------------
 * storage for parsed data from test data files
 * main structure is test_data_t
 *-----------------------------------------------------------------------------
 */

struct test_data_reg_val_t {
	int reg[160];				// get register's value from data(.txt);
	int nr_regs;				// number of parsed regs
};

struct test_data_t {
	int test_no;
	char test_name[64];
	struct test_data_reg_val_t rdma;
	struct test_data_reg_val_t wdma;
	struct test_data_reg_val_t wmix;
	struct test_data_reg_val_t sc;
	struct test_data_reg_val_t lut;
	struct test_data_reg_val_t outcfg;
	struct test_data_reg_val_t config;
	struct list_head list;
};

#endif /*__VIOC_H__*/
