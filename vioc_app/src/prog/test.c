#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <debug.h>
#include <vioc.h>
#include <parser.h>
#include <pmap.h>
#include <mmap.h>

extern unsigned int auto_test;

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
	int todo_disable_sc_id = -1;
	addr_t *backup_vioc_base_address = NULL;
	struct test_case_t *test_case;
	struct test_data_t *test_data, *td;
	struct pmap_t pmap;
	struct list_head *pos = NULL;
	struct image_file_t image;
	FILE *report_fp;
	char report_fname[SIZE_OF_TEST_NAME];
	char report_result[SIZE_OF_TEST_NAME];

	/*
	 * parse data from test_case.txt
	 */
	test_data = (struct test_data_t *)malloc(sizeof(struct test_data_t));
	INIT_LIST_HEAD(&test_data->list);

	nr_test = parse_test_case(file_name, test_data);
	if (nr_test <= 0) {
		DBG_ERR("parse_test_case()\n");
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
	backup_vioc_base_address = test_case->vioc_base_addr;

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
	DBG(DL_TEST, "Image base: paddr(0x%08x) -> vaddr(%p)\n", image.base_paddr, image.base_vaddr);

	for (i = 0; i < MAX_NUM_OF_RDMA + MAX_NUM_OF_WDMA; i++) {
		unsigned int offs;
		offs = IMAGE_SIZE * i;

		if (i < MAX_NUM_OF_RDMA) {
			image.input[i].vaddr = image.base_vaddr + INT32_OFFSET(offs);
			image.input[i].paddr = image.base_paddr + offs;
			DBG(DL_TEST, "  input%d: paddr(0x%08x) -> vaddr(%p)\n", i, image.input[i].paddr, image.input[i].vaddr);
		} else {
			image.output[i - MAX_NUM_OF_RDMA].vaddr = image.base_vaddr + INT32_OFFSET(offs);
			image.output[i - MAX_NUM_OF_RDMA].paddr = image.base_paddr + offs;
			DBG(DL_TEST, " output%d: paddr(0x%08x) -> vaddr(%p)\n", i - MAX_NUM_OF_RDMA, image.output[i - MAX_NUM_OF_RDMA].paddr, image.output[i - MAX_NUM_OF_RDMA].vaddr);
		}
	}

	/*
	 * RUN the test case on the list of test_data one by one
	 */
	list_for_each(pos, &test_data->list) {
		td = list_entry(pos, struct test_data_t, list);

		if (g_dbg_lvl & DL_PARSER) {
			print_parsed_data(td);
		}

		if (auto_test)
			goto run_test;

		printf("Do you want to run the test%d (%s)? [y/n] ", td->test_no, td->test_name);
		c = getchar();
		if (c == 'y' || c == 'Y') {
run_test:
			printf("\n============================================\n");
			printf(" TEST CASE No.%d - %s\n", td->test_no, td->test_name);
			printf("============================================\n");

			memset(test_case, 0, sizeof(struct test_case_t));
			test_case->vioc_base_addr = backup_vioc_base_address;
			test_case->todo_disable_prev_sc_id = todo_disable_sc_id;

			ret = run_test_single(test_case, td, &image);
			sleep(3);
			if (ret) {
				td->test_status = TEST_STATUS_ERR_RUN;
				printf("---> ERROR: run test\n");
			} else {
				/*
				 * If run_test_single is succeeded, compare output and reference image.
				 */
				ret = verify_test_single(test_case, td);
			}

			/* You must reset this components before testing next case */
			todo_disable_sc_id = test_case->sc.info.id;

		} else {
			td->test_status = TEST_STATUS_RUN_SKIP;
			printf("---> SKIP TEST%d:%s\n", td->test_no, td->test_name);
		}

		if (!auto_test)
			getchar();
	}

	printf("\n<----------- END OF TEST ----------->\n");

	printf("\n<----------- TEST STATUS  ----------->\n");

	sprintf(report_fname, "RESULT-%s.txt", file_name);
	report_fp = fopen(report_fname, "w");

	list_for_each(pos, &test_data->list) {
		td = list_entry(pos, struct test_data_t, list);

		if (td->test_status == TEST_STATUS_PASS) {
			sprintf(report_result, "[%s] PASS\n", td->test_name);
		} else if (td->test_status == TEST_STATUS_RUN_SKIP) {
			sprintf(report_result, "[%s] SKIP run test\n", td->test_name);
		} else if (td->test_status == TEST_STATUS_NO_COMPARE) {
			sprintf(report_result, "[%s] SKIP compare reference\n", td->test_name);
		} else {
			switch (td->test_status) {
			case TEST_STATUS_ERR_RUN:
				sprintf(report_result, "[%s] ERROR: run test\n", td->test_name);
				break;
			case TEST_STATUS_ERR_OUTPUT:
				sprintf(report_result,"[%s] ERROR: output file\n", td->test_name);
				break;
			case TEST_STATUS_ERR_INPUT:
				sprintf(report_result, "[%s] ERROR: input file\n", td->test_name);
				break;
			case TEST_STATUS_ERR_REFERENCE:
				sprintf(report_result, "[%s] ERROR: reference file\n", td->test_name);
				break;
			case TEST_STATUS_FAIL_COMPARE:
				sprintf(report_result, "[%s] FAIL: compare reference and output file\n", td->test_name);
				break;
			default:
				sprintf(report_result, "[%s] ERROR: no %d\n", td->test_name, td->test_status);
				break;
			}
		}

		printf("%s", report_result);

		if (report_fp == NULL) {
			DBG_ERR("create %s\n", report_fname);
			perror("fopen() failed");
		} else {
			fprintf(report_fp, "%s", report_result);
			fclose(report_fp);
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
	DBG(DL_TEST, "\n");

	/*
	 * setup vioc components & path
	 */
	ret = setup_test_case(test_case, test_data);
	if (ret) {
		DBG_ERR("setup_vioc_path()\n");
		goto exit;
	}

	/*
	 * setup input, output, reference image files
	 */
	ret = setup_image_file(test_case, image);
	if (ret) {
		DBG_ERR("setup_image_file()\n");
		goto exit;
	}

	/*
	 * setup vioc components & path
	 */
	ret = setup_vioc_path(test_case);
	if (ret) {
		DBG_ERR("setup_vioc_path()\n");
		goto exit;
	}

	/*
	 * run single test case
	 */
	ret = shoot_test(test_case);
	if (ret) {
		DBG_ERR("shoot_test()\n");
		goto exit;
	}

	/*
	 * write output iamge through wdma
	 */
	ret = write_output_image(test_case);
	if (ret) {
		DBG_ERR("write_output_image()\n");
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
			DBG_ERR("file open %s\n", tc->output_file[i].name);
			perror("  fopen() failed");
			td->test_status = TEST_STATUS_ERR_OUTPUT;
			continue;
		}

		fp_ref = fopen(tc->reference_file[i].name, "rb");
		if (fp_ref == NULL) {
			DBG_ERR("file open %s\n", tc->reference_file[i].name);
			perror("  fopen() failed");
			td->test_status = TEST_STATUS_ERR_REFERENCE;
			fclose(fp_out);
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
			DBG_ERR("%s fail: compare %s and %s (err_count %d)\n", tc->test_name,
					tc->output_file[i].name, tc->reference_file[i].name, err_count);
		} else {
			td->test_status = TEST_STATUS_PASS;
		}

		fclose(fp_out);
		fclose(fp_ref);
	}

	return ret;
}

static void delete_test_data_list(struct test_data_t *t)
{
	struct list_head *pos, *q;
	DBG(DL_TEST, "\n");

	list_for_each_safe(pos, q, &t->list) {
		struct test_data_t *k = list_entry(pos, struct test_data_t, list);
		DBG(DL_TEST, "remove test_data->test_no : %d\n", k->test_no);
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
	unsigned int output_len;

	DBG(DL_TEST, "\n");

	/*
	 * open input files if test_cast has file_name
	 */
	DBG(DL_TEST, "Input Files\n");
	for (i = 0; i < MAX_NUM_OF_RDMA; i++) {
		/*
		 * open files if test_cast has file_name
		 */
		fname_to_int = atoi(tc->input_file[i].name);
		DBG(DL_TEST, "  str(%s) to int(%d)\n", tc->input_file[i].name, fname_to_int);
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
			DBG_ERR("file open %s\n", tc->input_file[i].name);
			perror("fopen() failed");
			return -1;
		}

		/* get file length */
		fseek(fp, 0, SEEK_END);
		flen = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (flen == 0) {
			DBG_ERR("%s is empty file\n", tc->input_file[i].name);
			fclose(fp);
			return -1;
		}

		/* read binary file into pmap buf */
		tc->input_file[i].len = fread((char *)tc->input_file[i].vaddr, 1, flen, fp);
		if (tc->input_file[i].len == flen) {
			DBG(DL_TEST, "read %s %d bytes\n", tc->input_file[i].name, tc->input_file[i].len);
		} else {
			if (feof(fp)) {
				DBG_ERR("read %s: unexpected end of file\n", tc->input_file[i].name);
			} else if (ferror(fp)) {
				perror("fread() failed");
			} else {
				DBG_ERR("read %s %d bytes\n", tc->input_file[i].name, tc->input_file[i].len);
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

	DBG(DL_TEST, "Output Files\n");
	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		fname_to_int = atoi(tc->output_file[i].name);
		DBG(DL_TEST, "  str(%s) to int(%d)\n", tc->output_file[i].name, fname_to_int);
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

		/*
		 * clear output buffer as zero
		 */
		output_len = tcc_get_image_size(tc->output_file[i].fmt,
						tc->output_file[i].width, tc->output_file[i].height);
		memset((unsigned char *)tc->output_file[i].vaddr, 0, output_len);
	}

	if (g_dbg_lvl == DL_VERIFY) {
		/* for debugging - verify input_files */
		verify_image_buf(tc);
	}

	return ret;
}

static int verify_image_buf(struct test_case_t *tc)
{
	int i, ret = 0;
	unsigned int len = 0;
	FILE *fp = NULL;
	char name[SIZE_OF_TEST_NAME];

	for (i = 0; i < MAX_NUM_OF_RDMA; i++) {
		DBG(DL_VERIFY, "Input file(%s) id(%d) size(%dx%d) fmt(%d)\n", tc->input_file[i].name, tc->input_file[i].id,
			tc->input_file[i].width,tc->input_file[i].height, tc->input_file[i].fmt);
		if (tc->input_file[i].id != -1) {
			sprintf(name, "verify%d-%s", i, tc->input_file[i].name);
			DBG(DL_VERIFY, "\tbackup fname(%s)\n", name);
			fp = fopen(name, "wb");
			if (fp == NULL) {
				perror("  fopen() failed");
				continue;
			}
			len = fwrite(tc->input_file[i].vaddr, 1, tc->input_file[i].len, fp);
			if (len != tc->input_file[i].len) {
				DBG_ERR("\twrote %d bytes != %d\n", len, tc->input_file[i].len);
			} else {
				DBG_ERR("\tfile(%s) wrote(%d) requested(%d)\n", name, len, tc->input_file[i].len);
			}
			fclose(fp);
		}
	}

	return ret;
}

static int write_output_image(struct test_case_t *tc)
{
	int i, ret = 0;
	unsigned int written_len, output_len;
	FILE *fp = NULL;

	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		if (tc->output_file[i].id != -1) {
			DBG(DL_TEST, "%s writting...\n", tc->output_file[i].name);

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
				DBG_ERR("\t%s wrote %d bytes != %d\n", tc->output_file[i].name, written_len, output_len);
			} else {
				/*
				 * If succeed, output_file[i].len = written_len
				 */
				tc->output_file[i].len = written_len;
				DBG(DL_TEST, "\twrote(%d)\n", written_len);
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
		case VIOC_IMG_FMT_BPP1:
		case VIOC_IMG_FMT_BPP2:
		case VIOC_IMG_FMT_BPP4:
			bpp = 1;
			break;

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
			DBG_ERR("Not supported format(%d)\n", format);
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
			pfmt = TCC_PFMT_RGB;
			break;

		default:
			DBG_ERR("Not supported foramt(%d)\n", format);
			pfmt = -1;
			break;
	}

	return pfmt;
}
