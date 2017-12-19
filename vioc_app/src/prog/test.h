#ifndef __TEST_H__
#define __TEST_H__

#define SIZE_OF_TEST_NAME			(128)

#define MMAP_DEFAULT_ADDR			(BASE_ADDR_VIOC)	/* VIOC base address */
#define MMAP_DEFAULT_LEN			(0x00100000)		/* 1 MBytea */

#define NUMBER_OF_VIOC_COMPONENTS	(32)				/* max 32 vioc components */
#define NUMBER_OF_REG_DATA			(1024)				/* max 1024 data (single test case) */

#define REG_START_OFFSET_RDMA		(2)
#define REG_START_OFFSET_WDMA		(2)
#define REG_START_OFFSET_WMIX		(2)
#define REG_START_OFFSET_SC			(2)
#define REG_START_OFFSET_LUT		(2)
#define REG_START_OFFSET_OUTCFG		(0)
#define REG_START_OFFSET_CONFIG		(0)

#endif	//__TEST_H__
