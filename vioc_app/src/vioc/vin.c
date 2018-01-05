#include <stdio.h>

#include <vioc.h>


int vin_map_regs(struct vioc_vin_t *vin, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_VIN *reg;

	/* read value of physical register */
	vin->reg = *vin->addr;

	reg = &vin->reg;
	dat = data->reg;

	/* vin's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = REG_START_OFFSET_VIN;
	idx = reg_start_offset;

	/* VIN_CTRL */
	map_reg(reg->uVIN_CTRL.bREG.capture,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.skip,				dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.data_order,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.fmt,				dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.stream_en,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.gen_field_en,		dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.de_active_low,		dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.field_bfield_low,	dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.vs_active_low,		dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.hs_active_low,		dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.pxclk_pol,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.vs_mask,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.flush,				dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.hsde_connect_en,	dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.intpl_en,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.intl_en,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.conv_en,			dat[idx]); idx++;
	map_reg(reg->uVIN_CTRL.bREG.enable,				dat[idx]); idx++;

	/* VIN_MISC */
	map_reg(reg->uVIN_MISC.bREG.vs_delay,	dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.fvs,		dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.align,		dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.r2ym,		dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.r2yen,		dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.y2rm,		dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.y2ren,		dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.lutif,		dat[idx]); idx++;
	map_reg(reg->uVIN_MISC.bREG.luten,		dat[idx]); idx++;

	/* VIN_SYNC_M0 */
	map_reg(reg->uVIN_SYNC_M0.bREG.sync_byte,	dat[idx]); idx++;
	map_reg(reg->uVIN_SYNC_M0.bREG.pre_sta_loc,	dat[idx]); idx++;
	map_reg(reg->uVIN_SYNC_M0.bREG.field_pos,	dat[idx]); idx++;
	map_reg(reg->uVIN_SYNC_M0.bREG.vblank,		dat[idx]); idx++;
	map_reg(reg->uVIN_SYNC_M0.bREG.hblank,		dat[idx]); idx++;

	/* VIN_SYNC_M1 */
	map_reg(reg->uVIN_SYNC_M1.bREG.pre_third,	dat[idx]); idx++;
	map_reg(reg->uVIN_SYNC_M1.bREG.pre_second,	dat[idx]); idx++;
	map_reg(reg->uVIN_SYNC_M1.bREG.pre_first,	dat[idx]); idx++;

	/* VIN_SIZE */
	map_reg(reg->uVIN_SIZE.bREG.height,	dat[idx]); idx++;
	map_reg(reg->uVIN_SIZE.bREG.width,	dat[idx]); idx++;

	/* VIN_OFFS */
	map_reg(reg->uVIN_OFFS.bREG.offs_height,	dat[idx]); idx++;
	map_reg(reg->uVIN_OFFS.bREG.offs_width,		dat[idx]); idx++;

	/* VIN_OFFS_INTL */
	map_reg(reg->uVIN_OFFS_INTL.bREG.offs_height,	dat[idx]); idx++;

	/* VIN_CROP_SIZE */
	map_reg(reg->uVIN_CROP_SIZE.bREG.height,	dat[idx]); idx++;
	map_reg(reg->uVIN_CROP_SIZE.bREG.width,		dat[idx]); idx++;

	/* VIN_CROP_OFFS */
	map_reg(reg->uVIN_CROP_OFFS.bREG.offs_height,	dat[idx]); idx++;
	map_reg(reg->uVIN_CROP_OFFS.bREG.offs_width,	dat[idx]); idx++;

	/* VIN_LUT_CTRL */
	map_reg(reg->uVIN_LUT_CTRL.bREG.IND,	dat[idx]); idx++;

	/* VIN_INT */
	map_reg(reg->uVIN_INT.bREG.int_en,			dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.invs_int_mask,	dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.vs_int_mask,		dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.eof_int_mask,	dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.update_int_mask,	dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.frm_sq,			dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.invs_int,		dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.vs_int,			dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.eof_int,			dat[idx]); idx++;
	map_reg(reg->uVIN_INT.bREG.update_int,		dat[idx]); idx++;

	//TODO:VIN set VIN_LUT_C
	

	return (idx - reg_start_offset);
}

int vin_setup(struct vioc_vin_t *vin)
{
	int ret = 0;

	/* check EN (enable-bit) is 0 */
	if (vin->reg.uVIN_CTRL.bREG.enable) {
		vin->reg.uVIN_CTRL.bREG.enable = 0;
		printf("vin%d EN was set, so disable it\n", vin->info.id);
	}

	/* set physical register */
	*vin->addr = vin->reg;

	return ret;
}
int vin_verify_regs(struct vioc_vin_t *vin)
{
	int ret = 0;
	VIOC_VIN *s, *d;
	reg_t sv, dv;

	s = &vin->reg;
	d = vin->addr;

	printf("VERIFY VIN%d\n", vin->info.id);
	if (vin->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* VIN_CTRL */
	dv = read_reg(&d->uVIN_CTRL);
	sv = read_reg(&s->uVIN_CTRL);
	if (dv == sv) {
		printf("\tVIN.uVIN_CTRL: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_CTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_MISC */
	dv = read_reg(&d->uVIN_MISC);
	sv = read_reg(&s->uVIN_MISC);
	if (dv == sv) {
		printf("\tVIN.uVIN_MISC: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_MISC: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_SYNC_M0 */
	dv = read_reg(&d->uVIN_SYNC_M0);
	sv = read_reg(&s->uVIN_SYNC_M0);
	if (dv == sv) {
		printf("\tVIN.uVIN_SYNC_M0: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_SYNC_M0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_SYNC_M1 */
	dv = read_reg(&d->uVIN_SYNC_M1);
	sv = read_reg(&s->uVIN_SYNC_M1);
	if (dv == sv) {
		printf("\tVIN.uVIN_SYNC_M1: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_SYNC_M1: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_SIZE */
	dv = read_reg(&d->uVIN_SIZE);
	sv = read_reg(&s->uVIN_SIZE);
	if (dv == sv) {
		printf("\tVIN.uVIN_SIZE: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_SIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_OFFS */
	dv = read_reg(&d->uVIN_OFFS);
	sv = read_reg(&s->uVIN_OFFS);
	if (dv == sv) {
		printf("\tVIN.uVIN_OFFS: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_OFFS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_OFFS_INTL */
	dv = read_reg(&d->uVIN_OFFS_INTL);
	sv = read_reg(&s->uVIN_OFFS_INTL);
	if (dv == sv) {
		printf("\tVIN.uVIN_OFFS_INTL: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_OFFS_INTL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_CROP_SIZE */
	dv = read_reg(&d->uVIN_CROP_SIZE);
	sv = read_reg(&s->uVIN_CROP_SIZE);
	if (dv == sv) {
		printf("\tVIN.uVIN_CROP_SIZE: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_CROP_SIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_CROP_OFFS */
	dv = read_reg(&d->uVIN_CROP_OFFS);
	sv = read_reg(&s->uVIN_CROP_OFFS);
	if (dv == sv) {
		printf("\tVIN.uVIN_CROP_OFFS: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_CROP_OFFS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_LUT_CTRL */
	dv = read_reg(&d->uVIN_LUT_CTRL);
	sv = read_reg(&s->uVIN_LUT_CTRL);
	if (dv == sv) {
		printf("\tVIN.uVIN_LUT_CTRL: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_LUT_CTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* VIN_INT */
	dv = read_reg(&d->uVIN_INT);
	sv = read_reg(&s->uVIN_INT);
	if (dv == sv) {
		printf("\tVIN.uVIN_INT: 0x%08x\n", sv);
	} else {
		printf("\tVIN.uVIN_INT: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	//TODO:VIN set VIN_LUT_C

	return ret;
}
