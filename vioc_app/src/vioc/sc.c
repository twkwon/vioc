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
	reg_start_offset = REG_START_OFFSET_SC;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.MODE_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.UPD,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.BP,			dat[idx]); idx++;

	/* SSIZE */
	map_reg(reg->uSRCSIZE.bREG.HEIGHT,	dat[idx]); idx++;
	map_reg(reg->uSRCSIZE.bREG.WIDTH,	dat[idx]); idx++;

	/* DSIZE */
	map_reg(reg->uDSTSIZE.bREG.HEIGHT,	dat[idx]); idx++;
	map_reg(reg->uDSTSIZE.bREG.WIDTH,	dat[idx]); idx++;

	/* OPOS */
	map_reg(reg->uOUTPOS.bREG.YPOS, dat[idx]); idx++;
	map_reg(reg->uOUTPOS.bREG.XPOS, dat[idx]); idx++;

	/* OSIZE */
	map_reg(reg->uOUTSIZE.bREG.HEIGHT,	dat[idx]); idx++;
	map_reg(reg->uOUTSIZE.bREG.WIDTH,	dat[idx]); idx++;

	/* IRQSTS */
	map_reg(reg->uSTATUS.bREG.ERR,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFFALL,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFRISE,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.UPDDONE,	dat[idx]); idx++;

	/* IRQMSK */
	map_reg(reg->uIRQMSK.bREG.ERR,		dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFFALL,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFRISE,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.UPDDONE,	dat[idx]); idx++;

	return (idx - reg_start_offset);
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
int sc_verify_regs(struct vioc_sc_t *sc)
{
	int ret = 0;
	VIOC_SC *s, *d;
	reg_t sv, dv;

	s = &sc->reg;
	d = sc->addr;

	printf("VERIFY SC%d\n", sc->info.id);
	if (sc->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* CTRL */
	dv = read_reg(&d->uCTRL);
	sv = read_reg(&s->uCTRL);
	if (dv == sv) {
		printf("\tSC.uCTRL: 0x%08x\n", sv);
	} else {
		printf("\tSC.uCTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* SSIZE */
	dv = read_reg(&d->uSRCSIZE);
	sv = read_reg(&s->uSRCSIZE);
	if (dv == sv) {
		printf("\tSC.uSRCSIZE: 0x%08x\n", sv);
	} else {
		printf("\tSC.uSRCSIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DSIZE */
	dv = read_reg(&d->uDSTSIZE);
	sv = read_reg(&s->uDSTSIZE);
	if (dv == sv) {
		printf("\tSC.uDSTSIZE: 0x%08x\n", sv);
	} else {
		printf("\tSC.uDSTSIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* OPOS */
	dv = read_reg(&d->uOUTPOS);
	sv = read_reg(&s->uOUTPOS);
	if (dv == sv) {
		printf("\tSC.uOUTPOS: 0x%08x\n", sv);
	} else {
		printf("\tSC.uOUTPOS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* OSIZE */
	dv = read_reg(&d->uOUTSIZE);
	sv = read_reg(&s->uOUTSIZE);
	if (dv == sv) {
		printf("\tSC.uOUTSIZE: 0x%08x\n", sv);
	} else {
		printf("\tSC.uOUTSIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* IRQSTS */
	dv = read_reg(&d->uSTATUS);
	sv = read_reg(&s->uSTATUS);
	if (dv == sv) {
		printf("\tSC.uSTATUS: 0x%08x\n", sv);
	} else {
		printf("\tSC.uSTATUS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* IRQMSK */
	dv = read_reg(&d->uIRQMSK);
	sv = read_reg(&s->uIRQMSK);
	if (dv == sv) {
		printf("\tSC.uIRQMSK: 0x%08x\n", sv);
	} else {
		printf("\tSC.uIRQMSK: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	return ret;
}
