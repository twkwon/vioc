#include <stdio.h>

#include <vioc.h>


int sc_map_regs(struct vioc_sc_t *sc, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_SC *reg;

	/* read value of physical register */
	sc->reg = *sc->addr;

	reg = &sc->reg;
	dat = data->reg;

	/* sc's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = 2;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.MODE_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.UPD,		dat[idx++]);
	map_reg(reg->uCTRL.bREG.BP,			dat[idx++]);

	/* SSIZE */
	map_reg(reg->uSRCSIZE.bREG.HEIGHT,	dat[idx++]);
	map_reg(reg->uSRCSIZE.bREG.WIDTH,	dat[idx++]);

	/* DSIZE */
	map_reg(reg->uDSTSIZE.bREG.HEIGHT,	dat[idx++]);
	map_reg(reg->uDSTSIZE.bREG.WIDTH,	dat[idx++]);

	/* OPOS */
	map_reg(reg->uOUTPOS.bREG.YPOS, dat[idx++]);
	map_reg(reg->uOUTPOS.bREG.XPOS, dat[idx++]);

	/* OSIZE */
	map_reg(reg->uOUTSIZE.bREG.HEIGHT,	dat[idx++]);
	map_reg(reg->uOUTSIZE.bREG.WIDTH,	dat[idx++]);

	/* IRQSTS */
	map_reg(reg->uSTATUS.bREG.ERR,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFFALL,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFRISE,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.UPDDONE,	dat[idx++]);

	/* IRQMSK */
	map_reg(reg->uIRQMSK.bREG.ERR,		dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.EOFFALL,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.EOFRISE,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.UPDDONE,	dat[idx++]);

	return (idx - reg_start_offset - 1);
}

int sc_setup(struct vioc_sc_t *sc)
{
	int ret = 0;

	/* check UPD (update-bit) is 0 */
	if (sc->reg.uCTRL.bREG.UPD) {
		sc->reg.uCTRL.bREG.UPD = 0;
		printf("sc%d UPD was set, so disable it\n", sc->info.id);
	}

	/* set physical register */
	*sc->addr = sc->reg;

	return ret;
}
