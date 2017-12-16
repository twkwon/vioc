/*
 * linux/include/video/tcc/vioc_lut.h
 * Author:  <linux@telechips.com>
 * Created: June 10, 2016
 * Description: TCC VIOC h/w block 
 *
 * Copyright (C) 2008-2009 Telechips
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __VIOC_LUT_H__
#define	__VIOC_LUT_H__

#define	VIOC_LUT_DEV0		0
#define	VIOC_LUT_DEV1		1
#define	VIOC_LUT_DEV2		2
#define	VIOC_LUT_COMP0		3
#define	VIOC_LUT_COMP1		4
#define	VIOC_LUT_COMP2		5
#define	VIOC_LUT_COMP3		6

#define LUT_COLOR_DEPTH		10
#define LUT_TABLE_SIZE		(1 << LUT_COLOR_DEPTH)

#if 0	//TODO
typedef	struct {
	unsigned		RESERVED0	: 8;
	unsigned		EDR_S		: 1;
	unsigned		RESERVED1	: 7;
	unsigned		RESERVED2	:16;
} VIOC_CONFIG_PATH_EDR;

typedef	union {
	unsigned long			nREG;
	VIOC_CONFIG_PATH_EDR	bREG;
} VIOC_CONFIG_PATH_EDR_u;

typedef struct {
	unsigned 			L0_EVS_SEL	:  3;
	unsigned 						:  1;
	unsigned 			L1_EVS_SEL	:  3;
	unsigned 						:  1;
	unsigned 			L2_EVS_SEL	:  3;
	unsigned 						:  5;
	unsigned 			MIX00		:  1;
	unsigned 			MIX03		:  1;
	unsigned 			MIX10		:  1;
	unsigned 			MIX13		:  1;
	unsigned 						:  2;
	unsigned 			MIX30		:  1;
	unsigned 						:  1;
	unsigned 			MIX40		:  1;
	unsigned 						:  1;
	unsigned 			MIX50		:  1;
	unsigned 						:  1;
	unsigned 			MIX60		:  1;
	unsigned 						:  1;
	unsigned 			RD12		:  1;
	unsigned 			RD14		:  1;
} VIOC_CONFIG_MISC0;

typedef	union {
	unsigned long		nREG;
	VIOC_CONFIG_MISC0	bREG;
} VIOC_CONFIG_MISC0_u;

typedef	struct	_VIOC_IREQ_CONFIG
{
	volatile VIOC_IREQ_IREQ_u 			uRAWSTATUS;			// 0x00, 0x04
	volatile VIOC_IREQ_IREQ_u 			uSYNCSTATUS;		// 0x08, 0x0C
	volatile VIOC_IREQ_IREQ_u 			uIREQSELECT;		// 0x10, 0x14
	volatile VIOC_IREQ_IREQ_u 			nIRQMASKSET;		// 0x18, 0x1C
	volatile VIOC_IREQ_IREQ_u 			nIRQMASKCLR;		// 0x20, 0x24
	volatile VIOC_IREQ_VECTORID_u 		nVECTORID;			// 0x28
	unsigned int 						reserved0[3];		// 0x2C, 0x30, 0x34

	volatile VIOC_CONFIG_TEST_LOOP_u	TEST_LOOP;			// 0x38
	volatile VIOC_CONFIG_PATH_EDR_u		uPATH_EDR;			// 0x3C
	volatile VIOC_CONFIG_MISC0_u 		uMISC0;				// 0x40
	volatile VIOC_CONFIG_PATH_u 		uSC0;				// 0x44
	volatile VIOC_CONFIG_PATH_u 		uSC1;				// 0x48
	volatile VIOC_CONFIG_PATH_u 		uSC2;				// 0x4C
	volatile VIOC_CONFIG_PATH_u 		uSC3;				// 0x50
	volatile VIOC_CONFIG_PATH_u 		uVIQE;				// 0x54
	volatile VIOC_CONFIG_PATH_u 		uDEINTLS;			// 0x58
	unsigned int 						reserved1;			// 0x5C

	volatile VIOC_CONFIG_PATH_u 		uFCDEC0;			// 0x60
	volatile VIOC_CONFIG_PATH_u 		uFCDEC1;			// 0x64
	unsigned int 						reserved2[2];		// 0x68, 0x6C
	volatile VIOC_CONFIG_PATH_u 		uFCENC0;			// 0x70
	volatile VIOC_CONFIG_PATH_u 		uFCENC1;			// 0x74
	volatile VIOC_CONFIG_PATH_u 		uFDELAY0;			// 0x78
	volatile VIOC_CONFIG_PATH_u 		uFDELAY1;			// 0x7C
	unsigned int 						reserved3;			// 0x80
	volatile VIOC_CONFIG_MISC1_u 		uMISC1;				// 0x84

	unsigned int 						reserved4[12];		// 0x88~0xB4
	volatile VIOC_CONFIG_VIN_SEL_u		VIN_SEL;			// 0xB8
	volatile VIOC_CONFIG_DEV_SEL_u		DEV_SEL;			// 0xBC

	unsigned int						nARID;				// 0xC0
	unsigned int						nAWID;				// 0xC4
	volatile VIOC_POWER_AUTOPD_u 		uAUTOPD;			// 0xC8
	volatile VIOC_POWER_CLKCTRL_u 		uCLKCTRL;			// 0xCC
	volatile VIOC_POWER_BLOCKS_u 		uPOWERDOWN;			// 0xD0, 0xD4
	volatile VIOC_POWER_BLOCKS_u 		uSOFTRESET;			// 0xD8, 0xDC

	unsigned int 						reserved5[6];		// 0xE0~0xF4
	volatile VIOC_CONFIG_PATH_u 		uSC4;				// 0xF8
	unsigned int 						reserved6[11];		// 0xFC~0x124
	volatile VIOC_POWER_BLOCKS2_u 		uPOWERDOWN2;		// 0x128
	volatile VIOC_POWER_BLOCKS2_u 		uSOFTRESET2;		// 0x12C
	volatile VIOC_CONFIG_PATH_u 		uVIQE1;				// 0x130
	unsigned int 						reserved7[3];		// 0x134~13C
	volatile VIOC_CONFIG_WDMA_MISC_u	uWDMA_MISC;			// 0x140
}	VIOC_IREQ_CONFIG,*PVIOC_IREQ_CONFIG;
#endif

#endif
