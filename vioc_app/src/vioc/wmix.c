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
	reg_start_offset = REG_START_OFFSET_WMIX;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.MD3_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.MD2_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.MD1_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.MD0_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.UPD,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.EN3_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.EN2_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.EN1_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.EN0_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.STR,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.OVP,	dat[idx]); idx++;

	/* MBG0 */
	map_reg(reg->uBG.bREG.BG1, dat[idx]); idx++;
	map_reg(reg->uBG.bREG.BG0, dat[idx]); idx++;

	/* MBG1 */
	map_reg(reg->uBG.bREG.BG3, dat[idx]); idx++;
	map_reg(reg->uBG.bREG.BG2, dat[idx]); idx++;

	/* SIZE */
	map_reg(reg->uSIZE.bREG.HEIGHT,	dat[idx]); idx++;
	map_reg(reg->uSIZE.bREG.WIDTH,	dat[idx]); idx++;

	/* POS0 */
	map_reg(reg->uPOS0.bREG.SM_3D,	dat[idx]); idx++;
	map_reg(reg->uPOS0.bREG.YPOS,	dat[idx]); idx++;
	map_reg(reg->uPOS0.bREG.XPOS,	dat[idx]); idx++;

	/* POS1 */
	map_reg(reg->uPOS1.bREG.SM_3D,	dat[idx]); idx++;
	map_reg(reg->uPOS1.bREG.YPOS,	dat[idx]); idx++;
	map_reg(reg->uPOS1.bREG.XPOS,	dat[idx]); idx++;

	/* POS2 */
	map_reg(reg->uPOS2.bREG.SM_3D,	dat[idx]); idx++;
	map_reg(reg->uPOS2.bREG.YPOS,	dat[idx]); idx++;
	map_reg(reg->uPOS2.bREG.XPOS,	dat[idx]); idx++;

	/* POS3 */
	map_reg(reg->uPOS3.bREG.SM_3D,	dat[idx]); idx++;
	map_reg(reg->uPOS3.bREG.YPOS,	dat[idx]); idx++;
	map_reg(reg->uPOS3.bREG.XPOS,	dat[idx]); idx++;

	/*-----------------------------------------------------*/
	/* MKEY00 */
	map_reg(reg->uKEY0.bREG.KEYEN,	dat[idx]); idx++;
	map_reg(reg->uKEY0.bREG.KEYR,	dat[idx]); idx++;

	/* MKEY01 */
	map_reg(reg->uKEY0.bREG.KEYG,	dat[idx]); idx++;
	map_reg(reg->uKEY0.bREG.KEYB,	dat[idx]); idx++;

	/* MKEY02 */
	map_reg(reg->uKEY0.bREG.KEYMR,	dat[idx]); idx++;

	/* MKEY03 */
	map_reg(reg->uKEY1.bREG.KEYMG,	dat[idx]); idx++;
	map_reg(reg->uKEY1.bREG.KEYMB,	dat[idx]); idx++;

	/*-----------------------------------------------------*/
	/* MKEY10 */
	map_reg(reg->uKEY1.bREG.KEYEN,	dat[idx]); idx++;
	map_reg(reg->uKEY1.bREG.KEYR,	dat[idx]); idx++;

	/* MKEY11 */
	map_reg(reg->uKEY1.bREG.KEYG,	dat[idx]); idx++;
	map_reg(reg->uKEY1.bREG.KEYB,	dat[idx]); idx++;

	/* MKEY12 */
	map_reg(reg->uKEY1.bREG.KEYMR,	dat[idx]); idx++;

	/* MKEY13 */
	map_reg(reg->uKEY1.bREG.KEYMG,	dat[idx]); idx++;
	map_reg(reg->uKEY1.bREG.KEYMB,	dat[idx]); idx++;

	/*-----------------------------------------------------*/
	/* MKEY20 */
	map_reg(reg->uKEY2.bREG.KEYEN,	dat[idx]); idx++;
	map_reg(reg->uKEY2.bREG.KEYR,	dat[idx]); idx++;

	/* MKEY21 */
	map_reg(reg->uKEY2.bREG.KEYG,	dat[idx]); idx++;
	map_reg(reg->uKEY2.bREG.KEYB,	dat[idx]); idx++;

	/* MKEY22 */
	map_reg(reg->uKEY2.bREG.KEYMR,	dat[idx]); idx++;

	/* MKEY23 */
	map_reg(reg->uKEY2.bREG.KEYMG,	dat[idx]); idx++;
	map_reg(reg->uKEY2.bREG.KEYMB,	dat[idx]); idx++;

	/* STAT */
	map_reg(reg->uSTATUS.bREG.DN_INTL,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.DN_BFIELD,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.DN_ENABLE,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.DN_EOFWAIT,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.UP_INTL,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.UP_UPDATE,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.UP_ENABLE,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.UP_EOF,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFFALL,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFRISE,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFWAITRISE,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFWAITFALL,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.UPDDONE,		dat[idx]); idx++;

	/* IRQMSK */
	map_reg(reg->uIRQMSK.bREG.EOFFALL,		dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFRISE,		dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFWAITRISE,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFWAITFALL,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.UPDDONE,		dat[idx]); idx++;

	/*-----------------------------------------------------*/
	/* ACON0 */
	map_reg(reg->uACON0.bREG.ACON1_11,	dat[idx]); idx++;
	map_reg(reg->uACON0.bREG.ACON0_11,	dat[idx]); idx++;
	map_reg(reg->uACON0.bREG.ACON1_10,	dat[idx]); idx++;
	map_reg(reg->uACON0.bREG.ACON0_10,	dat[idx]); idx++;
	map_reg(reg->uACON0.bREG.ACON1_01,	dat[idx]); idx++;
	map_reg(reg->uACON0.bREG.ACON0_01,	dat[idx]); idx++;
	map_reg(reg->uACON0.bREG.ACON1_00,	dat[idx]); idx++;
	map_reg(reg->uACON0.bREG.ACON0_00,	dat[idx]); idx++;

	/* CCON0 */
	map_reg(reg->uCCON0.bREG.CCON1_11,	dat[idx]); idx++;
	map_reg(reg->uCCON0.bREG.CCON0_11,	dat[idx]); idx++;
	map_reg(reg->uCCON0.bREG.CCON1_10,	dat[idx]); idx++;
	map_reg(reg->uCCON0.bREG.CCON0_10,	dat[idx]); idx++;
	map_reg(reg->uCCON0.bREG.CCON1_01,	dat[idx]); idx++;
	map_reg(reg->uCCON0.bREG.CCON0_01,	dat[idx]); idx++;
	map_reg(reg->uCCON0.bREG.CCON1_00,	dat[idx]); idx++;
	map_reg(reg->uCCON0.bREG.CCON0_00,	dat[idx]); idx++;

	/* ROPC00 */
	map_reg(reg->uROPC0.bREG.ASEL,		dat[idx]); idx++;
	map_reg(reg->uROPC0.bREG.ROPMODE,	dat[idx]); idx++;

	/* ROPC01 */
	map_reg(reg->uROPC0.bREG.ALPHA1,	dat[idx]); idx++;
	map_reg(reg->uROPC0.bREG.ALPHA0,	dat[idx]); idx++;

	/* MAT01 */
	map_reg(reg->uPAT0.bREG.BLUE,		dat[idx]); idx++;
	map_reg(reg->uPAT0.bREG.RED,		dat[idx]); idx++;

	/* MAT02 */
	map_reg(reg->uPAT0.bREG.GREEN,		dat[idx]); idx++;

	/*-----------------------------------------------------*/
	/* ACON1 */
	map_reg(reg->uACON1.bREG.ACON1_11,	dat[idx]); idx++;
	map_reg(reg->uACON1.bREG.ACON0_11,	dat[idx]); idx++;
	map_reg(reg->uACON1.bREG.ACON1_10,	dat[idx]); idx++;
	map_reg(reg->uACON1.bREG.ACON0_10,	dat[idx]); idx++;
	map_reg(reg->uACON1.bREG.ACON1_01,	dat[idx]); idx++;
	map_reg(reg->uACON1.bREG.ACON0_01,	dat[idx]); idx++;
	map_reg(reg->uACON1.bREG.ACON1_00,	dat[idx]); idx++;
	map_reg(reg->uACON1.bREG.ACON0_00,	dat[idx]); idx++;

	/* CCON1 */
	map_reg(reg->uCCON1.bREG.CCON1_11,	dat[idx]); idx++;
	map_reg(reg->uCCON1.bREG.CCON0_11,	dat[idx]); idx++;
	map_reg(reg->uCCON1.bREG.CCON1_10,	dat[idx]); idx++;
	map_reg(reg->uCCON1.bREG.CCON0_10,	dat[idx]); idx++;
	map_reg(reg->uCCON1.bREG.CCON1_01,	dat[idx]); idx++;
	map_reg(reg->uCCON1.bREG.CCON0_01,	dat[idx]); idx++;
	map_reg(reg->uCCON1.bREG.CCON1_00,	dat[idx]); idx++;
	map_reg(reg->uCCON1.bREG.CCON0_00,	dat[idx]); idx++;

	/* ROPC10 */
	map_reg(reg->uROPC1.bREG.ASEL,		dat[idx]); idx++;
	map_reg(reg->uROPC1.bREG.ROPMODE,	dat[idx]); idx++;

	/* ROPC11 */
	map_reg(reg->uROPC1.bREG.ALPHA1,	dat[idx]); idx++;
	map_reg(reg->uROPC1.bREG.ALPHA0,	dat[idx]); idx++;

	/* MAT10 */
	map_reg(reg->uPAT1.bREG.BLUE,		dat[idx]); idx++;
	map_reg(reg->uPAT1.bREG.RED,		dat[idx]); idx++;

	/* MAT11 */
	map_reg(reg->uPAT1.bREG.GREEN,		dat[idx]); idx++;

	/*-----------------------------------------------------*/
	/* ACON2 */
	map_reg(reg->uACON2.bREG.ACON1_11,	dat[idx]); idx++;
	map_reg(reg->uACON2.bREG.ACON0_11,	dat[idx]); idx++;
	map_reg(reg->uACON2.bREG.ACON1_10,	dat[idx]); idx++;
	map_reg(reg->uACON2.bREG.ACON0_10,	dat[idx]); idx++;
	map_reg(reg->uACON2.bREG.ACON1_01,	dat[idx]); idx++;
	map_reg(reg->uACON2.bREG.ACON0_01,	dat[idx]); idx++;
	map_reg(reg->uACON2.bREG.ACON1_00,	dat[idx]); idx++;
	map_reg(reg->uACON2.bREG.ACON0_00,	dat[idx]); idx++;

	/* CCON2 */
	map_reg(reg->uCCON2.bREG.CCON1_11,	dat[idx]); idx++;
	map_reg(reg->uCCON2.bREG.CCON0_11,	dat[idx]); idx++;
	map_reg(reg->uCCON2.bREG.CCON1_10,	dat[idx]); idx++;
	map_reg(reg->uCCON2.bREG.CCON0_10,	dat[idx]); idx++;
	map_reg(reg->uCCON2.bREG.CCON1_01,	dat[idx]); idx++;
	map_reg(reg->uCCON2.bREG.CCON0_01,	dat[idx]); idx++;
	map_reg(reg->uCCON2.bREG.CCON1_00,	dat[idx]); idx++;
	map_reg(reg->uCCON2.bREG.CCON0_00,	dat[idx]); idx++;

	/* ROPC20 */
	map_reg(reg->uROPC2.bREG.ASEL,		dat[idx]); idx++;
	map_reg(reg->uROPC2.bREG.ROPMODE,	dat[idx]); idx++;

	/* ROPC21 */
	map_reg(reg->uROPC2.bREG.ALPHA1,	dat[idx]); idx++;
	map_reg(reg->uROPC2.bREG.ALPHA0,	dat[idx]); idx++;

	/* MAT20 */
	map_reg(reg->uPAT2.bREG.BLUE,		dat[idx]); idx++;
	map_reg(reg->uPAT2.bREG.RED,		dat[idx]); idx++;

	/* MAT21 */
	map_reg(reg->uPAT2.bREG.GREEN,		dat[idx]); idx++;

	return (idx - reg_start_offset);
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

int wmix_verify_regs(struct vioc_wmix_t *wmix)
{
	int ret = 0;
	VIOC_WMIX *s, *d;
	reg_t sv, dv;

	s = &wmix->reg;
	d = wmix->addr;

	printf("VERIFY WMIX%d", wmix->info.id);
	if (wmix->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* CTRL */
	dv = read_reg(&d->uCTRL);
	sv = read_reg(&s->uCTRL);
	if (dv == sv) {
		printf("\tWMIX.uCTRL: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uCTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MBG0 */
	dv = read_reg(&d->uBG.nREG[0]);
	sv = read_reg(&s->uBG.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uBG.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uBG.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MBG1 */
	dv = read_reg(&d->uBG.nREG[1]);
	sv = read_reg(&s->uBG.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uBG.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uBG.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* SIZE */
	dv = read_reg(&d->uSIZE);
	sv = read_reg(&s->uSIZE);
	if (dv == sv) {
		printf("\tWMIX.uSIZE: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uSIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* POS0 */
	dv = read_reg(&d->uPOS0);
	sv = read_reg(&s->uPOS0);
	if (dv == sv) {
		printf("\tWMIX.uPOS0: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPOS0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* POS1 */
	dv = read_reg(&d->uPOS1);
	sv = read_reg(&s->uPOS1);
	if (dv == sv) {
		printf("\tWMIX.uPOS1: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPOS1: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* POS2 */
	dv = read_reg(&d->uPOS2);
	sv = read_reg(&s->uPOS2);
	if (dv == sv) {
		printf("\tWMIX.uPOS2: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPOS2: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* POS3 */
	dv = read_reg(&d->uPOS3);
	sv = read_reg(&s->uPOS3);
	if (dv == sv) {
		printf("\tWMIX.uPOS3: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPOS3: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/*-----------------------------------------------------*/
	/* MKEY00 */
	dv = read_reg(&d->uKEY0.nREG[0]);
	sv = read_reg(&s->uKEY0.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uKEY0.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY0.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY01 */
	dv = read_reg(&d->uKEY0.nREG[1]);
	sv = read_reg(&s->uKEY0.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uKEY0.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY0.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY02 */
	dv = read_reg(&d->uKEY0.nREG[2]);
	sv = read_reg(&s->uKEY0.nREG[2]);
	if (dv == sv) {
		printf("\tWMIX.uKEY0.nREG[2]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY0.nREG[2]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY03 */
	dv = read_reg(&d->uKEY0.nREG[3]);
	sv = read_reg(&s->uKEY0.nREG[3]);
	if (dv == sv) {
		printf("\tWMIX.uKEY0.nREG[3]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY0.nREG[3]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/*-----------------------------------------------------*/
	/* MKEY10 */
	dv = read_reg(&d->uKEY1.nREG[0]);
	sv = read_reg(&s->uKEY1.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uKEY1.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY1.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY11 */
	dv = read_reg(&d->uKEY1.nREG[1]);
	sv = read_reg(&s->uKEY1.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uKEY1.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY1.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY12 */
	dv = read_reg(&d->uKEY1.nREG[2]);
	sv = read_reg(&s->uKEY1.nREG[2]);
	if (dv == sv) {
		printf("\tWMIX.uKEY1.nREG[2]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY1.nREG[2]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY13 */
	dv = read_reg(&d->uKEY1.nREG[3]);
	sv = read_reg(&s->uKEY1.nREG[3]);
	if (dv == sv) {
		printf("\tWMIX.uKEY1.nREG[3]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY1.nREG[3]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/*-----------------------------------------------------*/
	/* MKEY20 */
	dv = read_reg(&d->uKEY2.nREG[0]);
	sv = read_reg(&s->uKEY2.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uKEY2.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY2.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY21 */
	dv = read_reg(&d->uKEY2.nREG[1]);
	sv = read_reg(&s->uKEY2.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uKEY2.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY2.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY22 */
	dv = read_reg(&d->uKEY2.nREG[2]);
	sv = read_reg(&s->uKEY2.nREG[2]);
	if (dv == sv) {
		printf("\tWMIX.uKEY2.nREG[2]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY2.nREG[2]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MKEY23 */
	dv = read_reg(&d->uKEY2.nREG[3]);
	sv = read_reg(&s->uKEY2.nREG[3]);
	if (dv == sv) {
		printf("\tWMIX.uKEY2.nREG[3]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uKEY2.nREG[3]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* STAT */
	dv = read_reg(&d->uSTATUS);
	sv = read_reg(&s->uSTATUS);
	if (dv == sv) {
		printf("\tWMIX.uSTATUS: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uSTATUS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* IRQMSK */
	dv = read_reg(&d->uIRQMSK);
	sv = read_reg(&s->uIRQMSK);
	if (dv == sv) {
		printf("\tWMIX.uIRQMSK: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uIRQMSK: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/*-----------------------------------------------------*/
	/* ACON0 */
	dv = read_reg(&d->uACON0);
	sv = read_reg(&s->uACON0);
	if (dv == sv) {
		printf("\tWMIX.uACON0: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uACON0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CCON0 */
	dv = read_reg(&d->uCCON0);
	sv = read_reg(&s->uCCON0);
	if (dv == sv) {
		printf("\tWMIX.uCCON0: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uCCON0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* ROPC00 */
	dv = read_reg(&d->uROPC0.nREG[0]);
	sv = read_reg(&s->uROPC0.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uROPC0.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uROPC0.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* ROPC01 */
	dv = read_reg(&d->uROPC0.nREG[1]);
	sv = read_reg(&s->uROPC0.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uROPC0.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uROPC0.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MAT01 */
	dv = read_reg(&d->uPAT0.nREG[0]);
	sv = read_reg(&s->uPAT0.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uPAT0.nREG[0]: 0x%08x\n", sv);
		ret = -1;
	} else {
		printf("\tWMIX.uPAT0.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
	}

	/* MAT02 */
	dv = read_reg(&d->uPAT0.nREG[1]);
	sv = read_reg(&s->uPAT0.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uPAT0.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPAT0.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/*-----------------------------------------------------*/
	/* ACON1 */
	dv = read_reg(&d->uACON1);
	sv = read_reg(&s->uACON1);
	if (dv == sv) {
		printf("\tWMIX.uACON1: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uACON1: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CCON1 */
	dv = read_reg(&d->uCCON1);
	sv = read_reg(&s->uCCON1);
	if (dv == sv) {
		printf("\tWMIX.uCCON1: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uCCON1: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* ROPC10 */
	dv = read_reg(&d->uROPC1.nREG[0]);
	sv = read_reg(&s->uROPC1.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uROPC1.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uROPC1.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}
	/* ROPC11 */
	dv = read_reg(&d->uROPC1.nREG[1]);
	sv = read_reg(&s->uROPC1.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uROPC1.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uROPC1.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MAT10 */
	dv = read_reg(&d->uPAT1.nREG[0]);
	sv = read_reg(&s->uPAT1.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uPAT1.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPAT1.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MAT11 */
	dv = read_reg(&d->uPAT1.nREG[1]);
	sv = read_reg(&s->uPAT1.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uPAT1.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPAT1.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/*-----------------------------------------------------*/
	/* ACON2 */
	dv = read_reg(&d->uACON2);
	sv = read_reg(&s->uACON2);
	if (dv == sv) {
		printf("\tWMIX.uACON2: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uACON2: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CCON2 */
	dv = read_reg(&d->uCCON2);
	sv = read_reg(&s->uCCON2);
	if (dv == sv) {
		printf("\tWMIX.uCCON2: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uCCON2: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* ROPC20 */
	dv = read_reg(&d->uROPC2.nREG[0]);
	sv = read_reg(&s->uROPC2.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uROPC2.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uROPC2.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* ROPC21 */
	dv = read_reg(&d->uROPC2.nREG[1]);
	sv = read_reg(&s->uROPC2.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uROPC2.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uROPC2.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MAT20 */
	dv = read_reg(&d->uPAT2.nREG[0]);
	sv = read_reg(&s->uPAT2.nREG[0]);
	if (dv == sv) {
		printf("\tWMIX.uPAT2.nREG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPAT2.nREG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MAT21 */
	dv = read_reg(&d->uPAT2.nREG[1]);
	sv = read_reg(&s->uPAT2.nREG[1]);
	if (dv == sv) {
		printf("\tWMIX.uPAT2.nREG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWMIX.uPAT2.nREG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	return ret;
}
