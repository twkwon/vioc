#include <stdio.h>

#include <vioc.h>

#define V_OUTCFG_MRGBSEL_SHIFT	(16)
#define V_OUTCFG_MRGBSEL_MASK	(0x3 << V_OUTCFG_MRGBSEL_SHIFT)

#define V_OUTCFG_M80SEL_SHIFT	(12)
#define V_OUTCFG_M80SEL_MASK	(0x3 << V_OUTCFG_M80SEL_SHIFT)

#define V_OUTCFG_HDVESEL_SHIFT	(8)
#define V_OUTCFG_HDVESEL_MASK	(0x3 << V_OUTCFG_HDVESEL_SHIFT)

#define V_OUTCFG_SDVESEL_SHIFT	(4)
#define V_OUTCFG_SDVESEL_MASK	(0x3 << V_OUTCFG_SDVESEL_SHIFT)

#define V_OUTCFG_HDMISEL_SHIFT	(0)
#define V_OUTCFG_HDMISEL_MASK	(0x3 << V_OUTCFG_HDMISEL_SHIFT)

int outcfg_map_regs(struct vioc_outcfg_t *outcfg, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_OUTCFG *reg;

	/* read value of physical register */
	outcfg->reg = *outcfg->addr;

	reg = &outcfg->reg;
	dat = data->reg;

	/* outcfg's starting index is 0 */
	reg_start_offset = REG_START_OFFSET_OUTCFG;
	idx = reg_start_offset;

	/* MISC */
	map_reg(reg->uMISCCFG.bREG.MRGBSEL,	dat[idx]); idx++;
	map_reg(reg->uMISCCFG.bREG.M80SEL,	dat[idx]); idx++;
	map_reg(reg->uMISCCFG.bREG.HDVESEL,	dat[idx]); idx++;
	map_reg(reg->uMISCCFG.bREG.SDVESEL,	dat[idx]); idx++;
	map_reg(reg->uMISCCFG.bREG.HDMISEL,	dat[idx]); idx++;

	return (idx - reg_start_offset);
}

int outcfg_setup(struct vioc_outcfg_t *outcfg)
{
	int ret = 0;

	/* set physical register */
	*outcfg->addr = outcfg->reg;

	return ret;
}

int outcfg_verify_regs(struct vioc_outcfg_t *outcfg)
{
	int ret = 0;
	VIOC_OUTCFG *s, *d;
	reg_t sv, dv;

	s = &outcfg->reg;
	d = outcfg->addr;

	printf("VERIFY OUTCFG%d\n", outcfg->info.id);
	if (outcfg->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* CTRL */
	dv = read_reg(&d->uMISCCFG);
	sv = read_reg(&s->uMISCCFG);
	if (dv == sv) {
		printf("\tOUTCFG.uMISCCFG: 0x%08x\n", sv);
	} else {
		printf("\tOUTCFG.uMISCCFG: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	return ret;
}

int outcfg_config(struct test_case_t *tc)
{
	int ret = 0;
	reg_t val, reg;
	VIOC_OUTCFG *outcfg_reg;	// physical register
	VIOC_OUTCFG *outcfg_val;	// test data

	outcfg_reg = tc->outcfg.addr;
	outcfg_val = &tc->outcfg.reg;

	/* MISC */
	val = read_reg(&outcfg_reg->uMISCCFG);
	BITCSET(val, V_OUTCFG_MRGBSEL_MASK, outcfg_val->uMISCCFG.bREG.MRGBSEL << V_OUTCFG_MRGBSEL_SHIFT);
	BITCSET(val, V_OUTCFG_M80SEL_MASK, outcfg_val->uMISCCFG.bREG.M80SEL << V_OUTCFG_M80SEL_SHIFT);
	BITCSET(val, V_OUTCFG_HDVESEL_MASK, outcfg_val->uMISCCFG.bREG.HDVESEL << V_OUTCFG_HDVESEL_SHIFT);
	BITCSET(val, V_OUTCFG_SDVESEL_MASK, outcfg_val->uMISCCFG.bREG.SDVESEL << V_OUTCFG_SDVESEL_SHIFT);
	BITCSET(val, V_OUTCFG_HDMISEL_SHIFT, outcfg_val->uMISCCFG.bREG.HDMISEL << V_OUTCFG_HDMISEL_SHIFT);
	write_reg(&outcfg_reg->uMISCCFG, val);

	reg = read_reg(&outcfg_reg->uMISCCFG);
	if (val != reg) {
		printf("[%s] error: MISC(0x%08x) != val(0x%08x)\n", __func__, reg, val);
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}
