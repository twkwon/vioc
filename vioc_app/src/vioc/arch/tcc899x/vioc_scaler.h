#ifndef __VIOC_SCALER_H__
#define	__VIOC_SCALER_H__

typedef enum {
	SC_IMG_FMT_1PPP = 0,
	SC_IMG_FMT_2PPP,
	SC_IMG_FMT_4PPP,
	SC_IMG_FMT_8PPP,
	SC_IMG_FMT_RGB332 = 8, 				// 1 bytes aligned : R[7:5],G[4:2],B[1:0]
	SC_IMG_FMT_RGB444, 					// 2 bytes aligned : A[15:12],R[11:8],G[7:4],B[3:0]
	SC_IMG_FMT_RGB565, 					// 2 bytes aligned : R[15:11],G[10:5],B[4:0]
	SC_IMG_FMT_RGB555, 					// 2 bytes aligned : A[15],R[14:10],G[9:5],B[4:0]
	SC_IMG_FMT_ARGB8888, 					// 4 bytes aligned : A[31:24],R[23:16],G[15:8],B[7:0]
	SC_IMG_FMT_RGB666, 					// 4 bytes aligned : A[23:18],R[17:12],G[11:6],B[5:0]
	SC_IMG_FMT_RGB888, 					// 3 bytes aligned : B1[31:24],R0[23:16],G0[15:8],B0[7:0]
	SC_IMG_FMT_ARGB6666, 					// 3 bytes aligned : A[23:18],R[17:12],G[11:6],B[5:0]
	SC_IMG_FMT_FCDEC, 					// Frame Decompression Mode
	SC_IMG_FMT_YCbCr444_SEP_YUV = 21, 	// YCbCr 4:4:4 separated format
	SC_IMG_FMT_YCbCr422_SEQ_UYVY, 		// sequential format : LSB [U/Y/V/Y] MSB
	SC_IMG_FMT_YCbCr422_SEQ_VYUY,		// sequential format : LSB [V/Y/U/Y] MSB
	SC_IMG_FMT_YCbCr420_SEP, 				// YCbCr 4:2:0 separated format
	SC_IMG_FMT_YCbCr422_SEP, 				// YCbCr 4:2:2 separated foramt
	SC_IMG_FMT_YCbCr422_SEQ_YUYV, 		// sequential format : LSB [Y/U/Y/V] MSB
	SC_IMG_FMT_YCbCr422_SEQ_YVYU, 		// sequential format : LSB [Y/V/Y/U] MSB
	SC_IMG_FMT_YCbCr420_INT_TYPE0, 		// YCbCr 4:2:0 interleaved type 0 format
	SC_IMG_FMT_YCbCr420_INT_TYPE1, 		// YCbCr 4:2:0 interleaved type 1 format
	SC_IMG_FMT_YCbCr422_INT_TYPE0,		// YCbCr 4:2:2 interleaved type 0 format
	SC_IMG_FMT_YCbCr422_INT_TYPE1,		// YCbCr 4:2:2 interleaved type 1 format
} VIOC_SCALER_IMAGE_FORMAT_TYPE;

typedef	struct
{
	reg_t	BP			:  1;
	reg_t				: 15;
	reg_t	UPD 		:  1;
	reg_t	FFC			:  1;	// In/out fifo auto clear. reset value is 0x1 (new feature)
	reg_t				: 12;
	reg_t	MODE_3D		:  2;
} VIOC_SC_CTRL;

typedef	union
{
	reg_t				nREG;
	VIOC_SC_CTRL		bREG;
} VIOC_SC_CTRL_u;

typedef	struct
{
	reg_t	WIDTH   	: 13;
	reg_t				: 3;
	reg_t	HEIGHT   	: 13;
	reg_t				: 3;
} VIOC_SC_SIZE;

typedef	union
{
	reg_t				nREG;
	VIOC_SC_SIZE		bREG;
} VIOC_SC_SIZE_u;

typedef	struct
{
	reg_t	XPOS    	: 13;
	reg_t				: 3;
	reg_t	YPOS     	: 13;
	reg_t				: 3;
} VIOC_SC_POS;

typedef	union
{
	reg_t				nREG;
	VIOC_SC_POS			bREG;
} VIOC_SC_POS_u;

typedef	struct
{
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	EOFRISE		:  1;	// EOF Rising
	reg_t	EOFFALL		:  1;	// EOF Falling
	reg_t	ERR			:  1;	// Error
	reg_t				: 28;
} VIOC_SC_STATUS;

typedef	union
{
	reg_t				nREG;
	VIOC_SC_STATUS		bREG;
} VIOC_SC_STATUS_u;

typedef	struct
{
	reg_t	UPDDONE		:  1;	// Register Update Done
	reg_t	EOFRISE		:  1;	// EOF Rising
	reg_t	EOFFALL		:  1;	// EOF Falling
	reg_t	ERR			:  1;	// Error
	reg_t				: 28;
} VIOC_SC_IRQMSK;

typedef	union
{
	reg_t				nREG;
	VIOC_SC_IRQMSK		bREG;
} VIOC_SC_IRQMSK_u;

#define VIOC_SC_INT_MASK_UPDDONE 		0x00000001UL /*Status of Register Updated*/
#define VIOC_SC_INT_MASK_EOFRISE 		0x00000002UL /*Rising EOF Interrupt Masked*/
#define VIOC_SC_INT_MASK_EOFFALL 		0x00000004UL /*Falling EOF Interrupt Masked*/
#define VIOC_SC_INT_MASK_ERROR  		0x00000008UL /*Scaler Error Interrupt Masked*/
#define VIOC_SC_INT_MASK_ALL 			0x0000000FUL /*ALL*/

#define VIOC_SC_IREQ_UPDDONE_MASK 		0x00000001UL
#define VIOC_SC_IREQ_EOFRISE_MASK 		0x00000002UL
#define VIOC_SC_IREQ_EOFFALL_MASK  		0x00000004UL
#define VIOC_SC_IREQ_ERROR_MASK  		0x00000008UL

typedef	struct _VIOC_SC
{
	volatile VIOC_SC_CTRL_u		uCTRL;				// 0x00  R/W  0x00000000 Scaler Control Register
	volatile reg_t				reserved0;			// 0x04
	volatile VIOC_SC_SIZE_u		uSRCSIZE;			// 0x08  R       -              Scaler Source Size Register
	volatile VIOC_SC_SIZE_u		uDSTSIZE;			// 0x0C  R/W  0x00000000 Scaler Destination Size Register
	volatile VIOC_SC_POS_u 		uOUTPOS;			// 0x10  R/W  0x00000000 Scaler Output Position Register
	volatile VIOC_SC_SIZE_u		uOUTSIZE;			// 0x14  R/W  0x00000000 Scaler Output Size Register
	volatile VIOC_SC_STATUS_u	uSTATUS;			// 0x18  R/W  0x00000000 Scaler Interrupt Status Register
	volatile VIOC_SC_IRQMSK_u	uIRQMSK;			// 0x1C  R/W  0x0000000F Scaler Interrupt Mask Register
} VIOC_SC,*PVIOC_SC;

typedef struct
{
	unsigned short src_width; 	 	/* Scaler Input Source Width*/
	unsigned short src_height; 		/* Scaler Input Source Height*/
	unsigned short dest_width; 		/* Scaler Output Image Dest Width in Output Window(Scaler out image real size) */
	unsigned short dest_height; 	/* Scaler Output Image Dest Width in Output Window(Scaler out image real size) */
	unsigned short output_pos_x; 	/* Scaler Output Image Positon x in output Window */
	unsigned short output_pos_y; 	/* Scaler Output Image Positon y in output Window*/
	unsigned short output_width;	/* Scaler Output Window Size*/
	unsigned short output_height; 	/* Scaler Output Window Size*/
	unsigned short bypass;  		/* Scaler Bypass Option*/
} scaler_info_type;
	
#endif /*__VIOC_SCALER_H__*/
