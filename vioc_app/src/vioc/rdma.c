#include <stdio.h>

#include <vioc.h>


int rdma_setup(struct vioc_rdma_t *rdma)
{
	int ret = 0;
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

	return ret;
}