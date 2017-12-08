#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>

int parse_rdma(struct vioc_rdma_t *, addr_t *);


int parse_test_case(struct test_case_t *test_case)
{
	int ret = 0;
	int nr_case = 1;

	/*
	 * parse test case
	 */
	printf("parse test case\n");
	test_case = (struct test_case_t *)malloc(sizeof(struct test_case_t) * nr_case);
	memset(test_case, 0, (sizeof(struct test_case_t) * nr_case));

	test_case->vioc_base_addr = (addr_t *)vioc_mmap(0, 0);
	if (NULL == test_case->vioc_base_addr) {
		ret = -1;
		goto err;
	}

	/*
	 * parse vioc components
	 */
	printf("parse vioc components\n");
	parse_rdma(&(test_case->rdma), test_case->vioc_base_addr);
	printf("test_case->rdma.info.id(%d)\n", test_case->rdma.info.id);
	printf("test_case->rdma.info.addr_offset(0x%x)\n", test_case->rdma.info.addr_offset);
	printf("test_case->rdma.reg.uCTRL.bREG.R2Y(%d)\n", test_case->rdma.reg.uCTRL.bREG.R2Y);
	printf("test_case->rdma.reg.uCTRL.nREG.R2Y(0x%08x)\n", test_case->rdma.reg.uCTRL.nREG);
	printf("test_case->rdma.addr(0x%p)\n", test_case->rdma.addr);

	return ret;
err:
	free(test_case);
	return ret;
}

int parse_rdma(struct vioc_rdma_t *rdma, addr_t *base_addr)
{
	int ret = 0;

	//test
	rdma->info.id = 0;
	rdma->info.addr_offset = 0x00000400;
	rdma->addr = (VIOC_RDMA *)(base_addr + REG_OFFSET(rdma->info.addr_offset));

	rdma->reg.uCTRL.bREG.R2Y = 1;

err:
	return ret;
}
