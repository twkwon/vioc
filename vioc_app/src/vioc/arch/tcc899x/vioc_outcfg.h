#ifndef __VIOC_CPUIF_H__
#define	__VIOC_CPUIF_H__

////////////////////////////////////////////////////////////////////////////////
//
//	OUTPUT CONFIGURATION
//
#define		VIOC_OUTCFG_HDMI			0
#define		VIOC_OUTCFG_SDVENC			1
#define		VIOC_OUTCFG_HDVENC			2
#define		VIOC_OUTCFG_M80				3
#define		VIOC_OUTCFG_MRGB			4

#define		VIOC_OUTCFG_DISP0			0
#define		VIOC_OUTCFG_DISP1			1
#define		VIOC_OUTCFG_DISP2			2
#define		VIOC_OUTCFG_V_DV			3

typedef	struct {
	reg_t				FMT			:  4;
	reg_t							:  1;
	reg_t				XA			:  2;
	reg_t				CS			:  1;
	reg_t				SI			:  1;
	reg_t				EN			:  1;
	reg_t							:  4;
	reg_t				IV			:  1;
	reg_t				ID			:  1;
	reg_t							: 16;
}	VIOC_OUTCFG_CPUIF;

typedef	union {
	reg_t				nREG;
	VIOC_OUTCFG_CPUIF	bREG;
}	VIOC_OUTCFG_CPUIF_u;

typedef	struct {
	reg_t				HDMISEL	:  2;	// HDMI OUTPUT
	reg_t 						:  2;
	reg_t				SDVESEL	:  2;	// SDVENC OUTPUT
	reg_t 						:  2;
	reg_t				HDVESEL	:  2;	// HDVENC OUTPUT
	reg_t 						:  2;
	reg_t				M80SEL	:  2;	// MIPI I80 OUTPUT
	reg_t 						:  2;
	reg_t				MRGBSEL :  2;	// MIPI RGB OUTPUT
	reg_t 						: 14;
}	VIOC_OUTCFG_MISC;

typedef	union {
	reg_t 				nREG;
	VIOC_OUTCFG_MISC	bREG;
}	VIOC_OUTCFG_MISC_u;

typedef	struct	{
	volatile VIOC_OUTCFG_CPUIF_u	uD0CPUIF;		//0x00
	volatile VIOC_OUTCFG_CPUIF_u	uD1CPUIF;		//0x04
	volatile VIOC_OUTCFG_CPUIF_u	uD2CPUIF;		//0x08
	volatile VIOC_OUTCFG_MISC_u		uMISCCFG;		//0x0C
}	VIOC_OUTCFG;

#endif	/*__VIOC_CPUIF_H__*/
