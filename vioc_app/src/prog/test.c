#include <stdio.h>
#include <stdlib.h>

#include <vioc.h>

static int setup_vioc_path(struct test_case_t *);

int test_main(void)
{
	int ret = 0;
	struct test_case_t *test_case;

	/*
	 * parse data from test_case.txt
	 */
	parse_test_case(test_case);

	/*
	 * setup vioc components & path
	 */
	setup_vioc_path(test_case);

	/*
	 * release resouce
	 */
	vioc_munmap(test_case->vioc_base_addr);
	free(test_case);

	return ret;
}

static int setup_vioc_path(struct test_case_t *test_case)
{
	printf("setup vioc components\n");
	printf("0x%08x\n", *(addr_t *)(test_case->vioc_base_addr + REG_OFFSET(0x400)));
	printf("0x%08x\n", *(addr_t *)(test_case->vioc_base_addr + REG_OFFSET(0x408)));
	rdma_setup(&(test_case->rdma));
}
