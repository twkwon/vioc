#ifndef __TEST_H__
#define __TEST_H__

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

#endif	//__TEST_H__
