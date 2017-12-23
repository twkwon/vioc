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

#define INT32_OFFSET(offset)	(offset / sizeof(addr_t))

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

#define ALIGN_BIT	(0x8 - 1)
#define BIT_0 		(3)
#define GET_ADDR_YUV42X_spY(Base_addr) 		(((((unsigned int)Base_addr) + ALIGN_BIT) >> BIT_0) << BIT_0)
#define GET_ADDR_YUV42X_spU(Yaddr, x, y) 	(((((unsigned int)Yaddr+(x*y)) + ALIGN_BIT) >> BIT_0) << BIT_0)
#define GET_ADDR_YUV422_spV(Uaddr, x, y) 	(((((unsigned int)Uaddr+(x*y/2)) + ALIGN_BIT) >> BIT_0) << BIT_0)
#define GET_ADDR_YUV420_spV(Uaddr, x, y) 	(((((unsigned int)Uaddr+(x*y/4)) + ALIGN_BIT) >> BIT_0) << BIT_0)

/* Rounds an integer value up to the next multiple of num */
#define ROUND_UP_2(num)		(((num)+1)&~1)
#define ROUND_UP_4(num)		(((num)+3)&~3)
#define ROUND_UP_8(num)		(((num)+7)&~7)
#define ROUND_UP_16(num)	(((num)+15)&~15)
#define ROUND_UP_32(num)	(((num)+31)&~31)
#define ROUND_UP_64(num)	(((num)+63)&~63)
/* Rounds an integer value down to the next multiple of num */
#define ROUND_DOWN_2(num)	((num)&(~1))
#define ROUND_DOWN_4(num)	((num)&(~3))
#define ROUND_DOWN_8(num)	((num)&(~7))
#define ROUND_DOWN_16(num)	((num)&(~15))
#define ROUND_DOWN_32(num)	((num)&(~31))
#define ROUND_DOWN_64(num)	((num)&(~63))

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

enum vioc_img_fmt {
	TCC_LCDC_IMG_FMT_1BPP,
	TCC_LCDC_IMG_FMT_2BPP,
	TCC_LCDC_IMG_FMT_4BPP,
	TCC_LCDC_IMG_FMT_8BPP,
	TCC_LCDC_IMG_FMT_RGB332 = 8,
	TCC_LCDC_IMG_FMT_RGB444 = 9,
	TCC_LCDC_IMG_FMT_RGB565 = 10,
	TCC_LCDC_IMG_FMT_RGB555 = 11,
	TCC_LCDC_IMG_FMT_RGB888 = 12,
	TCC_LCDC_IMG_FMT_RGB666 = 13,
	TCC_LCDC_IMG_FMT_RGB888_3	= 14,		/* RGB888 - 3bytes aligned - B1[31:24],R0[23:16],G0[15:8],B0[7:0] newly supported : 3 bytes format*/
	TCC_LCDC_IMG_FMT_ARGB6666_3 = 15,		/* ARGB6666 - 3bytes aligned - A[23:18],R[17:12],G[11:6],B[5:0]newly supported : 3 bytes format */
	TCC_LCDC_IMG_FMT_COMP = 16,				// 4bytes
	TCC_LCDC_IMG_FMT_DECOMP	= (TCC_LCDC_IMG_FMT_COMP),
	TCC_LCDC_IMG_FMT_444SEP = 21,
	TCC_LCDC_IMG_FMT_UYVY = 22,		/* 2bytes	: YCbCr 4:2:2 Sequential format LSB [Y/U/Y/V] MSB : newly supported : 2 bytes format*/
	TCC_LCDC_IMG_FMT_VYUY = 23,		/* 2bytes	: YCbCr 4:2:2 Sequential format LSB [Y/V/Y/U] MSB : newly supported : 2 bytes format*/
	TCC_LCDC_IMG_FMT_YUV420SP = 24,
	TCC_LCDC_IMG_FMT_YUV422SP = 25,
	TCC_LCDC_IMG_FMT_YUYV = 26,
	TCC_LCDC_IMG_FMT_YVYU = 27,
	TCC_LCDC_IMG_FMT_YUV420ITL0 = 28,
	TCC_LCDC_IMG_FMT_YUV420ITL1 = 29,
	TCC_LCDC_IMG_FMT_YUV422ITL0 = 30,
	TCC_LCDC_IMG_FMT_YUV422ITL1 = 31,
	TCC_LCDC_IMG_FMT_MAX
};

struct image_buf_info_t {
	int id;							// -1 is no file
	char name[SIZE_OF_TEST_NAME];
	addr_t paddr;					// pmap physical address to set into RDMA
	addr_t *vaddr;					// mapped virtual buffer of pmap through mmap()
	unsigned int len;				// image file lenth
	int width;						// image width
	int height;						// image height
	int fmt;						// image format //TODO: create enum imag_fmt
};

struct image_file_t {
	addr_t base_paddr;
	addr_t *base_vaddr;
	struct image_buf_info_t input[MAX_NUM_OF_RDMA];
	struct image_buf_info_t output[MAX_NUM_OF_WDMA];
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

	struct image_buf_info_t input_file[MAX_NUM_OF_RDMA];
	struct image_buf_info_t output_file[MAX_NUM_OF_WDMA];

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
	int test_status;	// -1: test error, 0: test skip, 1: test run

	struct image_buf_info_t input_file[MAX_NUM_OF_RDMA];
	struct image_buf_info_t output_file[MAX_NUM_OF_WDMA];

	//struct test_data_reg_val_t rdma[MAX_NUM_OF_RDMA];
	struct test_data_reg_val_t rdma1;
	struct test_data_reg_val_t rdma2;
	struct test_data_reg_val_t rdma3;
	struct test_data_reg_val_t rdma4;
	//struct test_data_reg_val_t wdma[MAX_NUM_OF_WDMA];
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
extern int setup_test_case(struct test_case_t *, struct test_data_t *);
extern int setup_vioc_path(struct test_case_t *);
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
