#include <stdio.h>

#include <debug.h>
#include <vioc.h>

// CFG_PATH_EDR
#define V_CONFIG_PATH_EDR_SHIFT	(8)
#define V_CONFIG_PATH_EDR_MASK	(0x1 << V_CONFIG_PATH_EDR_SHIFT)

// MISC0
#define V_CONFIG_MISC0_MIX50_SHIFT	(26)
#define V_CONFIG_MISC0_MIX30_SHIFT	(22)
#define V_CONFIG_MISC0_MIX13_SHIFT	(19)
#define V_CONFIG_MISC0_MIX10_SHIFT	(18)
#define V_CONFIG_MISC0_MIX03_SHIFT	(17)
#define V_CONFIG_MISC0_MIX00_SHIFT	(16)

/* enable bit */
#define V_CONFIG_EN_SHIFT	(31)
#define V_CONFIG_EN_MASK	(0x1 << V_CONFIG_EN_SHIFT)

/* status */
#define V_CONFIG_STS_SHIFT	(16)
#define V_CONFIG_STS_MASK	(0x3 << V_CONFIG_STS_SHIFT)

/* path select */
#define V_CONFIG_PATH_SHIFT	(0)
#define V_CONFIG_PATH_MASK	(0xFF << V_CONFIG_PATH_SHIFT)

#define VIOC_CFG_MISC1		(0x0084)

/* VIOC PATH STATUS TYPE */
#define VIOC_PATH_DISCONNECTED	(0)
#define VIOC_PATH_CONNECTING  	(1)
#define VIOC_PATH_CONNECTED   	(2)
#define VIOC_PATH_DISCONNECTING	(3)

static int plugin_rdma(struct test_case_t *tc, enum vioc_components);
static int plugin_sc(struct test_case_t *, enum vioc_components);
static int plugin_lut(struct test_case_t *, enum vioc_components);
static int plugin_vin(struct test_case_t *, enum vioc_components);
static int reset_wdma_ctrl(struct test_case_t *, enum vioc_components, unsigned int);
static int reset_wmix_ctrl(struct test_case_t *, enum vioc_components, unsigned int);
static int reset_rdma_ctrl(struct test_case_t *, enum vioc_components, unsigned int);
static int reset_vin_ctrl(struct test_case_t *, enum vioc_components, unsigned int);
static int reset_sc_ctrl(struct test_case_t *, enum vioc_components, unsigned int);
static int reset_lut_ctrl(struct test_case_t *, enum vioc_components, unsigned int);


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

	DBG(DL_VIOC, "VERIFY CONFIG%d\n", config->info.id);
	//if (config->info.id < 0) {
	//	printf("\tN/A\n");
	//	return ret;
	//}

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

int config_config(struct test_case_t *tc)
{
	int ret = 0;
	reg_t val, reg;
	VIOC_IREQ_CONFIG *config_reg;	// physical register
	VIOC_IREQ_CONFIG *config_val;	// test data

	config_reg = tc->config.addr;
	config_val = &tc->config.reg;

	/* CFG_PATH_EDR */
	val = read_reg(&config_reg->uPATH_EDR);
	BITCSET(val, V_CONFIG_PATH_EDR_MASK, config_val->uPATH_EDR.bREG.EDR_S << V_CONFIG_PATH_EDR_SHIFT);
	write_reg(&config_reg->uPATH_EDR, val);

	reg = read_reg(&config_reg->uPATH_EDR);
	if (val != reg) {
		DBG_ERR("CFG_PATH_EDR(0x%08x) != val(0x%08x)\n", reg, val);
		ret = -1;
		goto exit;
	}

	/* CFG_MISC0 */
	val = read_reg(&config_reg->uMISC0);
	BITCSET(val, 0x1 << V_CONFIG_MISC0_MIX50_SHIFT, config_val->uMISC0.bREG.MIX50 << V_CONFIG_MISC0_MIX50_SHIFT);
	BITCSET(val, 0x1 << V_CONFIG_MISC0_MIX30_SHIFT, config_val->uMISC0.bREG.MIX30 << V_CONFIG_MISC0_MIX30_SHIFT);
	BITCSET(val, 0x1 << V_CONFIG_MISC0_MIX13_SHIFT, config_val->uMISC0.bREG.MIX13 << V_CONFIG_MISC0_MIX13_SHIFT);
	BITCSET(val, 0x1 << V_CONFIG_MISC0_MIX10_SHIFT, config_val->uMISC0.bREG.MIX10 << V_CONFIG_MISC0_MIX10_SHIFT);
	BITCSET(val, 0x1 << V_CONFIG_MISC0_MIX03_SHIFT, config_val->uMISC0.bREG.MIX03 << V_CONFIG_MISC0_MIX03_SHIFT);
	BITCSET(val, 0x1 << V_CONFIG_MISC0_MIX00_SHIFT, config_val->uMISC0.bREG.MIX00 << V_CONFIG_MISC0_MIX00_SHIFT);
	write_reg(&config_reg->uMISC0, val);

	reg = read_reg(&config_reg->uMISC0);
	if (val != reg) {
		DBG_ERR("CFG_MISC0(0x%08x) != val(0x%08x)\n", reg, val);
		ret = -1;
		goto exit;
	}

exit:
	return ret;
}

int config_plugin(struct test_case_t *tc, enum vioc_components comp)
{
	int ret = 0;

	switch (comp) {
	case VC_DISP_RDMA:
	case VC_RDMA_1st:
	case VC_RDMA_2nd:
	case VC_RDMA_3rd:
	case VC_RDMA_4th:
		ret = plugin_rdma(tc, comp);
		if (ret) {goto err;}
		break;
	case VC_SC:
		ret = plugin_sc(tc, comp);
		if (ret) {goto err;}
		break;
	case VC_LUT:
		ret = plugin_lut(tc, comp);
		if (ret) {goto err;}
		break;
	case VC_VIN:
		ret = plugin_vin(tc, comp);
		if (ret) {goto err;}
		break;
	default:
		ret = -1;
		goto err;
	}

err:
	return ret;
}

static int plugin_rdma(struct test_case_t *tc, enum vioc_components comp)
{
	int ret = 0;
	unsigned int loop;
	volatile VIOC_CONFIG_PATH_u *cfg_path_rdma;
	struct vioc_rdma_t *rdma;

	switch (comp) {
	case VC_DISP_RDMA:
		rdma = &tc->disp_rdma;
		break;
	case VC_RDMA_1st:
		rdma = &tc->rdma1;
		break;
	case VC_RDMA_2nd:
		rdma = &tc->rdma2;
		break;
	case VC_RDMA_3rd:
		rdma = &tc->rdma3;
		break;
	case VC_RDMA_4th:
		rdma = &tc->rdma4;
		break;
	default:
		ret = -1;
		DBG_ERR("component id(%d) is not RDMA\n", comp);
		goto err_comp;
	}

	switch (rdma->info.id) {
	case 0:
	case 1:
	case 5:
	case 8:
	case 9:
	case 10:
		DBG(DL_VIOC, "RDMA%d is GRDMA, so it doesn't need plug-in\n", rdma->info.id);
		ret = 0;
		goto err_comp;
		break;

	case 2:
		cfg_path_rdma = &tc->config.addr->uRDMA02;
		break;
	case 3:
		cfg_path_rdma = &tc->config.addr->uRDMA03;
		break;
	case 6:
		cfg_path_rdma = &tc->config.addr->uRDMA06;
		break;
	case 7:
		cfg_path_rdma = &tc->config.addr->uRDMA07;
		break;
	case 11:
		cfg_path_rdma = &tc->config.addr->uRDMA11;
		break;
	case 12:
		cfg_path_rdma = &tc->config.addr->uRDMA12;
		break;
	case 13:
		cfg_path_rdma = &tc->config.addr->uRDMA13;
		break;
	case 14:
		cfg_path_rdma = &tc->config.addr->uRDMA14;
		break;
	case 16:
		cfg_path_rdma = &tc->config.addr->uRDMA16;
		break;
	case 17:
		cfg_path_rdma = &tc->config.addr->uRDMA17;
		break;
	default:
		ret = -1;
		goto err_rdma_id;
	}

	BITCSET(cfg_path_rdma->nREG, V_CONFIG_PATH_MASK, rdma->info.plugin << V_CONFIG_PATH_SHIFT);
	BITCSET(cfg_path_rdma->nREG, V_CONFIG_EN_MASK, 1 << V_CONFIG_EN_SHIFT);

	loop = 0xf000;
	while(loop--) {
		if((read_reg(cfg_path_rdma) & V_CONFIG_STS_MASK) != VIOC_PATH_DISCONNECTED)
			break;
	}

	DBG(DL_VIOC, "plug-in RDMA%d -> SEL(0x%x)\n", rdma->info.id, rdma->info.plugin);
	return ret;

err_rdma_id:
	DBG_ERR("plug-in RDMA%d\n", rdma->info.id);
err_comp:
	return ret;
}

static int plugin_sc(struct test_case_t *tc, enum vioc_components comp)
{
	int ret = 0;
	unsigned int loop;
	volatile VIOC_CONFIG_PATH_u *cfg_path_sc;
	struct vioc_sc_t *sc;

	sc = &tc->sc;

	switch (tc->sc.info.id) {
	case 0:
		cfg_path_sc = &tc->config.addr->uSC0;
		break;
	case 1:
		cfg_path_sc = &tc->config.addr->uSC1;
		break;
	case 2:
		cfg_path_sc = &tc->config.addr->uSC2;
		break;
	case 3:
		cfg_path_sc = &tc->config.addr->uSC3;
		break;
	case 4:
		cfg_path_sc = &tc->config.addr->uSC4;
		break;
	default:
		ret = -1;
		goto err;
	}

	/* disable sc that has same plugin number */
	if (tc->config.addr->uSC0.bREG.SELECT == sc->info.plugin) {
		BITCSET(tc->config.addr->uSC0.nREG, V_CONFIG_EN_MASK, 0x0 << V_CONFIG_EN_SHIFT);
	}
	if (tc->config.addr->uSC1.bREG.SELECT == sc->info.plugin) {
		BITCSET(tc->config.addr->uSC1.nREG, V_CONFIG_EN_MASK, 0x0 << V_CONFIG_EN_SHIFT);
	}
	if (tc->config.addr->uSC2.bREG.SELECT == sc->info.plugin) {
		BITCSET(tc->config.addr->uSC2.nREG, V_CONFIG_EN_MASK, 0x0 << V_CONFIG_EN_SHIFT);
	}
	if (tc->config.addr->uSC3.bREG.SELECT == sc->info.plugin) {
		BITCSET(tc->config.addr->uSC3.nREG, V_CONFIG_EN_MASK, 0x0 << V_CONFIG_EN_SHIFT);
	}
	if (tc->config.addr->uSC4.bREG.SELECT == sc->info.plugin) {
		BITCSET(tc->config.addr->uSC4.nREG, V_CONFIG_EN_MASK, 0x0 << V_CONFIG_EN_SHIFT);
	}

	/* disable */
	BITCSET(cfg_path_sc->nREG, V_CONFIG_EN_MASK, 0x0 << V_CONFIG_EN_SHIFT);
	/* set plug-in */
	BITCSET(cfg_path_sc->nREG, V_CONFIG_PATH_MASK, sc->info.plugin << V_CONFIG_PATH_SHIFT);
	/* enable */
	BITCSET(cfg_path_sc->nREG, V_CONFIG_EN_MASK, 0x1 << V_CONFIG_EN_SHIFT);

	loop = 0xf000;
	while(loop--) {
		if((read_reg(cfg_path_sc) & V_CONFIG_STS_MASK) != VIOC_PATH_DISCONNECTED)
			break;
	}

	DBG(DL_VIOC, "plug-in SC%d -> SEL(0x%x)\n", sc->info.id, sc->info.plugin);
	return ret;
err:
	DBG_ERR("plug-in SC%d\n", sc->info.id);
	return ret;
}

static int plugin_lut(struct test_case_t *tc, enum vioc_components comp)
{
	int ret = 0;

	//TODO:LUT
	DBG_WARN(DL_WARN, "TODO: this is empty funtion!!!\n");

	return ret;
}

static int plugin_vin(struct test_case_t *tc, enum vioc_components comp)
{
	//TODO:VIN plug-in instead of RDMA or CFG_VIN_SEL ???

	int ret = 0;
	struct vioc_vin_t *vin;

	vin = &tc->vin;


	DBG(DL_VIOC, "plug-in VIN%d -> SEL(0x%x)\n", vin->info.id, vin->info.plugin);
	return ret;
//err:
	DBG_ERR("plug-in VIN%d\n", vin->info.id);
	return ret;
}

int config_plugout_sc(struct test_case_t *tc, unsigned int id)
{
	int ret = 0;
	volatile VIOC_CONFIG_PATH_u *cfg_path_sc;

	switch (id) {
	case 0:
		cfg_path_sc = &tc->config.addr->uSC0;
		break;
	case 1:
		cfg_path_sc = &tc->config.addr->uSC1;
		break;
	case 2:
		cfg_path_sc = &tc->config.addr->uSC2;
		break;
	case 3:
		cfg_path_sc = &tc->config.addr->uSC3;
		break;
	case 4:
		cfg_path_sc = &tc->config.addr->uSC4;
		break;
	default:
		ret = -1;
		goto err;
	}

	/* disable */
	BITCSET(cfg_path_sc->nREG, V_CONFIG_EN_MASK, 0x0 << V_CONFIG_EN_SHIFT);

	DBG(DL_VIOC, "plug-out SC%d\n", id);
	return ret;
err:
	DBG(DL_VIOC, "error: plug-out SC%d\n", id);
	return ret;
}

int config_reset(struct test_case_t *tc)
{
	int ret = 0;
	unsigned int reset_status;

	/*
	 * reset status
	 * - wdma -> wmix -> rdma/vin -> sc/lut
	 * - disp_rdma
	 */
	reset_status = 1;

	if (tc->wdma1.info.id != -1) {
		ret += reset_wdma_ctrl(tc, VC_WDMA_1st, reset_status);
	}
	if (tc->wdma2.info.id != -1) {
		ret += reset_wdma_ctrl(tc, VC_WDMA_2nd, reset_status);
	}

	if (tc->wmix.info.id != -1) {
		ret += reset_wmix_ctrl(tc, VC_WMIX, reset_status);
	}

	if (tc->rdma1.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_1st, reset_status);
	}
	if (tc->rdma2.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_2nd, reset_status);
	}
	if (tc->rdma3.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_3rd, reset_status);
	}
	if (tc->rdma4.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_4th, reset_status);
	}

	if (tc->vin.info.id != -1) {
		ret += reset_vin_ctrl(tc, VC_VIN, reset_status);
	}

	if (tc->sc.info.id != -1) {
		ret += reset_sc_ctrl(tc, VC_SC, reset_status);
	}

	if (tc->lut.info.id != -1) {
		ret += reset_lut_ctrl(tc, VC_LUT, reset_status);
	}

	if (tc->disp_rdma.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_DISP_RDMA, reset_status);
	}

	/*
	 * normal status
	 * - rdma/vin -> wmix -> sc/lut -> wdma
	 * - disp_rdma
	 */
	reset_status = 0;

	if (tc->rdma1.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_1st, reset_status);
	}
	if (tc->rdma2.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_2nd, reset_status);
	}
	if (tc->rdma3.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_3rd, reset_status);
	}
	if (tc->rdma4.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_RDMA_4th, reset_status);
	}

	if (tc->vin.info.id != -1) {
		ret += reset_vin_ctrl(tc, VC_VIN, reset_status);
	}

	if (tc->wmix.info.id != -1) {
		ret += reset_wmix_ctrl(tc, VC_WMIX, reset_status);
	}

	if (tc->sc.info.id != -1) {
		ret += reset_sc_ctrl(tc, VC_SC, reset_status);
	}

	if (tc->lut.info.id != -1) {
		ret += reset_lut_ctrl(tc, VC_LUT, reset_status);
	}

	if (tc->wdma1.info.id != -1) {
		ret += reset_wdma_ctrl(tc, VC_WDMA_1st, reset_status);
	}
	if (tc->wdma2.info.id != -1) {
		ret += reset_wdma_ctrl(tc, VC_WDMA_2nd, reset_status);
	}

	if (tc->disp_rdma.info.id != -1) {
		ret += reset_rdma_ctrl(tc, VC_DISP_RDMA, reset_status);
	}

	return ret;
}

static int reset_wdma_ctrl(struct test_case_t *tc, enum vioc_components comp, unsigned int reset)
{
	int ret = 0;
	volatile VIOC_POWER_BLOCKS_u *cfg_reset;
	struct vioc_wdma_t *wdma;

	switch (comp) {
	case VC_WDMA_1st:
		wdma = &tc->wdma1;
		break;
	case VC_WDMA_2nd:
		wdma = &tc->wdma2;
		break;
	default:
		ret = -1;
		DBG_ERR("component id(%d) is not WDMA\n", comp);
		return ret;
	}

	cfg_reset = &tc->config.addr->uSOFTRESET;

	BITCSET(cfg_reset->nREG[1], 0x1 << (0 + wdma->info.id), reset << (0 + wdma->info.id));

	DBG(DL_VIOC, "WDMA%d.reset = %s\n", wdma->info.id, reset ? "reset" : "normal");
	return ret;
}

static int reset_wmix_ctrl(struct test_case_t *tc, enum vioc_components comp, unsigned int reset)
{
	int ret = 0;
	volatile VIOC_POWER_BLOCKS_u *cfg_reset;
	struct vioc_wmix_t *wmix;

	switch (comp) {
	case VC_WMIX:
		wmix = &tc->wmix;
		break;
	default:
		ret = -1;
		DBG_ERR("component id(%d) is not WMIX\n", comp);
		return ret;
	}

	cfg_reset = &tc->config.addr->uSOFTRESET;

	BITCSET(cfg_reset->nREG[1], 0x1 << (9 + wmix->info.id), reset << (9 + wmix->info.id));

	DBG(DL_VIOC, "WMIX%d.reset = %s\n", wmix->info.id, reset ? "reset" : "normal");
	return ret;
}

static int reset_rdma_ctrl(struct test_case_t *tc, enum vioc_components comp, unsigned int reset)
{
	int ret = 0;
	volatile VIOC_POWER_BLOCKS_u *cfg_reset;
	struct vioc_rdma_t *rdma;

	switch (comp) {
	case VC_DISP_RDMA:
		rdma = &tc->disp_rdma;
		break;
	case VC_RDMA_1st:
		rdma = &tc->rdma1;
		break;
	case VC_RDMA_2nd:
		rdma = &tc->rdma2;
		break;
	case VC_RDMA_3rd:
		rdma = &tc->rdma3;
		break;
	case VC_RDMA_4th:
		rdma = &tc->rdma4;
		break;
	default:
		ret = -1;
		DBG_ERR("component id(%d) is not RDMA\n", comp);
		return ret;
	}

	cfg_reset = &tc->config.addr->uSOFTRESET;

	BITCSET(cfg_reset->nREG[0], 0x1 << (0 + rdma->info.id), reset << (0 + rdma->info.id));

	DBG(DL_VIOC, "RDMA%d.reset = %s\n", rdma->info.id, reset ? "reset" : "normal");
	return ret;
}

static int reset_vin_ctrl(struct test_case_t *tc, enum vioc_components comp, unsigned int reset)
{
	int ret = 0;
	volatile VIOC_POWER_BLOCKS_u *cfg_reset;
	struct vioc_vin_t *vin;

	switch (comp) {
	case VC_VIN:
		vin = &tc->vin;
		break;
	default:
		ret = -1;
		DBG_ERR("component id(%d) is not VIN\n", comp);
		return ret;
	}

	cfg_reset = &tc->config.addr->uSOFTRESET;

	BITCSET(cfg_reset->nREG[0], 0x1 << (24 + vin->info.id), reset << (24 + vin->info.id));

	DBG(DL_VIOC, "VIN%d.reset = %s\n", vin->info.id, reset ? "reset" : "normal");
	return ret;
}

static int reset_sc_ctrl(struct test_case_t *tc, enum vioc_components comp, unsigned int reset)
{
	int ret = 0;
	volatile VIOC_POWER_BLOCKS_u *cfg_reset;
	struct vioc_sc_t *sc;

	switch (comp) {
	case VC_SC:
		sc = &tc->sc;
		break;
	default:
		ret = -1;
		DBG_ERR("component id(%d) is not SC\n", comp);
		return ret;
	}

	cfg_reset = &tc->config.addr->uSOFTRESET;

	BITCSET(cfg_reset->nREG[0], 0x1 << (28 + sc->info.id), reset << (28 + sc->info.id));

	DBG(DL_VIOC, "SC%d.reset = %s\n", sc->info.id, reset ? "reset" : "normal");
	return ret;
}

//TODO:LUT
static int reset_lut_ctrl(struct test_case_t *tc, enum vioc_components comp, unsigned int reset)
{
	int ret = 0;
	//volatile VIOC_POWER_BLOCKS_u *cfg_reset;
	//struct vioc_lut_t *lut;
	//
	//switch (comp) {
	//case VC_LUT:
	//	lut = &tc->lut;
	//	break;
	//default:
	//	ret = -1;
	//	DBG_ERR("component id(%d) is not LUT\n", comp);
	//	return ret;
	//}
	//
	//cfg_reset = &tc->config.addr->uSOFTRESET;
	//
	//BITCSET(cfg_reset->nREG[0], 0x1 << (28 + lut->info.id), reset << (28 + lut->info.id));
	//
	//DBG(DL_VIOC, "LUT%d.reset = %s\n", lut->info.id, reset ? "reset" : "normal");
	return ret;
}