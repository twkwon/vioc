#include <stdio.h>

#include <vioc.h>


int lut_map_regs(struct vioc_lut_t *lut, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_LUT *reg;

	/* read value of physical register */
	lut->reg = *lut->addr;

	reg = &lut->reg;
	dat = data->reg;

	/* lut's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = 2;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.EN,	dat[idx++]);

	/* DEV0_CFG */
	map_reg(reg->uDEV0_CFG.bREG.EN,		dat[idx++]);
	map_reg(reg->uDEV0_CFG.bREG.SEL,	dat[idx++]);

	/* DEV1_CFG */
	map_reg(reg->uDEV1_CFG.bREG.EN,		dat[idx++]);
	map_reg(reg->uDEV1_CFG.bREG.SEL,	dat[idx++]);

	/* DEV2_CFG */
	map_reg(reg->uDEV2_CFG.bREG.EN,		dat[idx++]);
	map_reg(reg->uDEV2_CFG.bREG.SEL,	dat[idx++]);

	/* VIOC0_CFG */
	map_reg(reg->uVIOC0_CFG.bREG.EN,	dat[idx++]);
	map_reg(reg->uVIOC0_CFG.bREG.SEL,	dat[idx++]);

	/* VIOC1_CFG */
	map_reg(reg->uVIOC1_CFG.bREG.EN,	dat[idx++]);
	map_reg(reg->uVIOC1_CFG.bREG.SEL,	dat[idx++]);

	/* VIOC2_CFG */
	map_reg(reg->uVIOC2_CFG.bREG.EN,	dat[idx++]);
	map_reg(reg->uVIOC2_CFG.bREG.SEL,	dat[idx++]);

	/* VIOC3_CFG */
	map_reg(reg->uVIOC3_CFG.bREG.EN,	dat[idx++]);
	map_reg(reg->uVIOC3_CFG.bREG.SEL,	dat[idx++]);

	/* TABLE_IND */
	map_reg(reg->uTABLE_IND.bREG.IND,	dat[idx++]);

	/* UPDATE_PEND */
	map_reg(reg->uUPDATE_PEND.bREG.SEL,	dat[idx++]);

	/* CSC_COEFF11_12 */
	map_reg(reg->uCSC_COEFF11_12.bREG.COEFF12,	dat[idx++]);
	map_reg(reg->uCSC_COEFF11_12.bREG.COEFF11,	dat[idx++]);

	/* CSC_COEFF13_21 */
	map_reg(reg->uCSC_COEFF13_21.bREG.COEFF21,	dat[idx++]);
	map_reg(reg->uCSC_COEFF13_21.bREG.COEFF13,	dat[idx++]);

	/* CSC_COEFF22_23 */
	map_reg(reg->uCSC_COEFF22_23.bREG.COEFF23,	dat[idx++]);
	map_reg(reg->uCSC_COEFF22_23.bREG.COEFF22,	dat[idx++]);

	/* CSC_COEFF31_32 */
	map_reg(reg->uCSC_COEFF31_32.bREG.COEFF32,	dat[idx++]);
	map_reg(reg->uCSC_COEFF31_32.bREG.COEFF31,	dat[idx++]);

	/* CSC_COEFF33 */
	map_reg(reg->uCSC_COEFF33.bREG.COEFF33, 	dat[idx++]);

	/* MIX_CFG */
	map_reg(reg->uMIX_CFG.bREG.R2YSEL,	dat[idx++]);
	map_reg(reg->uMIX_CFG.bREG.BYP,		dat[idx++]);

	return (idx - reg_start_offset - 1);
}

int lut_setup(struct vioc_lut_t *lut)
{
	int ret = 0;

	/* set physical register */
	*lut->addr = lut->reg;

	/* set lut table */
	//TODO: set lut table

	return ret;
}

int lut_verify_regs(struct vioc_lut_t *lut)
{
	int ret = 0;
	VIOC_LUT *s, *d;
	reg_t sv, dv;

	s = &lut->reg;
	d = lut->addr;

	printf("VERIFY LUT%d", lut->info.id);
	if (lut->info.id < 0) {
		printf("%tN/A\n");
		return ret;
	}

	/* CTRL */
	dv = read_reg(&d->uCTRL);
	sv = read_reg(&s->uCTRL);
	if (dv == sv) {
		printf("%tLUT.uCTRL: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uCTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DEV0_CFG */
	dv = read_reg(&d->uDEV0_CFG);
	sv = read_reg(&s->uDEV0_CFG);
	if (dv == sv) {
		printf("%tLUT.uDEV0_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uDEV0_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DEV1_CFG */
	dv = read_reg(&d->uDEV1_CFG);
	sv = read_reg(&s->uDEV1_CFG);
	if (dv == sv) {
		printf("%tLUT.uDEV1_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uDEV1_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DEV2_CFG */
	dv = read_reg(&d->uDEV2_CFG);
	sv = read_reg(&s->uDEV2_CFG);
	if (dv == sv) {
		printf("%tLUT.uDEV2_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uDEV2_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC0_CFG */
	dv = read_reg(&d->uVIOC0_CFG);
	sv = read_reg(&s->uVIOC0_CFG);
	if (dv == sv) {
		printf("%tLUT.uVIOC0_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uVIOC0_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC1_CFG */
	dv = read_reg(&d->uVIOC1_CFG);
	sv = read_reg(&s->uVIOC1_CFG);
	if (dv == sv) {
		printf("%tLUT.uVIOC1_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uVIOC1_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC2_CFG */
	dv = read_reg(&d->uVIOC2_CFG);
	sv = read_reg(&s->uVIOC2_CFG);
	if (dv == sv) {
		printf("%tLUT.uVIOC2_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uVIOC2_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC3_CFG */
	dv = read_reg(&d->uVIOC3_CFG);
	sv = read_reg(&s->uVIOC3_CFG);
	if (dv == sv) {
		printf("%tLUT.uVIOC3_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uVIOC3_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* TABLE_IND */
	dv = read_reg(&d->uTABLE_IND);
	sv = read_reg(&s->uTABLE_IND);
	if (dv == sv) {
		printf("%tLUT.uTABLE_IND: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uTABLE_IND: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* UPDATE_PEND */
	dv = read_reg(&d->uUPDATE_PEND);
	sv = read_reg(&s->uUPDATE_PEND);
	if (dv == sv) {
		printf("%tLUT.uUPDATE_PEND: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uUPDATE_PEND: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF11_12 */
	dv = read_reg(&d->uCSC_COEFF11_12);
	sv = read_reg(&s->uCSC_COEFF11_12);
	if (dv == sv) {
		printf("%tLUT.uCSC_COEFF11_12: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uCSC_COEFF11_12: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF13_21 */
	dv = read_reg(&d->uCSC_COEFF13_21);
	sv = read_reg(&s->uCSC_COEFF13_21);
	if (dv == sv) {
		printf("%tLUT.uCSC_COEFF13_21: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uCSC_COEFF13_21: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF22_23 */
	dv = read_reg(&d->uCSC_COEFF22_23);
	sv = read_reg(&s->uCSC_COEFF22_23);
	if (dv == sv) {
		printf("%tLUT.uCSC_COEFF22_23: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uCSC_COEFF22_23: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF31_32 */
	dv = read_reg(&d->uCSC_COEFF31_32);
	sv = read_reg(&s->uCSC_COEFF31_32);
	if (dv == sv) {
		printf("%tLUT.uCSC_COEFF31_32: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uCSC_COEFF31_32: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF33 */
	dv = read_reg(&d->uCSC_COEFF33);
	sv = read_reg(&s->uCSC_COEFF33);
	if (dv == sv) {
		printf("%tLUT.uCSC_COEFF33: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uCSC_COEFF33: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MIX_CFG */
	dv = read_reg(&d->uMIX_CFG);
	sv = read_reg(&s->uMIX_CFG);
	if (dv == sv) {
		printf("%tLUT.uMIX_CFG: 0x%08x\n", sv);
	} else {
		printf("%tLUT.uMIX_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	return ret;
}
