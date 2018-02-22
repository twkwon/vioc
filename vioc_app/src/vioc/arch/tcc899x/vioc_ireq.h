#ifndef __VIOC_IREQ_H__
#define	__VIOC_IREQ_H__

typedef enum
{
	VIOC_IREQ_DEV0   = 0,
	VIOC_IREQ_DEV1   = 1,
	VIOC_IREQ_DEV2   = 2,
	VIOC_IREQ_TIMER  = 3,
	VIOC_IREQ_RDMA00 = 4,
	VIOC_IREQ_RDMA01 = 5,
	VIOC_IREQ_RDMA02 = 6,
	VIOC_IREQ_RDMA03 = 7,
	VIOC_IREQ_RDMA04 = 8,
	VIOC_IREQ_RDMA05 = 9,
	VIOC_IREQ_RDMA06 = 10,
	VIOC_IREQ_RDMA07 = 11,
	VIOC_IREQ_RDMA08 = 12,
	VIOC_IREQ_RDMA09 = 13,
	VIOC_IREQ_RDMA10 = 14,
	VIOC_IREQ_RDMA11 = 15,
	VIOC_IREQ_RDMA12 = 16,
	VIOC_IREQ_RDMA13 = 17,
	VIOC_IREQ_RDMA14 = 18,
	VIOC_IREQ_RDMA15 = 19,
	VIOC_IREQ_RDMA16 = 20,
	VIOC_IREQ_RDMA17 = 21,
	VIOC_IREQ_MC0    = 22,
	VIOC_IREQ_MC1    = 23,
	VIOC_IREQ_MMU    = 24,
	VIOC_IREQ_DTRC0  = 25,
	VIOC_IREQ_DTRC1  = 26,
	VIOC_IREQ_RESERVED_27,
	VIOC_IREQ_FIFO0  = 28,
	VIOC_IREQ_FIFO1  = 29,
	VIOC_IREQ_RESERVED_30,
	VIOC_IREQ_RESERVED_31,
	VIOC_IREQ_WDMA00 = 32,
	VIOC_IREQ_WDMA01 = 33,
	VIOC_IREQ_WDMA02 = 34,
	VIOC_IREQ_WDMA03 = 35,
	VIOC_IREQ_WDMA04 = 36,
	VIOC_IREQ_WDMA05 = 37,
	VIOC_IREQ_WDMA06 = 38,
	VIOC_IREQ_WDMA07 = 39,
	VIOC_IREQ_WDMA08 = 40,
	VIOC_IREQ_RESERVED_41,
	VIOC_IREQ_RESERVED_42,
	VIOC_IREQ_SC4    = 43,
	VIOC_IREQ_RESERVED_44,
	VIOC_IREQ_VIQE1  = 45,
	VIOC_IREQ_RESERVED_46,
	VIOC_IREQ_RESERVED_47,
	VIOC_IREQ_WMIX0  = 48,
	VIOC_IREQ_WMIX1  = 49,
	VIOC_IREQ_WMIX2  = 50,
	VIOC_IREQ_WMIX3  = 51,
	VIOC_IREQ_WMIX4  = 52,
	VIOC_IREQ_WMIX5  = 53,
	VIOC_IREQ_WMIX6  = 54,
	VIOC_IREQ_VIN0   = 55,
	VIOC_IREQ_VIN1   = 56,
	VIOC_IREQ_VIN2   = 57,
	VIOC_IREQ_VIN3   = 58,
	VIOC_IREQ_VIQE   = 59,
	VIOC_IREQ_SC0    = 60,
	VIOC_IREQ_SC1    = 61,
	VIOC_IREQ_SC2    = 62,
	VIOC_IREQ_SC3    = 63,
	VIOC_IREQ_MAX    = VIOC_IREQ_SC3
} VIOC_IREQ_VECTOR_ID_Type;

//check
typedef	struct 
{
	reg_t				DEV0		: 1;	// 0
	reg_t				DEV1		: 1;	// 1
	reg_t				DEV2		: 1;	// 2
	reg_t				TIMER		: 1;	// 3
	reg_t				RDMA00		: 1;	// 4
	reg_t				RDMA01		: 1;	// 5
	reg_t				RDMA02		: 1;	// 6
	reg_t				RDMA03		: 1;	// 7
	reg_t				RDMA04		: 1;	// 8
	reg_t				RDMA05		: 1;	// 9
	reg_t				RDMA06		: 1;	// 10
	reg_t				RDMA07		: 1;	// 11
	reg_t				RDMA08		: 1;	// 12
	reg_t				RDMA09		: 1;	// 13
	reg_t				RDMA10		: 1;	// 14
	reg_t				RDMA11		: 1;	// 15
	reg_t				RDMA12		: 1;	// 16
	reg_t				RDMA13		: 1;	// 17
	reg_t				RDMA14		: 1;	// 18
	reg_t				RDMA15		: 1;	// 19
	reg_t				RDMA16		: 1;	// 20
	reg_t				RDMA17		: 1;	// 21
	reg_t 				MC0			: 1;	// 22
	reg_t 				MC1			: 1;	// 23
	reg_t							: 3;	// 24, 25, 26
	reg_t 				V_DV		: 1;	// 27
	reg_t				FIFO0		: 1;	// 28
	reg_t				FIFO1		: 1;	// 29
	reg_t 				EDRBL		: 1;	// 30
	reg_t				EDREL		: 1;	// 31

	reg_t				WDMA00		: 1;	// 32
	reg_t				WDMA01		: 1;	// 33
	reg_t				WDMA02		: 1;	// 34
	reg_t				WDMA03		: 1;	// 35
	reg_t				WDMA04		: 1;	// 36
	reg_t				WDMA05		: 1;	// 37
	reg_t				WDMA06		: 1;	// 38
	reg_t				WDMA07		: 1;	// 39
	reg_t				WDMA08		: 1;	// 40
	reg_t 							: 2;	// 41, 42
	reg_t				SC4			: 1;	// 43
	reg_t 				SC5			: 1;	// 44
	reg_t 				VIQE1		: 1;	// 45
	reg_t 				OSD0		: 1;	// 46
	reg_t				OSD1		: 1;	// 47
	reg_t				WMIX0		: 1;	// 48
	reg_t				WMIX1		: 1;	// 49
	reg_t				WMIX2		: 1;	// 50
	reg_t				WMIX3		: 1;	// 51
	reg_t				WMIX4		: 1;	// 52
	reg_t				WMIX5		: 1;	// 53
	reg_t				WMIX6		: 1;	// 54
	reg_t				VIN0		: 1;	// 55
	reg_t				VIN1		: 1;	// 56
	reg_t				VIN2		: 1;	// 57
	reg_t				VIN3		: 1;	// 58
	reg_t				VIQE		: 1;	// 59
	reg_t				SC0			: 1;	// 60
	reg_t				SC1			: 1;	// 61
	reg_t				SC2			: 1;	// 62
	reg_t				SC3			: 1;	// 63

	reg_t				AFBC_DEC0	: 1;	// 64
	reg_t				AFBC_DEC1	: 1;	// 65
	reg_t				AFBC_ENC0	: 1;	// 66
	reg_t				AFBC_ENC1	: 1;	// 67
	reg_t				DV_IN		: 1;	// 68
	reg_t				SAR0		: 1;	// 69
	reg_t				SAR1		: 1;	// 70
	reg_t							:25;	// 71 ~ 95
}	VIOC_IREQ_IREQ;

typedef	union {
	reg_t					nREG[3];
	VIOC_IREQ_IREQ			bREG;
}	VIOC_IREQ_IREQ_u;

typedef struct {
	reg_t 				INDEX0	: 7;
	reg_t 		 		INDEX1	: 7;
	reg_t 				INDEX2	: 7;
	reg_t				INDEX3	: 7;
	reg_t 				IVALID0	: 1;
	reg_t 				IVALID1	: 1;
	reg_t 				IVALID2	: 1;
	reg_t 				IVALID3	: 1;
}VIOC_IREQ_VECTORID;

typedef union
{
	reg_t					nREG;
	VIOC_IREQ_VECTORID		bREG;
}VIOC_IREQ_VECTORID_u;

typedef struct {
	reg_t 				LVIN0	: 2;
	reg_t 				LVIN1	: 2;
	reg_t 				LVIN2	: 2;
	reg_t 				LVIN3	: 2;
	reg_t 		 				: 24;
} VIOC_CONFIG_TEST_LOOP;

typedef union
{
	reg_t					nREG;
	VIOC_CONFIG_TEST_LOOP	bREG;
} VIOC_CONFIG_TEST_LOOP_u;

typedef struct {
	reg_t						: 8;
	reg_t				EDR_S   : 1;
	reg_t						: 23;
} VIOC_CONFIG_PATH_EDR;

typedef union
{
	reg_t					nREG;
	VIOC_CONFIG_PATH_EDR	bREG;
} VIOC_CONFIG_PATH_EDR_u;


typedef struct 
{
	reg_t 			L0_EVS_SEL	:  3;
	reg_t 						:  1;
	reg_t 			L1_EVS_SEL	:  3;
	reg_t 						:  1;
	reg_t 			L2_EVS_SEL	:  3;
	reg_t 						:  5;
	reg_t 				MIX00	:  1;	// 16
	reg_t 				MIX03	:  1;	// 17
	reg_t 				MIX10	:  1;	// 18
	reg_t 				MIX13	:  1;	// 19
	reg_t 						:  2;	// 21
	reg_t 				MIX30	:  1;	// 22
	reg_t 						:  1;	// 23
	reg_t 				MIX40	:  1;	// 24
	reg_t 						:  1;	// 25
	reg_t 				MIX50	:  1;	// 26
	reg_t 						:  1;	// 27
	reg_t 				MIX60	:  1;	// 28
	reg_t 						:  1;	// 29
	reg_t 				RD12	:  1;	// 30
	reg_t 				RD14	:  1;	// 31
}VIOC_CONFIG_MISC0;

typedef	union {
	reg_t					nREG;
	VIOC_CONFIG_MISC0		bREG;
}	VIOC_CONFIG_MISC0_u;

typedef struct {
	reg_t 				SELECT    	:  8;
	reg_t 							:  8;
	reg_t 				STATUS     	:  2;
	reg_t 				ERR      	:  1;
	reg_t 							:  5;
	reg_t 							:  7;
	reg_t 				EN		    :  1;
}VIOC_CONFIG_PATH;

typedef	union {
	reg_t					nREG;
	VIOC_CONFIG_PATH		bREG;
}VIOC_CONFIG_PATH_u;

typedef struct {
	reg_t 			AXIWD		:  1;
	reg_t 						:  7;
	reg_t 			AXIRD		:  1;
	reg_t 						:  3;
	reg_t 			AXIRD_M0_TR	:  2;
	reg_t 			AXIRD_M1_TR	:  2;
	reg_t 						:  6;
	reg_t 			S_REQ		:  1;
	reg_t 			LCD0_SEL	:  2;
	reg_t 			LCD1_SEL	:  2;
	reg_t 			LCD2_SEL	:  2;
	reg_t 						:  2;
} VIOC_CONFIG_MISC1;

typedef	union {
	reg_t				nREG;
	VIOC_CONFIG_MISC1	bREG;
} VIOC_CONFIG_MISC1_u;

typedef struct {
	reg_t				PATH_SEL0	: 5;
	reg_t				PATH_SEL1	: 5;
	reg_t							: 2;
	reg_t				AXSM_SEL  	: 20;
} VIOC_CONFIG_PATH_AFBC;

typedef union {
	reg_t					nREG;
	VIOC_CONFIG_PATH_AFBC	bREG;
}VIOC_CONFIG_PATH_AFBC_u;

typedef struct {
	reg_t 			VIN0_PATH	:  2;
	reg_t 						:  2;
	reg_t			VIN0_STAT	:  3;
	reg_t 			VIN0_EN		:  1;
	reg_t 			VIN1_PATH	:  2;
	reg_t 						:  2;
	reg_t			VIN1_STAT	:  3;
	reg_t 			VIN1_EN		:  1;
	reg_t 			VIN2_PATH	:  2;
	reg_t 						:  2;
	reg_t			VIN2_STAT	:  3;
	reg_t 			VIN2_EN		:  1;
	reg_t 			VIN3_PATH	:  2;
	reg_t 						:  2;
	reg_t			VIN3_STAT	:  3;
	reg_t 			VIN3_EN		:  1;
} VIOC_CONFIG_VIN_SEL;

typedef	union {
	reg_t				nREG;
	VIOC_CONFIG_VIN_SEL	bREG;
} VIOC_CONFIG_VIN_SEL_u;

typedef struct {
	reg_t 			DEV0_PATH	:  2;
	reg_t 						:  2;
	reg_t			DEV0_STAT	:  3;
	reg_t 			DEV0_EN		:  1;
	reg_t 			DEV1_PATH	:  2;
	reg_t 						:  2;
	reg_t			DEV1_STAT	:  3;
	reg_t 			DEV1_EN		:  1;
	reg_t 			DEV2_PATH	:  2;
	reg_t 						:  2;
	reg_t			DEV2_STAT	:  3;
	reg_t 			DEV2_EN		:  1;
	reg_t 						:  8;
} VIOC_CONFIG_DEV_SEL;

typedef	union {
	reg_t				nREG;
	VIOC_CONFIG_DEV_SEL	bREG;
} VIOC_CONFIG_DEV_SEL_u;

typedef	struct {
	reg_t				RDMA 	: 1;
	reg_t				SCALER	: 1;
	reg_t				MIXER	: 1;
	reg_t				WDMA	: 1;
	reg_t						: 1;
	reg_t				VIQE	: 1;
	reg_t				DEINTLS	: 1;
	reg_t				FILT2D	: 1;
	reg_t				FCENC	: 1;
	reg_t				FCDEC	: 1;
	reg_t				CPUIF	: 1;
	reg_t				LCD2AHB	: 1;
	reg_t				VIDEOIN	: 1;
	reg_t				DEBLOCK	: 1;
	reg_t				DEVMX	: 1;
	reg_t				MC		: 1;
	reg_t						: 1;
	reg_t				VM		: 1;
	reg_t				CIN		: 1;
	reg_t				F2D		: 1;
	reg_t				PM		: 1;
	reg_t				DV_IN	: 1;
	reg_t				SAR		: 1;
	reg_t						: 9;
}	VIOC_POWER_AUTOPD;

typedef	union {
	reg_t				nREG;
	VIOC_POWER_AUTOPD	bREG;
}	VIOC_POWER_AUTOPD_u;

typedef	struct {
	reg_t				MIN			: 4;
	reg_t							: 4;
	reg_t				EN			: 1;
	reg_t				PROF_EN		: 1;
	reg_t							: 5;
	reg_t				PFDONE		: 1;
	reg_t				PFDATA		: 16;
}	VIOC_POWER_CLKCTRL;

typedef	union {
	reg_t		nREG;
	VIOC_POWER_CLKCTRL	bREG;
}	VIOC_POWER_CLKCTRL_u;

typedef	struct {
	reg_t				RDMA00		: 1;
	reg_t				RDMA01		: 1;
	reg_t				RDMA02		: 1;
	reg_t				RDMA03		: 1;
	reg_t				RDMA04		: 1;
	reg_t				RDMA05		: 1;
	reg_t				RDMA06		: 1;
	reg_t				RDMA07		: 1;
	reg_t				RDMA08		: 1;
	reg_t				RDMA09		: 1;
	reg_t				RDMA10		: 1;
	reg_t				RDMA11		: 1;
	reg_t				RDMA12		: 1;
	reg_t				RDMA13		: 1;
	reg_t				RDMA14		: 1;
	reg_t				RDMA15		: 1;
	reg_t				RDMA16		: 1;
	reg_t				RDMA17		: 1;
	reg_t							: 6;
	reg_t				VIDEOIN0	: 1;
	reg_t				VIDEOIN1	: 1;
	reg_t				VIDEOIN2	: 1;
	reg_t				VIDEOIN3	: 1;
	reg_t				SCALER0		: 1;
	reg_t				SCALER1		: 1;
	reg_t				SCALER2		: 1;
	reg_t				SCALER3		: 1;	// bit 31

	reg_t				WDMA00		: 1;	// bit 0
	reg_t				WDMA01		: 1;
	reg_t				WDMA02		: 1;
	reg_t				WDMA03		: 1;
	reg_t				WDMA04		: 1;
	reg_t				WDMA05		: 1;
	reg_t				WDMA06		: 1;
	reg_t				WDMA07		: 1;
	reg_t				WDMA08		: 1;	// bit 8
	reg_t				WMIX0 		: 1;
	reg_t				WMIX1 		: 1;
	reg_t				WMIX2 		: 1;
	reg_t				WMIX3 		: 1;
	reg_t				WMIX4 		: 1;
	reg_t				WMIX5 		: 1;
	reg_t				WMIX6 		: 1;
	reg_t				VIQE 		: 1;	// bit 16
	reg_t				DEINTLS		: 1;
	reg_t				VIQE1		: 1;
	reg_t							: 1;
	reg_t				DEV0 		: 1;
	reg_t				DEV1 		: 1;
	reg_t				DEV2 		: 1;
	reg_t					  		: 1;
	reg_t				FILT2D 		: 1;	// bit 24
	reg_t				DEBLOCK		: 1;
	reg_t				MC0			: 1;
	reg_t				MC1			: 1;
	reg_t							: 3;
	reg_t				FIFO		: 1;	// bit 31
}	VIOC_POWER_BLOCKS;

typedef	union {
	reg_t				nREG[2];
	VIOC_POWER_BLOCKS	bREG;
}	VIOC_POWER_BLOCKS_u;

typedef	struct {
	reg_t				RDMA00		: 1;
	reg_t				RDMA01		: 1;
	reg_t				RDMA02		: 1;
	reg_t				RDMA03		: 1;
	reg_t				RDMA04		: 1;
	reg_t				RDMA05		: 1;
	reg_t				RDMA06		: 1;
	reg_t				RDMA07		: 1;
	reg_t				RDMA08		: 1;
	reg_t				RDMA09		: 1;
	reg_t				RDMA10		: 1;
	reg_t				RDMA11		: 1;
	reg_t				RDMA12		: 1;
	reg_t				RDMA13		: 1;
	reg_t				RDMA14		: 1;
	reg_t				RDMA15		: 1;
	reg_t				RDMA16		: 1;
	reg_t				RDMA17		: 1;
	reg_t				CIN0		: 1;
	reg_t				CIN1		: 1;
	reg_t							: 4;
	reg_t				F2D0		: 1;
	reg_t				F2D1		: 1;
	reg_t							: 6;
} VIOC_POWER_BLOCKS3;

typedef	union {
	reg_t				nREG;
	VIOC_POWER_BLOCKS3	bREG;
} VIOC_POWER_BLOCKS3_u;

typedef	struct {
	reg_t				EDRBL		: 1;
	reg_t				EDREL		: 1;
	reg_t				OSD1		: 1;
	reg_t				OXD3		: 1;
	reg_t				V_DV		: 1;
	reg_t							: 9;
	reg_t				AD			: 2;
	reg_t				AE			: 8;
	reg_t							: 8;
}	VIOC_POWER_BLOCKS4_SWR;

typedef	union {
	reg_t					nREG;
	VIOC_POWER_BLOCKS4_SWR	bREG;
}	VIOC_POWER_BLOCKS4_SWR_u;

typedef struct {
	reg_t				EDRBL		: 1;
	reg_t				EDREL		: 1;
	reg_t				OSD1		: 1;
	reg_t				OXD3		: 1;
	reg_t				MD			: 1;
	reg_t							: 27;
} VIOC_POWER_BLOCKS4_PWDN;

typedef struct {
	reg_t						nREG;
	VIOC_POWER_BLOCKS4_PWDN		bREG;
} VIOC_POWER_BLOCKS4_PWDN_u;

typedef struct {
	reg_t		EN		: 1;
	reg_t		ENDIAN	: 1;
	reg_t		FULL	: 1;
	reg_t		EMPTY	: 1;
	reg_t 				: 4;
	reg_t		UPD		: 1;
	reg_t				: 7;
	reg_t		LINC	: 1;
	reg_t				: 15;
} VIOC_CONFIG_DV_DMA_CTRL;

typedef union {
	reg_t						nReg;
	VIOC_CONFIG_DV_DMA_CTRL		bReg;
} VIOC_CONFIG_DV_DMA_CTRL_u;

typedef struct {
	reg_t							: 16;
	reg_t				SC0			: 1;
	reg_t				SC1			: 1;
	reg_t				SC2			: 1;
	reg_t				SC3			: 1;
	reg_t				SC4			: 1;
	reg_t				SC5			: 1;
	reg_t				PM0			: 1;
	reg_t				PM1			: 1;
	reg_t				PM2			: 1;
	reg_t				DV_IN		: 1;
	reg_t				SAR0		: 1;
	reg_t				SAR1		: 1;
	reg_t							: 4;
} VIOC_POWER_BLOCKS2;

typedef struct {
	reg_t					nREG;
	VIOC_POWER_BLOCKS2		bREG;
} VIOC_POWER_BLOCKS2_u;

typedef	struct {
	reg_t		PORT_FIX_WDMA0		: 1;
	reg_t		PORT_FIX_WDMA1		: 1;
	reg_t		PORT_FIX_WDMA2		: 1;
	reg_t		PORT_FIX_WDMA3		: 1;
	reg_t		PORT_FIX_WDMA4		: 1;
	reg_t		PORT_FIX_WDMA5		: 1;
	reg_t		PORT_FIX_WDMA6		: 1;
	reg_t		PORT_FIX_WDMA7		: 1;
	reg_t		PORT_FIX_WDMA8		: 1;
	reg_t							: 2;
	reg_t		PORT_FIX_VIQE0		: 1;
	reg_t							: 1;
	reg_t		PORT_FIX_VIQE1		: 1;
	reg_t							: 2;
	reg_t		PORT_CH_WDMA0		: 1; //16
	reg_t		PORT_CH_WDMA1		: 1;
	reg_t		PORT_CH_WDMA2		: 1;
	reg_t		PORT_CH_WDMA3		: 1;
	reg_t		PORT_CH_WDMA4		: 1;
	reg_t		PORT_CH_WDMA5		: 1;
	reg_t		PORT_CH_WDMA6		: 1;
	reg_t		PORT_CH_WDMA7		: 1;
	reg_t		PORT_CH_WDMA8		: 1;
	reg_t							: 2;
	reg_t		PORT_CH_VIQE0		: 1;
	reg_t							: 1;
	reg_t		PORT_CH_VIQE1		: 1;
	reg_t							: 2;
} VIOC_CONFIG_WDMA_MISC;

typedef	union {
	reg_t					nREG;
	VIOC_CONFIG_WDMA_MISC	bREG;
} VIOC_CONFIG_WDMA_MISC_u;

typedef struct {
	reg_t				SELECT		: 5;
	reg_t							: 11;
	reg_t				STATUS		: 2;
	reg_t				ERR			: 1;
	reg_t 							: 5;
	reg_t 							: 7;
	reg_t				EN			: 1;
}VIOC_CONFIG_TYPE_SEL;

typedef union {
	reg_t					nREG;
	VIOC_CONFIG_TYPE_SEL	bREG;
}VIOC_CONFIG_TYPE_SEL_u;

typedef	struct	_VIOC_IREQ_CONFIG
{
	volatile VIOC_IREQ_IREQ_u 			uRAWSTATUS;			// 0x00, 0x04, 0x08
	reg_t 								reserved0;			// 0x0C
	volatile VIOC_IREQ_IREQ_u 			uSYNCSTATUS;		// 0x10, 0x14, 0x18
	reg_t 								reserved1[3];		// 0x1C, 0x20, 0x24
	volatile VIOC_IREQ_VECTORID_u 		nVECTORID;			// 0x28
	reg_t 								reserved2[3];		// 0x2C, 0x30, 0x34
	volatile VIOC_CONFIG_TEST_LOOP_u	TEST_LOOP;			// 0x38
	volatile VIOC_CONFIG_PATH_EDR_u		uPATH_EDR;			// 0x3C
	volatile VIOC_CONFIG_MISC0_u 		uMISC0;				// 0x40
	volatile VIOC_CONFIG_PATH_u 		uSC0;				// 0x44
	volatile VIOC_CONFIG_PATH_u 		uSC1;				// 0x48
	volatile VIOC_CONFIG_PATH_u 		uSC2;				// 0x4C
	volatile VIOC_CONFIG_PATH_u 		uSC3;				// 0x50
	volatile VIOC_CONFIG_PATH_u 		uVIQE;				// 0x54
	volatile VIOC_CONFIG_PATH_u 		uDEINTLS;			// 0x58
	reg_t								reserved3;			// 0x5C
	volatile VIOC_CONFIG_PATH_u 		uFCDEC0;			// 0x60
	volatile VIOC_CONFIG_PATH_u 		uFCDEC1;			// 0x64
	reg_t 								reserved4[2];		// 0x68, 0x6C
	volatile VIOC_CONFIG_PATH_u 		uFCENC0;			// 0x70
	volatile VIOC_CONFIG_PATH_u 		uDCENC1;			// 0x74
	reg_t 								reserved5[3];		// 0x78,0x7C, 0x80
	volatile VIOC_CONFIG_MISC1_u 		uMISC1;				// 0x84
	reg_t 								reserved6[10];		// 0x88~0xAC
	volatile VIOC_CONFIG_PATH_AFBC_u	uAFBC_DEC;			// 0xB0
	reg_t 								reserved7;			// 0x84
	volatile VIOC_CONFIG_VIN_SEL_u		VIN_SEL;			// 0xB8
	volatile VIOC_CONFIG_DEV_SEL_u		DEV_SEL;			// 0xBC
	reg_t								nARID;				// 0xC0
	reg_t								nAWID;				// 0xC4
	volatile VIOC_POWER_AUTOPD_u 		uAUTOPD;			// 0xC8
	volatile VIOC_POWER_CLKCTRL_u 		uCLKCTRL;			// 0xCC
	volatile VIOC_POWER_BLOCKS_u 		uPOWERDOWN;			// 0xD0, 0xD4
	volatile VIOC_POWER_BLOCKS_u 		uSOFTRESET;			// 0xD8, 0xDC
	volatile VIOC_POWER_BLOCKS3_u 		uPOWERDOWN3;		// 0xE0
	volatile VIOC_POWER_BLOCKS3_u 		uSOFTRESET3;		// 0xE4
	volatile VIOC_POWER_BLOCKS4_SWR_u 	uSOFTRESET4;		// 0xE8
	volatile VIOC_POWER_BLOCKS4_PWDN_u 	uPOWERDOWN4;		// 0xEC
	reg_t								uDVMDAADDR;			// 0xF0
	volatile VIOC_CONFIG_DV_DMA_CTRL_u	uDVDMACTRL;			// 0xF4
	volatile VIOC_CONFIG_PATH_u 		uSC4;				// 0xF8
	volatile VIOC_CONFIG_PATH_u 		uSC5;				// 0xFC
	reg_t 								reserved8[10];		// 0x100~0x124
	volatile VIOC_POWER_BLOCKS2_u 		uPOWERDOWN2;		// 0x128
	volatile VIOC_POWER_BLOCKS2_u 		uSOFTRESET2;		// 0x12C
	volatile VIOC_CONFIG_PATH_u 		uVIQE1;				// 0x130
	reg_t 								reserved9[3];		// 0x134~0x13C
	volatile VIOC_CONFIG_WDMA_MISC_u	uWDMA_MISC;			// 0x140
	volatile VIOC_CONFIG_PATH_u			uVM0;				// 0x144
	volatile VIOC_CONFIG_PATH_u			uVM1;				// 0x148
	volatile VIOC_CONFIG_PATH_u			uNG0;				// 0x14C
	volatile VIOC_CONFIG_PATH_u			uNG1;				// 0x150
	reg_t 								reserved20[2];		// 0x154, 0x158
	volatile VIOC_CONFIG_PATH_u			uCIN0;				// 0x15C
	volatile VIOC_CONFIG_PATH_u			uCIN1;				// 0x160
	reg_t 								reserved10[4];		// 0x164~0x170
	volatile VIOC_CONFIG_PATH_u			uF2D0;				// 0x174
	volatile VIOC_CONFIG_PATH_u			uF2D1;				// 0x178
	reg_t 								reserved11[4];		// 0x17C~0x188
	volatile VIOC_CONFIG_PATH_u			uPM0;				// 0x18C
	volatile VIOC_CONFIG_PATH_u			uPM1;				// 0x190
	volatile VIOC_CONFIG_PATH_u			uPM2;				// 0x194
	volatile VIOC_CONFIG_PATH_u			uSAR0;				// 0x198
	volatile VIOC_CONFIG_PATH_u			uSAR1;				// 0x19C
	reg_t 								reserved12[2];		// 0x200, 0x204
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA02;			// 0x208
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA03;			// 0x20C
	reg_t 								reserved13[2];		// 0x210, 0x214
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA06;			// 0x218
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA07;			// 0x21C
	reg_t 								reserved14[2];		// 0x220, 0x224
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA10;			// 0x228
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA11;			// 0x22C
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA12;			// 0x230
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA13;			// 0x234
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA14;			// 0x238
	reg_t 								reserved15;			// 0x23C
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA16;			// 0x240
	volatile VIOC_CONFIG_TYPE_SEL_u		uRDMA17;			// 0x244
	reg_t 								reserved16[14];		// 0x248~0x27C
	volatile VIOC_CONFIG_TYPE_SEL_u		uMC0;				// 0x280;
	volatile VIOC_CONFIG_TYPE_SEL_u		uMC1;				// 0x284;

	/* Skip Interrupt registers */
	// 0x400 ~ 0x4B8
}	VIOC_IREQ_CONFIG,*PVIOC_IREQ_CONFIG;

#endif	/*__VIOC_IREQ_H__*/

