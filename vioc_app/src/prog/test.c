#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>
#include <parser.h>
#include <pmap.h>

static int setup_vioc_path(struct test_case_t *);
void delete_test_data_list(struct test_data_t *);

int test_main(char *file_name, char *pmap_name)
{
	int ret = 0;
	int nr_test;	// number of test cases
	struct test_case_t *test_case;
	struct test_data_t *test_data;
	struct pmap_t pmap;

	/*
	 * parse data from test_case.txt
	 */
	test_data = (struct test_data_t *)malloc(sizeof(struct test_data_t));
	INIT_LIST_HEAD(&test_data->list);

	nr_test = parse_test_case(file_name, test_data);
	if (nr_test <= 0) {
		printf("error: parse_test_case()\n");
		ret = nr_test;
		goto err1;
	}

	/*
	 * get vioc base address and pmap info
	 */
	test_case = (struct test_case_t *)malloc(sizeof(struct test_case_t));
	memset(test_case, 0, sizeof(struct test_case_t));

	test_case->vioc_base_addr = (addr_t *)vioc_mmap(0, 0);
	if (NULL == test_case->vioc_base_addr) {
		ret = -1;
		goto err2;
	}

	get_pmap(pmap_name, &pmap);
	printf("pmap.video.name = %s\n", pmap_name);
	printf("pmap.video.base = 0x%08x\n", pmap.base);
	printf("pmap.video.size = 0x%08x\n", pmap.size);


	/*
	 * setup vioc components & path
	 */
	setup_vioc_path(test_case);


err3:
	vioc_munmap(test_case->vioc_base_addr);
err2:
	free(test_case);
	delete_test_data_list(test_data);
err1:
	free(test_data);
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

static int setup_vioc_path(struct test_case_t *test_case)
{
	printf("\n\n-------------------------------------\n");
	printf("parse vioc components\n");
	parse_rdma(&(test_case->rdma), test_case->vioc_base_addr);
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

void delete_test_data_list(struct test_data_t *t)
{
	struct list_head *pos, *q;
	printf("\n[%s]\n", __func__);

	list_for_each_safe(pos, q, &t->list) {
		struct test_data_t *k = list_entry(pos, struct test_data_t, list);
		printf("remove test_data->test_no : %d\n", k->test_no);
		list_del(pos);
		free(k);
	}
}

