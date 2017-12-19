#include <stdio.h>

#include <vioc.h>


int config_map_regs(struct vioc_config_t *config, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_IREQ_CONFIG *reg;

	/* read value of physical register */
	config->reg = *config->addr;

	reg = &config->reg;
	dat = data->reg;

	/* config's starting index is 0 */
	reg_start_offset = REG_START_OFFSET_CONFIG;
	idx = reg_start_offset;

	/* CFG_PATH_EDR */
	map_reg(reg->uPATH_EDR.bREG.EDR_S,		dat[idx]); idx++;

	/* CFG_MISC0 */
	map_reg(reg->uMISC0.bREG.RD14,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.RD12,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX60,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX50,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX40,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX30,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX13,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX10,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX03,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.MIX00,			dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.L2_EVS_SEL,	dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.L1_EVS_SEL,	dat[idx]); idx++;
	map_reg(reg->uMISC0.bREG.L0_EVS_SEL,	dat[idx]); idx++;

	return (idx - reg_start_offset);
}

int config_setup(struct vioc_config_t *config)
{
	int ret = 0;

	/* set physical register */
	*config->addr = config->reg;

	return ret;
}

int config_verify_regs(struct vioc_config_t *config)
{
	int ret = 0;
	VIOC_IREQ_CONFIG *s, *d;
	reg_t sv, dv;

	s = &config->reg;
	d = config->addr;

	printf("VERIFY CONFIG%d", config->info.id);
	if (config->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* CFG_PATH_EDR */
	dv = read_reg(&d->uPATH_EDR);
	sv = read_reg(&s->uPATH_EDR);
	if (dv == sv) {
		printf("\tCONFIG.uPATH_EDR: 0x%08x\n", sv);
	} else {
		printf("\tCONFIG.uPATH_EDR: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CFG_MISC0 */
	dv = read_reg(&d->uMISC0);
	sv = read_reg(&s->uMISC0);
	if (dv == sv) {
		printf("\tCONFIG.uMISC0: 0x%08x\n", sv);
	} else {
		printf("\tCONFIG.uMISC0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	return ret;
}
