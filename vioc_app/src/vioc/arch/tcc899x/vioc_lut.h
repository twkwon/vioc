#ifndef __VIOC_LUT_H__
#define __VIOC_LUT_H__

#define	VIOC_LUT_DEV0		0
#define	VIOC_LUT_DEV1		1
#define	VIOC_LUT_DEV2		2
#define	VIOC_LUT_COMP0		3
#define	VIOC_LUT_COMP1		4
#define	VIOC_LUT_COMP2		5
#define	VIOC_LUT_COMP3		6

#define LUT_TABLE_OFFSET	0	/*tcc899x=0, tcc898x=1*/
#define LUT_COLOR_DEPTH		10
#define LUT_TABLE_SIZE		(1 << LUT_COLOR_DEPTH)


typedef	struct {
	reg_t	EN				:11;	// SEL
	reg_t					:21;
} VIOC_LUT_CTRL;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_CTRL			bREG;
} VIOC_LUT_CTRL_u;

typedef	struct {
	reg_t					:31;
	reg_t	EN				: 1;
} VIOC_LUT_DEV_CFG;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_DEV_CFG		bREG;
} VIOC_LUT_DEV_CFG_u;

typedef	struct {
	reg_t	SEL				: 8;
	reg_t					:23;
	reg_t	EN				: 1;
} VIOC_LUT_VIOC_CFG;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_VIOC_CFG		bREG;
} VIOC_LUT_VIOC_CFG_u;

typedef	struct {
	reg_t	IND				: 2;
	reg_t					:30;
} VIOC_LUT_TABLE_IND;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_TABLE_IND		bREG;
} VIOC_LUT_TABLE_IND_u;

typedef	struct {
	reg_t	SEL				: 8;
	reg_t					:24;
} VIOC_LUT_UPDATE_PEND;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_UPDATE_PEND	bREG;
} VIOC_LUT_UPDATE_PEND_u;

typedef	struct {
	reg_t	COEFF11			:12;
	reg_t					: 4;
	reg_t	COEFF12			:12;
	reg_t					: 4;
} VIOC_LUT_CSC_COEFF11_12;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_CSC_COEFF11_12	bREG;
} VIOC_LUT_CSC_COEFF11_12_u;

typedef	struct {
	reg_t	COEFF13			:12;
	reg_t					: 4;
	reg_t	COEFF21			:12;
	reg_t					: 4;
} VIOC_LUT_CSC_COEFF13_21;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_CSC_COEFF13_21	bREG;
} VIOC_LUT_CSC_COEFF13_21_u;

typedef	struct {
	reg_t	COEFF22			:12;
	reg_t					: 4;
	reg_t	COEFF23			:12;
	reg_t					: 4;
} VIOC_LUT_CSC_COEFF22_23;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_CSC_COEFF22_23	bREG;
} VIOC_LUT_CSC_COEFF22_23_u;

typedef	struct {
	reg_t	COEFF31			:12;
	reg_t					: 4;
	reg_t	COEFF32			:12;
	reg_t					: 4;
} VIOC_LUT_CSC_COEFF31_32;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_CSC_COEFF31_32	bREG;
} VIOC_LUT_CSC_COEFF31_32_u;

typedef	struct {
	reg_t	COEFF33			:12;
	reg_t					:20;
} VIOC_LUT_CSC_COEFF33;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_CSC_COEFF33	bREG;
} VIOC_LUT_CSC_COEFF33_u;

typedef	struct {
	reg_t	BYP				: 1;
	reg_t					: 3;
	reg_t	R2YSEL			: 3;
	reg_t					:25;
} VIOC_LUT_MIX_CFG;

typedef	union {
	reg_t					nREG;
	VIOC_LUT_MIX_CFG		bREG;
} VIOC_LUT_MIX_CFG_u;

typedef	struct	_VIOC_LUT
{
	volatile VIOC_LUT_CTRL_u 			uCTRL;				// 0x00
	volatile VIOC_LUT_DEV_CFG_u 		uDEV0_CFG;			// 0x04
	volatile VIOC_LUT_DEV_CFG_u 		uDEV1_CFG;			// 0x08
	volatile VIOC_LUT_DEV_CFG_u 		uDEV2_CFG;			// 0x0C
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC0_CFG;			// 0x10
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC1_CFG;			// 0x14
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC2_CFG;			// 0x18
	volatile VIOC_LUT_VIOC_CFG_u 		uVIOC3_CFG;			// 0x1C
	volatile VIOC_LUT_TABLE_IND_u 		uTABLE_IND;			// 0x20
	volatile VIOC_LUT_UPDATE_PEND_u 	uUPDATE_PEND;		// 0x24
	volatile VIOC_LUT_CSC_COEFF11_12_u 	uCSC_COEFF11_12;	// 0x28
	volatile VIOC_LUT_CSC_COEFF13_21_u 	uCSC_COEFF13_21;	// 0x2C
	volatile VIOC_LUT_CSC_COEFF22_23_u 	uCSC_COEFF22_23;	// 0x30
	volatile VIOC_LUT_CSC_COEFF31_32_u 	uCSC_COEFF31_32;	// 0x34
	volatile VIOC_LUT_CSC_COEFF33_u 	uCSC_COEFF33;		// 0x38
	volatile VIOC_LUT_MIX_CFG_u 		uMIX_CFG;			// 0x3C
} VIOC_LUT,*PVIOC_LUT;

#endif	/*__VIOC_LUT_H__*/

