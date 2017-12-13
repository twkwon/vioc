#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>
#include <parser.h>
#include <pmap.h>

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

