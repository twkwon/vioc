#ifndef __TEST_H__
#define __TEST_H__

/*
 * Specific Information
 */
#define SIZE_OF_TEST_NAME	(128)

#define MMAP_DEFAULT_ADDR	(BASE_ADDR_VIOC)	/* VIOC base address */
#define MMAP_DEFAULT_LEN	(0x00101000)		/* 1 MByte +  4 KBytes */

#define MAX_NUM_OF_VIOC_COMPONENTS		(32)	/* max 32 vioc components */
#define MAX_NUM_OF_REG_DATA_COMPONENT	(256)	/* max 256 data for each vioc component */
#define MAX_NUM_OF_REG_DATA_TEST		(1024)	/* max 1024 data for single test case */
#define MAX_NUM_OF_RDMA					(4)		/* one vioc path has max 4 RDMAs */
#define MAX_NUM_OF_WDMA					(2)		/* one vioc path has max 2 WDMAs */

#define IMAGE_SIZE	(0x100000 * 8)	/* 8 MBytes */
#define IMAGE_NUM	(MAX_NUM_OF_RDMA + MAX_NUM_OF_WDMA)

#define REG_START_OFFSET_RDMA		(2)
#define REG_START_OFFSET_WDMA		(2)
#define REG_START_OFFSET_WMIX		(2)
#define REG_START_OFFSET_SC			(2)
#define REG_START_OFFSET_LUT		(2)
#define REG_START_OFFSET_VIN		(2)
#define REG_START_OFFSET_VIN_LUT	(0)
#define REG_START_OFFSET_OUTCFG		(2)
#define REG_START_OFFSET_CONFIG		(2)

#define REG_ALWAYS_READ	(1)

/* reset or not about display path */
#define DO_NOT_RESET_DISP_PATH

/*
 * Specific Type
 */
typedef unsigned int addr_t;
typedef unsigned int reg_t;

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
#define	ISSET(X, MASK)				((unsigned int)(X)&((unsigned int)(MASK)))

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

#endif	//__TEST_H__
