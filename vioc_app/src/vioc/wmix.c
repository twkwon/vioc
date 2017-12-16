#include <stdio.h>

#include <vioc.h>


int wmix_map_regs(struct vioc_wmix_t *wmix, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_WMIX *reg;

	/* read value of physical register */
	wmix->reg = *wmix->addr;

	reg = &wmix->reg;
	dat = data->reg;

	/* wmix's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = 2;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.MD3_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.MD2_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.MD1_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.MD0_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.UPD,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.EN3_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.EN2_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.EN1_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.EN0_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.STR,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.OVP,	dat[idx++]);

	/* MBG0 */
	map_reg(reg->uBG.bREG.BG1, dat[idx++]);
	map_reg(reg->uBG.bREG.BG0, dat[idx++]);

	/* MBG1 */
	map_reg(reg->uBG.bREG.BG3, dat[idx++]);
	map_reg(reg->uBG.bREG.BG2, dat[idx++]);

	/* SIZE */
	map_reg(reg->uSIZE.bREG.HEIGHT,	dat[idx++]);
	map_reg(reg->uSIZE.bREG.WIDTH,	dat[idx++]);

	/* POS0 */
	map_reg(reg->uPOS0.bREG.SM_3D,	dat[idx++]);
	map_reg(reg->uPOS0.bREG.YPOS,	dat[idx++]);
	map_reg(reg->uPOS0.bREG.XPOS,	dat[idx++]);

	/* POS1 */
	map_reg(reg->uPOS1.bREG.SM_3D,	dat[idx++]);
	map_reg(reg->uPOS1.bREG.YPOS,	dat[idx++]);
	map_reg(reg->uPOS1.bREG.XPOS,	dat[idx++]);

	/* POS2 */
	map_reg(reg->uPOS2.bREG.SM_3D,	dat[idx++]);
	map_reg(reg->uPOS2.bREG.YPOS,	dat[idx++]);
	map_reg(reg->uPOS2.bREG.XPOS,	dat[idx++]);

	/* POS3 */
	map_reg(reg->uPOS3.bREG.SM_3D,	dat[idx++]);
	map_reg(reg->uPOS3.bREG.YPOS,	dat[idx++]);
	map_reg(reg->uPOS3.bREG.XPOS,	dat[idx++]);

	/*-----------------------------------------------------*/
	/* MKEY00 */
	map_reg(reg->uKEY0.bREG.KEYEN,	dat[idx++]);
	map_reg(reg->uKEY0.bREG.KEYR,	dat[idx++]);

	/* MKEY01 */
	map_reg(reg->uKEY0.bREG.KEYG,	dat[idx++]);
	map_reg(reg->uKEY0.bREG.KEYB,	dat[idx++]);

	/* MKEY02 */
	map_reg(reg->uKEY0.bREG.KEYMR,	dat[idx++]);

	/* MKEY03 */
	map_reg(reg->uKEY1.bREG.KEYMG,	dat[idx++]);
	map_reg(reg->uKEY1.bREG.KEYMB,	dat[idx++]);

	/*-----------------------------------------------------*/
	/* MKEY10 */
	map_reg(reg->uKEY1.bREG.KEYEN,	dat[idx++]);
	map_reg(reg->uKEY1.bREG.KEYR,	dat[idx++]);

	/* MKEY11 */
	map_reg(reg->uKEY1.bREG.KEYG,	dat[idx++]);
	map_reg(reg->uKEY1.bREG.KEYB,	dat[idx++]);

	/* MKEY12 */
	map_reg(reg->uKEY1.bREG.KEYMR,	dat[idx++]);

	/* MKEY13 */
	map_reg(reg->uKEY1.bREG.KEYMG,	dat[idx++]);
	map_reg(reg->uKEY1.bREG.KEYMB,	dat[idx++]);

	/*-----------------------------------------------------*/
	/* MKEY20 */
	map_reg(reg->uKEY2.bREG.KEYEN,	dat[idx++]);
	map_reg(reg->uKEY2.bREG.KEYR,	dat[idx++]);

	/* MKEY21 */
	map_reg(reg->uKEY2.bREG.KEYG,	dat[idx++]);
	map_reg(reg->uKEY2.bREG.KEYB,	dat[idx++]);

	/* MKEY22 */
	map_reg(reg->uKEY2.bREG.KEYMR,	dat[idx++]);

	/* MKEY23 */
	map_reg(reg->uKEY2.bREG.KEYMG,	dat[idx++]);
	map_reg(reg->uKEY2.bREG.KEYMB,	dat[idx++]);

	/* STAT */
	map_reg(reg->uSTATUS.bREG.DN_INTL,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.DN_BFIELD,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.DN_ENABLE,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.DN_EOFWAIT,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.UP_INTL,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.UP_UPDATE,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.UP_ENABLE,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.UP_EOF,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFFALL,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFRISE,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFWAITRISE,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFWAITFALL,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.UPDDONE,		dat[idx++]);

	/* IRQMSK */
	map_reg(reg->uIRQMSK.bREG.EOFFALL,		dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.EOFRISE,		dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.EOFWAITRISE,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.EOFWAITFALL,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.UPDDONE,		dat[idx++]);

	/*-----------------------------------------------------*/
	/* ACON0 */
	map_reg(reg->uACON0.bREG.ACON1_11,	dat[idx++]);
	map_reg(reg->uACON0.bREG.ACON0_11,	dat[idx++]);
	map_reg(reg->uACON0.bREG.ACON1_10,	dat[idx++]);
	map_reg(reg->uACON0.bREG.ACON0_10,	dat[idx++]);
	map_reg(reg->uACON0.bREG.ACON1_01,	dat[idx++]);
	map_reg(reg->uACON0.bREG.ACON0_01,	dat[idx++]);
	map_reg(reg->uACON0.bREG.ACON1_00,	dat[idx++]);
	map_reg(reg->uACON0.bREG.ACON0_00,	dat[idx++]);

	/* CCON0 */
	map_reg(reg->uCCON0.bREG.CCON1_11,	dat[idx++]);
	map_reg(reg->uCCON0.bREG.CCON0_11,	dat[idx++]);
	map_reg(reg->uCCON0.bREG.CCON1_10,	dat[idx++]);
	map_reg(reg->uCCON0.bREG.CCON0_10,	dat[idx++]);
	map_reg(reg->uCCON0.bREG.CCON1_01,	dat[idx++]);
	map_reg(reg->uCCON0.bREG.CCON0_01,	dat[idx++]);
	map_reg(reg->uCCON0.bREG.CCON1_00,	dat[idx++]);
	map_reg(reg->uCCON0.bREG.CCON0_00,	dat[idx++]);

	/* ROPC00 */
	map_reg(reg->uROPC0.bREG.ASEL,		dat[idx++]);
	map_reg(reg->uROPC0.bREG.ROPMODE,	dat[idx++]);

	/* ROPC01 */
	map_reg(reg->uROPC0.bREG.ALPHA1,	dat[idx++]);
	map_reg(reg->uROPC0.bREG.ALPHA0,	dat[idx++]);

	/* MAT01 */
	map_reg(reg->uPAT0.bREG.BLUE,		dat[idx++]);
	map_reg(reg->uPAT0.bREG.RED,		dat[idx++]);

	/* MAT02 */
	map_reg(reg->uPAT0.bREG.GREEN,		dat[idx++]);

	/*-----------------------------------------------------*/
	/* ACON1 */
	map_reg(reg->uACON1.bREG.ACON1_11,	dat[idx++]);
	map_reg(reg->uACON1.bREG.ACON0_11,	dat[idx++]);
	map_reg(reg->uACON1.bREG.ACON1_10,	dat[idx++]);
	map_reg(reg->uACON1.bREG.ACON0_10,	dat[idx++]);
	map_reg(reg->uACON1.bREG.ACON1_01,	dat[idx++]);
	map_reg(reg->uACON1.bREG.ACON0_01,	dat[idx++]);
	map_reg(reg->uACON1.bREG.ACON1_00,	dat[idx++]);
	map_reg(reg->uACON1.bREG.ACON0_00,	dat[idx++]);

	/* CCON1 */
	map_reg(reg->uCCON1.bREG.CCON1_11,	dat[idx++]);
	map_reg(reg->uCCON1.bREG.CCON0_11,	dat[idx++]);
	map_reg(reg->uCCON1.bREG.CCON1_10,	dat[idx++]);
	map_reg(reg->uCCON1.bREG.CCON0_10,	dat[idx++]);
	map_reg(reg->uCCON1.bREG.CCON1_01,	dat[idx++]);
	map_reg(reg->uCCON1.bREG.CCON0_01,	dat[idx++]);
	map_reg(reg->uCCON1.bREG.CCON1_00,	dat[idx++]);
	map_reg(reg->uCCON1.bREG.CCON0_00,	dat[idx++]);

	/* ROPC10 */
	map_reg(reg->uROPC1.bREG.ASEL,		dat[idx++]);
	map_reg(reg->uROPC1.bREG.ROPMODE,	dat[idx++]);

	/* ROPC11 */
	map_reg(reg->uROPC1.bREG.ALPHA1,	dat[idx++]);
	map_reg(reg->uROPC1.bREG.ALPHA0,	dat[idx++]);

	/* MAT10 */
	map_reg(reg->uPAT1.bREG.BLUE,		dat[idx++]);
	map_reg(reg->uPAT1.bREG.RED,		dat[idx++]);

	/* MAT11 */
	map_reg(reg->uPAT1.bREG.GREEN,		dat[idx++]);

	/*-----------------------------------------------------*/
	/* ACON2 */
	map_reg(reg->uACON2.bREG.ACON1_11,	dat[idx++]);
	map_reg(reg->uACON2.bREG.ACON0_11,	dat[idx++]);
	map_reg(reg->uACON2.bREG.ACON1_10,	dat[idx++]);
	map_reg(reg->uACON2.bREG.ACON0_10,	dat[idx++]);
	map_reg(reg->uACON2.bREG.ACON1_01,	dat[idx++]);
	map_reg(reg->uACON2.bREG.ACON0_01,	dat[idx++]);
	map_reg(reg->uACON2.bREG.ACON1_00,	dat[idx++]);
	map_reg(reg->uACON2.bREG.ACON0_00,	dat[idx++]);

	/* CCON2 */
	map_reg(reg->uCCON2.bREG.CCON1_11,	dat[idx++]);
	map_reg(reg->uCCON2.bREG.CCON0_11,	dat[idx++]);
	map_reg(reg->uCCON2.bREG.CCON1_10,	dat[idx++]);
	map_reg(reg->uCCON2.bREG.CCON0_10,	dat[idx++]);
	map_reg(reg->uCCON2.bREG.CCON1_01,	dat[idx++]);
	map_reg(reg->uCCON2.bREG.CCON0_01,	dat[idx++]);
	map_reg(reg->uCCON2.bREG.CCON1_00,	dat[idx++]);
	map_reg(reg->uCCON2.bREG.CCON0_00,	dat[idx++]);

	/* ROPC20 */
	map_reg(reg->uROPC2.bREG.ASEL,		dat[idx++]);
	map_reg(reg->uROPC2.bREG.ROPMODE,	dat[idx++]);

	/* ROPC21 */
	map_reg(reg->uROPC2.bREG.ALPHA1,	dat[idx++]);
	map_reg(reg->uROPC2.bREG.ALPHA0,	dat[idx++]);

	/* MAT20 */
	map_reg(reg->uPAT2.bREG.BLUE,		dat[idx++]);
	map_reg(reg->uPAT2.bREG.RED,		dat[idx++]);

	/* MAT21 */
	map_reg(reg->uPAT2.bREG.GREEN,		dat[idx++]);

	return (idx - reg_start_offset - 1);
}

int wmix_setup(struct vioc_wmix_t *wmix)
{
	int ret = 0;

	/* check UPD (update-bit) is 0 */
	if (wmix->reg.uCTRL.bREG.UPD) {
		wmix->reg.uCTRL.bREG.UPD = 0;
		printf("wmix%d UPD was set, so disable it\n", wmix->info.id);
	}

	/* set physical register */
	*wmix->addr = wmix->reg;

	return ret;
}
