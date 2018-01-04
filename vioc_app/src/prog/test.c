#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vioc.h>
#include <parser.h>
#include <pmap.h>
#include <mmap.h>

static void delete_test_data_list(struct test_data_t *);
static int run_test_single(struct test_case_t *, struct test_data_t *, struct image_file_t *);
static int verify_test_single(struct test_case_t *, struct test_data_t *);
static int setup_image_file(struct test_case_t *, struct image_file_t *);
static int verify_image_buf(struct test_case_t *);
static int write_output_image(struct test_case_t *);
static unsigned int tcc_get_image_size(unsigned int, unsigned int, unsigned int);
static int tcc_get_pfmt(unsigned int);
static int tcc_get_bpp(unsigned int);


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

#if 0
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
#else
	for (i = 0; i < MAX_NUM_OF_RDMA + MAX_NUM_OF_WDMA; i++) {
		unsigned int offs;
		offs = IMAGE_SIZE * i;

		if (i < MAX_NUM_OF_RDMA) {
			image.input[i].vaddr = image.base_vaddr + INT32_OFFSET(offs);
			image.input[i].paddr = image.base_paddr + offs;
			printf("  input%d: paddr(0x%08x) -> vaddr(%p)\n", i, image.input[i].paddr, image.input[i].vaddr);
		} else {
			image.output[i - MAX_NUM_OF_RDMA].vaddr = image.base_vaddr + INT32_OFFSET(offs);
			image.output[i - MAX_NUM_OF_RDMA].paddr = image.base_paddr + offs;
			printf(" output%d: paddr(0x%08x) -> vaddr(%p)\n", i - MAX_NUM_OF_RDMA, image.output[i - MAX_NUM_OF_RDMA].paddr, image.output[i - MAX_NUM_OF_RDMA].vaddr);
		}
	}
#endif

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
				td->test_status = TEST_STATUS_ERR_RUN;
				printf("---> ERROR: run test\n");
			} else {
				/*
				 * If run_test_single is succeeded, compare output and reference image.
				 */
				ret = verify_test_single(test_case, td);
			}
		} else {
			td->test_status = TEST_STATUS_RUN_SKIP;
			printf("---> SKIP TEST%d:%s\n", td->test_no, td->test_name);
		}
		getchar();
	}

	printf("\n<----------- END OF TEST ----------->\n");

	printf("\n<----------- TEST STATUS  ----------->\n");
	list_for_each(pos, &test_data->list) {
		td = list_entry(pos, struct test_data_t, list);

		if (td->test_status == TEST_STATUS_PASS) {
			printf("[%s] PASS\n", td->test_name);
		} else if (td->test_status == TEST_STATUS_RUN_SKIP) {
			printf("[%s] SKIP run test\n", td->test_name);
		} else if (td->test_status == TEST_STATUS_NO_COMPARE) {
			printf("[%s] SKIP compare reference\n", td->test_name);
		} else {
			switch (td->test_status) {
			case TEST_STATUS_ERR_RUN:
				printf("[%s] ERROR: run test\n", td->test_name);
				break;
			case TEST_STATUS_ERR_OUTPUT:
				printf("[%s] ERROR: output file\n", td->test_name);
				break;
			case TEST_STATUS_ERR_INPUT:
				printf("[%s] ERROR: input file\n", td->test_name);
				break;
			case TEST_STATUS_ERR_REFERENCE:
				printf("[%s] ERROR: reference file\n", td->test_name);
				break;
			case TEST_STATUS_FAIL_COMPARE:
				printf("[%s] FAIL: compare reference and output file\n", td->test_name);
				break;
			default:
				printf("[%s] ERROR: no %d\n", td->test_name, td->test_status);
				break;
			}
		}
	}

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
	 * setup input, output, reference image files
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

	/*
	 * write output iamge through wdma
	 */
	ret = write_output_image(test_case);
	if (ret) {
		printf("[%s] error: write_output_image()\n", __func__);
		goto exit;
	}

exit:
	return ret;
}

static int verify_test_single(struct test_case_t *tc, struct test_data_t *td)
{
	int i, ret = 0;
	int file_idx, err_count;
	FILE *fp_out = NULL;
	FILE *fp_ref = NULL;

	td->test_status = TEST_STATUS_NO_COMPARE;

	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		if (tc->output_file[i].id == -1) {
			continue;
		}
		if (tc->reference_file[i].id == -1) {
			continue;
		}
		if (tc->output_file[i].len == 0) {
			continue;
		}

		fp_out = fopen(tc->output_file[i].name, "rb");
		if (fp_out == NULL) {
			printf("[%s] error: file open %s\n", __func__, tc->output_file[i].name);
			perror("  fopen() failed");
			td->test_status = TEST_STATUS_ERR_OUTPUT;
			continue;
		}

		fp_ref = fopen(tc->reference_file[i].name, "rb");
		if (fp_ref == NULL) {
			printf("[%s] error: file open %s\n", __func__, tc->reference_file[i].name);
			perror("  fopen() failed");
			td->test_status = TEST_STATUS_ERR_REFERENCE;
			continue;
		}

		/*
		 * Compare reference and output file
		 */
		for (err_count = 0, file_idx = 0; file_idx < tc->output_file[i].len; file_idx++) {
			if (fgetc(fp_ref) != fgetc(fp_out)) {
				err_count++;
			}
		}

		if (err_count) {
			td->test_status = TEST_STATUS_FAIL_COMPARE;
			printf("[%s] %s fail: compare %s and %s (err_count %d)\n", __func__, tc->test_name,
					tc->output_file[i].name, tc->reference_file[i].name, err_count);
		} else {
			td->test_status = TEST_STATUS_PASS;
		}
	}

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
	printf("[%s] Input Files\n", __func__);
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
			tc->input_file[i].len = 0;
		}

		/* store image info */
		img->input[i].id = tc->input_file[i].id;
		img->input[i].len = tc->input_file[i].len;
		img->input[i].width = tc->input_file[i].width;
		img->input[i].height = tc->input_file[i].height;
		img->input[i].fmt = tc->input_file[i].fmt;

		fclose(fp);
	}

	printf("[%s] Output Files\n", __func__);
	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		fname_to_int = atoi(tc->output_file[i].name);
		printf("  str(%s) to int(%d)\n", tc->output_file[i].name, fname_to_int);
		if (fname_to_int == -1) {
			tc->output_file[i].id = -1;
			continue;
		} else {
			tc->output_file[i].id = i;
		}

		tc->output_file[i].fmt = tc->wdma1.reg.uCTRL.bREG.FMT;
		tc->output_file[i].width = tc->wdma1.reg.uSIZE.bREG.WIDTH;
		tc->output_file[i].height = tc->wdma1.reg.uSIZE.bREG.HEIGHT;

		/*
		 * Now we only use 'test_case_t' after copy v/paddr.
		 */
		tc->output_file[i].vaddr = img->output[i].vaddr;
		tc->output_file[i].paddr = img->output[i].paddr;

		/* store image info */
		img->output[i].id = tc->output_file[i].id;
		img->output[i].fmt = tc->output_file[i].fmt;
		img->output[i].width = tc->output_file[i].width;
		img->output[i].height = tc->output_file[i].height;
	}

	/*
	 * for debugging - verify input_files
	 */
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

	printf("===================================================================\n\n");
	return ret;
}

static int write_output_image(struct test_case_t *tc)
{
	int i, ret = 0;
	unsigned int written_len, output_len;
	FILE *fp = NULL;

	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		if (tc->output_file[i].id != -1) {
			printf("[%s] %s writting...\n", __func__, tc->output_file[i].name);

			output_len = tcc_get_image_size(tc->output_file[i].fmt,
						tc->output_file[i].width, tc->output_file[i].height);

			fp = fopen(tc->output_file[i].name, "wb");
			if (fp == NULL) {
				perror("  fopen() failed");
				continue;
			}

			written_len = fwrite(tc->output_file[i].vaddr, 1, output_len, fp);
			if (written_len != output_len) {
				/*
				 * If failed, output_file[i].len = 0
				 */
				tc->output_file[i].len = 0;
				printf("  error: wrote %d bytes != %d\n", written_len, output_len);
			} else {
				/*
				 * If succeed, output_file[i].len = written_len
				 */
				tc->output_file[i].len = written_len;
				printf("  wrote(%d)\n", written_len);
			}
			fclose(fp);
		}
	}

	return ret;
}

static unsigned int tcc_get_image_size(unsigned int format, unsigned int width, unsigned int height)
{
	int bpp, pfmt;
	unsigned int imagesize;
	unsigned int bytesperline;

	bpp = tcc_get_bpp(format);
	pfmt = tcc_get_pfmt(format);
	bytesperline = width * bpp;

	switch (pfmt) {
		case TCC_PFMT_YUV422:
			imagesize = width * height * 2;
			break;
		case TCC_PFMT_YUV420:
			imagesize = width * height * 3 / 2;
			break;
		case TCC_PFMT_RGB:
		default:
			imagesize = bytesperline * height;
			break;
	}

	//imagesize = PAGE_ALIGN(imagesize);

	return imagesize;
}

int tcc_get_bpp(unsigned int format)
{
	int bpp;

	switch (format) {

		/* What is these ? */
		case VIOC_IMG_FMT_BPP1:
		case VIOC_IMG_FMT_BPP2:
		case VIOC_IMG_FMT_BPP4:
			//bpp = ????;
			//break;

			/* RGB formats */
		case VIOC_IMG_FMT_BPP8:
		case VIOC_IMG_FMT_RGB332:
			bpp = 1;
			break;
		case VIOC_IMG_FMT_ARGB4444:
		case VIOC_IMG_FMT_RGB565:
		case VIOC_IMG_FMT_ARGB1555:
			bpp = 2;
			break;
		case VIOC_IMG_FMT_RGB888:
		case VIOC_IMG_FMT_ARGB6666_3:
		case VIOC_IMG_FMT_444SEP:		//TODO: is it 3bytes ?
			bpp = 3;
			break;
		case VIOC_IMG_FMT_ARGB8888:
		case VIOC_IMG_FMT_ARGB6666_4:
			bpp = 4;
			break;

			/* YUV formats */
		case VIOC_IMG_FMT_YUV420SEP:
		case VIOC_IMG_FMT_YUV420IL0:
		case VIOC_IMG_FMT_YUV420IL1:
		case VIOC_IMG_FMT_YUV422SEP:
		case VIOC_IMG_FMT_YUV422IL0:
		case VIOC_IMG_FMT_YUV422IL1:
			bpp = 1;
			break;

			/* YUV422 Sequential formats */
		case VIOC_IMG_FMT_YUYV:
		case VIOC_IMG_FMT_YVYU:
		case VIOC_IMG_FMT_UYVY:
		case VIOC_IMG_FMT_VYUY:
			bpp = 2;
			break;

		default:
			printf("[%s] Not supported format(%d)\n", __func__, format);
			bpp = -1;
			break;
	}

	return bpp;
}

static int tcc_get_pfmt(unsigned int format)
{
	enum tcc_pix_fmt pfmt;

	switch (format) {
		/* YUV420 formats */
		case VIOC_IMG_FMT_YUV420SEP:
		case VIOC_IMG_FMT_YUV420IL0:
		case VIOC_IMG_FMT_YUV420IL1:
			pfmt = TCC_PFMT_YUV420;
			break;
			/* YUV422 formats */
		case VIOC_IMG_FMT_YUYV:
		case VIOC_IMG_FMT_YVYU:
		case VIOC_IMG_FMT_UYVY:
		case VIOC_IMG_FMT_VYUY:
		case VIOC_IMG_FMT_YUV422SEP:
		case VIOC_IMG_FMT_YUV422IL0:
		case VIOC_IMG_FMT_YUV422IL1:
			pfmt = TCC_PFMT_YUV422;
			break;
			/* RGB formats */
		case VIOC_IMG_FMT_RGB332:
		case VIOC_IMG_FMT_ARGB4444:
		case VIOC_IMG_FMT_ARGB1555:
		case VIOC_IMG_FMT_RGB565:
		case VIOC_IMG_FMT_RGB888:
		case VIOC_IMG_FMT_ARGB6666_3:
		case VIOC_IMG_FMT_ARGB8888:
		case VIOC_IMG_FMT_ARGB6666_4:
			pfmt = TCC_PFMT_RGB;
			break;

			//TODO: what is these?
		case VIOC_IMG_FMT_BPP1:
		case VIOC_IMG_FMT_BPP2:
		case VIOC_IMG_FMT_BPP4:
		case VIOC_IMG_FMT_BPP8:
		case VIOC_IMG_FMT_444SEP:
			//pfmt = ????;
			//break;

		default:
			printf("[%s] Not supported foramt(%d)\n", __func__, format);
			pfmt = -1;
			break;
	}

	return pfmt;
}
