#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>

static int vioc_get_component_info(struct test_case_t *, struct test_data_t *);
static int vioc_map_component_regs(struct test_case_t *, struct test_data_t *);
static int vioc_set_component_regs(struct test_case_t *);
static int vioc_config_path(struct test_case_t *);
static int vioc_verify_regs(struct test_case_t *);


const char *name_vioc_compoent[] = {
	/* enum vioc_components */
	[VC_RDMA_1st]	= "1st RDMA",
	[VC_RDMA_2nd]	= "2nd RDMA",
	[VC_RDMA_3rd]	= "3rd RDMA",
	[VC_RDMA_4th]	= "4th RDMA",
	[VC_WDMA_1st]	= "1st WDMA",
	[VC_WDMA_2nd]	= "2nd WDMA",
	[VC_WMIX]		= "WMIX",
	[VC_SC]			= "SC",
	[VC_LUT]		= "LUT",
	[VC_OUTCFG]		= "OUTCFG",
	[VC_CONFIG]		= "CONFIG",
};

/*
 * HOWTO set vioc register
 * -----------------------
 * 1. Read rdma registers using test_case_t->rdma.addr
 * 2. Store the rdma regs to test_case_t->rdma.reg
 * 3. Map test_data_t->rmda.reg onto test_case_t->rdma.reg
 *    - Only map the setting values and don't overwrite the Don't-Care values.
 * 4. Set rdma register using write_reg() 
 *    - write_reg(rdma->addr, test_case_t->rdma.reg.uCTRL.nREG)
 *    - write_reg(rdma->addr, test_case_t->rdma.reg.uSIZE.nREG)
 *    - ......
 */

int setup_test_case(struct test_case_t *test_case, struct test_data_t *test_data)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * get address each components
	 */
	ret = vioc_get_component_info(test_case, test_data);
	if (ret) {
		printf("[%s] error: vioc_get_component_info()\n", __func__);
		goto exit;
	}

	/*
	 * map test_data->[components].reg onto test_case->[components].reg
	 */
	ret = vioc_map_component_regs(test_case, test_data);
	if (ret) {
		printf("[%s] error: vioc_map_component_regs()\n", __func__);
		goto exit;
	}

exit:
	return ret;
}

int setup_vioc_path(struct test_case_t *test_case)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * set vioc registers
	 */
	ret = vioc_set_component_regs(test_case);
	if (ret) {
		printf("[%s] error: vioc_set_component_regs()\n", __func__);
		goto exit;
	}

	/*
	 * config vioc path
	 */
	ret = vioc_config_path(test_case);
	if (ret) {
		printf("[%s] error: vioc_config_path()\n", __func__);
		goto exit;
	}

	/* for debugging */
	ret = vioc_verify_regs(test_case);

exit:
	return ret;
}

int shoot_test(struct test_case_t *tc)
{
	int ret = 0;
	printf("\n\nSHOOTING !!!!!\n\n");

	/*
	 * bit order
	 *  1: set enable bit
	 *  2: set update bit
	 * component order (M2M path)
	 *  RDMA -> [SC] -> [LUT] -> WMIX -> [SC] -> [LUT]  -> WDMA
	 */
	if (tc->rdma1.info.id != -1) {
		BITCSET(tc->rdma1.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		BITCSET(tc->rdma1.addr->uCTRL.nREG, 1 << 16, 1 << 16);
	}
	if (tc->rdma2.info.id != -1) {
		BITCSET(tc->rdma2.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		BITCSET(tc->rdma2.addr->uCTRL.nREG, 1 << 16, 1 << 16);
	}
	if (tc->rdma3.info.id != -1) {
		BITCSET(tc->rdma3.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		BITCSET(tc->rdma3.addr->uCTRL.nREG, 1 << 16, 1 << 16);
	}
	if (tc->rdma4.info.id != -1) {
		BITCSET(tc->rdma4.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		BITCSET(tc->rdma4.addr->uCTRL.nREG, 1 << 16, 1 << 16);
	}

	if (tc->sc.info.id != -1) {
		if (tc->sc.info.plugin >= 0 && tc->sc.info.plugin <= 0x13) {
			//BITCSET(tc->sc.addr->uCTRL.nREG, 1 << 28, 1 << 28);
			BITCSET(tc->sc.addr->uCTRL.nREG, 1 << 16, 1 << 16);
		}
	}

	if (tc->lut.info.id != -1) {
		//if (tc->lut.info.plugin >= 0 && tc->lut.info.plugin <= 0x13) {
		//	BITCSET(tc->lut.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		//	BITCSET(tc->lut.addr->uCTRL.nREG, 1 << 16, 1 << 16);
		//}
	}

	if (tc->wmix.info.id != -1) {
		//BITCSET(tc->wmix.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		BITCSET(tc->wmix.addr->uCTRL.nREG, 1 << 16, 1 << 16);
	}

	if (tc->sc.info.id != -1) {
		if (tc->sc.info.plugin >= 0x14 && tc->sc.info.plugin <= 0x1C) {
			//BITCSET(tc->sc.addr->uCTRL.nREG, 1 << 28, 1 << 28);
			BITCSET(tc->sc.addr->uCTRL.nREG, 1 << 16, 1 << 16);
		}
	}

	if (tc->lut.info.id != -1) {
		//if (tc->lut.info.plugin >= 0 && tc->lut.info.plugin <= 0x13) {
		//	BITCSET(tc->lut.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		//	BITCSET(tc->lut.addr->uCTRL.nREG, 1 << 16, 1 << 16);
		//}
	}

	if (tc->wdma1.info.id != -1) {
		BITCSET(tc->wdma1.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		BITCSET(tc->wdma1.addr->uCTRL.nREG, 1 << 16, 1 << 16);
	}
	if (tc->wdma2.info.id != -1) {
		BITCSET(tc->wdma2.addr->uCTRL.nREG, 1 << 28, 1 << 28);
		BITCSET(tc->wdma2.addr->uCTRL.nREG, 1 << 16, 1 << 16);
	}

	return 0;
}

static int vioc_get_component_info(struct test_case_t *tc, struct test_data_t *td)
{
	int i, ret = 0;
	printf("[%s]\n", __func__);

	/* get infos (test no, test name, input/output files) */
	tc->test_no = td->test_no;
	memcpy(tc->test_name, td->test_name, sizeof(tc->test_name));
	memcpy(tc->input_file, td->input_file, sizeof(tc->input_file));
	memcpy(tc->output_file, td->output_file, sizeof(tc->output_file));

	/*
	 * RDMA - max 4 RDMAs
	 */
	tc->rdma1.info.id = td->rdma1.reg[0];
	tc->rdma1.info.plugin = td->rdma1.reg[1];
	tc->rdma1.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma1.info.id);
	tc->rdma1.addr = (VIOC_RDMA *)(tc->vioc_base_addr + INT32_OFFSET(tc->rdma1.info.addr_offset));

	tc->rdma2.info.id = td->rdma2.reg[0];
	tc->rdma2.info.plugin = td->rdma2.reg[1];
	tc->rdma2.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma2.info.id);
	tc->rdma2.addr = (VIOC_RDMA *)(tc->vioc_base_addr + INT32_OFFSET(tc->rdma2.info.addr_offset));

	tc->rdma3.info.id = td->rdma3.reg[0];
	tc->rdma3.info.plugin = td->rdma3.reg[1];
	tc->rdma3.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma3.info.id);
	tc->rdma3.addr = (VIOC_RDMA *)(tc->vioc_base_addr + INT32_OFFSET(tc->rdma3.info.addr_offset));

	tc->rdma4.info.id = td->rdma4.reg[0];
	tc->rdma4.info.plugin = td->rdma4.reg[1];
	tc->rdma4.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma4.info.id);
	tc->rdma4.addr = (VIOC_RDMA *)(tc->vioc_base_addr + INT32_OFFSET(tc->rdma4.info.addr_offset));

	/*
	 * WDMA - max 2 RDMAs
	 */
	tc->wdma1.info.id = td->wdma1.reg[0];
	tc->wdma1.info.plugin = td->wdma1.reg[1];
	tc->wdma1.info.addr_offset = OFFSET_WDMA_FROM_VIOC_BASE(tc->wdma1.info.id);
	tc->wdma1.addr = (VIOC_WDMA *)(tc->vioc_base_addr + INT32_OFFSET(tc->wdma1.info.addr_offset));

	tc->wdma2.info.id = td->wdma2.reg[0];
	tc->wdma2.info.plugin = td->wdma2.reg[1];
	tc->wdma2.info.addr_offset = OFFSET_WDMA_FROM_VIOC_BASE(tc->wdma2.info.id);
	tc->wdma2.addr = (VIOC_WDMA *)(tc->vioc_base_addr + INT32_OFFSET(tc->wdma2.info.addr_offset));

	/*
	 * WMIX
	 */
	tc->wmix.info.id = td->wmix.reg[0];
	tc->wmix.info.plugin = td->wmix.reg[1];
	tc->wmix.info.addr_offset = OFFSET_WMIX_FROM_VIOC_BASE(tc->wmix.info.id);
	tc->wmix.addr = (VIOC_WMIX *)(tc->vioc_base_addr + INT32_OFFSET(tc->wmix.info.addr_offset));

	/*
	 * SC
	 */
	tc->sc.info.id = td->sc.reg[0];
	tc->sc.info.plugin = td->sc.reg[1];
	tc->sc.info.addr_offset = OFFSET_SC_FROM_VIOC_BASE(tc->sc.info.id);
	tc->sc.addr = (VIOC_SC *)(tc->vioc_base_addr + INT32_OFFSET(tc->sc.info.addr_offset));

	/*
	 * LUT
	 */
	tc->lut.info.id = td->lut.reg[0];
	tc->lut.info.plugin = td->lut.reg[1];
	tc->lut.info.addr_offset = OFFSET_LUT_FROM_VIOC_BASE;
	tc->lut.addr = (VIOC_LUT *)(tc->vioc_base_addr + INT32_OFFSET(tc->lut.info.addr_offset));

	/*
	 * OUTCFG
	 */
	tc->outcfg.info.id = td->outcfg.reg[0];
	tc->outcfg.info.plugin = td->outcfg.reg[1];
	tc->outcfg.info.addr_offset = OFFSET_OUTCFG_FROM_VIOC_BASE;
	tc->outcfg.addr = (VIOC_OUTCFG *)(tc->vioc_base_addr + INT32_OFFSET(tc->outcfg.info.addr_offset));

	/*
	 * CONFIG
	 */
	tc->config.info.id = td->config.reg[0];
	tc->config.info.plugin = td->config.reg[1];
	tc->config.info.addr_offset = OFFSET_CONFIG_FROM_VIOC_BASE;
	tc->config.addr = (VIOC_IREQ_CONFIG *)(tc->vioc_base_addr + INT32_OFFSET(tc->config.info.addr_offset));

	return ret;
}

static int vioc_map_component_regs(struct test_case_t *tc, struct test_data_t *td)
{
	int ret = 0;
	int mapped = 0, nr_regs_data = 0;
	printf("[%s]\n", __func__);

	/*
	 * RDMA - max 4 RDMAs
	 */
	if (tc->rdma1.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma1, &td->rdma1);
		nr_regs_data = td->rdma1.nr_regs - REG_START_OFFSET_RDMA;
		tc->rdma1.info.nr_regs = mapped;

		printf("mapping: 1st RDMA%d %d register values[%d] %s\n", tc->rdma1.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	if (tc->rdma2.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma2, &td->rdma2);
		nr_regs_data = td->rdma2.nr_regs - REG_START_OFFSET_RDMA;
		tc->rdma2.info.nr_regs = mapped;

		printf("mapping: 2nd RDMA%d %d register values[%d] %s\n", tc->rdma2.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	if (tc->rdma3.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma3, &td->rdma3);
		nr_regs_data = td->rdma3.nr_regs - REG_START_OFFSET_RDMA;
		tc->rdma3.info.nr_regs = mapped;

		printf("mapping: 3rd RDMA%d %d register values[%d] %s\n", tc->rdma3.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	if (tc->rdma4.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma4, &td->rdma4);
		nr_regs_data = td->rdma4.nr_regs - REG_START_OFFSET_RDMA;
		tc->rdma4.info.nr_regs = mapped;
		ret += mapped - nr_regs_data;
		printf("mapping: 4th RDMA%d %d register values[%d] %s\n", tc->rdma4.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");
	}

	/*
	 * WDMA - max 2 WDMAs
	 */
	if (tc->wdma1.info.id != -1) {
		mapped = wdma_map_regs(&tc->wdma1, &td->wdma1);
		nr_regs_data = td->wdma1.nr_regs - REG_START_OFFSET_WDMA;
		tc->wdma1.info.nr_regs = mapped;

		printf("mapping: 1st WDMA%d %d register values[%d] %s\n", tc->wdma1.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	if (tc->wdma2.info.id != -1) {
		mapped = wdma_map_regs(&tc->wdma2, &td->wdma2);
		nr_regs_data = td->wdma2.nr_regs - REG_START_OFFSET_WDMA;
		tc->wdma2.info.nr_regs = mapped;

		printf("mapping: 2nd WDMA%d %d register values[%d] %s\n", tc->wdma2.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	/*
	 * WMIX
	 */
	if (tc->wmix.info.id != -1) {
		mapped = wmix_map_regs(&tc->wmix, &td->wmix);
		nr_regs_data = td->wmix.nr_regs - REG_START_OFFSET_WMIX;
		tc->wmix.info.nr_regs = mapped;

		printf("mapping: WMIX%d %d register values[%d] %s\n", tc->wmix.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	/*
	 * SC
	 */
	if (tc->sc.info.id != -1) {
		mapped = sc_map_regs(&tc->sc, &td->sc);
		nr_regs_data = td->sc.nr_regs - REG_START_OFFSET_SC;
		tc->sc.info.nr_regs = mapped;

		printf("mapping: SC%d %d register values[%d] %s\n", tc->sc.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	/*
	 * LUT
	 */
	if (tc->lut.info.id != -1) {
		mapped = lut_map_regs(&tc->lut, &td->lut);
		nr_regs_data = td->lut.nr_regs - REG_START_OFFSET_LUT;
		tc->lut.info.nr_regs = mapped;

		printf("mapping: LUT%d %d register values[%d] %s\n", tc->lut.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	/*
	 * OUTCFG
	 */
	if (tc->outcfg.info.id != -1) {
		mapped = outcfg_map_regs(&tc->outcfg, &td->outcfg);
		nr_regs_data = td->outcfg.nr_regs - REG_START_OFFSET_OUTCFG;
		tc->outcfg.info.nr_regs = mapped;

		printf("mapping: OUTCFG%d %d register values[%d] %s\n", tc->outcfg.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	/*
	 * CONFIG
	 */
	if (tc->config.info.id != -1) {
		mapped = config_map_regs(&tc->config, &td->config);
		nr_regs_data = td->config.nr_regs - REG_START_OFFSET_CONFIG;
		tc->config.info.nr_regs = mapped;

		printf("mapping: CONFIG%d %d register values[%d] %s\n", tc->config.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

exit:
	return ret;
}

static int vioc_set_component_regs(struct test_case_t *tc)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * RDMA - max 4 RDMAs
	 */
	if (tc->rdma1.info.id != -1) {
		ret = rdma_setup(&tc->rdma1);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_RDMA_1st]);
			goto exit;
		}
	}
	if (tc->rdma2.info.id != -1) {
		ret = rdma_setup(&tc->rdma2);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_RDMA_2nd]);
			goto exit;
		}
	}
	if (tc->rdma3.info.id != -1) {
		ret = rdma_setup(&tc->rdma3);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_RDMA_3rd]);
			goto exit;
		}
	}
	if (tc->rdma4.info.id != -1) {
		ret = rdma_setup(&tc->rdma4);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_RDMA_4th]);
			goto exit;
		}
	}

	/*
	 * WDMA - max 2 RDMAs
	 */
	if (tc->wdma1.info.id != -1) {
		ret = wdma_setup(&tc->wdma1);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_WDMA_1st]);
			goto exit;
		}
	}
	if (tc->wdma2.info.id != -1) {
		ret = wdma_setup(&tc->wdma2);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_WDMA_2nd]);
			goto exit;
		}
	}

	/*
	 * WMIX
	 */
	if (tc->wmix.info.id != -1) {
		ret = wmix_setup(&tc->wmix);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_WMIX]);
			goto exit;
		}
	}

	/*
	 * SC
	 */
	if (tc->sc.info.id != -1) {
		ret = sc_setup(&tc->sc);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_SC]);
			goto exit;
		}
	}

	/*
	 * LUT
	 */
	if (tc->lut.info.id != -1) {
		ret = lut_setup(&tc->lut);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_LUT]);
			goto exit;
		}
	}

	/*
	 * OUTCFG - WARNING: do not set whole registers
	 */
	if (tc->outcfg.info.id != -1 && 0) {
		ret = outcfg_setup(&tc->outcfg);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_OUTCFG]);
			goto exit;
		}
	}

	/*
	 * CONFIG - WARNING: do not set whole registers
	 */
	if (tc->config.info.id != -1 && 0) {
		ret = config_setup(&tc->config);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_CONFIG]);
			goto exit;
		}
	}

exit:
	return ret;
}

static int vioc_config_path(struct test_case_t *tc)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * RDMA - max 4 RDMAs
	 */
	if (tc->rdma1.info.id != -1 && tc->rdma1.info.plugin != -1) {
		ret = config_plugin(tc, VC_RDMA_1st);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_RDMA_1st]);
			goto exit;
		}
	}
	if (tc->rdma2.info.id != -1 && tc->rdma2.info.plugin != -1) {
		ret = config_plugin(tc, VC_RDMA_2nd);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_RDMA_2nd]);
			goto exit;
		}
	}
	if (tc->rdma3.info.id != -1 && tc->rdma3.info.plugin != -1) {
		ret = config_plugin(tc, VC_RDMA_3rd);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_RDMA_3rd]);
			goto exit;
		}
	}
	if (tc->rdma4.info.id != -1 && tc->rdma4.info.plugin != -1) {
		ret = config_plugin(tc, VC_RDMA_4th);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_RDMA_4th]);
			goto exit;
		}
	}

	/*
	 * SC
	 */
	if (tc->sc.info.id != -1 && tc->sc.info.plugin != -1) {
		ret = config_plugin(tc, VC_SC);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_SC]);
			goto exit;
		}
	}

	/*
	 * LUT
	 */
	if (tc->lut.info.id != -1 && tc->lut.info.plugin != -1) {
		ret = config_plugin(tc, VC_LUT);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_LUT]);
			goto exit;
		}
	}

	/*
	 * OUTCFG - ONLY set the specified registers
	 */
	ret = outcfg_config(tc);
	if (ret) {
		printf("[%s] error: %s", __func__, name_vioc_compoent[VC_LUT]);
		goto exit;
	}

	/*
	 * CONFIG - ONLY set the specified registers
	 */
	ret = config_config(tc);
	if (ret) {
		printf("[%s] error: %s", __func__, name_vioc_compoent[VC_LUT]);
		goto exit;
	}

exit:
	return ret;
}

static int vioc_verify_regs(struct test_case_t *tc)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * RDMA - max 4 RDMAs
	 */
	ret = rdma_verify_regs(&tc->rdma1);
	ret = rdma_verify_regs(&tc->rdma2);
	ret = rdma_verify_regs(&tc->rdma3);
	ret = rdma_verify_regs(&tc->rdma4);

	/*
	 * WDMA - max 2 RDMAs
	 */
	ret = wdma_verify_regs(&tc->wdma1);
	ret = wdma_verify_regs(&tc->wdma2);

	/*
	 * WMIX
	 */
	ret = wmix_verify_regs(&tc->wmix);

	/*
	 * SC
	 */
	ret = sc_verify_regs(&tc->sc);

	/*
	 * LUT
	 */
	ret = lut_verify_regs(&tc->lut);

	/*
	 * OUTCFG
	 */
	ret = outcfg_verify_regs(&tc->outcfg);

	/*
	 * CONFIG
	 */
	ret = config_verify_regs(&tc->config);

	return ret;
}
