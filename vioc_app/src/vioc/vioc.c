#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>

static void get_vioc_component_info(struct test_case_t *);


int setup_vioc_path(struct test_case_t *test_case)
{
	/* get address each components */
	get_vioc_component_info(test_case);

	/* */
	//TODO: next step !!!!
}

static void get_vioc_component_info(struct test_case_t *t)
{
	t->rdma.info.id = VC_RDMA;
	t->rdma.info.addr_offset = OFFSET_RDMA_FROM_VIOC_BASE;
	t->rdma.addr = (VIOC_RDMA *)(t->vioc_base_addr + REG_OFFSET(t->rdma.info.addr_offset));

	t->wdma.info.id = VC_WDMA;
	t->wdma.info.addr_offset = OFFSET_WDMA_FROM_VIOC_BASE;
	t->wdma.addr = (VIOC_WDMA *)(t->vioc_base_addr + REG_OFFSET(t->wdma.info.addr_offset));

	t->wmix.info.id = VC_WMIX;
	t->wmix.info.addr_offset = OFFSET_WMIX_FROM_VIOC_BASE;
	t->wmix.addr = (VIOC_WMIX *)(t->vioc_base_addr + REG_OFFSET(t->wmix.info.addr_offset));

	t->sc.info.id = VC_SC;
	t->sc.info.addr_offset = OFFSET_SC_FROM_VIOC_BASE;
	t->sc.addr = (VIOC_SC *)(t->vioc_base_addr + REG_OFFSET(t->sc.info.addr_offset));

	t->lut.info.id = VC_LUT;
	t->lut.info.addr_offset = OFFSET_LUT_FROM_VIOC_BASE;
	//t->lut.addr = (VIOC_LUT *)(t->vioc_base_addr + REG_OFFSET(t->lut.info.addr_offset));	//TODO:

	t->outcfg.info.id = VC_OUTCFG;
	t->outcfg.info.addr_offset = OFFSET_OUTCFG_FROM_VIOC_BASE;
	t->outcfg.addr = (VIOC_OUTCFG *)(t->vioc_base_addr + REG_OFFSET(t->outcfg.info.addr_offset));

	t->config.info.id = VC_CONFIG;
	t->config.info.addr_offset = OFFSET_CONFIG_FROM_VIOC_BASE;
	t->config.addr = (VIOC_IREQ_CONFIG *)(t->vioc_base_addr + REG_OFFSET(t->config.info.addr_offset));
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
#endif