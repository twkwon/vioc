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
	unsigned		FMT		:  5;
	unsigned		Y2RM2	:  1;
	unsigned		R2YM2	:  1;
	unsigned		BR  	:  1;
	unsigned		Y2R 	:  1;
	unsigned		Y2RMD	:  2;
	unsigned		AEN  	:  1;
	unsigned		SWAP 	:  3;
	unsigned		PD   	:  1;//16
	unsigned		UPD  	:  1;
	unsigned		R2Y		:  1;
	unsigned		R2YMD	:  2;
	unsigned		DITS	:  1;
	unsigned		DIT		:  1;
	unsigned		NUVIH	:  1;
	unsigned		UVI  	:  1;
	unsigned		ASEL 	:  1;
	unsigned		DM_3D	:  2;
	unsigned		STRM 	:  1;
	unsigned		IEN  	:  1;
	unsigned		BFIELD	:  1;
	unsigned		BFMD	:  1;
	unsigned		INTL	:  1;
}	VIOC_RDMA_CTRL;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_CTRL		bREG;
} VIOC_RDMA_CTRL_u;

typedef	struct
{
	unsigned	TOP		: 16;
	unsigned	BOT		: 16;
} VIOC_RDMA_PTS;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_PTS		bREG;
} VIOC_RDMA_PTS_u;

typedef	struct
{
	unsigned	WIDTH   	: 13;
	unsigned	RESERVE0	:  3;
	unsigned	HEIGHT   	: 13;
	unsigned	RESERVE1	:  3;
} VIOC_RDMA_SIZE;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_SIZE		bREG;
} VIOC_RDMA_SIZE_u;

typedef	struct
{
	unsigned	OFFSET0   	: 16;
	unsigned	OFFSET1   	: 16;
} VIOC_RDMA_OFFS;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_OFFS		bREG;
} VIOC_RDMA_OFFS_u;

typedef	struct
{
	unsigned	FMT		: 5;
	unsigned	FMT10	: 3;
	unsigned	Y2RMD	: 3;
	unsigned	RESERVE0: 1;
	unsigned	R2YMD	: 3;
	unsigned	RESERVE1: 1;
	unsigned	LPEN	: 1;
	unsigned	RESERVE2: 3;
	unsigned	ISSUE	: 12;
} VIOC_RDMA_MISC;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_MISC		bREG;
} VIOC_RDMA_MISC_u;

typedef	struct
{
	unsigned	ALPHA0		: 10;
	unsigned				: 2;
	unsigned	SEL			: 1;
	unsigned				: 3;
	unsigned	ALPHA1		: 10;
	unsigned				: 6;
} VIOC_RDMA_ALPHA;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_ALPHA		bREG;
} VIOC_RDMA_ALPHA_u;

typedef	struct
{
	unsigned	CFGDONE	:  1;	// update
	unsigned	EOFRISE		:  1;	// Device EOF Rising
	unsigned	EOFFALL	:  1;	// Device EOF Falling
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	EOFWAITR	:  1;	// EOF Wait Rising - Frame End
	unsigned	TOPRDY		:  1;	// Top Ready
	unsigned	BOTRDY		:  1;	// Bottom Ready
	unsigned	RESERVE0	:  9;
	unsigned	STS_TOPRDY :  1;	// TOP field Ready
	unsigned	STS_BOTRDY :  1;	// BOTTOM field Ready
	unsigned	STS_EOFWAIT:  1;	// RDMA eof-wait status
	unsigned	STS_DEVEOF	:  1;	// Device EOF
	unsigned	STS_BFIELD	:  1;	// Bottom field indicator
	unsigned	RESERVE1	:  7;
	unsigned	STS_FDLY	:  4;	// frame delay
} VIOC_RDMA_STATUS;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_STATUS	bREG;
} VIOC_RDMA_STATUS_u;

typedef	struct
{
	unsigned	CFGDONE	:  1;	// update
	unsigned	DEOFR	:  1;	// Device EOF Rising
	unsigned	DEOFF	:  1;	// Device EOF Falling
	unsigned	UPDDONE	:  1;	// Register Update Done
	unsigned	EOFWAITR:  1;	// EOF Wait Rising - Frame End
	unsigned	TOPRDY	:  1;	// Top Ready
	unsigned	BOTRDY	:  1;	// Bottom Ready
	unsigned	RESERVE0		:  9;
	unsigned	RESERVE1		: 16;
} VIOC_RDMA_IRQMSK;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_IRQMSK	bREG;
} VIOC_RDMA_IRQMSK_u;

typedef	struct
{
	unsigned	WIDTH	: 16;
	unsigned	HEIGHT	: 16;
} VIOC_RDMA_CROP_SIZE;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_CROP_SIZE	bREG;
} VIOC_RDMA_CROP_SIZE_u;

typedef	struct
{
	unsigned	XPOS	: 16;
	unsigned	YPOS	: 16;
} VIOC_RDMA_CROP_POS;

typedef	union
{
	unsigned long		nREG;
	VIOC_RDMA_CROP_POS	bREG;
}VIOC_RDMA_CROP_POS_u;

typedef struct {
	unsigned	enable	:  1;
	unsigned			: 15;
	unsigned	cfgupd	:  1;
	unsigned			: 15;
}VIOC_DEC_CTRL;

typedef union {
	unsigned		nReg;
	VIOC_DEC_CTRL	bReg;
}VIOC_DEC_CTRL_u;

typedef struct {
	unsigned	has_alpha	:  4;
	unsigned	has_comp	:  4;
	unsigned				:  8;
	unsigned	bypass		:  1;
	unsigned				: 15;
}VIOC_DEC_HAS;

typedef union {
	unsigned		nReg;
	VIOC_DEC_HAS	bReg;
}VIOC_DEC_HAS_u;

typedef struct {
	unsigned	debug		:  6;
	unsigned				: 26;
}VIOC_DEC_DEBUG;

typedef union {
	unsigned		nReg;
	VIOC_DEC_DEBUG	bReg;
}VIOC_DEC_DEBUG_u;

typedef struct
{
    unsigned    CFGDONE :  1;   // Update
    unsigned    DEOFR   :  1;   // Device EOF Rising
    unsigned    DEOFF   :  1;   // Device EOF Falling
    unsigned    EOFWAITR:  1;   // EOF Wait Rising - Frame End
    unsigned    RESERVE0        : 12;
    unsigned    RESERVE1        : 16;
} VIOC_DEC_IRQMSK;

typedef union
{
    unsigned long       nREG;
    VIOC_DEC_IRQMSK    	bREG;
} VIOC_DEC_IRQMSK_u;

typedef	struct _VIOC_RDMA
{
	volatile VIOC_RDMA_CTRL_u	uCTRL;				// 0x00  R/W  0x00000000  RDMA Control Register
	volatile VIOC_RDMA_PTS_u	uPTS;				// 0x04  R/W  0x00000000  RDMA Image PTS Register
	volatile VIOC_RDMA_SIZE_u	uSIZE;				// 0x08  R/W  0x00000000  RDMA Image Size information Register 
	volatile unsigned  int		nBASE0;			// 0x0C  R/W  0x00000000  RDMA Base0 Address for Each Images Register
	volatile unsigned  int 		nCBASE;			// 0x10  R/W	 0x00000000  RDMA Current Address for Each Images Register
	volatile unsigned  int		nBASE1;			// 0x14  R/W  0x00000000  RDMA Base1 Address for Each Images Register
	volatile unsigned  int		nBASE2;			// 0x18  R/W  0x00000000  RDMA Base2 Address for Each Images Register
	volatile VIOC_RDMA_OFFS_u  	uOFFSET;		// 0x1C  R/W  0x00000000  RDMA Offset Information for Each Images Register
	volatile VIOC_RDMA_MISC_u	uMISC;			// 0x20  R/W  0x00000000  RDMA Misc. for Each images Register
	volatile VIOC_RDMA_ALPHA_u 	uALPHA;			// 0x24  R/W  0x00000000  RDMA Alpha Information for Each Imgaes Register
	volatile VIOC_RDMA_STATUS_u	uSTATUS;		// 0x28  R/W	 0x00000000  RDMA Status Register
	volatile VIOC_RDMA_IRQMSK_u	uIRQMSK;		// 0x2C  R/W  0x0000007F  RDMA interrupt mask Register
	volatile unsigned  int			nSBASE0;		// 0x30  R/W  0x00000000  RDMA Sync base address
	volatile unsigned  int			nRBASE0;		// 0x34  R/W  0x00000000  RDMA 3D base0 address
	volatile unsigned  int			nRBASE1;		// 0x38  R/W  0x00000000  RDMA 3D base1 address
	volatile unsigned  int			nRBASE2;		// 0x3C  R/W  0x00000000  RDMA 3D base2 address
	volatile VIOC_RDMA_CROP_SIZE_u	uCROPSIZE;		// 0x40  R/W  0x00000000  RDMA Crop Size for Each Images Register
	volatile VIOC_RDMA_CROP_POS_u	uCROPPOS;		// 0x44  R/W  0x00000000  RDMA Crop Posisiton for Each Images Register
	volatile unsigned  int			reserved2[14];
	volatile VIOC_DEC_CTRL_u		uDEC_CTRL;		// 0x80  R/W  0x00000000  RDMA DEC100 Control Register
	volatile unsigned  int			nDEC_BASE;		// 0x84  R/W  0x00000000  RDMA DEC100 Base Address Register
	volatile unsigned  int			nDEC_LENGTH;	// 0x88  R/W  0x00000000  RDMA DEC100 Length Register
	volatile VIOC_DEC_HAS_u			uDEC_HAS;		// 0x8c  R/W  0x00000000  RDMA DEC100 HAS CFG Register
	volatile VIOC_DEC_DEBUG_u		uDEC_DEBUG;		// 0x90  R/W  0x00000000  RDMA DEC100 Debug Register
	volatile unsigned  int			nDEC_CADDR;		// 0x94  R/W  0x00000000  RDMA DEC100 Current Address register
	volatile unsigned  int			nDEC_EOF;		// 0x98  R/W  0x00000000  RDMA DEC100 Interrupt Register
	volatile VIOC_DEC_IRQMSK_u		uDEC_IRQMSK;	// 0x9C  R/W  0x00000000  RDMA DEC100 Interrupt Mask Register
} VIOC_RDMA, *PVIOC_RDMA;

// RDMA Control Reg
#define HwDMA_INTL                      Hw31                                // Interlaced Image
#define HwDMA_BFMD                      Hw30                                // Bfield mode
#define HwDMA_BF                        Hw29                                // Bottom field
#define HwDMA_IEN                       Hw28                                // Image Display Function for Each Image
#define HwDMA_STRM                      Hw27                                // streaming mode
#define HwDMA_3DMD				Hw26 + Hw25				// 3D mode type
#define HwDMA_ASEL                      Hw24                                // Image Displaying Function for Each Image
#define HwDMA_UVI                       Hw23                                // UV ineterpolation
#define HwDMA_R2YMD				Hw19+Hw18				// RGB to YUV converter mode register
#define HWDMA_R2Y				Hw17					// RGB to YUV converter enable register
#define HwDMA_UPD                       Hw16                                // data update enable
#define HwDMA_PD                        Hw15                                // Bit padding
#define HwDMA_SWAP				Hw14+Hw13+Hw12			// RGB swap register
#define HwDMA_AEN				Hw11					// Alpha enable register
#define HwDMA_Y2RMD                     (Hw10+Hw9)                          // YCbCr to RGB Conversion Option
#define HwDMA_Y2R                       Hw8                                 // YCbCr to RGB Conversion Enable Bit
#define HwDMA_BR                        Hw7                                 // Bit Reverse
#define HwDMA_R2YM2						Hw6                                 // R2YM[2]
#define HwDMA_Y2RM2						Hw5                                 // Y2RM[2]
#define HwDMA_FMT                       (Hw4+Hw3+Hw2+Hw1+Hw0)               // Image Format

#endif
