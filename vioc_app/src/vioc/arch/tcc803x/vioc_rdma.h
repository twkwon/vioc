#ifndef __VIOC_RDMA_H__
#define	__VIOC_RDMA_H__

#define VIOC_RDMA_STAT_ICFG		0x00000001UL
#define VIOC_RDMA_STAT_IEOFR		0x00000002UL
#define VIOC_RDMA_STAT_IEOFF 		0x00000004UL
#define VIOC_RDMA_STAT_IUPDD 		0x00000008UL
#define VIOC_RDMA_STAT_IEOFFW		0x00000010UL
#define VIOC_RDMA_STAT_ITOPR 		0x00000020UL
#define VIOC_RDMA_STAT_IBOTR 		0x00000040UL
#define VIOC_RDMA_STAT_ALL 		0x0000007FUL

#define VIOC_RDMA_IREQ_ICFG_MASK		0x00000001UL
#define VIOC_RDMA_IREQ_IEOFR_MASK		0x00000002UL
#define VIOC_RDMA_IREQ_IEOFF_MASK 	0x00000004UL
#define VIOC_RDMA_IREQ_IUPDD_MASK 	0x00000008UL
#define VIOC_RDMA_IREQ_IEOFFW_MASK 	0x00000010UL
#define VIOC_RDMA_IREQ_ITOPR_MASK 	0x00000020UL
#define VIOC_RDMA_IREQ_IBOTR_MASK 	0x00000040UL
#define VIOC_RDMA_IREQ_ALL_MASK		0x0000007FUL

typedef	struct
{
	reg_t		FMT		:  5;
	reg_t		Y2RM2	:  1;
	reg_t		R2YM2	:  1;
	reg_t		BR  	:  1;
	reg_t		Y2R 	:  1;
	reg_t		Y2RMD	:  2;
	reg_t		AEN  	:  1;
	reg_t		SWAP 	:  3;
	reg_t		PD   	:  1;	//16
	reg_t		UPD  	:  1;
	reg_t		R2Y		:  1;
	reg_t		R2YMD	:  2;
	reg_t		DITS	:  1;
	reg_t		DIT		:  1;
	reg_t		NUVIH	:  1;
	reg_t		UVI  	:  1;
	reg_t		ASEL 	:  1;
	reg_t		DM_3D	:  2;
	reg_t		STRM 	:  1;
	reg_t		IEN  	:  1;
	reg_t		BFIELD	:  1;
	reg_t		BFMD	:  1;
	reg_t		INTL	:  1;
}	VIOC_RDMA_CTRL;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_CTRL		bREG;
} VIOC_RDMA_CTRL_u;

typedef	struct
{
	reg_t	TOP			: 16;
	reg_t	BOT			: 16;
} VIOC_RDMA_PTS;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_PTS		bREG;
} VIOC_RDMA_PTS_u;

typedef	struct
{
	reg_t	WIDTH   	: 13;
	reg_t				:  3;
	reg_t	HEIGHT   	: 13;
	reg_t				:  3;
} VIOC_RDMA_SIZE;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_SIZE		bREG;
} VIOC_RDMA_SIZE_u;

typedef	struct
{
	reg_t	OFFSET0   	: 16;
	reg_t	OFFSET1   	: 16;
} VIOC_RDMA_OFFS;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_OFFS		bREG;
} VIOC_RDMA_OFFS_u;

//typedef	struct
//{
//	reg_t	XSCALE		: 3;
//	reg_t				:13;
//	reg_t	YSCALE		: 3;
//	reg_t				: 1;
//	reg_t	ISSUE		:12;
//} VIOC_RDMA_SCALE;
//
//typedef	union
//{
//	reg_t				nREG;
//	VIOC_RDMA_SCALE		bREG;
//} VIOC_RDMA_SCALE_u;

typedef	struct
{
	reg_t	FMT			: 5;
	reg_t	FMT10		: 3;
	reg_t	Y2RMD		: 3;
	reg_t				: 1;
	reg_t	R2YMD		: 3;
	reg_t				: 1;
	reg_t	LPEN		: 1;
	reg_t				: 3;
	reg_t	ISSUE		: 12;
} VIOC_RDMA_MISC;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_MISC		bREG;
} VIOC_RDMA_MISC_u;

typedef	struct
{
	reg_t	ALPHA0		: 8;
	reg_t				: 8;
	reg_t	ALPHA1		: 8;
	reg_t				: 8;
} VIOC_RDMA_ALPHA;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_ALPHA		bREG;
} VIOC_RDMA_ALPHA_u;

typedef	struct
{
	reg_t	CFGDONE		:  1;	// update
	reg_t	EOFRISE		:  1;	// Device EOF Rising
	reg_t	EOFFALL		:  1;	// Device EOF Falling
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	EOFWAITR	:  1;	// EOF Wait Rising - Frame End
	reg_t	TOPRDY		:  1;	// Top Ready
	reg_t	BOTRDY		:  1;	// Bottom Ready
	reg_t				:  9;
	reg_t	STS_TOPRDY	:  1;	// TOP field Ready
	reg_t	STS_BOTRDY	:  1;	// BOTTOM field Ready
	reg_t	STS_EOFWAIT	:  1;	// RDMA eof-wait status
	reg_t	STS_DEVEOF	:  1;	// Device EOF
	reg_t	STS_BFIELD	:  1;	// Bottom field indicator
	reg_t				:  7;
	reg_t	STS_FDLY	:  4;	// frame delay
} VIOC_RDMA_STATUS;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_STATUS	bREG;
} VIOC_RDMA_STATUS_u;

typedef	struct
{
	reg_t	CFGDONE		:  1;	// update
	reg_t	DEOFR		:  1;	// Device EOF Rising
	reg_t	DEOFF		:  1;	// Device EOF Falling
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	EOFWAITR	:  1;	// EOF Wait Rising - Frame End
	reg_t	TOPRDY		:  1;	// Top Ready
	reg_t	BOTRDY		:  1;	// Bottom Ready
	reg_t				: 25;
} VIOC_RDMA_IRQMSK;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_IRQMSK	bREG;
} VIOC_RDMA_IRQMSK_u;

typedef	struct
{
	reg_t	WIDTH		: 16;
	reg_t	HEIGHT		: 16;
} VIOC_RDMA_CROP_SIZE;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_CROP_SIZE	bREG;
} VIOC_RDMA_CROP_SIZE_u;

typedef	struct
{
	reg_t	XPOS		: 16;
	reg_t	YPOS		: 16;
} VIOC_RDMA_CROP_POS;

typedef	union
{
	reg_t				nREG;
	VIOC_RDMA_CROP_POS	bREG;
}VIOC_RDMA_CROP_POS_u;


typedef	struct _VIOC_RDMA
{
	volatile VIOC_RDMA_CTRL_u		uCTRL;			// 0x00  R/W  0x00000000  RDMA Control Register
	volatile VIOC_RDMA_PTS_u		uPTS;			// 0x04  R/W  0x00000000  RDMA Image PTS Register
	volatile VIOC_RDMA_SIZE_u		uSIZE;			// 0x08  R/W  0x00000000  RDMA Image Size information Register 
	volatile reg_t					nBASE0;			// 0x0C  R/W  0x00000000  RDMA Base0 Address for Each Images Register
	volatile reg_t					nCBASE;			// 0x10  R/W	0x00000000  RDMA Current Address for Each Images Register
	volatile reg_t					nBASE1;			// 0x14  R/W  0x00000000  RDMA Base1 Address for Each Images Register
	volatile reg_t					nBASE2;			// 0x18  R/W  0x00000000  RDMA Base2 Address for Each Images Register
	volatile VIOC_RDMA_OFFS_u  		uOFFSET;		// 0x1C  R/W  0x00000000  RDMA Offset Information for Each Images Register
	//volatile VIOC_RDMA_SCALE_u		uMISC;			// 0x20  R/W  0x00000000  RDMA SCALE. for Each images Register
	volatile VIOC_RDMA_MISC_u		uMISC;			// 0x20  R/W  0x00000000  RDMA Misc. for Each images Register
	volatile VIOC_RDMA_ALPHA_u 		uALPHA;			// 0x24  R/W  0x00000000  RDMA Alpha Information for Each Imgaes Register
	volatile VIOC_RDMA_STATUS_u		uSTATUS;		// 0x28  R/W	0x00000000  RDMA Status Register
	volatile VIOC_RDMA_IRQMSK_u		uIRQMSK;		// 0x2C  R/W  0x0000007F  RDMA interrupt mask Register
	volatile reg_t					nSBASE0;		// 0x30  R/W  0x00000000  RDMA Sync base address
	volatile reg_t					nRBASE0;		// 0x34  R/W  0x00000000  RDMA 3D base0 address
	volatile reg_t					nRBASE1;		// 0x38  R/W  0x00000000  RDMA 3D base1 address
	volatile reg_t					nRBASE2;		// 0x3C  R/W  0x00000000  RDMA 3D base2 address
	volatile VIOC_RDMA_CROP_SIZE_u	uCROPSIZE;		// 0x40  R/W  0x00000000  RDMA Crop Size for Each Images Register
	volatile VIOC_RDMA_CROP_POS_u	uCROPPOS;		// 0x44  R/W  0x00000000  RDMA Crop Posisiton for Each Images Register
} VIOC_RDMA, *PVIOC_RDMA;

#endif	/*__VIOC_RDMA_H__*/