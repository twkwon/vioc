#include <stdio.h>

#include <vioc.h>


int rdma_map_regs(struct vioc_rdma_t *rdma, struct test_data_reg_val_t *data)
{
	int idx;
	int *dat;
	VIOC_RDMA *reg;

	/* read value of physical register */
	rdma->reg = *rdma->addr;

	reg = &rdma->reg;
	dat = data->reg;

	/* rdma's starting index is 2 (0:id, 1:plugin) */
	idx = 2;

	/* CTRL */
	map_reg(reg->uCTRL.bREG.INTL,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.BFMD,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.BFIELD,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.IEN,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.STRM,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.DM_3D,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.ASEL,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.UVI,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.NUVIH,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.DIT,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.DITS,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.R2YMD,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.R2Y,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.UPD,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.PD,		dat[idx++]);
	map_reg(reg->uCTRL.bREG.SWAP,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.AEN,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.Y2RMD,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.Y2R,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.BR,		dat[idx++]);
	map_reg(reg->uCTRL.bREG.R2YM2,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.Y2RM2,	dat[idx++]);
	map_reg(reg->uCTRL.bREG.FMT,	dat[idx++]);

	/* PTS */
	map_reg(reg->uPTS.bREG.BOT, dat[idx++]);
	map_reg(reg->uPTS.bREG.TOP, dat[idx++]);

	/* SIZE */
	map_reg(reg->uSIZE.bREG.HEIGHT,	dat[idx++]);
	map_reg(reg->uSIZE.bREG.WIDTH,	dat[idx++]);

	/* BASE0 */
	map_reg(reg->nBASE0, dat[idx++]);

	/* CADDR (RO) */
	map_reg(reg->nCBASE, dat[idx++]);

	/* BASE1 */
	map_reg(reg->nBASE1, dat[idx++]);

	/* BASE2 */
	map_reg(reg->nBASE2, dat[idx++]);

	/* OFFS */
	map_reg(reg->uOFFSET.bREG.OFFSET1,	dat[idx++]);
	map_reg(reg->uOFFSET.bREG.OFFSET0,	dat[idx++]);

	/* MISC */
	map_reg(reg->uMISC.bREG.ISSUE,	dat[idx++]);
	map_reg(reg->uMISC.bREG.LPEN,	dat[idx++]);
	map_reg(reg->uMISC.bREG.R2YMD,	dat[idx++]);
	map_reg(reg->uMISC.bREG.Y2RMD,	dat[idx++]);
	map_reg(reg->uMISC.bREG.FMT10, 	dat[idx++]);
	map_reg(reg->uMISC.bREG.FMT,	dat[idx++]);

	/* ALPHA */
	map_reg(reg->uALPHA.bREG.ALPHA1, dat[idx++]);
	map_reg(reg->uALPHA.bREG.ALPHA0, dat[idx++]);

	/* STAT */
	map_reg(reg->uSTATUS.bREG.STS_FDLY,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.STS_BFIELD,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.STS_DEVEOF,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.STS_EOFWAIT,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.STS_BOTRDY,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.STS_TOPRDY,	dat[idx++]);
	map_reg(reg->uSTATUS.bREG.BOTRDY,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.TOPRDY,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFWAITR,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.UPDDONE,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFFALL,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.EOFRISE,		dat[idx++]);
	map_reg(reg->uSTATUS.bREG.CFGDONE,		dat[idx++]);

	/* IRQMSK */
	map_reg(reg->uIRQMSK.bREG.BOTRDY,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.TOPRDY,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.EOFWAITR,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.UPDDONE,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.DEOFF,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.DEOFR,	dat[idx++]);
	map_reg(reg->uIRQMSK.bREG.CFGDONE,	dat[idx++]);

	/* SBASE0 */
	map_reg(reg->nSBASE0, dat[idx++]);

	/* RBASE0 */
	map_reg(reg->nRBASE0, dat[idx++]);

	/* RBASE1 */
	map_reg(reg->nRBASE1, dat[idx++]);

	/* RBASE2 */
	map_reg(reg->nRBASE2, dat[idx++]);

	/* CROP_SIZE */
	map_reg(reg->uCROPSIZE.bREG.HEIGHT,	dat[idx++]);
	map_reg(reg->uCROPSIZE.bREG.WIDTH,	dat[idx++]);

	/* CROP_POS */
	map_reg(reg->uCROPPOS.bREG.YPOS,	dat[idx++]);
	map_reg(reg->uCROPPOS.bREG.XPOS,	dat[idx++]);

	return (idx - 1 - 2);
}

int rdma_setup(struct vioc_rdma_t *rdma)
{
	int ret = 0;

	/* check UPD (update-bit) is 0 */
	if (rdma->reg.uCTRL.bREG.UPD) {
		rdma->reg.uCTRL.bREG.UPD = 0;
		printf("RDMA%d UPD was set, so disable it\n", rdma->info.id);
	}

	/* set physical register */
	*rdma->addr = rdma->reg;

	return ret;
}
