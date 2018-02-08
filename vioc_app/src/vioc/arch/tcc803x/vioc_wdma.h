#ifndef __VIOC_WDMA_H__
#define	__VIOC_WDMA_H__

#define 	VIOC_WDMA_IREQ_UPD_MASK 		0x00000001UL
#define 	VIOC_WDMA_IREQ_SREQ_MASK 		0x00000002UL
#define 	VIOC_WDMA_IREQ_ROLL_MASK 		0x00000004UL
#define 	VIOC_WDMA_IREQ_ENR_MASK 		0x00000008UL
#define 	VIOC_WDMA_IREQ_ENF_MASK 		0x00000010UL
#define 	VIOC_WDMA_IREQ_EOFR_MASK 		0x00000020UL
#define 	VIOC_WDMA_IREQ_EOFF_MASK 		0x00000040UL
#define 	VIOC_WDMA_IREQ_SEOFR_MASK 		0x00000080UL
#define 	VIOC_WDMA_IREQ_SEOFF_MASK 		0x00000100UL
#define 	VIOC_WDMA_IREQ_STSEN_MASK 		0x20000000UL
#define 	VIOC_WDMA_IREQ_STBF_MASK 		0x40000000UL
#define 	VIOC_WDMA_IREQ_STEOF_MASK 		0x80000000UL
#define 	VIOC_WDMA_IREQ_ALL_MASK 		0xFFFFFFFFUL

/******************************************************************************
*
*  WDMA YUV-to-RGB Converter Mode Register
*
*  0 - The Range for RGB is 16 ~ 235,"Studio Color". Normally SDTV
*  1 - The Range for RGB is  0 ~ 255,"Conputer System Color". Normally SDTV
*  2 - The Range for RGB is 16 ~ 235,"Studio Color". Normally HDTV
*  3 - The Range for RGB is  0 ~ 255,"Conputer System Color". Normally HDTV
*  4 - The Range for RGB is 16 ~ 235,"Studio Color". Normally UHDTV
*  5 - The Range for RGB is  0 ~ 255,"Conputer System Color". Normally UHDTV
*
*******************************************************************************/
#define 	R2YMD_SDTV_LR	0
#define		R2YMD_SDTV_FR	1
#define		R2YMD_HDTV_LR	2
#define		R2YMD_HDTV_FR	3
#define		R2YMD_UHDTV_LR	4
#define		R2YMD_UHDTV_FR	5

typedef struct
{
	reg_t ImgSizeWidth;
	reg_t ImgSizeHeight;
	reg_t TargetWidth;
	reg_t TargetHeight;
	reg_t ImgFormat;
	reg_t BaseAddress;
	reg_t BaseAddress1;
	reg_t BaseAddress2;
	reg_t Interlaced;
	reg_t ContinuousMode;
	reg_t SyncMode;
	reg_t AlphaValue;
	reg_t Hue;
	reg_t Bright;
	reg_t Contrast;
} VIOC_WDMA_IMAGE_INFO_Type;

typedef	struct
{
	reg_t	FMT		:  5;
	reg_t			:  2;
	reg_t	BR		:  1;
	reg_t	R2Y 	:  1;
	reg_t	R2YMD	:  2;
	reg_t			:  1;
	reg_t	SWAP 	:  3;
	reg_t			:  1;	//16
	reg_t	UPD  	:  1;
	reg_t	Y2R		:  1;
	reg_t	Y2RMD	:  2;
	reg_t			:  2;
	reg_t	SREQ	:  1;
	reg_t	CONT	:  1;
	reg_t	DITHEN	:  1;
	reg_t			:  2;
	reg_t	DITHSEL	:  1;
	reg_t	IEN		:  1;
	reg_t	FU		:  1;
	reg_t			:  1;
	reg_t	INTL	:  1;
}	VIOC_WDMA_CTRL;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_CTRL		bREG;
}	VIOC_WDMA_CTRL_u;

typedef	struct
{
	reg_t	SYNCSEL 	:  8;
	reg_t	SEN			:  1;
	reg_t	SYNCMD		:  3;
	reg_t				:  4;
	reg_t	MAXRATE		:  8;
	reg_t				:  7;
	reg_t	REN			:  1;
}	VIOC_WDMA_RATE;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_RATE		bREG;
}	VIOC_WDMA_RATE_u;

typedef	struct
{
	reg_t	WIDTH   	: 13;
	reg_t				:  3;
	reg_t	HEIGHT   	: 13;
	reg_t				:  3;
}	VIOC_WDMA_SIZE;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_SIZE		bREG;
}	VIOC_WDMA_SIZE_u;

typedef	struct
{
	reg_t	OFFSET0   	: 16;
	reg_t	OFFSET1   	: 16;
}	VIOC_WDMA_OFFS;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_OFFS		bREG;
}VIOC_WDMA_OFFS_u;

typedef	struct
{
	reg_t	BG0		: 8;
	reg_t	BG1		: 8;
	reg_t	BG2		: 8;
	reg_t	BG3		: 8;
}VIOC_WDMA_BG;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_BG		bREG;
}VIOC_WDMA_BG_u;

typedef	struct
{
	reg_t	PTS			: 16;
	reg_t			    : 16;
}VIOC_WDMA_PTS;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_PTS		bREG;
}VIOC_WDMA_PTS_u;

typedef	struct
{
	reg_t	DITH00	: 3;
	reg_t			: 1;
	reg_t	DITH01	: 3;
	reg_t			: 1;
	reg_t	DITH02	: 3;
	reg_t			: 1;
	reg_t	DITH03	: 3;
	reg_t			: 1;
	reg_t	DITH10	: 3;
	reg_t			: 1;
	reg_t	DITH11	: 3;
	reg_t			: 1;
	reg_t	DITH12	: 3;
	reg_t			: 1;
	reg_t	DITH13	: 3;
	reg_t			: 1;

	reg_t	DITH20	: 3;
	reg_t			: 1;
	reg_t	DITH21	: 3;
	reg_t			: 1;
	reg_t	DITH22	: 3;
	reg_t			: 1;
	reg_t	DITH23	: 3;
	reg_t			: 1;
	reg_t	DITH30	: 3;
	reg_t			: 1;
	reg_t	DITH31	: 3;
	reg_t			: 1;
	reg_t	DITH32	: 3;
	reg_t			: 1;
	reg_t	DITH33	: 3;
	reg_t			: 1;
}VIOC_WDMA_DMAT;

typedef	union
{
	reg_t				nREG[2];
	VIOC_WDMA_DMAT		bREG;
}VIOC_WDMA_DMAT_u;

//AnD_TODO: check EHN register that is no exist in the datasheet !!!!
typedef	struct
{
	reg_t	CONTRAST	: 8;
	reg_t	BRIGHT		: 8;
	reg_t	HUE			: 8;
	reg_t				: 8;
}VIOC_WDMA_ENH;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_ENH		bREG;
}VIOC_WDMA_ENH_u;

typedef	struct
{
	reg_t	ROLLCNT   	: 16;
	reg_t		     	: 15;
	reg_t	ROLL     	:  1;
}VIOC_WDMA_ROLL;

typedef	union
{
	reg_t				nREG;
	VIOC_WDMA_ROLL		bREG;
}VIOC_WDMA_ROLL_u;

typedef	struct
{
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	STOPREQ		:  1;	// Stop Request
	reg_t	ROLL 		:  1;	// Roll Interrupt
	reg_t	ENRISE		:  1;	// Frame Synchronized Enable Rising
	reg_t	ENFALL		:  1;	// Frame Synchronized Enable Falling (Disable-Done)
	reg_t	EOFRISE		:  1;	// EOF Rising
	reg_t	EOFFALL		:  1;	// EOF Falling
	reg_t	SEOFRISE	:  1;	// SYNC EOF Rising
	reg_t	SEOFFALL	:  1;	// SYNC EOF Falling
	reg_t				:  20;
	reg_t	STS_SEN		:  1;	// Synchronized Enable
	reg_t	STS_BFIELD	:  1;	// Bottom field indicator
	reg_t	STS_EOF		:  1;	// EOF
}VIOC_WDMA_STATUS;

typedef	union
{
	reg_t		nREG;
	VIOC_WDMA_STATUS	bREG;
}VIOC_WDMA_STATUS_u;

typedef	struct
{
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	STOPREQ		:  1;	// Stop Request
	reg_t	ROLL 		:  1;	// Roll Interrupt
	reg_t	ENRISE		:  1;	// Synchronized Enable Rising
	reg_t	ENFALL		:  1;	// Synchronized Enable Falling (Disable-Done)
	reg_t	EOFRISE		:  1;	// EOF Rising
	reg_t	EOFFALL		:  1;	// EOF Falling
	reg_t	SEOFRISE	:  1;	// SYNC EOF Rising
	reg_t	SEOFFALL	:  1;	// SYNC EOF Falling
	reg_t				:  23;
}VIOC_WDMA_IRQMSK;

typedef	union
{
	reg_t		nREG;
	VIOC_WDMA_IRQMSK	bREG;
}VIOC_WDMA_IRQMSK_u;

typedef	struct _VIOC_WDMA
{
	volatile VIOC_WDMA_CTRL_u 		uCTRL; 			// 0x00  R/W  0x00000400 WMDA Control Register
	volatile VIOC_WDMA_RATE_u 		uRATE; 			// 0x04  R/W  0x00000000 WDMA Rate Control Register
	volatile VIOC_WDMA_SIZE_u 		uSIZE; 			// 0x08  R/W  0x00000000 WDMA Size Register
	volatile reg_t 					nBASE0; 		// 0x0C  R/W  0x00000000 WDMA Base Address 0 Register
	volatile reg_t 					nCBASE; 		// 0x10  R/W  0x00000000 WDMA Current Address Register
	volatile reg_t 					nBASE1; 		// 0x14  R/W  0x00000000 WDMA Base Address 1 Register
	volatile reg_t 					nBASE2; 		// 0x18  R/W  0x00000000 WDMA Base Address 2 Register
	volatile VIOC_WDMA_OFFS_u 		uOFFSET;		// 0x1C  R/W  0x00000000 WDMA Offset Register
	volatile reg_t 					reserved0; 		// 0x20
	volatile VIOC_WDMA_BG_u 		uBG; 			// 0x24  R/W  0x00000000 WDMA Back Ground Color Register
	volatile VIOC_WDMA_PTS_u 		uPTS; 			// 0x2C  R/W  0x00000000 WDMA PTS Register
	volatile VIOC_WDMA_DMAT_u 		uDMAT; 			// 0x30, 0x34  R/W  0x00000000 WDMA Dither Matrix 0,1 Register
	volatile VIOC_WDMA_ENH_u 		uENH; 			// 0x38  R/W  0x00000000 WDMA Color Enhancement Register
	volatile VIOC_WDMA_ROLL_u 		uROLL; 			// 0x3C  R/W  0x00000000 WDMA Rolling Control Register
	volatile reg_t 					nSBASE; 		// 0x40  R/W  0x00000000 WMA Synchronized Base Address Register
	volatile VIOC_WDMA_STATUS_u 	uIRQSTS; 		// 0x44  R/W  0x00000000 WDMA Interrupt Status Register
	volatile VIOC_WDMA_IRQMSK_u 	uIRQMSK; 		// 0x48  R/W  0x000001FF WDMA Interrupt Mask Register
}VIOC_WDMA, *PVIOC_WDMA;

#endif	/*__VIOC_WDMA_H__*/
