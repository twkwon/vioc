#include <stdio.h>

#include <vioc.h>


int outcfg_map_regs(struct vioc_outcfg_t *outcfg, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_OUTCFG *reg;

	/* read value of physical register */
	outcfg->reg = *outcfg->addr;

	reg = &outcfg->reg;
	dat = data->reg;

	/* outcfg's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = 0;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uMISCCFG.bREG.MRGBSEL,	dat[idx++]);
	map_reg(reg->uMISCCFG.bREG.M80SEL,	dat[idx++]);
	map_reg(reg->uMISCCFG.bREG.HDVESEL,	dat[idx++]);
	map_reg(reg->uMISCCFG.bREG.SDVESEL,	dat[idx++]);
	map_reg(reg->uMISCCFG.bREG.HDMISEL,	dat[idx++]);

	return (idx - reg_start_offset - 1);
}

int outcfg_setup(struct vioc_outcfg_t *outcfg)
{
	int ret = 0;

	/* set physical register */
	*outcfg->addr = outcfg->reg;

	return ret;
}
