#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>
#include <parser.h>
#include <pmap.h>
#include <mmap.h>

static void delete_test_data_list(struct test_data_t *);
static int run_test_single(struct test_case_t *, struct test_data_t *);


int test_main(char *file_name, char *pmap_name)
{
	char c;
	int ret = 0;
	int nr_test;	// number of test cases
	struct test_case_t *test_case;
	struct test_data_t *test_data, *td;
	struct pmap_t pmap;
	struct list_head *pos = NULL;

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
	 * get vioc base address
	 */
	test_case = (struct test_case_t *)malloc(sizeof(struct test_case_t));
	memset(test_case, 0, sizeof(struct test_case_t));

	test_case->vioc_base_addr = (addr_t *)vioc_mmap(0, 0);
	if (NULL == test_case->vioc_base_addr) {
		ret = -1;
		goto err2;
	}

	/*
	 * get pmap info for image buffers
	 */
	get_pmap(pmap_name, &pmap);

	/*
	 * RUN the test case on the list of test_data one by one
	 */
	list_for_each(pos, &test_data->list) {
		td = list_entry(pos, struct test_data_t, list);

		/* for debugging */
		print_parsed_data(td);

		/* run single test case */
		printf("Do you want to run the test%d (%s)? [y/n] ", td->test_no, td->test_name);
		c = getchar();
		if (c == 'y' || c == 'Y') {
			printf("\n\n==================================\n");
			printf("     RUN TEST CASE No.%d\n", td->test_no);
			printf("      - %s\n", td->test_name);
			printf("==================================\n");

			ret = run_test_single(test_case, td);
			if (ret) {
				printf("---> TEST FAILED\n");
			}
		} else {
			printf("---> SKIP TEST%d:%s\n", td->test_no, td->test_name);
		}
		getchar();
	}

	printf("\n<----------- END OF TEST ----------->\n");

err3:
	vioc_munmap(test_case->vioc_base_addr);
err2:
	free(test_case);
	delete_test_data_list(test_data);
err1:
	free(test_data);
	return ret;
}

static int run_test_single(struct test_case_t *test_case, struct test_data_t *test_data)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * setup vioc components & path
	 */
	ret = setup_vioc_path(test_case, test_data);
	if (ret) {
		printf("[%s] error: setup_vioc_path()\n", __func__);
		goto exit;
	}

	/*
	 * run single test case
	 */
	ret = shoot_test(test_case);
	if (ret) {
		printf("[%s] error: shoot_test()\n", __func__);
		goto exit;
	}

exit:
	return ret;
}

static void delete_test_data_list(struct test_data_t *t)
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

