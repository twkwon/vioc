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

int setup_vioc_path(struct test_case_t *test_case, struct test_data_t *test_data)
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
		//goto exit;
	}

	/*
	 * set vioc registers
	 */
	ret = vioc_set_component_regs(test_case);

	/* for debugging */
	ret = vioc_verify_regs(test_case);

	/*
	 * config vioc path
	 */
	ret = vioc_config_path(test_case);

exit:
	return ret;
}

int shoot_test(struct test_case_t *test_case)
{
	int ret = 0;

	return 0;
}

static int vioc_get_component_info(struct test_case_t *tc, struct test_data_t *td)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/* test name & index */
	tc->test_no = td->test_no;
	memcpy(tc->test_name, td->test_name, sizeof(tc->test_name));

	/*
	 * RDMA - max 4 RDMAs
	 */
	tc->rdma1.info.id = td->rdma1.reg[0];
	tc->rdma1.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma1.info.id);
	tc->rdma1.addr = (VIOC_RDMA *)(tc->vioc_base_addr + REG_OFFSET(tc->rdma1.info.addr_offset));

	tc->rdma2.info.id = td->rdma2.reg[0];
	tc->rdma2.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma2.info.id);
	tc->rdma2.addr = (VIOC_RDMA *)(tc->vioc_base_addr + REG_OFFSET(tc->rdma2.info.addr_offset));

	tc->rdma3.info.id = td->rdma3.reg[0];
	tc->rdma3.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma3.info.id);
	tc->rdma3.addr = (VIOC_RDMA *)(tc->vioc_base_addr + REG_OFFSET(tc->rdma3.info.addr_offset));

	tc->rdma4.info.id = td->rdma4.reg[0];
	tc->rdma4.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE(tc->rdma4.info.id);
	tc->rdma4.addr = (VIOC_RDMA *)(tc->vioc_base_addr + REG_OFFSET(tc->rdma4.info.addr_offset));

	/*
	 * WDMA - max 2 RDMAs
	 */
	tc->wdma1.info.id = td->wdma1.reg[0];
	tc->wdma1.info.addr_offset = OFFSET_WDMA_FROM_VIOC_BASE(tc->wdma1.info.id);
	tc->wdma1.addr = (VIOC_WDMA *)(tc->vioc_base_addr + REG_OFFSET(tc->wdma1.info.addr_offset));

	tc->wdma2.info.id = td->wdma2.reg[0];
	tc->wdma2.info.addr_offset = OFFSET_WDMA_FROM_VIOC_BASE(tc->wdma2.info.id);
	tc->wdma2.addr = (VIOC_WDMA *)(tc->vioc_base_addr + REG_OFFSET(tc->wdma2.info.addr_offset));

	/*
	 * WMIX
	 */
	tc->wmix.info.id =td->wmix.reg[0];
	tc->wmix.info.addr_offset = OFFSET_WMIX_FROM_VIOC_BASE(tc->wmix.info.id);
	tc->wmix.addr = (VIOC_WMIX *)(tc->vioc_base_addr + REG_OFFSET(tc->wmix.info.addr_offset));

	/*
	 * SC
	 */
	tc->sc.info.id = td->sc.reg[0];
	tc->sc.info.addr_offset = OFFSET_SC_FROM_VIOC_BASE(tc->sc.info.id);
	tc->sc.addr = (VIOC_SC *)(tc->vioc_base_addr + REG_OFFSET(tc->sc.info.addr_offset));

	/*
	 * LUT
	 */
	tc->lut.info.id = td->lut.reg[0];
	tc->lut.info.addr_offset = OFFSET_LUT_FROM_VIOC_BASE;
	tc->lut.addr = (VIOC_LUT *)(tc->vioc_base_addr + REG_OFFSET(tc->lut.info.addr_offset));

	/*
	 * OUTCFG
	 */
	tc->outcfg.info.addr_offset = OFFSET_OUTCFG_FROM_VIOC_BASE;
	tc->outcfg.addr = (VIOC_OUTCFG *)(tc->vioc_base_addr + REG_OFFSET(tc->outcfg.info.addr_offset));

	/*
	 * CONFIG
	 */
	tc->config.info.addr_offset = OFFSET_CONFIG_FROM_VIOC_BASE;
	tc->config.addr = (VIOC_IREQ_CONFIG *)(tc->vioc_base_addr + REG_OFFSET(tc->config.info.addr_offset));

	return ret;
}

static int vioc_map_component_regs(struct test_case_t *tc, struct test_data_t *td)
{
	int ret = 0, mapped = 0;
	printf("[%s]\n", __func__);

	/*
	 * RDMA - max 4 RDMAs
	 */
	if (tc->rdma1.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma1, &td->rdma1);
		tc->rdma1.info.nr_regs = mapped;
		ret += mapped - td->rdma1.nr_regs;
		printf("mapping: 1st RDMA%d %d register values[%d] %s\n", tc->rdma1.info.id,
			mapped, td->rdma1.nr_regs, (mapped == td->rdma1.nr_regs) ? "" : "<-- error");
	}

	if (tc->rdma2.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma2, &td->rdma2);
		tc->rdma2.info.nr_regs = mapped;
		ret += mapped - td->rdma2.nr_regs;
		printf("mapping: 2nd RDMA%d %d register values[%d] %s\n", tc->rdma2.info.id,
			mapped, td->rdma2.nr_regs, (mapped == td->rdma2.nr_regs) ? "" : "<-- error");
	}

	if (tc->rdma3.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma3, &td->rdma3);
		tc->rdma3.info.nr_regs = mapped;
		ret += mapped - td->rdma3.nr_regs;
		printf("mapping: 3rd RDMA%d %d register values[%d] %s\n", tc->rdma3.info.id,
			mapped, td->rdma3.nr_regs, (mapped == td->rdma3.nr_regs) ? "" : "<-- error");
	}

	if (tc->rdma4.info.id != -1) {
		mapped = rdma_map_regs(&tc->rdma4, &td->rdma4);
		tc->rdma4.info.nr_regs = mapped;
		ret += mapped - td->rdma4.nr_regs;
		printf("mapping: 4th RDMA%d %d register values[%d] %s\n", tc->rdma4.info.id,
			mapped, td->rdma4.nr_regs, (mapped == td->rdma4.nr_regs) ? "" : "<-- error");
	}

	/*
	 * WDMA - max 2 WDMAs
	 */
	if (tc->wdma1.info.id != -1) {
		mapped = wdma_map_regs(&tc->wdma1, &td->wdma1);
		tc->wdma1.info.nr_regs = mapped;
		ret += mapped - td->wdma1.nr_regs;
		printf("mapping: 1st WDMA%d %d register values[%d] %s\n", tc->wdma1.info.id,
			mapped, td->wdma1.nr_regs, (mapped == td->wdma1.nr_regs) ? "" : "<-- error");
	}

	if (tc->wdma2.info.id != -1) {
		mapped = wdma_map_regs(&tc->wdma2, &td->wdma2);
		tc->wdma2.info.nr_regs = mapped;
		ret += mapped - td->wdma2.nr_regs;
		printf("mapping: 2nd WDMA%d %d register values[%d] %s\n", tc->wdma2.info.id,
			mapped, td->wdma2.nr_regs, (mapped == td->wdma2.nr_regs) ? "" : "<-- error");
	}

	/*
	 * WMIX
	 */
	if (tc->wmix.info.id != -1) {
		mapped = wmix_map_regs(&tc->wmix, &td->wmix);
		tc->wmix.info.nr_regs = mapped;
		ret += mapped - td->wmix.nr_regs;
		printf("mapping: WMIX%d %d register values[%d] %s\n", tc->wmix.info.id,
			mapped, td->wmix.nr_regs, (mapped == td->wmix.nr_regs) ? "" : "<-- error");
	}

	/*
	 * SC
	 */
	if (tc->sc.info.id != -1) {
		mapped = sc_map_regs(&tc->sc, &td->sc);
		tc->sc.info.nr_regs = mapped;
		ret += mapped - td->sc.nr_regs;
		printf("mapping: SC%d %d register values[%d] %s\n", tc->sc.info.id,
			mapped, td->sc.nr_regs, (mapped == td->sc.nr_regs) ? "" : "<-- error");
	}

	/*
	 * LUT
	 */
	if (tc->lut.info.id != -1) {
		mapped = lut_map_regs(&tc->lut, &td->lut);
		tc->lut.info.nr_regs = mapped;
		ret += mapped - td->lut.nr_regs;
		printf("mapping: LUT%d %d register values[%d] %s\n", tc->lut.info.id,
			mapped, td->lut.nr_regs, (mapped == td->lut.nr_regs) ? "" : "<-- error");
	}

	/*
	 * OUTCFG
	 */
	if (tc->outcfg.info.id != -1) {
		mapped = outcfg_map_regs(&tc->outcfg, &td->outcfg);
		tc->outcfg.info.nr_regs = mapped;
		ret += mapped - td->outcfg.nr_regs;
		printf("mapping: OUTCFG%d %d register values[%d] %s\n", tc->outcfg.info.id,
			mapped, td->outcfg.nr_regs, (mapped == td->outcfg.nr_regs) ? "" : "<-- error");
	}

	/*
	 * CONFIG
	 */
	if (tc->config.info.id != -1) {
		mapped = config_map_regs(&tc->config, &td->config);
		tc->config.info.nr_regs = mapped;
		ret += mapped - td->config.nr_regs;
		printf("mapping: CONFIG%d %d register values[%d] %s\n", tc->config.info.id,
			mapped, td->config.nr_regs, (mapped == td->config.nr_regs) ? "" : "<-- error");
	}

exit:
	return ret;
}

static int vioc_set_component_regs(struct test_case_t *tc)
{
	int ret = 0;
	printf("[%s]", __func__);

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
	 * OUTCFG
	 */
	if (tc->outcfg.info.id != -1) {
		ret = outcfg_setup(&tc->outcfg);
		if (ret) {
			printf("[%s] error: %s", __func__, name_vioc_compoent[VC_OUTCFG]);
			goto exit;
		}
	}

	/*
	 * CONFIG
	 */
	if (tc->config.info.id != -1) {
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

	return ret;
}

static int vioc_verify_regs(struct test_case_t *tc)
{
	int ret = 0;

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







#ifdef __ALAN__
int test_parse_rdma(struct vioc_rdma_t *rdma, addr_t *base_addr)
{
	int ret = 0;

	//test
	rdma->info.id = VC_RDMA;
	rdma->info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE;
	rdma->addr = (VIOC_RDMA *)(base_addr + REG_OFFSET(rdma->info.addr_offset));

	rdma->reg.uCTRL.bREG.R2Y = 1;

err:
	return ret;
}

int test_setup_vioc_path(struct test_case_t *test_case)
{
	printf("\n\n-------------------------------------\n");
	printf("parse vioc components\n");
	test_parse_rdma(&(test_case->rdma), test_case->vioc_base_addr);
	printf("test_case->rdma.info.id(%d)\n", test_case->rdma.info.id);
	printf("test_case->rdma.info.addr_offset(0x%x)\n", test_case->rdma.info.addr_offset);
	printf("test_case->rdma.reg.uCTRL.bREG.R2Y(%d)\n", test_case->rdma.reg.uCTRL.bREG.R2Y);
	printf("test_case->rdma.reg.uCTRL.nREG.R2Y(0x%08x)\n", test_case->rdma.reg.uCTRL.nREG);
	printf("test_case->rdma.addr(0x%p)\n", test_case->rdma.addr);


	printf("\nsetup vioc components\n");
	printf("0x%08x\n", *(addr_t *)(test_case->vioc_base_addr + REG_OFFSET(0x400)));
	printf("0x%08x\n", *(addr_t *)(test_case->vioc_base_addr + REG_OFFSET(0x408)));
	rdma_setup(&(test_case->rdma));
}

void test_reg_rw(struct vioc_rdma_t *rdma, struct test_data_reg_val_t *data)
{
	rdma->reg.uCROPSIZE.nREG = read_reg(&rdma->addr->uCROPSIZE.nREG);
	printf("uCROPSIZE.nREG 0x%08x\n", rdma->reg.uCROPSIZE.nREG);

	rdma->reg.uCTRL.nREG = read_reg(rdma->addr);
	printf("uCTRL.nREG 0x%08x\n", rdma->reg.uCTRL.nREG);

	rdma->reg.uMISC.nREG = read_reg(&rdma->addr->uMISC);
	printf("uMISC.nREG 0x%08x\n", rdma->reg.uMISC.nREG);

	rdma->reg = *rdma->addr;
	printf("uCTRL   0x%08x\n", rdma->reg.uCTRL.nREG);
	printf("uSIZE   0x%08x\n", rdma->reg.uSIZE.nREG);
	printf("uOFFSET 0x%08x\n", rdma->reg.uOFFSET.nREG);
	printf("nBASE0  0x%08x\n", rdma->reg.nBASE0);
	printf("nBASE1  0x%08x\n", rdma->reg.nBASE1);
	printf("nBASE2  0x%08x\n", rdma->reg.nBASE2);
	printf("uSTATUS 0x%08x\n", rdma->reg.uSTATUS.nREG);
	printf("uALPHA  0x%08x\n", rdma->reg.uALPHA.nREG);

	reg_t val0, val1;

	//test1 --> OK
	printf("%s: 0x%08x\n", __func__, *(addr_t *)(rdma->addr));

	val0 = read_reg(rdma->addr);
	printf("before: 0x%08x\n", val0);

	BITCSET(val0, 1<<17, rdma->reg.uCTRL.bREG.R2Y<<17);
	printf(" after: 0x%08x\n", val0);

	write_reg(rdma->addr, val0);
	printf("write: 0x%08x\n", rdma->reg.uCTRL.nREG);

	val1 = read_reg(rdma->addr);

	printf("\n\n=============== TEST ======================\n");
	printf(" read: 0x%08x\n", val0);
	printf("write: 0x%08x\n", rdma->reg.uCTRL.nREG);
	printf(" read: 0x%08x\n", val1);
	printf("=============== TEST ======================\n\n");

}
#endif
