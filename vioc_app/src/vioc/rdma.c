#include <stdio.h>

#include <debug.h>
#include <vioc.h>


void rdma_en_ctrl(struct vioc_rdma_t *rdma, unsigned int enable)
{
	BITCSET(rdma->addr->uCTRL.nREG, 1 << 28, enable << 28);
	BITCSET(rdma->addr->uCTRL.nREG, 1 << 16, enable << 16);
}

void rdma_set_fmt(struct vioc_rdma_t *rdma, unsigned int fmt)
{
	BITCSET(rdma->addr->uMISC.nREG, 0x1f << 0, fmt << 0);
}

void rdma_set_fmt10(struct vioc_rdma_t *rdma, unsigned int fmt10)
{
	BITCSET(rdma->addr->uMISC.nREG, 0x7 << 5, fmt10 << 5);
}

void rdma_set_y2r(struct vioc_rdma_t *rdma, unsigned int y2r)
{
	BITCSET(rdma->addr->uCTRL.nREG, 0x1 << 8, y2r << 8);
}

void rdma_set_y2rmd(struct vioc_rdma_t *rdma, unsigned int y2rmd)
{
	BITCSET(rdma->addr->uMISC.nREG, 0x7 << 8, y2rmd << 8);
}

void rdma_set_r2y(struct vioc_rdma_t *rdma, unsigned int r2y)
{
	BITCSET(rdma->addr->uCTRL.nREG, 0x1 << 17, r2y << 17);
}

void rdma_set_r2ymd(struct vioc_rdma_t *rdma, unsigned int r2ymd)
{
	BITCSET(rdma->addr->uMISC.nREG, 0x7, r2ymd << 12);
}

void rdma_set_size(struct vioc_rdma_t *rdma, unsigned int width, unsigned int height)
{
	BITCSET(rdma->addr->uSIZE.nREG, 0xffffffff, (height << 16) | width);
}

void rdma_set_offset(struct vioc_rdma_t *rdma, unsigned int offset0, unsigned int offset1)
{
	BITCSET(rdma->addr->uOFFSET.nREG, 0xffffffff, (offset1 << 16) | offset0);
}

void rdma_set_address(struct vioc_rdma_t *rdma, addr_t base0, addr_t base1, addr_t base2)
{
	BITCSET(rdma->addr->nBASE0, 0xffffffff, base0);
	BITCSET(rdma->addr->nBASE1, 0xffffffff, base1);
	BITCSET(rdma->addr->nBASE2, 0xffffffff, base2);
}

int rdma_map_regs(struct vioc_rdma_t *rdma, struct test_data_reg_val_t *data)
{
	int idx, reg_start_offset;
	int *dat;
	VIOC_RDMA *reg;
	unsigned int auto_set = 0;	// select automatic setting about FMT, SIZE, BASE0/1/2, OFFS0/1

	/* read value of physical register */
	rdma->reg = *rdma->addr;

	reg = &rdma->reg;
	dat = data->reg;

	/* rdma's starting index is 2 (0:id, 1:plugin) */
	reg_start_offset = REG_START_OFFSET_RDMA;
	idx = reg_start_offset;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.INTL,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.BFMD,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.BFIELD,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.IEN,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.STRM,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.DM_3D,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.ASEL,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.UVI,	dat[idx]); idx++;
#if defined(__ARCH_TCC898X__)
	map_reg(reg->uCTRL.bREG.NUVIH,	dat[idx]); idx++;
#elif defined(__ARCH_TCC899X__)
	idx++;	//TODO: CTRL[22].NUVIH does not exist in TCC899X
#else
	#error "ERROR: Not defined ARCH in configure"
#endif
	map_reg(reg->uCTRL.bREG.DIT,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.DITS,	dat[idx]); idx++;

	map_reg(reg->uCTRL.bREG.R2YMD,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.R2Y,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_R2Y, AUTO_DMA_R2Y);}
	idx++;

	map_reg(reg->uCTRL.bREG.UPD,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.PD,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.SWAP,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.AEN,	dat[idx]); idx++;

	map_reg(reg->uCTRL.bREG.Y2RMD,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.Y2R,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_Y2R, AUTO_DMA_Y2R);}
	idx++;

	map_reg(reg->uCTRL.bREG.BR,		dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.R2YM2,	dat[idx]); idx++;
	map_reg(reg->uCTRL.bREG.Y2RM2,	dat[idx]); idx++;

	map_reg(reg->uCTRL.bREG.FMT,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_FMT, AUTO_DMA_FMT);}
	idx++;

	/* PTS */
	map_reg(reg->uPTS.bREG.BOT, dat[idx]); idx++;
	map_reg(reg->uPTS.bREG.TOP, dat[idx]); idx++;

	/* SIZE */
	map_reg(reg->uSIZE.bREG.HEIGHT,	dat[idx]); idx++;
	map_reg(reg->uSIZE.bREG.WIDTH,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_SIZE, AUTO_DMA_SIZE);}
	idx++;

	/* BASE0 */
	map_reg(reg->nBASE0, dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_BASE, AUTO_DMA_BASE);}
	idx++;

	/* CADDR (RO) */
	map_reg(reg->nCBASE, dat[idx]); idx++;

	/* BASE1 */
	map_reg(reg->nBASE1, dat[idx]); idx++;

	/* BASE2 */
	map_reg(reg->nBASE2, dat[idx]); idx++;

	/* OFFS */
	map_reg(reg->uOFFSET.bREG.OFFSET1,	dat[idx]); idx++;
	map_reg(reg->uOFFSET.bREG.OFFSET0,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_OFFS, AUTO_DMA_OFFS);}
	idx++;

	/* MISC */
	map_reg(reg->uMISC.bREG.ISSUE,	dat[idx]); idx++;
	map_reg(reg->uMISC.bREG.LPEN,	dat[idx]); idx++;

	map_reg(reg->uMISC.bREG.R2YMD,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_R2YMD, AUTO_DMA_R2YMD);}
	idx++;
	map_reg(reg->uMISC.bREG.Y2RMD,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_Y2RMD, AUTO_DMA_Y2RMD);}
	idx++;

	map_reg(reg->uMISC.bREG.FMT10,	dat[idx]);
	if (dat[idx] == -1) {BITCSET(auto_set, AUTO_DMA_FMT10, AUTO_DMA_FMT10);}
	idx++;
	map_reg(reg->uMISC.bREG.FMT,	dat[idx]);
	if (dat[idx] == -1 && ISSET(auto_set, AUTO_DMA_FMT)) {
		BITCSET(auto_set, AUTO_DMA_FMT, AUTO_DMA_FMT);
	}
	idx++;

	/* ALPHA */
	map_reg(reg->uALPHA.bREG.ALPHA1, dat[idx]); idx++;
	map_reg(reg->uALPHA.bREG.ALPHA0, dat[idx]); idx++;

	/* STAT */
	map_reg(reg->uSTATUS.bREG.STS_FDLY,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.STS_BFIELD,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.STS_DEVEOF,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.STS_EOFWAIT,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.STS_BOTRDY,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.STS_TOPRDY,	dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.BOTRDY,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.TOPRDY,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFWAITR,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.UPDDONE,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFFALL,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.EOFRISE,		dat[idx]); idx++;
	map_reg(reg->uSTATUS.bREG.CFGDONE,		dat[idx]); idx++;

	/* IRQMSK */
	map_reg(reg->uIRQMSK.bREG.BOTRDY,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.TOPRDY,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.EOFWAITR,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.UPDDONE,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.DEOFF,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.DEOFR,	dat[idx]); idx++;
	map_reg(reg->uIRQMSK.bREG.CFGDONE,	dat[idx]); idx++;

	/* SBASE0 */
	map_reg(reg->nSBASE0, dat[idx]); idx++;

	/* RBASE0 */
	map_reg(reg->nRBASE0, dat[idx]); idx++;

	/* RBASE1 */
	map_reg(reg->nRBASE1, dat[idx]); idx++;

	/* RBASE2 */
	map_reg(reg->nRBASE2, dat[idx]); idx++;

	/* CROP_SIZE */
	map_reg(reg->uCROPSIZE.bREG.HEIGHT,	dat[idx]); idx++;
	map_reg(reg->uCROPSIZE.bREG.WIDTH,	dat[idx]); idx++;

	/* CROP_POS */
	map_reg(reg->uCROPPOS.bREG.YPOS,	dat[idx]); idx++;
	map_reg(reg->uCROPPOS.bREG.XPOS,	dat[idx]); idx++;

	rdma->auto_set = auto_set;

	return (idx - reg_start_offset);
}

int rdma_setup(struct vioc_rdma_t *rdma)
{
	int ret = 0;

	/* check EN (enable-bit) is 0 */
	if (rdma->reg.uCTRL.bREG.IEN) {
		rdma->reg.uCTRL.bREG.IEN = 0;
		DBG(DL_VIOC, "rdma%d IEN was set, so disable it\n", rdma->info.id);
	}

	/* check UPD (update-bit) is 0 */
	if (rdma->reg.uCTRL.bREG.UPD) {
		rdma->reg.uCTRL.bREG.UPD = 0;
		DBG(DL_VIOC, "rdma%d UPD was set, so disable it\n", rdma->info.id);
	}

	/* set physical register */
	*rdma->addr = rdma->reg;

	return ret;
}

int rdma_verify_regs(struct vioc_rdma_t *rdma)
{
	int ret = 0;
	VIOC_RDMA *s, *d;
	reg_t sv, dv;

	s = &rdma->reg;
	d = rdma->addr;

	DBG(DL_VIOC, "VERIFY RDMA%d\n", rdma->info.id);
	if (rdma->info.id < 0) {
		printf("\tN/A\n");
		return ret;
	}

	/* CTRL */
	dv = read_reg(&d->uCTRL);
	sv = read_reg(&s->uCTRL);
	if (dv == sv) {
		printf("\tRDMA.uCTRL: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uCTRL: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* PTS */
	dv = read_reg(&d->uPTS);
	sv = read_reg(&s->uPTS);
	if (dv == sv) {
		printf("\tRDMA.uPTS: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uPTS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* SIZE */
	dv = read_reg(&d->uSIZE);
	sv = read_reg(&s->uSIZE);
	if (dv == sv) {
		printf("\tRDMA.uSIZE: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uSIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BASE0 */
	dv = read_reg(&d->nBASE0);
	sv = read_reg(&s->nBASE0);
	if (dv == sv) {
		printf("\tRDMA.nBASE0: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nBASE0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CADDR (RO) */
	dv = read_reg(&d->nCBASE);
	sv = read_reg(&s->nCBASE);
	if (dv == sv) {
		printf("\tRDMA.nCBASE: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nCBASE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BASE1 */
	dv = read_reg(&d->nBASE1);
	sv = read_reg(&s->nBASE1);
	if (dv == sv) {
		printf("\tRDMA.nBASE1: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nBASE1: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* BASE2 */
	dv = read_reg(&d->nBASE2);
	sv = read_reg(&s->nBASE2);
	if (dv == sv) {
		printf("\tRDMA.nBASE2: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nBASE2: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* OFFS */
	dv = read_reg(&d->uOFFSET);
	sv = read_reg(&s->uOFFSET);
	if (dv == sv) {
		printf("\tRDMA.uOFFSET: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uOFFSET: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* MISC */
	dv = read_reg(&d->uMISC);
	sv = read_reg(&s->uMISC);
	if (dv == sv) {
		printf("\tRDMA.uMISC: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uMISC: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* ALPHA */
	dv = read_reg(&d->uALPHA);
	sv = read_reg(&s->uALPHA);
	if (dv == sv) {
		printf("\tRDMA.uALPHA: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uALPHA: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* STAT */
	dv = read_reg(&d->uSTATUS);
	sv = read_reg(&s->uSTATUS);
	if (dv == sv) {
		printf("\tRDMA.uSTATUS: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uSTATUS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* IRQMSK */
	dv = read_reg(&d->uIRQMSK);
	sv = read_reg(&s->uIRQMSK);
	if (dv == sv) {
		printf("\tRDMA.uIRQMSK: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uIRQMSK: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* SBASE0 */
	dv = read_reg(&d->nSBASE0);
	sv = read_reg(&s->nSBASE0);
	if (dv == sv) {
		printf("\tRDMA.nSBASE0: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nSBASE0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* RBASE0 */
	dv = read_reg(&d->nRBASE0);
	sv = read_reg(&s->nRBASE0);
	if (dv == sv) {
		printf("\tRDMA.nRBASE0: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nRBASE0: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* RBASE1 */
	dv = read_reg(&d->nRBASE1);
	sv = read_reg(&s->nRBASE1);
	if (dv == sv) {
		printf("\tRDMA.nRBASE1: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nRBASE1: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* RBASE2 */
	dv = read_reg(&d->nRBASE2);
	sv = read_reg(&s->nRBASE2);
	if (dv == sv) {
		printf("\tRDMA.nRBASE2: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.nRBASE2: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CROP_SIZE */
	dv = read_reg(&d->uCROPSIZE);
	sv = read_reg(&s->uCROPSIZE);
	if (dv == sv) {
		printf("\tRDMA.uCROPSIZE: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uCROPSIZE: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	/* CROP_POS */
	dv = read_reg(&d->uCROPPOS);
	sv = read_reg(&s->uCROPPOS);
	if (dv == sv) {
		printf("\tRDMA.uCROPPOS: 0x%08x\n", sv);
	} else {
		printf("\tRDMA.uCROPPOS: 0x%08x != 0x%08x\n", sv, dv);
		ret = -1;
	}

	return ret;
}

int prev_disp_rdma_id = -2;
VIOC_RDMA prev_disp_rdma_regs;
int disp_rdma_clear_regs(struct test_case_t *tc)
{
	int ret = 0;

	DBG(DL_VIOC, "\n");

	if (tc->disp_rdma.info.id != -1) {
		return ret;
	}

	/*
	 * If first setting disp_rdma after starting app
	 */
	if (-2 == prev_disp_rdma_id) {
		/* disable & reset */
		rdma_en_ctrl(&tc->disp_rdma, 0);
		rdma_set_size(&tc->disp_rdma, 0, 0);
		ret += reset_rdma_ctrl(tc, VC_DISP_RDMA, 1);
		ret += reset_rdma_ctrl(tc, VC_DISP_RDMA, 0);

		/* backup disp_rdma regs */
		prev_disp_rdma_id = tc->disp_rdma.info.id;
		prev_disp_rdma_regs = *tc->disp_rdma.addr;		// backup regs after resetting

		DBG(DL_VIOC, "first disp_rdma(%d)\n",prev_disp_rdma_id );
		return ret;
	}

	/*
	 * If the currrnt disp_rdma is equal to the previous disp_rdma
	 */
	if (prev_disp_rdma_id == tc->disp_rdma.info.id) {
		rdma_en_ctrl(&tc->disp_rdma, 0);
		*tc->disp_rdma.addr = prev_disp_rdma_regs;		// restore regs

		DBG(DL_VIOC, "restore disp_rdma(%d)\n",prev_disp_rdma_id );
		return ret;
	}

	/*
	 * If new disp_rdma
	 */
	if (prev_disp_rdma_id != tc->disp_rdma.info.id) {
		/* disable & reset */
		rdma_en_ctrl(&tc->disp_rdma, 0);
		rdma_set_size(&tc->disp_rdma, 0, 0);
		ret += reset_rdma_ctrl(tc, VC_DISP_RDMA, 1);
		ret += reset_rdma_ctrl(tc, VC_DISP_RDMA, 0);

		/* backup disp_rdma regs */
		prev_disp_rdma_id = tc->disp_rdma.info.id;
		prev_disp_rdma_regs = *tc->disp_rdma.addr;		// backup regs after resetting

		DBG(DL_VIOC, "new disp_rdma(%d)\n",prev_disp_rdma_id );
		return ret;
	}

	return ret;
}