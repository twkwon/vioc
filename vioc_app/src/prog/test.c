#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>
#include <parser.h>
#include <pmap.h>
#include <mmap.h>

static void delete_test_data_list(struct test_data_t *);
static int run_test_single(struct test_case_t *, struct test_data_t *, struct image_file_t *);
static int setup_image_file(struct test_case_t *, struct image_file_t *);
static int verify_image_buf(struct test_case_t *);


int test_main(char *file_name, char *pmap_name)
{
	char c;
	int i, ret = 0;
	int nr_test;	// number of test cases
	struct test_case_t *test_case;
	struct test_data_t *test_data, *td;
	struct pmap_t pmap;
	struct list_head *pos = NULL;
	struct image_file_t image;

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
	 * setup image buffers
	 */
	image.base_vaddr = (addr_t *)vioc_mmap(pmap.base, IMAGE_NUM * IMAGE_SIZE);
	image.base_paddr = (addr_t)pmap.base;
	memset(image.base_vaddr, 0, sizeof(IMAGE_NUM * IMAGE_SIZE));
	printf("Image base: paddr(0x%08x) -> vaddr(%p)\n", image.base_paddr, image.base_vaddr);

	for (i = 0; i < MAX_NUM_OF_RDMA; i++) {
		image.input[i].vaddr = image.base_vaddr + INT32_OFFSET(IMAGE_SIZE * i);
		image.input[i].paddr = image.base_paddr + (IMAGE_SIZE * i);
		printf("  input%d: paddr(0x%08x) -> vaddr(%p)\n", i, image.input[i].paddr, image.input[i].vaddr);
	}
	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		image.output[i].vaddr = image.input[MAX_NUM_OF_RDMA - 1].vaddr + INT32_OFFSET(IMAGE_SIZE + (IMAGE_SIZE * i));
		image.output[i].paddr = image.input[MAX_NUM_OF_RDMA - 1].paddr + IMAGE_SIZE + (IMAGE_SIZE * i);
		printf(" output%d: paddr(0x%08x) -> vaddr(%p)\n", i, image.output[i].paddr, image.output[i].vaddr);
	}

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

			ret = run_test_single(test_case, td, &image);
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

static int run_test_single(struct test_case_t *test_case, struct test_data_t *test_data, struct image_file_t *image)
{
	int ret = 0;
	printf("[%s]\n", __func__);

	/*
	 * setup vioc components & path
	 */
	ret = setup_test_case(test_case, test_data);
	if (ret) {
		printf("[%s] error: setup_vioc_path()\n", __func__);
		goto exit;
	}

	/*
	 * setup input & output image files
	 */
	ret = setup_image_file(test_case, image);
	if (ret) {
		printf("[%s] error: setup_image_file()\n", __func__);
		goto exit;
	}

	/*
	 * setup vioc components & path
	 */
	ret = setup_vioc_path(test_case);
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

static int setup_image_file(struct test_case_t *tc, struct image_file_t *img)
{
	int i, ret = 0;
	int fname_to_int;
	long flen;
	FILE *fp;
	printf("[%s]\n", __func__);

	/*
	 * open input files if test_cast has file_name
	 */
	for (i = 0; i < MAX_NUM_OF_RDMA; i++) {
		/*
		 * open files if test_cast has file_name
		 */
		fname_to_int = atoi(tc->input_file[i].name);
		printf("  str(%s) to int(%d)\n", tc->input_file[i].name, fname_to_int);
		if (fname_to_int == -1) {
			tc->input_file[i].id = -1;
			continue;
		} else {
			tc->input_file[i].id = i;	// enum vioc_components
		}

		/*
		 * Now we only use 'test_case_t' after copy v/paddr.
		 */
		tc->input_file[i].vaddr = img->input[i].vaddr;
		tc->input_file[i].paddr = img->input[i].paddr;

		fp = fopen(tc->input_file[i].name, "rb");
		if (fp == NULL) {
			printf("[%s] error: file open %s\n", __func__, tc->input_file[i].name);
			perror("fopen() failed");
			return -1;
		}

		/* get file length */
		fseek(fp, 0, SEEK_END);
		flen = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (flen == 0) {
			printf("[%s] %s is empty file\n", __func__, tc->input_file[i].name);
			fclose(fp);
			return -1;
		}

		/* read binary file into pmap buf */
		tc->input_file[i].len = fread((char *)tc->input_file[i].vaddr, 1, flen, fp);
		if (tc->input_file[i].len == flen) {
			printf("[%s] read %s %d bytes\n", __func__, tc->input_file[i].name, tc->input_file[i].len);
		} else {
			if (feof(fp)) {
				printf("[%s] error: read %s: unexpected end of file\n", __func__, tc->input_file[i].name);
			} else if (ferror(fp)) {
				perror("fread() failed");
			} else {
				printf("[%s] error: read %s %d bytes\n", __func__, tc->input_file[i].name, tc->input_file[i].len);
			}
			ret = -1;
		}

		/* store image info */
		img->input[i].id = tc->input_file[i].id;
		img->input[i].len = tc->input_file[i].len;
		img->input[i].width = tc->input_file[i].width;
		img->input[i].height = tc->input_file[i].height;
		img->input[i].fmt = tc->input_file[i].fmt;

		fclose(fp);
	}

	/* for debugging */
	verify_image_buf(tc);

	return ret;
}

static int verify_image_buf(struct test_case_t *tc)
{
	int i, ret = 0;
	unsigned int len = 0;
	FILE *fp = NULL;
	char name[SIZE_OF_TEST_NAME];
	printf("\n[%s]===================================================================\n", __func__);

	for (i = 0; i < MAX_NUM_OF_RDMA; i++) {
		printf("file(%s) id(%d) size(%dx%d) fmt(%d)\n", tc->input_file[i].name, tc->input_file[i].id,
			tc->input_file[i].width,tc->input_file[i].height, tc->input_file[i].fmt);
		if (tc->input_file[i].id != -1) {
			sprintf(name, "verify-%s", tc->input_file[i].name);
			printf("  backup fname(%s)\n", name);
			fp = fopen(name, "wb");
			if (fp == NULL) {
				perror("  fopen() failed");
				continue;
			}
			len = fwrite(tc->input_file[i].vaddr, 1, tc->input_file[i].len, fp);
			if (len != tc->input_file[i].len) {
				printf("  error: wrote %d bytes != %d\n", len, tc->input_file[i].len);
			} else {
				printf("  file(%s) wrote(%d) requested(%d)\n", name, len, tc->input_file[i].len);
			}
			fclose(fp);
		}
	}

	printf("===================================================================\n\n", __func__);
	return ret;
}