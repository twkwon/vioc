#include <stdio.h>

#include <debug.h>
#include <vioc.h>


void lut_en_ctrl(struct vioc_lut_t *lut, unsigned int enable)
{
	//TODO:LUT
	//BITCSET(lut->addr->uCTRL.nREG, 1 << 16, enable << 16);
}

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
	reg_start_offset = REG_START_OFFSET_LUT;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.EN,	dat[idx]); idx++;

	/* DEV0_CFG */
	map_reg(reg->uDEV0_CFG.bREG.EN,		dat[idx]); idx++;
#if defined(__ARCH_TCC898X__)
	map_reg(reg->uDEV0_CFG.bREG.SEL,	dat[idx]); idx++;
#elif defined(__ARCH_TCC899X__) || defined(__ARCH_TCC803X__)
	idx++;	//TODO: DEVk_CFG[7:0].SEL does not exist.
#else
	#error "ERROR: Not defined ARCH in configure"
#endif

	/* DEV1_CFG */
	map_reg(reg->uDEV1_CFG.bREG.EN,		dat[idx]); idx++;
#if defined(__ARCH_TCC898X__)
	map_reg(reg->uDEV1_CFG.bREG.SEL,	dat[idx]); idx++;
#elif defined(__ARCH_TCC899X__) || defined(__ARCH_TCC803X__)
	idx++;	//TODO: DEVk_CFG[7:0].SEL does not exist.
#else
	#error "ERROR: Not defined ARCH in configure"
#endif

	/* DEV2_CFG */
	map_reg(reg->uDEV2_CFG.bREG.EN,		dat[idx]); idx++;
#if defined(__ARCH_TCC898X__)
	map_reg(reg->uDEV2_CFG.bREG.SEL,	dat[idx]); idx++;
#elif defined(__ARCH_TCC899X__) || defined(__ARCH_TCC803X__)
	idx++;	//TODO: DEVk_CFG[7:0].SEL does not exist.
#else
	#error "ERROR: Not defined ARCH in configure"
#endif

	/* VIOC0_CFG */
	map_reg(reg->uVIOC0_CFG.bREG.EN,	dat[idx]); idx++;
	map_reg(reg->uVIOC0_CFG.bREG.SEL,	dat[idx]); idx++;

	/* VIOC1_CFG */
	map_reg(reg->uVIOC1_CFG.bREG.EN,	dat[idx]); idx++;
	map_reg(reg->uVIOC1_CFG.bREG.SEL,	dat[idx]); idx++;

	/* VIOC2_CFG */
	map_reg(reg->uVIOC2_CFG.bREG.EN,	dat[idx]); idx++;
	map_reg(reg->uVIOC2_CFG.bREG.SEL,	dat[idx]); idx++;

	/* VIOC3_CFG */
	map_reg(reg->uVIOC3_CFG.bREG.EN,	dat[idx]); idx++;
	map_reg(reg->uVIOC3_CFG.bREG.SEL,	dat[idx]); idx++;

#if defined(__ARCH_TCC898X__) || defined(__ARCH_TCC899X__)
	/* TABLE_IND */
	map_reg(reg->uTABLE_IND.bREG.IND,	dat[idx]); idx++;

	/* UPDATE_PEND */
	map_reg(reg->uUPDATE_PEND.bREG.SEL,	dat[idx]); idx++;

	/* CSC_COEFF11_12 */
	map_reg(reg->uCSC_COEFF11_12.bREG.COEFF12,	dat[idx]); idx++;
	map_reg(reg->uCSC_COEFF11_12.bREG.COEFF11,	dat[idx]); idx++;

	/* CSC_COEFF13_21 */
	map_reg(reg->uCSC_COEFF13_21.bREG.COEFF21,	dat[idx]); idx++;
	map_reg(reg->uCSC_COEFF13_21.bREG.COEFF13,	dat[idx]); idx++;

	/* CSC_COEFF22_23 */
	map_reg(reg->uCSC_COEFF22_23.bREG.COEFF23,	dat[idx]); idx++;
	map_reg(reg->uCSC_COEFF22_23.bREG.COEFF22,	dat[idx]); idx++;

	/* CSC_COEFF31_32 */
	map_reg(reg->uCSC_COEFF31_32.bREG.COEFF32,	dat[idx]); idx++;
	map_reg(reg->uCSC_COEFF31_32.bREG.COEFF31,	dat[idx]); idx++;

	/* CSC_COEFF33 */
	map_reg(reg->uCSC_COEFF33.bREG.COEFF33, 	dat[idx]); idx++;

	/* MIX_CFG */
	map_reg(reg->uMIX_CFG.bREG.R2YSEL,	dat[idx]); idx++;
	map_reg(reg->uMIX_CFG.bREG.BYP,		dat[idx]); idx++;
#elif defined(__ARCH_TCC803X__)
	idx++;			// TABLE_IND
	idx++;			// UPDATE_PEND
	idx++;idx++;	// CSC_COEFF11_12
	idx++;idx++;	// CSC_COEFF13_21
	idx++;idx++;	// CSC_COEFF22_23
	idx++;idx++;	// CSC_COEFF31_32
	idx++;			// CSC_COEFF33
	idx++;idx++;	// MIX_CFG
#endif

	return (idx - reg_start_offset);
}

int lut_setup(struct vioc_lut_t *lut)
{
	int ret = 0;
    unsigned int i,  reg_off, lut_index;
#if defined(__ARCH_TCC898X__) || defined(__ARCH_TCC899X__)
    unsigned int ind_v;
#endif
    unsigned int r, g, b;
    volatile unsigned int color = 0;

	int lut_table_addr_offset;
	addr_t *lut_table_addr;
	

	/* check EN and UPD regs that is enabled */
	// -> no need

	// disable vioc_lut0, vioc_lut1
	lut->addr->uVIOC0_CFG.nREG = 0;
	lut->addr->uVIOC1_CFG.nREG = 0;

	/* set lut table */
	lut_table_addr_offset = OFFSET_LUT_TABLE_FROM_LUT;
	lut_table_addr = (unsigned int *)((unsigned int *)lut->addr + INT32_OFFSET(lut_table_addr_offset));

	lut_index = lut->info.id;
	write_reg(&lut->addr->uCTRL.nREG, lut_index);

	/* -> color inversion */
    for(i = 0; i < LUT_TABLE_SIZE; i++)
    {
		r = (LUT_TABLE_SIZE - i) - 1;
		g = (LUT_TABLE_SIZE - i) - 1;
		b = (LUT_TABLE_SIZE - i) - 1;
		
#if defined(__ARCH_TCC898X__) || defined(__ARCH_TCC899X__)

        color = ((r & 0x3FF) << 20) | ((g & 0x3FF) << 10) | (b & 0x3FF);
        ind_v = i >> 8;
		write_reg(&lut->addr->uTABLE_IND.nREG, ind_v);
        reg_off = (0xFF & i);
		//write_reg((unsigned int)lut_table_addr + (reg_off * 0x4), color);
		write_reg(((unsigned long)lut_table_addr + (reg_off * 0x4)), color);

#elif defined(__ARCH_TCC803X__)

        color = ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
        reg_off = (0xFF & i);
		//write_reg((unsigned int)lut_table_addr + (reg_off * 0x4), color);
		write_reg(((unsigned long)lut_table_addr + (reg_off * 0x4)), color);

#endif
    }

#if defined(__ARCH_TCC898X__) || defined(__ARCH_TCC899X__)
   if(lut_index >= VIOC_LUT_COMP0)
   		write_reg(&lut->addr->uUPDATE_PEND.nREG, 1 << ((lut_index - VIOC_LUT_COMP0) << LUT_TABLE_OFFSET));
#endif

	/* set physical register */
	*lut->addr = lut->reg;

	return ret;
}

int lut_verify_regs(struct vioc_lut_t *lut)
{
	int ret = 0;
	VIOC_LUT *s, *d;
	reg_t sv, dv;

	s = &lut->reg;
	d = lut->addr;

	DBG(DL_VIOC, "VERIFY LUT%d\n", lut->info.id);
	if (lut->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* CTRL */
	dv = read_reg(&d->uCTRL);
	sv = read_reg(&s->uCTRL);
	if (dv == sv) {
		printf("\tLUT.uCTRL: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uCTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DEV0_CFG */
	dv = read_reg(&d->uDEV0_CFG);
	sv = read_reg(&s->uDEV0_CFG);
	if (dv == sv) {
		printf("\tLUT.uDEV0_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uDEV0_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DEV1_CFG */
	dv = read_reg(&d->uDEV1_CFG);
	sv = read_reg(&s->uDEV1_CFG);
	if (dv == sv) {
		printf("\tLUT.uDEV1_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uDEV1_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DEV2_CFG */
	dv = read_reg(&d->uDEV2_CFG);
	sv = read_reg(&s->uDEV2_CFG);
	if (dv == sv) {
		printf("\tLUT.uDEV2_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uDEV2_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC0_CFG */
	dv = read_reg(&d->uVIOC0_CFG);
	sv = read_reg(&s->uVIOC0_CFG);
	if (dv == sv) {
		printf("\tLUT.uVIOC0_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uVIOC0_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC1_CFG */
	dv = read_reg(&d->uVIOC1_CFG);
	sv = read_reg(&s->uVIOC1_CFG);
	if (dv == sv) {
		printf("\tLUT.uVIOC1_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uVIOC1_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC2_CFG */
	dv = read_reg(&d->uVIOC2_CFG);
	sv = read_reg(&s->uVIOC2_CFG);
	if (dv == sv) {
		printf("\tLUT.uVIOC2_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uVIOC2_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIOC3_CFG */
	dv = read_reg(&d->uVIOC3_CFG);
	sv = read_reg(&s->uVIOC3_CFG);
	if (dv == sv) {
		printf("\tLUT.uVIOC3_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uVIOC3_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

#if !defined(__ARCH_TCC803X__)
	/* TABLE_IND */
	dv = read_reg(&d->uTABLE_IND);
	sv = read_reg(&s->uTABLE_IND);
	if (dv == sv) {
		printf("\tLUT.uTABLE_IND: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uTABLE_IND: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* UPDATE_PEND */
	dv = read_reg(&d->uUPDATE_PEND);
	sv = read_reg(&s->uUPDATE_PEND);
	if (dv == sv) {
		printf("\tLUT.uUPDATE_PEND: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uUPDATE_PEND: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF11_12 */
	dv = read_reg(&d->uCSC_COEFF11_12);
	sv = read_reg(&s->uCSC_COEFF11_12);
	if (dv == sv) {
		printf("\tLUT.uCSC_COEFF11_12: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uCSC_COEFF11_12: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF13_21 */
	dv = read_reg(&d->uCSC_COEFF13_21);
	sv = read_reg(&s->uCSC_COEFF13_21);
	if (dv == sv) {
		printf("\tLUT.uCSC_COEFF13_21: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uCSC_COEFF13_21: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF22_23 */
	dv = read_reg(&d->uCSC_COEFF22_23);
	sv = read_reg(&s->uCSC_COEFF22_23);
	if (dv == sv) {
		printf("\tLUT.uCSC_COEFF22_23: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uCSC_COEFF22_23: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF31_32 */
	dv = read_reg(&d->uCSC_COEFF31_32);
	sv = read_reg(&s->uCSC_COEFF31_32);
	if (dv == sv) {
		printf("\tLUT.uCSC_COEFF31_32: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uCSC_COEFF31_32: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CSC_COEFF33 */
	dv = read_reg(&d->uCSC_COEFF33);
	sv = read_reg(&s->uCSC_COEFF33);
	if (dv == sv) {
		printf("\tLUT.uCSC_COEFF33: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uCSC_COEFF33: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MIX_CFG */
	dv = read_reg(&d->uMIX_CFG);
	sv = read_reg(&s->uMIX_CFG);
	if (dv == sv) {
		printf("\tLUT.uMIX_CFG: 0x%08x\n", sv);
	} else {
		printf("\tLUT.uMIX_CFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}
#endif

	return ret;
}
