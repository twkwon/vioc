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

#include <test.h>

#define REG_OFFSET(offset)	(offset / sizeof(addr_t))

#define read_reg(reg)		(*(volatile addr_t *)(reg))
#define write_reg(reg, val)	(*(volatile addr_t *)(reg) = (val))
#define map_reg(reg, val)	do {if (val >= 0) reg = val;} while (0);

#define BITSET(X, MASK)				((X) |= (unsigned int)(MASK))
#define BITSCLR(X, SMASK, CMASK)	((X) = ((((unsigned int)(X)) | ((unsigned int)(SMASK))) & ~((unsigned int)(CMASK))) )
#define BITCSET(X, CMASK, SMASK)	((X) = ((((unsigned int)(X)) & ~((unsigned int)(CMASK))) | ((unsigned int)(SMASK))) )
#define BITCLR(X, MASK)				((X) &= ~((unsigned int)(MASK)) )
#define BITXOR(X, MASK)				((X) ^= (unsigned int)(MASK) )
#define ISZERO(X, MASK)				(!(((unsigned int)(X))&((unsigned int)(MASK))))
#define	ISSET(X, MASK)				((unsigned long)(X)&((unsigned long)(MASK)))

/* offset from vioc base address */
#define OFFSET_RDMA_FROM_VIOC_BASE(id)	(HwVIOC_RDMA00 + (HwVIOC_RDMA_GAP * id) - BASE_ADDR_VIOC)
#define OFFSET_WDMA_FROM_VIOC_BASE(id)	(HwVIOC_WDMA00 + (HwVIOC_WDMA_GAP * id) - BASE_ADDR_VIOC)
#define OFFSET_WMIX_FROM_VIOC_BASE(id)	(HwVIOC_WMIX0 + (HwVIOC_WMIX_GAP * id) - BASE_ADDR_VIOC)
#define OFFSET_SC_FROM_VIOC_BASE(id)	(HwVIOC_SC0 + (HwVIOC_SC_GAP * id) - BASE_ADDR_VIOC)
#define OFFSET_LUT_FROM_VIOC_BASE		(HwVIOC_LUT - BASE_ADDR_VIOC)
#define OFFSET_OUTCFG_FROM_VIOC_BASE	(HwVIOC_OUTCFG - BASE_ADDR_VIOC)
#define OFFSET_CONFIG_FROM_VIOC_BASE	(HwVIOC_CONFIG - BASE_ADDR_VIOC)


typedef unsigned int addr_t;
typedef unsigned int reg_t;

enum vioc_components {
	VC_START = 0,
	VC_RDMA_1st = VC_START,
	VC_RDMA_2nd,
	VC_RDMA_3rd,
	VC_RDMA_4th,
	VC_WDMA_1st,
	VC_WDMA_2nd,
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
	int nr_regs;				// number of mapped regs
	int plugin;					// SELECT bits of plug-in register in the Configuration & Interrupt part
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
	VIOC_LUT *addr;
	VIOC_LUT reg;
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
	int test_no;
	char test_name[SIZE_OF_TEST_NAME];
	addr_t *vioc_base_addr;
	struct vioc_rdma_t rdma1;
	struct vioc_rdma_t rdma2;
	struct vioc_rdma_t rdma3;
	struct vioc_rdma_t rdma4;
	struct vioc_wdma_t wdma1;
	struct vioc_wdma_t wdma2;
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
	int reg[MAX_NUM_OF_REG_DATA_COMPONENT];	// get register's value from data(.txt);
	int nr_regs;							// number of parsed regs
};

struct test_data_t {
	int test_no;
	char test_name[SIZE_OF_TEST_NAME];
	struct test_data_reg_val_t rdma1;
	struct test_data_reg_val_t rdma2;
	struct test_data_reg_val_t rdma3;
	struct test_data_reg_val_t rdma4;
	struct test_data_reg_val_t wdma1;
	struct test_data_reg_val_t wdma2;
	struct test_data_reg_val_t wmix;
	struct test_data_reg_val_t sc;
	struct test_data_reg_val_t lut;
	struct test_data_reg_val_t outcfg;
	struct test_data_reg_val_t config;
	struct list_head list;
};

/*
 *-----------------------------------------------------------------------------
 * extern functions
 *-----------------------------------------------------------------------------
 */
 
extern int setup_vioc_path(struct test_case_t *, struct test_data_t *);
extern int shoot_test(struct test_case_t *);

extern int rdma_map_regs(struct vioc_rdma_t *, struct test_data_reg_val_t *);
extern int rdma_verify_regs(struct vioc_rdma_t *);
extern int rdma_setup(struct vioc_rdma_t *);

extern int wdma_map_regs(struct vioc_wdma_t *, struct test_data_reg_val_t *);
extern int wdma_verify_regs(struct vioc_wdma_t *);
extern int wdma_setup(struct vioc_wdma_t *);

extern int wmix_map_regs(struct vioc_wmix_t *, struct test_data_reg_val_t *);
extern int wmix_verify_regs(struct vioc_wmix_t *);
extern int wmix_setup(struct vioc_wmix_t *);

extern int sc_map_regs(struct vioc_sc_t *, struct test_data_reg_val_t *);
extern int sc_verify_regs(struct vioc_sc_t *);
extern int sc_setup(struct vioc_sc_t *);

extern int lut_map_regs(struct vioc_lut_t *, struct test_data_reg_val_t *);
extern int lut_verify_regs(struct vioc_lut_t *);
extern int lut_setup(struct vioc_lut_t *);

extern int outcfg_map_regs(struct vioc_outcfg_t *, struct test_data_reg_val_t *);
extern int outcfg_verify_regs(struct vioc_outcfg_t *);
extern int outcfg_setup(struct vioc_outcfg_t *);
extern int outcfg_config(struct test_case_t *);

extern int config_map_regs(struct vioc_config_t *, struct test_data_reg_val_t *);
extern int config_verify_regs(struct vioc_config_t *);
extern int config_setup(struct vioc_config_t *);
extern int config_plugin(struct test_case_t *, enum vioc_components);
extern int config_config(struct test_case_t *);

#endif /*__VIOC_H__*/
