#ifndef __VIOC_H__
#define __VIOC_H__

#include <list.h>
#include <test.h>

#if defined(__ARCH_TCC898X__)
#include <arch/tcc898x/vioc_regs.h>
#elif defined(__ARCH_TCC899X__)
#include <arch/tcc899x/vioc_regs.h>
#elif defined(__ARCH_TCC803X__)
#include <arch/tcc803x/vioc_regs.h>
#else
#error "ERROR: Not defined ARCH in configure"
#endif

#define ALIGN_BIT	(0x8 - 1)
#define BIT_0 		(3)
#define GET_ADDR_YUV42X_spY(Base_addr) 		(((((unsigned int)Base_addr) + ALIGN_BIT) >> BIT_0) << BIT_0)
#define GET_ADDR_YUV42X_spU(Yaddr, x, y) 	(((((unsigned int)Yaddr+(x*y)) + ALIGN_BIT) >> BIT_0) << BIT_0)
#define GET_ADDR_YUV422_spV(Uaddr, x, y) 	(((((unsigned int)Uaddr+(x*y/2)) + ALIGN_BIT) >> BIT_0) << BIT_0)
#define GET_ADDR_YUV420_spV(Uaddr, x, y) 	(((((unsigned int)Uaddr+(x*y/4)) + ALIGN_BIT) >> BIT_0) << BIT_0)

enum vioc_components {
	VC_START = 0,
	VC_DISP_RDMA = VC_START,
	VC_RDMA_1st,
	VC_RDMA_2nd,
	VC_RDMA_3rd,
	VC_RDMA_4th,
	VC_WDMA_1st,
	VC_WDMA_2nd,
	VC_DISP_WMIX,
	VC_WMIX,
	VC_SC,
	VC_LUT,
	VC_VIN,
	VC_VIN_LUT,
	VC_OUTCFG,
	VC_CONFIG,
	VC_END = VC_CONFIG,
};

enum tcc_pix_fmt {
    TCC_PFMT_YUV420,
    TCC_PFMT_YUV422,
    TCC_PFMT_RGB,
};

/* FMT10 */
#define DATA_FMT_8BIT		0
#define DATA_FMT_10BIT		1
#define DATA_FMT_16BIT		3

/* IMAGE FORMAT */
#define VIOC_IMG_FMT_BPP1           0       // 1bit
#define VIOC_IMG_FMT_BPP2           1       // 2bits
#define VIOC_IMG_FMT_BPP4           2       // 4bits
#define VIOC_IMG_FMT_BPP8           3       // 1byte
#define VIOC_IMG_FMT_RGB332         8       // 1byte
#define VIOC_IMG_FMT_ARGB4444       9       // 2bytes
#define VIOC_IMG_FMT_RGB565         10      // 2bytes
#define VIOC_IMG_FMT_ARGB1555       11      // 2bytes
#define VIOC_IMG_FMT_ARGB8888       12      // 4bytes
#define VIOC_IMG_FMT_ARGB6666_4     13      // 4bytes
#define VIOC_IMG_FMT_RGB888         14      // 3bytes   : newly supported : 3 bytes format
#define VIOC_IMG_FMT_ARGB6666_3     15      // 3bytes   : newly supported : 3 bytes format
#define VIOC_IMG_FMT_COMP           16      // 4bytes
#define VIOC_IMG_FMT_DECOMP         (VIOC_IMG_FMT_COMP)
#define VIOC_IMG_FMT_444SEP         21      // 3bytes
#define VIOC_IMG_FMT_UYVY           22      // 2bytes   : LSB [Y/U/Y/V] MSB : newly supported : 2 bytes format
#define VIOC_IMG_FMT_VYUY           23      // 2bytes   : LSB [Y/V/Y/U] MSB : newly supported : 2 bytes format
#define VIOC_IMG_FMT_YUV420SEP      24      // 1,1byte
#define VIOC_IMG_FMT_YUV422SEP      25      // 1,1byte
#define VIOC_IMG_FMT_YUYV           26      // 2bytes   : LSB [Y/U/Y/V] MSB : previous SEQ
#define VIOC_IMG_FMT_YVYU           27      // 2bytes   : LSB [Y/V/Y/U] MSB : newly supported : 2 bytes format
#define VIOC_IMG_FMT_YUV420IL0      28      // 1,2byte
#define VIOC_IMG_FMT_YUV420IL1      29      // 1,2byte
#define VIOC_IMG_FMT_YUV422IL0      30      // 1,2bytes
#define VIOC_IMG_FMT_YUV422IL1      31      // 1,2bytes

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

#define AUTO_DMA_FMT		(0x00000001)
#define AUTO_DMA_FMT10		(0x00000002)
#define AUTO_DMA_Y2R		(0x00000010)
#define AUTO_DMA_Y2RMD		(0x00000020)
#define AUTO_DMA_R2Y		(0x00000040)
#define AUTO_DMA_R2YMD		(0x00000080)
#define AUTO_DMA_SIZE		(0x00000100)
#define AUTO_DMA_BASE		(0x00001000)
#define AUTO_DMA_OFFS		(0x00010000)

struct vioc_rdma_t {
	struct vioc_component_t info;
	VIOC_RDMA *addr;			// virtual address of physical register (by mmap)
	VIOC_RDMA reg;				// parsed register values

	int auto_set;
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

struct vioc_vin_t {
	struct vioc_component_t info;
	VIOC_VIN *addr;
	VIOC_VIN reg;
};

struct vioc_vin_lut_t {
	struct vioc_component_t info;
	VIOC_VIN_LUT_C *addr;
	VIOC_VIN_LUT_C reg;
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

	struct image_buf_info_t input_file[MAX_NUM_OF_RDMA];		// input images
	struct image_buf_info_t output_file[MAX_NUM_OF_WDMA];		// output images
	struct image_buf_info_t reference_file[MAX_NUM_OF_WDMA];	// reference images to compare output files

	addr_t *vioc_base_addr;
	struct vioc_rdma_t disp_rdma;
	struct vioc_rdma_t rdma1;
	struct vioc_rdma_t rdma2;
	struct vioc_rdma_t rdma3;
	struct vioc_rdma_t rdma4;
	struct vioc_wdma_t wdma1;
	struct vioc_wdma_t wdma2;
	struct vioc_wmix_t disp_wmix;
	struct vioc_wmix_t wmix;
	struct vioc_sc_t sc;
	struct vioc_lut_t lut;
	struct vioc_vin_t vin;
	struct vioc_vin_lut_t vin_lut;
	struct vioc_outcfg_t outcfg;
	struct vioc_config_t config;

	unsigned int todo_disable_prev_sc_id;	// You must reset this components before testing next case
};


/*
 *-----------------------------------------------------------------------------
 * storage for parsed data from test data files
 * main structure is test_data_t
 *-----------------------------------------------------------------------------
 */

enum e_test_status {
	TEST_STATUS_PASS,			// pass the test case

	TEST_STATUS_RUN_PASS,		// pass run test
	TEST_STATUS_RUN_SKIP,		// skip run test

	TEST_STATUS_NO_INPUT,
	TEST_STATUS_NO_OUTPUT,
	TEST_STATUS_NO_REFERENCE,
	TEST_STATUS_NO_COMPARE,

	/* errors */
	TEST_STATUS_ERR_RUN,
	TEST_STATUS_ERR_OUTPUT,
	TEST_STATUS_ERR_INPUT,
	TEST_STATUS_ERR_REFERENCE,

	/* fail */
	TEST_STATUS_FAIL_COMPARE,
};

struct test_data_reg_val_t {
	int reg[MAX_NUM_OF_REG_DATA_COMPONENT];	// get register's value from data(.txt);
	int nr_regs;							// number of parsed regs
};

struct test_data_t {
	int test_no;
	char test_name[SIZE_OF_TEST_NAME];
	enum e_test_status test_status;

	struct image_buf_info_t input_file[MAX_NUM_OF_RDMA];		// input images
	struct image_buf_info_t output_file[MAX_NUM_OF_WDMA];		// output images
	struct image_buf_info_t reference_file[MAX_NUM_OF_WDMA];	// reference images to compare output files

	//struct test_data_reg_val_t rdma[MAX_NUM_OF_RDMA];
	struct test_data_reg_val_t disp_rdma;	// rdma of display path
	struct test_data_reg_val_t rdma1;
	struct test_data_reg_val_t rdma2;
	struct test_data_reg_val_t rdma3;
	struct test_data_reg_val_t rdma4;

	//struct test_data_reg_val_t wdma[MAX_NUM_OF_WDMA];
	struct test_data_reg_val_t wdma1;
	struct test_data_reg_val_t wdma2;

	struct test_data_reg_val_t disp_wmix;	// wmix of display path
	struct test_data_reg_val_t wmix;

	struct test_data_reg_val_t sc;
	struct test_data_reg_val_t lut;

	struct test_data_reg_val_t vin;			// vin
	struct test_data_reg_val_t vin_lut;		// VIN_LUT_C registers

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
extern void rdma_set_offset(struct vioc_rdma_t *, unsigned int, unsigned int);
extern void rdma_set_address(struct vioc_rdma_t *, addr_t, addr_t, addr_t);
extern void rdma_set_fmt(struct vioc_rdma_t *, unsigned int);
extern void rdma_set_fmt10(struct vioc_rdma_t *, unsigned int);
extern void rdma_set_y2r(struct vioc_rdma_t *, unsigned int);
extern void rdma_set_y2rmd(struct vioc_rdma_t *, unsigned int);
extern void rdma_set_r2y(struct vioc_rdma_t *, unsigned int);
extern void rdma_set_r2ymd(struct vioc_rdma_t *, unsigned int);
extern void rdma_set_size(struct vioc_rdma_t *, unsigned int, unsigned int);
extern void rdma_en_ctrl(struct vioc_rdma_t *, unsigned int);
extern int disp_rdma_clear_regs(struct test_case_t *);

extern int wdma_map_regs(struct vioc_wdma_t *, struct test_data_reg_val_t *);
extern int wdma_verify_regs(struct vioc_wdma_t *);
extern int wdma_setup(struct vioc_wdma_t *);
extern void wdma_set_offset(struct vioc_wdma_t *, addr_t, addr_t);
extern void wdma_set_address(struct vioc_wdma_t *, addr_t, addr_t, addr_t);
extern void wdma_en_ctrl(struct vioc_wdma_t *, unsigned int);

extern int wmix_map_regs(struct vioc_wmix_t *, struct test_data_reg_val_t *);
extern int wmix_verify_regs(struct vioc_wmix_t *);
extern int wmix_setup(struct vioc_wmix_t *);
extern void wmix_en_ctrl(struct vioc_wmix_t *, unsigned int);

extern int sc_map_regs(struct vioc_sc_t *, struct test_data_reg_val_t *);
extern int sc_verify_regs(struct vioc_sc_t *);
extern int sc_setup(struct vioc_sc_t *);
extern void sc_en_ctrl(struct vioc_sc_t *, unsigned int);

extern int lut_map_regs(struct vioc_lut_t *, struct test_data_reg_val_t *);
extern int lut_verify_regs(struct vioc_lut_t *);
extern int lut_setup(struct vioc_lut_t *);
extern void lut_en_ctrl(struct vioc_lut_t *, unsigned int);

extern int vin_map_regs(struct vioc_vin_t *, struct test_data_reg_val_t *);
extern int vin_verify_regs(struct vioc_vin_t *);
extern int vin_setup(struct vioc_vin_t *);
extern int vin_lut_map_regs(struct vioc_vin_lut_t *, struct test_data_reg_val_t *);
extern int vin_lut_verify_regs(struct vioc_vin_lut_t *);
extern int vin_lut_setup(struct vioc_vin_lut_t *);
extern void vin_en_ctrl(struct vioc_vin_t *, unsigned int);

extern int outcfg_map_regs(struct vioc_outcfg_t *, struct test_data_reg_val_t *);
extern int outcfg_verify_regs(struct vioc_outcfg_t *);
extern int outcfg_setup(struct vioc_outcfg_t *);
extern int outcfg_config(struct test_case_t *);

extern int config_map_regs(struct vioc_config_t *, struct test_data_reg_val_t *);
extern int config_verify_regs(struct vioc_config_t *);
extern int config_setup(struct vioc_config_t *);
extern int config_plugin(struct test_case_t *, enum vioc_components);
extern int config_plugout_sc(struct test_case_t *, unsigned int);
extern int config_config(struct test_case_t *);
extern int config_reset(struct test_case_t *);
extern int reset_rdma_ctrl(struct test_case_t *, enum vioc_components, unsigned int);

#endif /*__VIOC_H__*/
