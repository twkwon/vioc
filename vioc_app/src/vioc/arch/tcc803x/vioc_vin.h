#ifndef __VIOC_VIN_H__
#define	__VIOC_VIN_H__

#define 	ORDER_RGB 			0
#define 	ORDER_RBG 			1
#define 	ORDER_GRB 			2
#define 	ORDER_GBR 			3
#define 	ORDER_BRG 			4
#define 	ORDER_BGR 			5

#define 	FMT_YUV422_16BIT 		0
#define 	FMT_YUV422_8BIT 		1
#define 	FMT_YUVK4444_16BIT 		2
#define 	FMT_YUVK4224_24BIT 		3
#define 	FMT_RGBK4444_16BIT 		4
#define 	FMT_RGB444_24BIT 		9
#define 	FMT_SD_PROG 			12 // NOT USED

#define 	EXT_PORT0 			0
#define 	EXT_PORT1 			1
#define 	EXT_PORT2 			2
#define 	EXT_PORT3 			3
#define 	MUL_PORT0 			4
#define 	MUL_PORT1 			5
#define 	MUL_PORT2 			6
#define 	MUL_PORT3 			7

#define 	CLK_DOUBLE_EDGE 			0
#define 	CLK_DOUBLE_FREQ 			1
#define 	CLK_DOUBLE_EDGE_FREQ 		2
#define 	CLK_DOUBLE_4TIME_FREQ 		3

#define 	ON 				1
#define 	OFF 			0


/************************************************************************
*   3.6.3   VIDEO IN                         (Base Addr = 0x12004000)
*************************************************************************/

typedef struct
{
	reg_t		enable				:	 1;
	reg_t		conv_en				:	 1;
	reg_t		intl_en				:	 1;
	reg_t		intpl_en			: 	 1;
	reg_t		hsde_connect_en		:	 1;
	reg_t		flush				:	 1;
	reg_t		vs_mask				:	 1;
	reg_t							:	 1;
	reg_t		pxclk_pol			:	 1;
	reg_t		hs_active_low		:	 1;
	reg_t		vs_active_low		:	 1;
	reg_t		field_bfield_low	:	 1;
	reg_t		de_active_low		:	 1;
	reg_t		gen_field_en		:	 1;
	reg_t		stream_en			:	 1;
	reg_t							:	 1;
	reg_t		fmt					:	 4;
	reg_t		data_order			:	 3;
	reg_t							:	 1;
	reg_t		skip				:	 4;
	reg_t							:	 3;
	reg_t		capture				:	 1;
}	VIN_CTRL;

typedef union
{
	reg_t				nREG;
	VIN_CTRL			bREG;
}	VIN_CTRL_u;

typedef struct
{
	reg_t	luten				:	 3;
	reg_t	lutif				:	 1;
	reg_t	y2ren				:	 1;
	reg_t	y2rm				:	 3;
	reg_t	r2yen				:	 1;
	reg_t	r2ym				:	 3;
	reg_t	align				:	 2;
	reg_t						:    2;
	reg_t	fvs					:	 1;
	reg_t						:    3;
	reg_t	vs_delay			:	 4;
	reg_t						:    8;
}	VIN_MISC;

typedef union
{
	reg_t					nREG;
	VIN_MISC				bREG;
}	VIN_MISC_u;

typedef struct
{
	reg_t	hblank				:	 4;
	reg_t	vblank				:	 4;
	reg_t	field_pos			:	 4;
	reg_t						:	 4;
	reg_t	pre_sta_loc			:	 2;
	reg_t	sync_byte			:	 2;
	reg_t						:	12;
}	VIN_SYNC_M0;

typedef union
{
	reg_t			nREG;
	VIN_SYNC_M0		bREG;
}	VIN_SYNC_M0_u;

typedef struct
{
	reg_t	pre_first			:	8;
	reg_t	pre_second			:	8;
	reg_t	pre_third			:	8;
	reg_t						:	8;
}	VIN_SYNC_M1;

typedef union
{
	reg_t			nREG;
	VIN_SYNC_M1		bREG;
}	VIN_SYNC_M1_u;

typedef	struct
{
	reg_t	width				:	16;
	reg_t	height				:	16;
}	VIN_SIZE;

typedef union
{
	reg_t						nREG;
	VIN_SIZE					bREG;
}	VIN_SIZE_u;

typedef	struct
{
	reg_t	offs_width			:	16;
	reg_t	offs_height			:	16;
}	VIN_OFFS;

typedef union
{
	reg_t					nREG;
	VIN_OFFS				bREG;
}	VIN_OFFS_u;

typedef	struct
{
	reg_t	update_int				:	 1;
	reg_t	eof_int					:	 1;
	reg_t	vs_int					:	 1;
	reg_t	invs_int				:	 1;
	reg_t							:	 7;	
	reg_t	frm_sq					:	 1;	
	reg_t 							:	 4;
	reg_t	update_int_mask			:	 1;
	reg_t	eof_int_mask			:	 1;
	reg_t	vs_int_mask				: 	 1;
	reg_t	invs_int_mask			: 	 1;
	reg_t							: 	11;	
	reg_t	int_en					:	 1;
}	VIN_INT;

typedef	union
{
	reg_t					nREG;
	VIN_INT					bREG;
}	VIN_INT_u;

typedef	struct
{
	reg_t	width_cnt			:	16;
	reg_t	height_cnt			:	16;
}	VIN_STAT_CNT;

typedef union
{
	reg_t					nREG;
	VIN_STAT_CNT			bREG;
}	VIN_STAT_CNT_u;

typedef	struct
{
	reg_t	RESERVE0			:   30;
	reg_t	IND					:	 2;
}	VIN_LUT_CTRL;

typedef union
{
	reg_t					nREG;
	VIN_LUT_CTRL			bREG;
}	VIN_LUT_CTRL_u;

typedef	struct
{
	reg_t	value0			:	8;
	reg_t	value1			:	8;
	reg_t	value2			:	8;
	reg_t	value3			:	8;
}	VIN_LUT;

typedef union
{
	reg_t	 				nREG[54];
	VIN_LUT 				bREG;
}	VIN_LUT_C0_u;

typedef union
{
	reg_t	 				nREG[54];
	VIN_LUT 				bREG;
}	VIN_LUT_C1_u;

typedef union
{
	reg_t	 				nREG[54];
	VIN_LUT 				bREG;
}	VIN_LUT_C2_u;

typedef	struct
{
	reg_t	width			:	16;
	reg_t	height			:	16;
}	VIN_CROP_SIZE;

typedef union
{
	reg_t					nREG;
	VIN_CROP_SIZE			bREG;
}	VIN_CROP_SIZE_u;

typedef	struct
{
	reg_t	offs_width		:	16;
	reg_t	offs_height		:	16;
}	VIN_CROP_OFFS;

typedef union
{
	reg_t					nREG;
	VIN_CROP_OFFS			bREG;
}	VIN_CROP_OFFS_u;

typedef struct _VIOC_VIN
{
	volatile VIN_CTRL_u 			uVIN_CTRL; 			// 	0x000  R/W  0x00000000 VIN Control Register
	volatile VIN_MISC_u 			uVIN_MISC; 			// 	0x004  R/W  0x00000000 VIN Misc Register
	volatile VIN_SYNC_M0_u 			uVIN_SYNC_M0; 		// 	0x008  R/W  0x00030CB9 VIN Sync Misc. 0 Register
	volatile VIN_SYNC_M1_u 			uVIN_SYNC_M1; 		// 	0x00c  R/W  0x000000FF VIN Sync misc. 1 Register
	volatile VIN_SIZE_u 			uVIN_SIZE; 			// 	0x010  R/W  0x00000000 VIN Size Register
	volatile VIN_OFFS_u 			uVIN_OFFS; 			// 	0x014  R/W  0x00000000 VIN Offset Register
	volatile VIN_OFFS_u 			uVIN_OFFS_INTL; 	// 	0x018  R/W  0x00000000 VIN Offset Interlaced Register
	volatile VIN_CROP_SIZE_u 		uVIN_CROP_SIZE; 	// 	0x01c  R/W  0x00000000 VIN Crop Size Register
	volatile VIN_CROP_OFFS_u 		uVIN_CROP_OFFS; 	// 	0x020  R/W  0x00000000 VIN Crop Offset Register
	reg_t	 						undef0[14]; 		// 	0x024 ~ 0x58
	volatile VIN_LUT_CTRL_u			uVIN_LUT_CTRL;		//   0x05c  R/W  0x00000000 VIN Look-up Table Control Register
	volatile VIN_INT_u 				uVIN_INT; 			// 	0x060  R/W  0x00000000 VIN Iterrupt Register	

	//TODO:VIN LUT struct
	//reg_t				 				undef1[534]; 		// 	0x064 ~ 0xFFC
	//volatile VIN_LUT_C0_u 			uVIN_LUT_C0;		// 	0x1000 ~ 0x10FC R/W VIN Look-up Table set in Channel 0
	//volatile VIN_LUT_C1_u 			uVIN_LUT_C1;		// 	0x1100 ~ 0x11FC R/W VIN Look-up Table set in Channel 1
	//volatile VIN_LUT_C2_u 			uVIN_LUT_C2;		// 	0x1200 ~ 0x12FC R/W VIN Look-up Table set in Channel 2
} VIOC_VIN, *PVIOC_VIN;

typedef struct
{
	reg_t		VALUE_K_CH0		:	10;
	reg_t		VALUE_K_CH1		:	10;
	reg_t		VALUE_K_CH2		:	10;
	reg_t						:	 2;
} VIN_LUT_C;

typedef union
{
	reg_t			nREG;
	VIN_LUT_C		bREG;
} VIN_LUT_C_u;

typedef struct _VIOC_VIN_LUT_C
{
	volatile VIN_LUT_C_u			uVIN_LUT_C[256];	// VIN base + 0x400~0x7FC	VIN Look-up Table Initialize Register
} VIOC_VIN_LUT_C, *PVIOC_VIN_LUT_C;


typedef struct
{
	reg_t	enable				:	 1;
	reg_t						:	 3;
	reg_t	byte_select			:	 2;
	reg_t						:	 2;
	reg_t	clk_mode			:	 3;
	reg_t						:	 5;
	reg_t	sel0				:	 3;
	reg_t						:	 1;
	reg_t	sel1				:	 3;
	reg_t 						:	 1;
	reg_t	sel2				: 	 3;
	reg_t 						:	 1;
	reg_t	sel3				:	 3;
	reg_t 						:	 1;
}	VIN_DEMUX_CTRL;

typedef	union
{
	reg_t				nREG;
	VIN_DEMUX_CTRL		bVIN_DEMUX_CTRL;
}	VIN_DEMUX_CTRL_u;

typedef	struct
{
	reg_t	hb					:	 4;
	reg_t	vb					:	 4;
	reg_t	fp					:	 4;
	reg_t						:	 4;
	reg_t	psl					:	 2;
	reg_t	sync_byte			:	 2;
	reg_t						:	12;
}	VIN_DEMUX_BLANK0;

typedef union
{
	reg_t			nREG;
	VIN_DEMUX_BLANK0		bREG;
}	VIN_DEMUX_BLANK0_u;

typedef struct
{
	reg_t	pf					:	 8;
	reg_t	ps					:	 8;
	reg_t	pt					:	 8;
	reg_t	RESERVE0			:	 8;
}	VIN_DEMUX_BLANK1;

typedef union
{
	reg_t			nREG;
	VIN_DEMUX_BLANK1		bREG;
}	VIN_DEMUX_BLANK1_u;

typedef struct
{
	reg_t	dsel0				:	 4;
	reg_t	dsel1				:	 4;
	reg_t	dsel2				:	 4;
	reg_t	dsel3				:	 4;
	reg_t	RESERVE0			:	16;
}	VIN_DEMUX_MISC;

typedef union
{
	reg_t			nREG;
	VIN_DEMUX_MISC		bREG;
}	VIN_DEMUX_MISC_u;

typedef struct
{
	reg_t	idx0				:	 4;
	reg_t	idx1				:	 4;
	reg_t	idx2				:	 4;
	reg_t	idx3				:	 4;
	reg_t	RESERVE0			:	16;
}	VIN_DEMUX_STS;

typedef	union
{
	reg_t					nREG;
	VIN_DEMUX_STS			bREG;
}	VIN_DEMUX_STS_u;


typedef	struct _VOIC_VIN_DEMUX
{
	volatile VIN_DEMUX_CTRL_u		uVIN_DEMUX_CTRL;	//	0x000 R/W 0x32100000 VIN DEMUX Control register
	volatile VIN_DEMUX_BLANK0_u		uVIN_DEMUX_BLANK0;	//	0x004 R/W 0x00030CB9 VIN DEMUX Blank Set 0 Register
	volatile VIN_DEMUX_BLANK1_u		uVIN_DEMUX_BLANK1;	//	0x008 R/W 0x000000FF VIN DEMUX Black Set 1 Register
	volatile VIN_DEMUX_MISC_u		uVIN_DEMUX_MISC;	//	0x00C R/W 0x00004444 VIN DEMUX Miscellaneous Register
	volatile VIN_DEMUX_STS_u		uVIN_DEMUX_STS;		//	0x010 R     -                VIN DEMUX Status Register
}	VIOC_VIN_DEMUX,*PVIOC_VIN_DEMUX;

#endif	/*__VIOC_VIN_H__*/
