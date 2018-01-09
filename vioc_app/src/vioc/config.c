#include <stdio.h>

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

	printf("VERIFY CONFIG%d\n", config->info.id);
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
		printf("[%s] error: CFG_PATH_EDR(0x%08x) != val(0x%08x)\n", __func__, reg, val);
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
		printf("[%s] error: CFG_MISC0(0x%08x) != val(0x%08x)\n", __func__, reg, val);
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
		printf("[%s] component id(%d) is not RDMA\n", __func__, comp);
		goto err_comp;
	}

	switch (rdma->info.id) {
	case 0:
	case 1:
	case 5:
	case 8:
	case 9:
	case 10:
		printf("[%s] RDMA%d is GRDMA, so it doesn't need plug-in\n", __func__, rdma->info.id);
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

	printf("[%s] plug-in RDMA%d -> SEL(0x%x)\n", __func__, rdma->info.id, rdma->info.plugin);
	return ret;

err_rdma_id:
	printf("[%s] error: plug-in RDMA%d\n", __func__, rdma->info.id);
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

	BITCSET(cfg_path_sc->nREG, V_CONFIG_PATH_MASK, sc->info.plugin << V_CONFIG_PATH_SHIFT);
	BITCSET(cfg_path_sc->nREG, V_CONFIG_EN_MASK, 1 << V_CONFIG_EN_SHIFT);

	loop = 0xf000;
	while(loop--) {
		if((read_reg(cfg_path_sc) & V_CONFIG_STS_MASK) != VIOC_PATH_DISCONNECTED)
			break;
	}

	printf("[%s] plug-in SC%d -> SEL(0x%x)\n", __func__, sc->info.id, sc->info.plugin);
	return ret;
err:
	printf("[%s] error: plug-in SC%d\n", __func__, sc->info.id);
	return ret;
}

static int plugin_lut(struct test_case_t *tc, enum vioc_components comp)
{
	int ret = 0;

	//TODO:
	printf("[%s] TODO: this is empty funtion!!!\n", __func__);

	return ret;
}

static int plugin_vin(struct test_case_t *tc, enum vioc_components comp)
{
	//TODO:VIN plug-in instead of RDMA or CFG_VIN_SEL ???

	int ret = 0;
	struct vioc_vin_t *vin;

	vin = &tc->vin;


	printf("[%s] plug-in VIN%d -> SEL(0x%x)\n", __func__, vin->info.id, vin->info.plugin);
	return ret;
//err:
	printf("[%s] error: plug-in VIN%d\n", __func__, vin->info.id);
	return ret;
}