#include <stdio.h>

#include <vioc.h>


void wdma_set_offset(struct vioc_wdma_t *wdma, addr_t offset0, addr_t offset1)
{
	BITCSET(wdma->addr->uOFFSET.nREG, 0xffffffff, (offset1 << 16) | offset0);
}

void wdma_set_address(struct vioc_wdma_t *wdma, addr_t base0, addr_t base1, addr_t base2)
{
	BITCSET(wdma->addr->nBASE0, 0xffffffff, base0);
	BITCSET(wdma->addr->nBASE1, 0xffffffff, base1);
	BITCSET(wdma->addr->nBASE2, 0xffffffff, base2);
}

int wdma_map_regs(struct vioc_wdma_t *wdma, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_WDMA *reg;

	/* read value of physical register */
	wdma->reg = *wdma->addr;

	reg = &wdma->reg;
	dat = data->reg;

	/* wdma's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = REG_START_OFFSET_WDMA;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.INTL,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.FU,			dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.IEN,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.DITHSEL,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.DITHEN,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.CONT,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.SREQ,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.Y2RMD,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.Y2R,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.UPD,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.BR,			dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.SWAP,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.R2YMD,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.R2Y,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.FMT10,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.FMT,		dat[idx]); idx++;

	/* RATE */
	map_reg(reg->uRATE.bREG.REN,		dat[idx]); idx++;
	map_reg(reg->uRATE.bREG.MAXRATE,	dat[idx]); idx++;
	map_reg(reg->uRATE.bREG.SYNCMD,		dat[idx]); idx++;
	map_reg(reg->uRATE.bREG.SEN,		dat[idx]); idx++;
	map_reg(reg->uRATE.bREG.SYNCSEL,	dat[idx]); idx++;

	/* SIZE */
	map_reg(reg->uSIZE.bREG.HEIGHT,	dat[idx]); idx++;
	map_reg(reg->uSIZE.bREG.WIDTH,	dat[idx]); idx++;

	/* BASE0 */
	map_reg(reg->nBASE0, dat[idx]); idx++;

	/* CADDR (RO) */
	map_reg(reg->nCBASE, dat[idx]); idx++;

	/* BASE1 */
	map_reg(reg->nBASE1, dat[idx]); idx++;

	/* BASE2 */
	map_reg(reg->nBASE2, dat[idx]); idx++;

	/* OFFS */
	map_reg(reg->uOFFSET.bREG.OFFSET1, dat[idx]); idx++;
	map_reg(reg->uOFFSET.bREG.OFFSET0, dat[idx]); idx++;

	/* BG0 */
	map_reg(reg->uBG.bREG.BG1, dat[idx]); idx++;
	map_reg(reg->uBG.bREG.BG0, dat[idx]); idx++;

	/* BG1 */
	map_reg(reg->uBG.bREG.BG3, dat[idx]); idx++;
	map_reg(reg->uBG.bREG.BG2, dat[idx]); idx++;

	/* PTS */
	map_reg(reg->uPTS.bREG.PTS, dat[idx]); idx++;

	/* DMAT0 */
	map_reg(reg->uDMAT.bREG.DITH13, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH12, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH11, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH10, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH03, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH02, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH01, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH00, dat[idx]); idx++;

	/* DMAT1 */
	map_reg(reg->uDMAT.bREG.DITH33, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH32, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH31, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH30, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH23, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH22, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH21, dat[idx]); idx++;
	map_reg(reg->uDMAT.bREG.DITH20, dat[idx]); idx++;

	/* ROLL */
	map_reg(reg->uROLL.bREG.ROLL,		dat[idx]); idx++;
	map_reg(reg->uROLL.bREG.ROLLCNT,	dat[idx]); idx++;

	/* SBASE */
	map_reg(reg->nSBASE, dat[idx]); idx++;

	/* IRQSTS */
	map_reg(reg->uIRQSTS.bREG.STS_EOF,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.STS_BFIELD,	dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.STS_SEN,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.SEOFFALL,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.SEOFRISE,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.EOFFALL,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.EOFRISE,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.ENFALL,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.ENRISE,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.ROLL,			dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.STOPREQ,		dat[idx]); idx++;
	map_reg(reg->uIRQSTS.bREG.UPDDONE,		dat[idx]); idx++;

	/* IRQMSK */
	map_reg(reg->uIRQMSK.bREG.SEOFFALL,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.SEOFRISE,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFFALL,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFRISE,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.ENFALL,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.ENRISE,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.ROLL,		dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.STOPREQ,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.UPDDONE,	dat[idx]); idx++;

	return (idx - reg_start_offset);
}

int wdma_setup(struct vioc_wdma_t *wdma)
{
	int ret = 0;

	/* check EN (enable-bit) is 0 */
	if (wdma->reg.uCTRL.bREG.IEN) {
		wdma->reg.uCTRL.bREG.IEN = 0;
		printf("wdma%d IEN was set, so disable it\n", wdma->info.id);
	}

	/* check UPD (update-bit) is 0 */
	if (wdma->reg.uCTRL.bREG.UPD) {
		wdma->reg.uCTRL.bREG.UPD = 0;
		printf("wdma%d UPD was set, so disable it\n", wdma->info.id);
	}

	/* set physical register */
	*wdma->addr = wdma->reg;

	return ret;
}

int wdma_verify_regs(struct vioc_wdma_t *wdma)
{
	int ret = 0;
	VIOC_WDMA *s, *d;
	reg_t sv, dv;

	s = &wdma->reg;
	d = wdma->addr;

	printf("VERIFY WDMA%d\n", wdma->info.id);
	if (wdma->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* CTRL */
	dv = read_reg(&d->uCTRL);
	sv = read_reg(&s->uCTRL);
	if (dv == sv) {
		printf("\tWDMA.uCTRL: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uCTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* RATE */
	dv = read_reg(&d->uRATE);
	sv = read_reg(&s->uRATE);
	if (dv == sv) {
		printf("\tWDMA.uRATE: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uRATE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* SIZE */
	dv = read_reg(&d->uSIZE);
	sv = read_reg(&s->uSIZE);
	if (dv == sv) {
		printf("\tWDMA.uSIZE: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uSIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BASE0 */
	dv = read_reg(&d->nBASE0);
	sv = read_reg(&s->nBASE0);
	if (dv == sv) {
		printf("\tWDMA.nBASE0: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.nBASE0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CADDR (RO) */
	dv = read_reg(&d->nCBASE);
	sv = read_reg(&s->nCBASE);
	if (dv == sv) {
		printf("\tWDMA.nCBASE: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.nCBASE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BASE1 */
	dv = read_reg(&d->nBASE1);
	sv = read_reg(&s->nBASE1);
	if (dv == sv) {
		printf("\tWDMA.nBASE1: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.nBASE1: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BASE2 */
	dv = read_reg(&d->nBASE2);
	sv = read_reg(&s->nBASE2);
	if (dv == sv) {
		printf("\tWDMA.nBASE2: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.nBASE2: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* OFFS */
	dv = read_reg(&d->uOFFSET);
	sv = read_reg(&s->uOFFSET);
	if (dv == sv) {
		printf("\tWDMA.uOFFSET: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uOFFSET: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BG0 */
	dv = read_reg(&d->uBG.nREG[0]);
	sv = read_reg(&s->uBG.nREG[0]);
	if (dv == sv) {
		printf("\tWDMA.uBG[0]: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uBG[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BG1 */
	dv = read_reg(&d->uBG.nREG[1]);
	sv = read_reg(&s->uBG.nREG[1]);
	if (dv == sv) {
		printf("\tWDMA.uBG[1]: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uBG[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* PTS */
	dv = read_reg(&d->uRATE);
	sv = read_reg(&s->uRATE);
	if (dv == sv) {
		printf("\tWDMA.uRATE: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uRATE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DMAT0 */
	dv = read_reg(&d->uDMAT.nREG[0]);
	sv = read_reg(&s->uDMAT.nREG[0]);
	if (dv == sv) {
		printf("\tWDMA.uDMAT[0]: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uDMAT[0]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* DMAT1 */
	dv = read_reg(&d->uDMAT.nREG[1]);
	sv = read_reg(&s->uDMAT.nREG[1]);
	if (dv == sv) {
		printf("\tWDMA.uDMAT[1]: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uDMAT[1]: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* ROLL */
	dv = read_reg(&d->uROLL);
	sv = read_reg(&s->uROLL);
	if (dv == sv) {
		printf("\tWDMA.uRATE: 0x%08x\n", sv);
		ret = -1;
	} else {
		printf("\tWDMA.uRATE: 0x%08x != 0x%08x\n", sv, dv);
	}

	/* SBASE */
	dv = read_reg(&d->nSBASE);
	sv = read_reg(&s->nSBASE);
	if (dv == sv) {
		printf("\tWDMA.nSBASE: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.nSBASE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* IRQSTS */
	dv = read_reg(&d->uIRQSTS);
	sv = read_reg(&s->uIRQSTS);
	if (dv == sv) {
		printf("\tWDMA.uIRQSTS: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uIRQSTS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* IRQMSK */
	dv = read_reg(&d->uIRQMSK);
	sv = read_reg(&s->uIRQMSK);
	if (dv == sv) {
		printf("\tWDMA.uIRQMSK: 0x%08x\n", sv);
	} else {
		printf("\tWDMA.uIRQMSK: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	return ret;
}
