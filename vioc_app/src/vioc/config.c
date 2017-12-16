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

	/* config's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = 0;
	idx = reg_start_offset;

	/* CFG_PATH_EDR */
	map_reg(reg->uPATH_EDR.bREG.EDR_S,		dat[idx++]);

	/* CFG_MISC0 */
	map_reg(reg->uMISC0.bREG.RD14,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.RD12,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX60,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX50,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX40,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX30,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX13,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX10,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX03,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.MIX00,			dat[idx++]);
	map_reg(reg->uMISC0.bREG.L2_EVS_SEL,	dat[idx++]);
	map_reg(reg->uMISC0.bREG.L1_EVS_SEL,	dat[idx++]);
	map_reg(reg->uMISC0.bREG.L0_EVS_SEL,	dat[idx++]);

	return (idx - reg_start_offset - 1);
}

int config_setup(struct vioc_config_t *config)
{
	int ret = 0;

	/* set physical register */
	*config->addr = config->reg;

	return ret;
}
