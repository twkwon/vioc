#ifndef __VIOC_LUT_H__
#define __VIOC_LUT_H__

#define	VIOC_LUT_DEV0		0
#define	VIOC_LUT_DEV1		1
#define	VIOC_LUT_DEV2		2
#define	VIOC_LUT_COMP0		3
#define	VIOC_LUT_COMP1		4
#define	VIOC_LUT_COMP2		5
#define	VIOC_LUT_COMP3		6

#define LUT_COLOR_DEPTH		10
#define LUT_TABLE_SIZE		(1 << LUT_COLOR_DEPTH)


typedef	struct {
	unsigned	EN			: 4;
	unsigned	RESERVED0	:28;
} VIOC_LUT_CTRL;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_CTRL			bREG;
} VIOC_LUT_CTRL_u;

typedef	struct {
	unsigned	SEL			: 8;
	unsigned	RESERVED0	:23;
	unsigned	EN			: 1;
} VIOC_LUT_DEV_CFG;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_DEV_CFG		bREG;
} VIOC_LUT_DEV_CFG_u;

typedef	struct {
	unsigned	SEL			: 8;
	unsigned	RESERVED0	:23;
	unsigned	EN			: 1;
} VIOC_LUT_VIOC_CFG;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_VIOC_CFG		bREG;
} VIOC_LUT_VIOC_CFG_u;

typedef	struct {
	unsigned	IND			: 2;
	unsigned	RESERVED0	:30;
} VIOC_LUT_TABLE_IND;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_TABLE_IND		bREG;
} VIOC_LUT_TABLE_IND_u;

typedef	struct {
	unsigned	SEL			: 8;
	unsigned	RESERVED0	:24;
} VIOC_LUT_UPDATE_PEND;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_UPDATE_PEND	bREG;
} VIOC_LUT_UPDATE_PEND_u;

typedef	struct {
	unsigned	COEFF11		:12;
	unsigned	RESERVED0	: 4;
	unsigned	COEFF12		:12;
	unsigned	RESERVED1	: 4;
} VIOC_LUT_CSC_COEFF11_12;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_CSC_COEFF11_12	bREG;
} VIOC_LUT_CSC_COEFF11_12_u;

typedef	struct {
	unsigned	COEFF13		:12;
	unsigned	RESERVED0	: 4;
	unsigned	COEFF21		:12;
	unsigned	RESERVED1	: 4;
} VIOC_LUT_CSC_COEFF13_21;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_CSC_COEFF13_21	bREG;
} VIOC_LUT_CSC_COEFF13_21_u;

typedef	struct {
	unsigned	COEFF22		:12;
	unsigned	RESERVED0	: 4;
	unsigned	COEFF23		:12;
	unsigned	RESERVED1	: 4;
} VIOC_LUT_CSC_COEFF22_23;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_CSC_COEFF22_23	bREG;
} VIOC_LUT_CSC_COEFF22_23_u;

typedef	struct {
	unsigned	COEFF31		:12;
	unsigned	RESERVED0	: 4;
	unsigned	COEFF32		:12;
	unsigned	RESERVED1	: 4;
} VIOC_LUT_CSC_COEFF31_32;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_CSC_COEFF31_32	bREG;
} VIOC_LUT_CSC_COEFF31_32_u;

typedef	struct {
	unsigned	COEFF33		:12;
	unsigned	RESERVED0	:20;
} VIOC_LUT_CSC_COEFF33;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_CSC_COEFF33	bREG;
} VIOC_LUT_CSC_COEFF33_u;

typedef	struct {
	unsigned	BYP			: 1;
	unsigned	RESERVED0	: 3;
	unsigned	R2YSEL		: 3;
	unsigned	RESERVED1	:25;
} VIOC_LUT_MIX_CFG;

typedef	union {
	unsigned long			nREG;
	VIOC_LUT_MIX_CFG		bREG;
} VIOC_LUT_MIX_CFG_u;

typedef	struct	_VIOC_LUT
{
	volatile VIOC_LUT_CTRL_u 			uCTRL;			// 0x00
	volatile VIOC_LUT_DEV_CFG_u 		uDEV0_CFG;		// 0x04
	volatile VIOC_LUT_DEV_CFG_u 		uDEV1_CFG;		// 0x08
	volatile VIOC_LUT_DEV_CFG_u 		uDEV2_CFG;		// 0x0C
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC0_CFG;		// 0x10
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC1_CFG;		// 0x14
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC2_CFG;		// 0x18
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC3_CFG;		// 0x1C
	volatile VIOC_LUT_TABLE_IND_u 		uTABLE_IND;		// 0x20
	volatile VIOC_LUT_UPDATE_PEND_u 	uUPDATE_PEND;	// 0x24
	volatile VIOC_LUT_CSC_COEFF11_12_u 	uCSC_COEFF11_12;// 0x28
	volatile VIOC_LUT_CSC_COEFF13_21_u 	uCSC_COEFF13_21;// 0x2C
	volatile VIOC_LUT_CSC_COEFF22_23_u 	uCSC_COEFF22_23;// 0x30
	volatile VIOC_LUT_CSC_COEFF31_32_u 	uCSC_COEFF31_32;// 0x34
	volatile VIOC_LUT_CSC_COEFF33_u 	uCSC_COEFF33;	// 0x38
	volatile VIOC_LUT_MIX_CFG_u 		uMIX_CFG;		// 0x3C
}	VIOC_LUT,*PVIOC_LUT;

#endif	/*__VIOC_LUT_H__*/
