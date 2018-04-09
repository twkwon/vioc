#ifndef __VIOC_WMIX_H__
#define __VIOC_WMIX_H__

typedef	struct
{
	reg_t		OVP		:  5;
	reg_t			 	:  1;
	reg_t		STR		:  2;
	reg_t		EN0_3D	:  1;
	reg_t		EN1_3D	:  1;
	reg_t		EN2_3D	:  1;
	reg_t		EN3_3D	:  1;
	reg_t				:  4;	
	reg_t		UPD 	:  1;		//16
	reg_t		MD0_3D	:  3;
	reg_t		MD1_3D	:  3;
	reg_t		MD2_3D	:  3;
	reg_t		MD3_3D	:  3;
	reg_t				:  3;	
} VIOC_WMIX_CTRL;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_CTRL		bREG;
} VIOC_WMIX_CTRL_u;

typedef	struct
{
	reg_t	BG0		: 8;
	reg_t	BG1		: 8;
	reg_t	BG2		: 8;
	reg_t	BG3		: 8;
} VIOC_WMIX_BG;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_BG		bREG;
} VIOC_WMIX_BG_u;

typedef	struct
{
	reg_t	WIDTH    	: 13;
	reg_t 	RESERVED0	:  3;
	reg_t	HEIGHT     	: 13;
	reg_t 	RESERVED1	:  3;
} VIOC_WMIX_SIZE;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_SIZE		bREG;
} VIOC_WMIX_SIZE_u;

typedef	struct
{
	reg_t	XPOS    	: 13;
	reg_t 				:  3;
	reg_t	YPOS     	: 13;
	reg_t 				:  1;
	reg_t	SM_3D		:  2;
} VIOC_WMIX_POS;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_POS		bREG;
} VIOC_WMIX_POS_u;

typedef	struct
{
	reg_t	KEYB    	:  8;
	reg_t	KEYG		:  8;
	reg_t	KEYR     	:  8;
	reg_t 				:  7;
	reg_t 	KEYEN		:  1;

	reg_t	KEYMB    	:  8;
	reg_t	KEYMG		:  8;
	reg_t	KEYMR     	:  8;
	reg_t 				:  8;
} VIOC_WMIX_KEY;

typedef	union
{
	reg_t				nREG[2];
	VIOC_WMIX_KEY		bREG;
} VIOC_WMIX_KEY_u;

typedef	struct
{
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	EOFWAITFALL	:  1;	// EOF Rising
	reg_t	EOFWAITRISE	:  1;	// EOF Falling
	reg_t	EOFRISE		:  1;	// Error
	reg_t	EOFFALL		:  1;	// Error
	reg_t				: 11;
	reg_t				:  8;
	reg_t	UP_EOF    	:  1;	// Source
	reg_t	UP_ENABLE 	:  1;	// Source
	reg_t	UP_UPDATE 	:  1;	// Source
	reg_t	UP_INTL   	:  1;	// Source
	reg_t	DN_EOFWAIT	:  1;	// Source
	reg_t	DN_ENABLE	:  1;	// Source
	reg_t	DN_BFIELD	:  1;	// Source
	reg_t	DN_INTL		:  1;	// Source
} VIOC_WMIX_STATUS;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_STATUS	bREG;
} VIOC_WMIX_STATUS_u;

typedef	struct
{
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	EOFWAITFALL	:  1;	// EOF Rising
	reg_t	EOFWAITRISE	:  1;	// EOF Falling
	reg_t	EOFRISE		:  1;	// Error
	reg_t	EOFFALL		:  1;	// Error
	reg_t				: 11;
	reg_t				: 16;
} VIOC_WMIX_IRQMSK;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_IRQMSK	bREG;
} VIOC_WMIX_IRQMSK_u;

/*WMIX ALPHA CONTROL*/
typedef	struct
{
	reg_t	ACON0_00		:  3;
	reg_t			   		:  1;
	reg_t	ACON1_00		:  3;
	reg_t			   		:  1;
	reg_t	ACON0_01		:  3;
	reg_t			   		:  1;
	reg_t	ACON1_01		:  3;
	reg_t			   		:  1;
	reg_t	ACON0_10		:  3;
	reg_t			   		:  1;
	reg_t	ACON1_10		:  3;
	reg_t			   		:  1;
	reg_t	ACON0_11		:  3;
	reg_t			   		:  1;
	reg_t	ACON1_11		:  3;
	reg_t			   		:  1;
} VIOC_WMIX_MACON;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_MACON		bREG;
} VIOC_WMIX_MACON_u;

typedef	struct
{
	reg_t	CCON0_00		:  4;
	reg_t	CCON1_00		:  4;
	reg_t	CCON0_01		:  4;
	reg_t	CCON1_01		:  4;
	reg_t	CCON0_10		:  4;
	reg_t	CCON1_10		:  4;
	reg_t	CCON0_11		:  4;
	reg_t	CCON1_11		:  4;
} VIOC_WMIX_MCCON;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_MCCON		bREG;
} VIOC_WMIX_MCCON_u;

typedef	struct
{
	reg_t	ROPMODE			:  5;
	reg_t					:  9;
	reg_t	ASEL			:  2;
	reg_t	ALPHA0			:  8;
	reg_t	ALPHA1			:  8;
} VIOC_WMIX_MROPC;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_MROPC		bREG;
} VIOC_WMIX_MROPC_u;

typedef	struct
{
	reg_t	BLUE		: 8;
	reg_t	GREEN		: 8;
	reg_t	RED			: 8;
	reg_t				: 8;
} VIOC_WMIX_MPAT;

typedef	union
{
	reg_t				nREG;
	VIOC_WMIX_MPAT		bREG;
} VIOC_WMIX_MPAT_u;

typedef	struct _VIOC_WMIX
{
	volatile VIOC_WMIX_CTRL_u 		uCTRL; 			// 0x00  R/W  0x00000005 WMIX Control Register
	volatile VIOC_WMIX_BG_u 		uBG; 			// 0x04  R/W  0x00000000 WMIX Background Color 0, 1 Register
	volatile VIOC_WMIX_SIZE_u 		uSIZE; 			// 0x08  R/W  0x00000000 WMIx Size Register
	volatile reg_t                  reserved0;      // 0x0C
	volatile VIOC_WMIX_POS_u 		uPOS0; 			// 0x10  R/W  0x00000000 WMIX Position 0 Register
	volatile VIOC_WMIX_POS_u 		uPOS1; 			// 0x14  R/W  0x00000000 WMIX Position 1 Register
	volatile VIOC_WMIX_POS_u 		uPOS2; 			// 0x18  R/W  0x00000000 WMIX Position 2 Register
	volatile VIOC_WMIX_POS_u 		uPOS3; 			// 0x1C  R/W  0x00000000 WMIX Position 3 Register
	volatile VIOC_WMIX_KEY_u 		uKEY0; 			// 0x20, 0x24 R/W  0x00000000 WMIX Chroma-Key 0,1 Image 0 Register
	volatile VIOC_WMIX_KEY_u 		uKEY1; 			// 0x28, 0x2C R/W  0x00000000 WMIX Chroma-key 0,1 Image 1 Register
	volatile VIOC_WMIX_KEY_u 		uKEY2; 			// 0x30, 0x34 R/W  0x00000000 WMIX Chroma-key 0,1 Image 2 Register
	volatile VIOC_WMIX_STATUS_u 	uSTATUS; 		// 0x38  R/W  0x00000000 WMIX Status Register
	volatile VIOC_WMIX_IRQMSK_u 	uIRQMSK; 		// 0x3C  R/W  0x0000001F WMIX Interrupt Masked Register

	volatile VIOC_WMIX_MACON_u 		uACON0; 		// 0x40  R/W  0x10622662 WMIX Alpha Control Register for 0'th Layer
	volatile VIOC_WMIX_MCCON_u 		uCCON0; 		// 0x44  R/W  0x20C44CC4 WMIX Color Control Register for 0'th Layer
	volatile VIOC_WMIX_MROPC_u 		uROPC0; 		// 0x48  R/W  0x00000018 WMIX ROP Control Register for 0'th Layer
	volatile VIOC_WMIX_MPAT_u 		uPAT0; 			// 0x4C  R/W  0x00000000 WMIX Patterm Register for 0'th Layer
	
	volatile VIOC_WMIX_MACON_u 		uACON1; 		// 0x50  R/W  0x10622662 WMIX Alpha Control Register for 1'th Layer
	volatile VIOC_WMIX_MCCON_u 		uCCON1; 		// 0x54  R/W  0x20C44CC4 WMIX Color Control Register for 1'th Layer
	volatile VIOC_WMIX_MROPC_u 		uROPC1; 		// 0x58  R/W  0x0000C018 WMIX ROP Control Register for 1'th Layer
	volatile VIOC_WMIX_MPAT_u 		uPAT1; 			// 0x5C  R/W  0x00000000 WMIX Patterm Register for 1'th Layer
	
	volatile VIOC_WMIX_MACON_u 		uACON2; 		// 0x60  R/W  0x10622662 WMIX Alpha Control Register for 2'th Layer
	volatile VIOC_WMIX_MCCON_u 		uCCON2; 		// 0x64  R/W  0x20C44CC4 WMIX Color Control Register for 2'th Layer
	volatile VIOC_WMIX_MROPC_u 		uROPC2; 		// 0x68  R/W  0x0000C018 WMIX ROP Control Register for 2'th Layer
	volatile VIOC_WMIX_MPAT_u 		uPAT2; 			// 0x6C  R/W  0x00000000 WMIX Patterm Register for 2'th Layer
} VIOC_WMIX,*PVIOC_WMIX;

typedef	struct
{
	volatile VIOC_WMIX_MACON_u	uACON;				// 0
	volatile VIOC_WMIX_MCCON_u	uCCON;				// 1
	volatile VIOC_WMIX_MROPC_u	uROPC;				// 2
	volatile VIOC_WMIX_MPAT_u 	uPAT;				// 3
} VIOC_WMIX_ALPHA;

/* 4X2 */
typedef enum
{
	VIOC_WMIX0_ALPHA_L0 =0,
	VIOC_WMIX0_ALPHA_L1,
	VIOC_WMIX0_ALPHA_L2,
	VIOC_WMIX0_ALPHA_LAYER_MAX	
}VIOC_WMIX0_ALPHA_LAYER_Type;

/* 2X2 #1*/
typedef enum
{
	VIOC_WMIX1_ALPHA_L0 =0,
	VIOC_WMIX1_ALPHA_LAYER_MAX	
}VIOC_WMIX1_ALPHA_LAYER_Type;

/* 2X2 #2*/
typedef enum
{
	VIOC_WMIX2_ALPHA_L0 =0,
	VIOC_WMIX2_ALPHA_LAYER_MAX	
}VIOC_WMIX2_ALPHA_LAYER_Type;

typedef enum
{
	VIOC_WMIX_ALPHA_SEL0 = 0, 	/* image alpha 0 ~ 255 ( 0% ~ 99.6% )*/
	VIOC_WMIX_ALPHA_SEL1, 		/*not defined*/
	VIOC_WMIX_ALPHA_SEL2, 		/*image alpha 1 ~ 256 ( 0.39% ~ 100% )*/
	VIOC_WMIX_ALPHA_SEL3, 		/*image alpha 0 ~ 127, 129 ~ 256 ( 0% ~ 49.6%, 50.3% ~ 100% )*/
	VIOC_WMIX_ALPHA_SEL_MAX
}VIOC_WMIX_ALPHA_SELECT_Type;

typedef enum
{
	VIOC_WMIX_ALPHA_ROP_NOTDEFINE = 0, 	/*not defined*/
	VIOC_WMIX_ALPHA_ROP_GLOBAL = 2, 	/*Global Alpha*/
	VIOC_WMIX_ALPHA_ROP_PIXEL, 	/*Pixel Alpha*/
	VIOC_WMIX_ALPHA_ROP_MAX
}VIOC_WMIX_ALPHA_ROPMODE_Type;

typedef enum
{
	VIOC_WMIX_ALPHA_ACON0_0 = 0, 	/* Result_A = ALPHA0 * SEL0_Out, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_1, 		/* Result_A = ALPHA0 * SEL0_Out, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_2, 		/* Result_A = ALPHA0 * 256, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_3, 		/* Result_A = ALPHA0 * 256, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_4, 		/* Result_A = ALPHA0 * 128, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_5, 		/* Result_A = ALPHA0 * 128, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_6, 		/* Result_A = 0, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_7, 		/* Result_A = 0, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_ACON0_MAX		/* Result : AlphaDataOut = (Result_A + Result_B + 128) / 256*/
}VIOC_WMIX_ALPHA_ACON0_Type;

typedef enum
{
	VIOC_WMIX_ALPHA_ACON1_0 = 0, 	/* Result_B = ALPHA1 * SEL1_Out, SEL1_Out = ALPHA0*/            
	VIOC_WMIX_ALPHA_ACON1_1, 		/* Result_B = ALPHA1 * SEL1_Out, SEL1_Out = 256 - ALPHA0*/      
	VIOC_WMIX_ALPHA_ACON1_2, 		/* Result_B = ALPHA1 * 256, SEL1_Out = ALPHA0*/                 
	VIOC_WMIX_ALPHA_ACON1_3, 		/* Result_B = ALPHA1 * 256, SEL1_Out = 256 - ALPHA0*/           
	VIOC_WMIX_ALPHA_ACON1_4, 		/* Result_B = ALPHA1 * 128, SEL1_Out = ALPHA0*/                 
	VIOC_WMIX_ALPHA_ACON1_5, 		/* Result_B = ALPHA1 * 128, SEL1_Out = 256 - ALPHA0*/           
	VIOC_WMIX_ALPHA_ACON1_6, 		/* Result_B = 0, SEL1_Out = ALPHA0*/                            
	VIOC_WMIX_ALPHA_ACON1_7, 		/* Result_B = 0, SEL1_Out = 256 - ALPHA0*/                      
	VIOC_WMIX_ALPHA_ACON1_MAX		/* Result : AlphaDataOut = (Result_A + Result_B + 128) / 256*/  
}VIOC_WMIX_ALPHA_ACON1_Type;

typedef enum
{
	VIOC_WMIX_ALPHA_CCON0_0 = 0, 	/* Result_A = PixelDataA * SEL0_Out, SEL0_Out = ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_1, 		/* Result_A = PixelDataA * SEL0_Out, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_2, 		/* Result_A = PixelDataA * SEL0_Out, SEL0_Out = 256 - ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_3, 		/* Result_A = PixelDataA * SEL0_Out, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_4, 		/* Result_A = PixelDataA * 256, SEL0_Out = ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_5, 		/* Result_A = PixelDataA * 256, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_6, 		/* Result_A = PixelDataA * 256, SEL0_Out = 256 - ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_7, 		/* Result_A = PixelDataA * 256, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_8, 		/* Result_A = PixelDataA * 128, SEL0_Out = ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_9, 		/* Result_A = PixelDataA * 128, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_10, 		/* Result_A = PixelDataA * 128, SEL0_Out = 256 - ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_11, 		/* Result_A = PixelDataA * 128, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_12, 		/* Result_A = 0, SEL0_Out = ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_13, 		/* Result_A = 0, SEL0_Out = ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_14, 		/* Result_A = 0, SEL0_Out = 256 - ALPHA0*/
	VIOC_WMIX_ALPHA_CCON0_15, 		/* Result_A = 0, SEL0_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_CCON0_MAX		/* Result : PixelDataOut = (Result_A + Result_B + 128) / 256*/
}VIOC_WMIX_ALPHA_CCON0_Type;

typedef enum
{
	VIOC_WMIX_ALPHA_CCON1_0 = 0,		/* Result_B = PixelDataB * SEL1_Out, SEL1_Out = ALPHA0*/      
	VIOC_WMIX_ALPHA_CCON1_1, 		/* Result_B = PixelDataB * SEL1_Out, SEL1_Out = ALPHA1*/      
	VIOC_WMIX_ALPHA_CCON1_2, 		/* Result_B = PixelDataB * SEL1_Out, SEL1_Out = 256 - ALPHA0*/
	VIOC_WMIX_ALPHA_CCON1_3, 		/* Result_B = PixelDataB * SEL1_Out, SEL1_Out = 256 - ALPHA1*/
	VIOC_WMIX_ALPHA_CCON1_4, 		/* Result_B = PixelDataB * 256, SEL1_Out = ALPHA0*/           
	VIOC_WMIX_ALPHA_CCON1_5, 		/* Result_B = PixelDataB * 256, SEL1_Out = ALPHA1*/           
	VIOC_WMIX_ALPHA_CCON1_6, 		/* Result_B = PixelDataB * 256, SEL1_Out = 256 - ALPHA0*/     
	VIOC_WMIX_ALPHA_CCON1_7, 		/* Result_B = PixelDataB * 256, SEL1_Out = 256 - ALPHA1*/     
	VIOC_WMIX_ALPHA_CCON1_8, 		/* Result_B = PixelDataB * 128, SEL1_Out = ALPHA0*/           
	VIOC_WMIX_ALPHA_CCON1_9, 		/* Result_B = PixelDataB * 128, SEL1_Out = ALPHA1*/           
	VIOC_WMIX_ALPHA_CCON1_10, 		/* Result_B = PixelDataB * 128, SEL1_Out = 256 - ALPHA0*/     
	VIOC_WMIX_ALPHA_CCON1_11, 		/* Result_B = PixelDataB * 128, SEL1_Out = 256 - ALPHA1*/     
	VIOC_WMIX_ALPHA_CCON1_12, 		/* Result_B = 0, SEL1_Out = ALPHA0*/                          
	VIOC_WMIX_ALPHA_CCON1_13, 		/* Result_B = 0, SEL1_Out = ALPHA1*/                          
	VIOC_WMIX_ALPHA_CCON1_14, 		/* Result_B = 0, SEL1_Out = 256 - ALPHA0*/                    
	VIOC_WMIX_ALPHA_CCON1_15, 		/* Result_B = 0, SEL1_Out = 256 - ALPHA1*/                    
	VIOC_WMIX_ALPHA_CCON1_MAX		/* Result : PixelDataOut = (Result_A + Result_B + 128) / 256*/
}VIOC_WMIX_ALPHA_CCON1_Type;

#define VIOC_WMIX_ALPHA_REGION_A		0x0
#define VIOC_WMIX_ALPHA_REGION_B		0x1
#define VIOC_WMIX_ALPHA_REGION_C		0x2
#define VIOC_WMIX_ALPHA_REGION_D		0x3

#endif	/*__VIOC_WMIX_H__*/
