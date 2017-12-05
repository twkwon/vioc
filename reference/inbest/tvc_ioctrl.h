/*
 *
 * Copyright (C) 2012 by Telechips
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _TVC_IOCTL_H
#define _TVC_IOCTL_H

#define __iomem        __attribute__((noderef, address_space(2)))

#ifndef ADDRESS_ALIGNED
#define ADDRESS_ALIGNED
#define ALIGN_BIT (0x8-1)
#define BIT_0 3
#define GET_ADDR_YUV42X_spY(Base_addr) 		(((((unsigned int)Base_addr) + ALIGN_BIT)>> BIT_0)<<BIT_0)
#define GET_ADDR_YUV42X_spU(Yaddr, x, y) 	(((((unsigned int)Yaddr+(x*y)) + ALIGN_BIT)>> BIT_0)<<BIT_0)
#define GET_ADDR_YUV422_spV(Uaddr, x, y) 	(((((unsigned int)Uaddr+(x*y/2)) + ALIGN_BIT) >> BIT_0)<<BIT_0)
#define GET_ADDR_YUV420_spV(Uaddr, x, y) 	(((((unsigned int)Uaddr+(x*y/4)) + ALIGN_BIT) >> BIT_0)<<BIT_0)
#endif

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

#define MAX_INPUT_CHANNELS		4	/* Window Mixer max input channels */

typedef enum {
	TVC_POLLING,
	TVC_INTERRUPT,
	TVC_NOWAIT
} TVC_RESPONSE_TYPE;

#if 0
struct tvc_dither
{
	/*
		endither : dithering enable
		dithersel:Dither Mode Select
			0 : Normal (LSB toggle mode)
			1 : Adder Mode

           	dithermatrix : 4 x 4 dithering matrix
	*/
	unsigned int endither;
	unsigned int dithersel;
	char dithermatrix[4][4];
};

struct tvc_wdma_info
{
	struct tvc_imagesize size;
	struct tvc_address addr;
	struct tvc_format format;
	unsigned int r2ymode, y2rmode;
	unsigned int interlace;
	struct tvc_dither dither;
};
#endif

struct vioc_block_info {
	void __iomem *reg;
	unsigned int	id;
	unsigned int type;
	unsigned int path;
	unsigned int irq;
};

struct tvc_scan_info
{
	unsigned int interlace;		/* 0 : progress data, 1 : interlace */
	unsigned int field;
	unsigned int strm;			/* streaming mode enable */
};

struct tvc_crop_info {
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
};

struct tvc_size_info {
	unsigned int width;
	unsigned int height;
};

struct tvc_pos_info {
	unsigned int x;
	unsigned int y;
};

struct tvc_format_info {
	/* forma t : TVC_IMAGE_FORMAT_TYPE */
	/* swap :
		3¡¯b000 : R-G-B
		3¡¯b001 : R-B-G
		3¡¯b010 : G-R-B
		3¡¯b011 : G-B-R
		3¡¯b100 : B-R-G
		3¡¯b101 : B-G-R
	*/
	unsigned int format;
	unsigned int swap;
	unsigned int bitdepth;
	unsigned int r2ymode;
	unsigned int y2rmode;
};

struct tvc_dma_info
{
	unsigned int addr0;
	unsigned int addr1;
	unsigned int addr2;
	struct tvc_format_info fmt;
	struct tvc_crop_info crop;
	struct tvc_size_info size;
	struct tvc_scan_info scan;
};

struct tvc_wmix_info
{
	struct tvc_size_info size;
	struct tvc_pos_info pos;
};

struct tvc_scale_info
{
	struct tvc_size_info dst;
	struct tvc_size_info out;
	struct tvc_pos_info out_pos;
};

/*  common ioctl structure */
struct tvc_common_ioctl_info {
	unsigned int responsetype;
	struct tvc_dma_info	src;
	struct tvc_dma_info	dst;
	unsigned int dmabuf_fd;
};

#define TVC_DRV_MIXING_IOCTL				0x10
#define TVC_DRV_MIXING_IOCTL_KERNEL			0x11

#endif //_TVC_IOCTL_H
