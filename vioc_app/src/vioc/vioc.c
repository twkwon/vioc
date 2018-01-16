#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>

static int vioc_get_component_info(struct test_case_t *, struct test_data_t *);
static int vioc_map_component_regs(struct test_case_t *, struct test_data_t *);
static int vioc_reset_path(struct test_case_t *);
static int vioc_set_component_regs(struct test_case_t *);
static int vioc_config_path(struct test_case_t *);
static int vioc_config_disp_path(struct test_case_t *);
static int vioc_verify_regs(struct test_case_t *);
static int vioc_set_dma_address(struct test_case_t *);
static void vioc_get_dma_address(unsigned char, unsigned int,
										unsigned int, unsigned int,
										unsigned int, unsigned int,
										unsigned int *, unsigned int *, unsigned int *);
static void vioc_get_dma_offset(unsigned int, unsigned int,
									unsigned int *, unsigned int *);


const char *name_vioc_compoent[] = {
	/* enum vioc_components */
	[VC_DISP_RDMA]	= "DISP_RDMA",
	[VC_RDMA_1st]	= "1st RDMA",
	[VC_RDMA_2nd]	= "2nd RDMA",
	[VC_RDMA_3rd]	= "3rd RDMA",
	[VC_RDMA_4th]	= "4th RDMA",
	[VC_WDMA_1st]	= "1st WDMA",
	[VC_WDMA_2nd]	= "2nd WDMA",
	[VC_DISP_WMIX]	= "DISP_WMIX",
	[VC_WMIX]		= "WMIX",
	[VC_SC]			= "SC",
	[VC_LUT]		= "LUT",
	[VC_VIN]		= "VIN",
	[VC_VIN_LUT]	= "VIN_LUT",
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
	 * reset vioc components
	 */
	ret = vioc_reset_path(test_case);

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

	/*
	 * setup RDMA/WDMA address and offset
	 */
	ret = vioc_set_dma_address(test_case);
	if (ret) {
		printf("[%s] error: vioc_set_address()\n", __func__);
		goto exit;
	}

	/*
	 * Configuration Display Path
	 * - RDMA: fmt, size, base0/1/2, offs0/1
	 * - WMIX: ovp
	 */
	if (test_case->disp_rdma.info.id != -1) {
		ret = vioc_config_disp_path(test_case);
		if (ret) {
			printf("[%s] error: vioc_config_disp_path()\n", __func__);
			goto exit;
		}
	}

	if (debug_level == DEBUG_VERIFY) {
		/* for debugging */
		vioc_verify_regs(test_case);
	}

exit:
	return ret;
}

int shoot_test(struct test_case_t *tc)
{
	int ret = 0;
	printf("\n\nSHOOTING !!!!!\n\n");

	/*
	 * bit order
	 *  1st: set enable bit
	 *  2nd: set update bit
	 * component order (M2M path)
	 *  RDMA -> [SC] -> [LUT] -> WMIX -> [SC] -> [LUT]  -> WDMA
	 *  VIN -> [SC] -> WMIX -> [SC] -> WDMA
	 */
	if (tc->rdma1.info.id != -1) {
		rdma_en_ctrl(&tc->rdma1, 1);
	}
	if (tc->rdma2.info.id != -1) {
		rdma_en_ctrl(&tc->rdma2, 1);
	}
	if (tc->rdma3.info.id != -1) {
		rdma_en_ctrl(&tc->rdma3, 1);
	}
	if (tc->rdma4.info.id != -1) {
		rdma_en_ctrl(&tc->rdma4, 1);
	}

	if (tc->vin.info.id != -1) {
		vin_en_ctrl(&tc->vin, 1);
	}

	if (tc->sc.info.id != -1) {
		sc_en_ctrl(&tc->sc, 1);
	}

	if (tc->lut.info.id != -1) {
		lut_en_ctrl(&tc->lut, 1);
	}

	if (tc->wmix.info.id != -1) {
		wmix_en_ctrl(&tc->wmix, 1);
	}

	if (tc->wdma1.info.id != -1) {
		wdma_en_ctrl(&tc->wdma1, 1);
	}
	if (tc->wdma2.info.id != -1) {
		wdma_en_ctrl(&tc->wdma2, 1);
	}

	/*
	 * Display path
	 */
	if (tc->disp_rdma.info.id != -1) {
		rdma_en_ctrl(&tc->disp_rdma, 1);
	}
	//if (tc->disp_wmix.info.id != -1) {
	//	wmix_en_ctrl(&tc->disp_wmix, 1);
	//}

	return ret;
}

static int vioc_get_component_info(struct test_case_t *tc, struct test_data_t *td)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/* get infos (test no, test name, input/output files) */
	//TODO: is it ok? 64bits machine has to use (sizeof(struct image_buf_info_t) * MAX_NUM_OF_RDMA)
	tc->test_no = td->test_no;
	memcpy(tc->test_name, td->test_name, sizeof(tc->test_name));
	memcpy(tc->input_file, td->input_file, sizeof(tc->input_file));
	memcpy(tc->output_file, td->output_file, sizeof(tc->output_file));
	memcpy(tc->reference_file, td->reference_file, sizeof(tc->reference_file));

	/*
	 * Display Path - RDMA-WMIX-DISP
	 */
	tc->disp_rdma.info.id = td->disp_rdma.reg[0];
	tc->disp_rdma.info.plugin = td->disp_rdma.reg[1];
	tc->disp_rdma.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->disp_rdma.info.id);
	tc->disp_rdma.addr = (VIOC_RDMA *)(tc->vioc_base_addr + INT32_OFFSET(tc->disp_rdma.info.addr_offset));

	tc->disp_wmix.info.id = td->disp_wmix.reg[0];
	tc->disp_wmix.info.plugin = td->disp_wmix.reg[1];
	tc->disp_wmix.info.addr_offset = OFFSET_WMIX_FROM_VIOC_BASE(tc->disp_wmix.info.id);
	tc->disp_wmix.addr = (VIOC_WMIX *)(tc->vioc_base_addr + INT32_OFFSET(tc->disp_wmix.info.addr_offset));

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
	 * VIN & VIN_LUT
	 */
	tc->vin.info.id = td->vin.reg[0];
	tc->vin.info.plugin = td->vin.reg[1];
	tc->vin.info.addr_offset = OFFSET_VIN_FROM_VIOC_BASE(tc->vin.info.id);
	tc->vin.addr = (VIOC_VIN *)(tc->vioc_base_addr + INT32_OFFSET(tc->vin.info.addr_offset));

	tc->vin_lut.info.id = td->vin.reg[0];
	tc->vin_lut.info.plugin = td->vin.reg[1];
	tc->vin_lut.info.addr_offset = tc->vin.info.addr_offset + OFFSET_VIN_LUT_FROM_VIN;
	tc->vin_lut.addr = (VIOC_VIN_LUT_C *)(tc->vioc_base_addr + INT32_OFFSET(tc->vin_lut.info.addr_offset));

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
	 * Display Path - RDMA-WMIX-DISP
	 */
	if (tc->disp_rdma.info.id != -1) {
		mapped = rdma_map_regs(&tc->disp_rdma, &td->disp_rdma);
		nr_regs_data = td->disp_rdma.nr_regs - REG_START_OFFSET_RDMA;
		tc->disp_rdma.info.nr_regs = mapped;

		printf("mapping: DISP_RDMA%d %d register values[%d] %s\n", tc->disp_rdma.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	if (tc->disp_wmix.info.id != -1) {
		mapped = wmix_map_regs(&tc->disp_wmix, &td->disp_wmix);
		nr_regs_data = td->disp_wmix.nr_regs - REG_START_OFFSET_WMIX;
		tc->disp_wmix.info.nr_regs = mapped;

		printf("mapping: DISP_WMIX%d %d register values[%d] %s\n", tc->disp_wmix.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

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
	 * VIN & VIN_LUT
	 */
	if (tc->vin.info.id != -1) {
		mapped = vin_map_regs(&tc->vin, &td->vin);
		nr_regs_data = td->vin.nr_regs - REG_START_OFFSET_VIN;
		tc->vin.info.nr_regs = mapped;

		printf("mapping: VIN%d %d register values[%d] %s\n", tc->vin.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}
	if (tc->vin_lut.info.id != -1) {
		mapped = vin_lut_map_regs(&tc->vin_lut, &td->vin_lut);
		nr_regs_data = td->vin_lut.nr_regs - REG_START_OFFSET_VIN_LUT;
		tc->vin_lut.info.nr_regs = mapped;

		printf("mapping: VIN_LUT%d %d register values[%d] %s\n", tc->vin.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	/*
	 * OUTCFG
	 */
	if (tc->outcfg.info.id != -1 || REG_ALWAYS_READ) {
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
	if (tc->config.info.id != -1 || REG_ALWAYS_READ) {
		mapped = config_map_regs(&tc->config, &td->config);
		nr_regs_data = td->config.nr_regs - REG_START_OFFSET_CONFIG;
		tc->config.info.nr_regs = mapped;

		printf("mapping: CONFIG%d %d register values[%d] %s\n", tc->config.info.id,
			mapped, nr_regs_data, (mapped == nr_regs_data) ? "" : "<-- error");

		ret += mapped - nr_regs_data;
	}

	return ret;
}

static int vioc_reset_path(struct test_case_t *tc)
{
	int ret = 0;

	/* Disable previous sc */
	config_plugout_sc(tc, tc->todo_disable_prev_sc_id);

	/*
	 * Disable components
	 */
	if (tc->wdma1.info.id != -1) {
		wdma_en_ctrl(&tc->wdma1, 0);
	}
	if (tc->wdma2.info.id != -1) {
		wdma_en_ctrl(&tc->wdma2, 0);
	}

	if (tc->wmix.info.id != -1) {
		wmix_en_ctrl(&tc->wmix, 0);
	}

	if (tc->rdma1.info.id != -1) {
		rdma_en_ctrl(&tc->rdma1, 0);
	}
	if (tc->rdma2.info.id != -1) {
		rdma_en_ctrl(&tc->rdma2, 0);
	}
	if (tc->rdma3.info.id != -1) {
		rdma_en_ctrl(&tc->rdma3, 0);
	}
	if (tc->rdma4.info.id != -1) {
		rdma_en_ctrl(&tc->rdma4, 0);
	}

	if (tc->vin.info.id != -1) {
		vin_en_ctrl(&tc->vin, 0);
	}

	if (tc->sc.info.id != -1) {
		sc_en_ctrl(&tc->sc, 0);
	}

	if (tc->lut.info.id != -1) {
		lut_en_ctrl(&tc->lut, 0);
	}

	if (tc->disp_rdma.info.id != -1) {
		rdma_en_ctrl(&tc->disp_rdma, 0);
	}

	/*
	 * Reset components
	 */
	config_reset(tc);

	return ret;
}

static int vioc_set_component_regs(struct test_case_t *tc)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * Display Path - RDMA-WMIX-DISP
	 */
	if (tc->disp_rdma.info.id != -1) {
		ret = rdma_setup(&tc->disp_rdma);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_DISP_RDMA]);
			goto exit;
		}
	}

	if (tc->disp_wmix.info.id != -1) {
		ret = wmix_setup(&tc->disp_wmix);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_DISP_WMIX]);
			goto exit;
		}
	}

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
	 * VIN & VIN_LUT
	 */
	if (tc->vin.info.id != -1) {
		ret = vin_setup(&tc->vin);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_VIN]);
			goto exit;
		}
	}
	if (tc->vin_lut.info.id != -1) {
		ret = vin_lut_setup(&tc->vin_lut);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_VIN_LUT]);
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
	 * Display Path - RDMA
	 */
	if (tc->disp_rdma.info.id != -1 && tc->disp_rdma.info.plugin != -1) {
		ret = config_plugin(tc, VC_DISP_RDMA);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_DISP_RDMA]);
			goto exit;
		}
	}

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
	 * VIN
	 */
	if (tc->vin.info.id != -1 && tc->vin.info.plugin != -1) {
		ret = config_plugin(tc, VC_VIN);
		if (ret) {
			printf("[%s] error: plug-in %s", __func__, name_vioc_compoent[VC_VIN]);
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

/*
 * Configuration Display Path
 * - RDMA: fmt, size, base0/1/2, offs0/1
 * - WMIX: ovp
 */
static int vioc_config_disp_path(struct test_case_t *tc)
{
	int ret = 0;
	addr_t base0, base1, base2;
	unsigned int offset0, offset1;
	unsigned int fmt;
	unsigned int width, height;
	struct vioc_rdma_t *rdma;
	struct vioc_wdma_t *wdma;

	rdma = &tc->disp_rdma;

	/* select wdma to display */
	if (tc->wdma1.info.plugin == 1) {
		wdma = &tc->wdma1;
	} else if (tc->wdma2.info.plugin == 1) {
		wdma = &tc->wdma2;
	} else {
		return ret;
	}

	if (ISSET(rdma->auto_set, AUTO_RDMA_FMT)) {
		fmt = wdma->reg.uCTRL.bREG.FMT;
		rdma_set_fmt(rdma, fmt);
	}

	if (ISSET(rdma->auto_set, AUTO_RDMA_SIZE)) {
		width = wdma->reg.uSIZE.bREG.WIDTH;
		height = wdma->reg.uSIZE.bREG.HEIGHT;
		rdma_set_size(rdma, width, height);
	}

	if (ISSET(rdma->auto_set, AUTO_RDMA_BASE)) {
		base0 = wdma->reg.nBASE0;
		base1 = wdma->reg.nBASE1;
		base2 = wdma->reg.nBASE2;
		rdma_set_address(rdma, base0, base1, base2);
	}

	if (ISSET(rdma->auto_set, AUTO_RDMA_OFFS)) {
		offset0 = wdma->reg.uOFFSET.bREG.OFFSET0;
		offset1 = wdma->reg.uOFFSET.bREG.OFFSET1;
		rdma_set_offset(rdma, offset0, offset1);
	}

	return ret;
}

static int vioc_verify_regs(struct test_case_t *tc)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * RDMA - max 4 RDMAs
	 */
	ret += rdma_verify_regs(&tc->disp_rdma);
	ret += rdma_verify_regs(&tc->rdma1);
	ret += rdma_verify_regs(&tc->rdma2);
	ret += rdma_verify_regs(&tc->rdma3);
	ret += rdma_verify_regs(&tc->rdma4);

	/*
	 * WDMA - max 2 RDMAs
	 */
	ret += wdma_verify_regs(&tc->wdma1);
	ret += wdma_verify_regs(&tc->wdma2);

	/*
	 * WMIX
	 */
	ret += wmix_verify_regs(&tc->disp_wmix);
	ret += wmix_verify_regs(&tc->wmix);

	/*
	 * SC
	 */
	ret += sc_verify_regs(&tc->sc);

	/*
	 * LUT
	 */
	ret += lut_verify_regs(&tc->lut);

	/*
	 * VIN & VIN_LUT
	 */
	ret += vin_verify_regs(&tc->vin);
	ret += vin_lut_verify_regs(&tc->vin_lut);

	/*
	 * OUTCFG
	 */
	ret += outcfg_verify_regs(&tc->outcfg);

	/*
	 * CONFIG
	 */
	ret += config_verify_regs(&tc->config);

	return ret;
}

/** @brief Setup RDMA/WDMA address and offset.
 *  It depends on the input image (size, format) and SC.
 *  @param struct test_case_t
 *  @return returns 0 if successful
 */
static int vioc_set_dma_address(struct test_case_t *tc)
{
	int i, ret = 0;
	addr_t base0, base1, base2;
	unsigned int offset0, offset1;
	unsigned int width, height;
	unsigned int start_x, start_y;

	for (i = 0; i < MAX_NUM_OF_RDMA; i++) {
		if (tc->input_file[i].id != -1) {
			struct vioc_rdma_t *rdma;

			switch (i) {
			case 0:
				rdma = &tc->rdma1;
				break;
			case 1:
				rdma = &tc->rdma2;
				break;
			case 2:
				rdma = &tc->rdma3;
				break;
			case 3:
				rdma = &tc->rdma4;
				break;
			}

			/* not supported crop */
			start_x = 0;
			start_y = 0;
			base1 = 0;
			base2 = 0;

			base0 = tc->input_file[i].paddr;
			width = tc->input_file[i].width;
			height = tc->input_file[i].height;

			vioc_get_dma_offset(tc->input_file[i].fmt, width, &offset0, &offset1);
			vioc_get_dma_address(tc->input_file[i].fmt, base0, width, height, start_x, start_y, &base0, &base1, &base2);

			rdma->reg.nBASE0 = base0;
			rdma->reg.nBASE1 = base1;
			rdma->reg.nBASE2 = base2;
			rdma->reg.uOFFSET.bREG.OFFSET0 = offset0;
			rdma->reg.uOFFSET.bREG.OFFSET1 = offset1;

			printf("rdma[%d] base0: 0x%08x = 0x%08x\n", i, rdma->reg.nBASE0, base0);
			printf("rdma[%d] base1: 0x%08x = 0x%08x\n", i, rdma->reg.nBASE1, base1);
			printf("rdma[%d] base2: 0x%08x = 0x%08x\n", i, rdma->reg.nBASE2, base2);
			printf("rdma[%d]offset: 0x%08lx (0x%x, 0x%x)\n", i, rdma->reg.uOFFSET.nREG, offset1, offset0);

			rdma_set_offset(rdma, offset0, offset1);
			rdma_set_address(rdma, base0, base1, base2);
		}
	}

	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		if (tc->output_file[i].id != -1) {
			int format;
			struct vioc_wdma_t *wdma;

			switch (i) {
			case 0:
				wdma = &tc->wdma1;
				break;
			case 1:
				wdma = &tc->wdma2;
				break;
			}

			/* not supported crop */
			start_x = 0;
			start_y = 0;
			base1 = 0;
			base2 = 0;

			base0 = tc->output_file[i].paddr;
			width = wdma->reg.uSIZE.bREG.WIDTH;
			height = wdma->reg.uSIZE.bREG.HEIGHT;
			format = wdma->reg.uCTRL.bREG.FMT;

			vioc_get_dma_offset(format, width, &offset0, &offset1);
			vioc_get_dma_address(format, base0, width, height, start_x, start_y, &base0, &base1, &base2);

			wdma->reg.nBASE0 = base0;
			wdma->reg.nBASE1 = base1;
			wdma->reg.nBASE2 = base2;
			wdma->reg.uOFFSET.bREG.OFFSET0 = offset0;
			wdma->reg.uOFFSET.bREG.OFFSET1 = offset1;

			printf("wdma[%d] base0: 0x%08x = 0x%08x\n", i, wdma->reg.nBASE0, base0);
			printf("wdma[%d] base1: 0x%08x = 0x%08x\n", i, wdma->reg.nBASE1, base1);
			printf("wdma[%d] base2: 0x%08x = 0x%08x\n", i, wdma->reg.nBASE2, base2);
			printf("wdma[%d]offset: 0x%08lx (0x%x, 0x%x)\n", i, wdma->reg.uOFFSET.nREG, offset1, offset0);

			wdma_set_offset(wdma, offset0, offset1);
			wdma_set_address(wdma, base0, base1, base2);
		}
	}

	return ret;
}

static void vioc_get_dma_address(unsigned char format, addr_t base_Yaddr,
										unsigned int src_imgx, unsigned int src_imgy,
										unsigned int start_x, unsigned int start_y,
										addr_t *Y, addr_t *U, addr_t *V)
{
	int bpp;
	unsigned int Uaddr, Vaddr, Yoffset, UVoffset, start_yPos, start_xPos;

	Uaddr = Vaddr = 0;
	Yoffset = UVoffset = 0;
	start_yPos = start_xPos = 0;

	start_yPos = (start_y >> 1) << 1;
	start_xPos = (start_x >> 1) << 1;
	Yoffset = (src_imgx * start_yPos) + start_xPos;

	if((format >= TCC_LCDC_IMG_FMT_RGB332) && (format <= TCC_LCDC_IMG_FMT_ARGB6666_3)) {
		if(format == TCC_LCDC_IMG_FMT_RGB332)
			bpp = 1;
		else if((format >= TCC_LCDC_IMG_FMT_RGB444) && (format <= TCC_LCDC_IMG_FMT_RGB555))
			bpp = 2;
		else if((format >= TCC_LCDC_IMG_FMT_RGB888) && (format <= TCC_LCDC_IMG_FMT_ARGB6666_3))
			bpp = 4;
		else 
			bpp = 2;

		*Y = base_Yaddr + Yoffset * bpp;
	}

	if ((format == TCC_LCDC_IMG_FMT_UYVY) || (format == TCC_LCDC_IMG_FMT_VYUY)
		|| (format == TCC_LCDC_IMG_FMT_YUYV) || (format == TCC_LCDC_IMG_FMT_YVYU)) {
		Yoffset = 2 * Yoffset;
	}

	*Y = base_Yaddr + Yoffset;

	if (*U == 0 && *V == 0) {
		Uaddr = GET_ADDR_YUV42X_spU(base_Yaddr, src_imgx, src_imgy);
		if(format == TCC_LCDC_IMG_FMT_YUV420SP)
			Vaddr = GET_ADDR_YUV420_spV(Uaddr, src_imgx, src_imgy);
		else
			Vaddr = GET_ADDR_YUV422_spV(Uaddr, src_imgx, src_imgy);
	} else {
		Uaddr = *U;
		Vaddr = *V;
	}

	if ((format == TCC_LCDC_IMG_FMT_YUV420SP) || (format == TCC_LCDC_IMG_FMT_YUV420ITL0) || (format == TCC_LCDC_IMG_FMT_YUV420ITL1)) {
		if(format == TCC_LCDC_IMG_FMT_YUV420SP)
			UVoffset = ((src_imgx * start_yPos) / 4 + start_xPos / 2);
		else
			UVoffset = ((src_imgx * start_yPos) / 2 + start_xPos);
	} else {
		if(format == TCC_LCDC_IMG_FMT_YUV422ITL1)
			UVoffset = ((src_imgx * start_yPos) + start_xPos);
		else
			UVoffset = ((src_imgx * start_yPos) / 2 + start_xPos / 2);
	}

	*U = Uaddr + UVoffset;
	*V = Vaddr + UVoffset;
}

static void vioc_get_dma_offset(unsigned int format, unsigned int width,
									unsigned int *offset0, unsigned int *offset1)
{
	*offset0 = 0;
	*offset1 = 0;

	switch (format) {
		case TCC_LCDC_IMG_FMT_1BPP:	// 1bpp indexed color
			*offset0 = (1 * width) / 8;
			break;
		case TCC_LCDC_IMG_FMT_2BPP:	// 2bpp indexed color
			*offset0 = (1 * width) / 4;
			break;
		case TCC_LCDC_IMG_FMT_4BPP:	// 4bpp indexed color
			*offset0 = (1 * width) / 2;
			break;
		case TCC_LCDC_IMG_FMT_8BPP:	// 8bpp indexed color
			*offset0 = 1 * width;
			break;
		case TCC_LCDC_IMG_FMT_RGB332:	// RGB332 - 1bytes aligned - R[7:5],G[4:2],B[1:0]
			*offset0 = 1 * width;
			break;
		case TCC_LCDC_IMG_FMT_RGB444:	// RGB444 - 2bytes aligned - A[15:12],R[11:8],G[7:3],B[3:0]
		case TCC_LCDC_IMG_FMT_RGB565:	// RGB565 - 2bytes aligned - R[15:11],G[10:5],B[4:0]
		case TCC_LCDC_IMG_FMT_RGB555:	// RGB555 - 2bytes aligned - A[15],R[14:10],G[9:5],B[4:0]
			*offset0 = 2 * width;
			break;
		case TCC_LCDC_IMG_FMT_RGB888:	// RGB888 - 4bytes aligned - A[31:24],R[23:16],G[15:8],B[7:0]
		case TCC_LCDC_IMG_FMT_RGB666:	// RGB666 - 4bytes aligned - A[23:18],R[17:12],G[11:6],B[5:0]
			*offset0 = 4 * width;
			break;
		case TCC_LCDC_IMG_FMT_RGB888_3: //RGB888 - 3 bytes aligned : B1[31:24],R0[23:16],G0[15:8],B0[7:0]
		case TCC_LCDC_IMG_FMT_ARGB6666_3: //ARGB6666 - 3 bytes aligned : A[23:18],R[17:12],G[11:6],B[5:0]
			*offset0 = 3 * width;
			break;
		case TCC_LCDC_IMG_FMT_444SEP:	/* YUV444 or RGB444 Format */
			*offset0 = width;
			*offset1 = width;
			break;
		case TCC_LCDC_IMG_FMT_YUV420SP:	// YCbCr 4:2:0 Separated format - Not Supported for Image 1 and 2
			*offset0 = width;
			*offset1 = width / 2;
			break;
		case TCC_LCDC_IMG_FMT_YUV422SP:		// YCbCr 4:2:2 Separated format - Not Supported for Image 1 and 2
			*offset0 = width;
			*offset1 = width / 2;
			break;
		case TCC_LCDC_IMG_FMT_UYVY:	// YCbCr 4:2:2 Sequential format
		case TCC_LCDC_IMG_FMT_VYUY:	// YCbCr 4:2:2 Sequential format
		case TCC_LCDC_IMG_FMT_YUYV:	// YCbCr 4:2:2 Sequential format
		case TCC_LCDC_IMG_FMT_YVYU:	// YCbCr 4:2:2 Sequential format
			*offset0 = 2 * width;
			break;
		case TCC_LCDC_IMG_FMT_YUV420ITL0:	// YCbCr 4:2:0 interleved type 0 format - Not Supported for Image 1 and 2
		case TCC_LCDC_IMG_FMT_YUV420ITL1:	// YCbCr 4:2:0 interleved type 1 format - Not Supported for Image 1 and 2
			*offset0 = width;
			*offset1 = width;
			break;
		case TCC_LCDC_IMG_FMT_YUV422ITL0:	// YCbCr 4:2:2 interleved type 0 format - Not Supported for Image 1 and 2
		case TCC_LCDC_IMG_FMT_YUV422ITL1:	// YCbCr 4:2:2 interleved type 1 format - Not Supported for Image 1 and 2
			*offset0 = width;
			*offset1 = width;
			break;
		default:
			*offset0 = width;
			*offset1 = width;
			break;
	}
}
