#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <debug.h>
#include <parser.h>
#include <vioc.h>

int read_data(FILE *, struct getbuf_t *);
int parser(struct test_data_t *, struct getbuf_t *, int);
void delete_all_list(struct getbuf_t *);


/** @brief  function
 *  @return returns number of test cases
 *          returns -1 if failed
 */
int parse_test_case(char *file_name, struct test_data_t *test_data)
{
	FILE *fp;
	struct getbuf_t *tcase;
	int nr_test;
	int i;

	/* open the test case file */
	fp = fopen(file_name, "r");
	if (fp == NULL) {
		DBG_ERR("file open %s\n", file_name);
		perror("fopen() failed");
		return -1;
	}

	/* set up the getting buffer */
	tcase = (struct getbuf_t *)malloc(sizeof(struct getbuf_t));
	INIT_LIST_HEAD(&tcase->list);

	/* read data from the file */
	nr_test = read_data(fp, tcase);

	/* parsing data */
	for (i = 1; i <= nr_test; i++) {
		struct test_data_t *tdata;
		tdata = (struct test_data_t *)malloc(sizeof(struct test_data_t));
		memset(tdata, 0, sizeof(struct test_data_t));

		parser(tdata, tcase, i);

		list_add_tail(&tdata->list, &test_data->list);
	}

	if (g_dbg_lvl & DL_PARSER) {
		print_all_list(tcase, test_data);
	}

	delete_all_list(tcase);
	free(tcase);

	return nr_test;
}

/** @brief Read data file that is written about vioc test cases
 *  @return returns number of test cases
 *          returns -1 if failed
 */
int read_data(FILE *fp, struct getbuf_t *tcase)
{
	int nr_test;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	//DBG(DL_TEST, "%d\n", nr_test);
	DBG(DL_TEST, "\n");

	nr_test = 1;
	while ((read = getline(&line, &len, fp)) != -1) {
		struct getbuf_t *gb;
		gb = (struct getbuf_t *)malloc(sizeof(struct getbuf_t));

		DBG(DL_PARSER, "Retrieved line of length %zu (excluding terminate NULL character)\n", read);
		DBG(DL_PARSER, "getline: %s", line);

		gb->idx = nr_test++;
		gb->buf_size = read + 1;
		gb->buf = calloc(gb->buf_size, sizeof(char));
		memcpy(gb->buf, line, (gb->buf_size * sizeof(char)));
		DBG(DL_PARSER, "gb->buf (size %d): %s", gb->buf_size, gb->buf);

		list_add_tail(&gb->list, &tcase->list);
		//list_add(&gb->list, &tcase->list);
	}

	return (nr_test - 1);
}

int parser(struct test_data_t *tdata, struct getbuf_t *tcase, int test_no)
{
	int ret = 0;
	struct list_head *pos = NULL;
	struct getbuf_t *gb = NULL;
	int i, j, start_pos;
	int nr_vioc, nr_vioc_start[MAX_NUM_OF_VIOC_COMPONENTS] = {0};
	int nr_regs, nr_regs_end[MAX_NUM_OF_REG_DATA_TEST] = {0};
	char c;
	char *buf = NULL;   // temp buf
	char *start_addr;	// start address of vioc component
	int offset;			// read reg val from start_addr + offset

	DBG(DL_TEST, "\n");

	list_for_each(pos, &tcase->list) {
		gb = list_entry(pos, struct getbuf_t, list);
		if (gb->idx == test_no) {
			DBG(DL_PARSER, "found gb->idx(%d) gb->buf_size(%d)\n", gb->idx, gb->buf_size);
			tdata->test_no = gb->idx;
			buf = calloc(gb->buf_size, sizeof(char));
			memcpy(buf, gb->buf, (gb->buf_size * sizeof(char)));
			continue;
		}
	}

	DBG(DL_PARSER, "test_no: %d buf: %s", tdata->test_no, buf);

	i = 0;
	nr_vioc = 0;
	nr_regs = 0;
	do {
		sscanf(buf+i, "%c", &c);
		//DBG(DL_PARSER, "%c", c);
		if (',' == c) {
			//printf("',' is %dth\n", i);
			nr_regs_end[nr_regs++] = i;
		} else if (':' == c) {
			//DBG(DL_PARSER, "':' is %dth\n", i);
			nr_vioc_start[nr_vioc++] = i + 1;
		}
		i++;
	} while ('\n' != c);

	nr_vioc = nr_vioc - 1;
	DBG(DL_PARSER, "nr_vioc(%d) nr_regs(%d)\n", nr_vioc, nr_regs);

	/* get test name */
	sscanf(buf, "%[^','],%[^','],%d,%d,%d,%[^','],%d,%d,%d,%[^','],%d,%d,%d,%[^','],%d,%d,%d,%[^','],%[^','],%[^','],%[^',']",
			tdata->test_name,
			tdata->input_file[0].name, &tdata->input_file[0].width, &tdata->input_file[0].height, &tdata->input_file[0].fmt,
			tdata->input_file[1].name, &tdata->input_file[1].width, &tdata->input_file[1].height, &tdata->input_file[1].fmt,
			tdata->input_file[2].name, &tdata->input_file[2].width, &tdata->input_file[2].height, &tdata->input_file[2].fmt,
			tdata->input_file[3].name, &tdata->input_file[3].width, &tdata->input_file[3].height, &tdata->input_file[3].fmt,
			tdata->output_file[0].name, tdata->output_file[1].name,
			tdata->reference_file[0].name, tdata->reference_file[1].name);

	start_pos = 0;
	for (i = 0; i < nr_vioc; i++) {
		int count = 0;
		int isfirst = 0;

		/* finding first address of vioc component & number of regs */
		for (j = 0; j < nr_regs; j++) {
			if (nr_regs_end[j] > nr_vioc_start[i] && !isfirst) {
				start_pos = j;
				isfirst = 1;
			}
			if (nr_regs_end[j] > nr_vioc_start[i] && nr_regs_end[j] < nr_vioc_start[i + 1]) {
				count++;
			}
		}
		DBG(DL_PARSER, "case(%d): start_pos(%d) count(%d)\n", i, start_pos, count);
		start_addr = buf + nr_vioc_start[i];
		offset = 0;

		for (j = 0; j < count; j++) {
			int val, diff;

			sscanf(start_addr + offset, "%d,", &val);

			if (j == 0) {
				diff = nr_regs_end[start_pos] - (nr_vioc_start[i] - 1);
				//DBG(DL_PARSER, "%d = %d - %d\n", diff, nr_regs_end[start_pos], (nr_vioc_start[i] - 1));
			} else {
				diff = nr_regs_end[start_pos + (j - 1) + 1] - nr_regs_end[start_pos + (j - 1)];
				//DBG(DL_PARSER, "%d = %d - %d\n", diff, nr_regs_end[start_pos + (j - 1) + 1], nr_regs_end[start_pos + (j - 1)]);
			}
			offset += diff;
			//DBG(DL_PARSER, "offset %d\n", offset);

			switch (i) {
				case VC_DISP_RDMA:
					tdata->disp_rdma.reg[j] = val;
					tdata->disp_rdma.nr_regs = count;
					break;
				case VC_RDMA_1st:
					tdata->rdma1.reg[j] = val;
					tdata->rdma1.nr_regs = count;
					break;
				case VC_RDMA_2nd:
					tdata->rdma2.reg[j] = val;
					tdata->rdma2.nr_regs = count;
					break;
				case VC_RDMA_3rd:
					tdata->rdma3.reg[j] = val;
					tdata->rdma3.nr_regs = count;
					break;
				case VC_RDMA_4th:
					tdata->rdma4.reg[j] = val;
					tdata->rdma4.nr_regs = count;
					break;
				case VC_WDMA_1st:
					tdata->wdma1.reg[j] = val;
					tdata->wdma1.nr_regs = count;
					break;
				case VC_WDMA_2nd:
					tdata->wdma2.reg[j] = val;
					tdata->wdma2.nr_regs = count;
					break;
				case VC_DISP_WMIX:
					tdata->disp_wmix.reg[j] = val;
					tdata->disp_wmix.nr_regs = count;
					break;
				case VC_WMIX:
					tdata->wmix.reg[j] = val;
					tdata->wmix.nr_regs = count;
					break;
				case VC_SC:
					tdata->sc.reg[j] = val;
					tdata->sc.nr_regs = count;
					break;
				case VC_LUT:
					tdata->lut.reg[j] = val;
					tdata->lut.nr_regs = count;
					break;
				case VC_VIN:
					tdata->vin.reg[j] = val;
					tdata->vin.nr_regs = count;
					break;
				case VC_VIN_LUT:
					tdata->vin_lut.reg[j] = val;
					tdata->vin_lut.nr_regs = count;
				case VC_OUTCFG:
					tdata->outcfg.reg[j] = val;
					tdata->outcfg.nr_regs = count;
					break;
				case VC_CONFIG:
					tdata->config.reg[j] = val;
					tdata->config.nr_regs = count;
					break;
				default:
					break;
			}
		}
	}

	//if (g_dbg_lvl & DL_PARSER)
		//print_parsed_data(tdata);

	free(buf);

	return ret;
}

void print_all_list(struct getbuf_t *t, struct test_data_t *td)
{
	struct list_head *pos;
	DBG(DL_TEST, "\n");

	if (t != NULL) {
		DBG(DL_PARSER, "PRINT getbuf_t list\n");
		list_for_each(pos, &t->list) {
			struct getbuf_t *k = list_entry(pos, struct getbuf_t, list);
			DBG(DL_PARSER, "idx : %d\n", k->idx);
			DBG(DL_PARSER, "size: %d\n", k->buf_size);
			DBG(DL_PARSER, "buf : %s\n", k->buf);
		}
	}

	if (td != NULL) {
		DBG(DL_PARSER, "PRINT test_data_t list");
		list_for_each(pos, &td->list) {
			struct test_data_t *tmp = list_entry(pos, struct test_data_t, list);
			print_parsed_data(tmp);
		}
	}
}

void delete_all_list(struct getbuf_t *t)
{
	struct list_head *pos, *q;
	DBG(DL_TEST, "\n");

	list_for_each_safe(pos, q, &t->list) {
		struct getbuf_t *k = list_entry(pos, struct getbuf_t, list);
		DBG(DL_PARSER, "remove tcase->idx : %d\n", k->idx);
		free(k->buf);
		list_del(pos);
		free(k);
	}
}

void print_parsed_data(struct test_data_t *t)
{
	int i;

	printf("\n=======================================\n");
	printf(" PARSED DATA\n");
	printf(" test_no(%d) test_name(%s)\n", t->test_no, t->test_name);
	printf("=========================================\n");

	for (i = 0; i < MAX_NUM_OF_RDMA; i++) {
		printf("   %d RDMA input: %s, size(%dx%d), fmt(%d)\n", i, t->input_file[i].name,
			t->input_file[i].width, t->input_file[i].height, t->input_file[i].fmt);
	}
	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		printf("  %d WDMA output: %s\n", i, t->output_file[i].name);
	}
	for (i = 0; i < MAX_NUM_OF_WDMA; i++) {
		printf("  %d Reference  : %s\n", i, t->reference_file[i].name);
	}

	printf("\n DISP_RDMA:");
	for (i = 0; i < t->disp_rdma.nr_regs; i++) {
		printf("%d,", t->disp_rdma.reg[i]);
	}
	printf("\n DISP_WMIX:");
	for (i = 0; i < t->disp_wmix.nr_regs; i++) {
		printf("%d,", t->disp_wmix.reg[i]);
	}

	printf("\n  1st RDMA:");
	for (i = 0; i < t->rdma1.nr_regs; i++) {
		printf("%d,", t->rdma1.reg[i]);
	}
	printf("\n  2nd RDMA:");
	for (i = 0; i < t->rdma2.nr_regs; i++) {
		printf("%d,", t->rdma2.reg[i]);
	}
	printf("\n  3rd RDMA:");
	for (i = 0; i < t->rdma3.nr_regs; i++) {
		printf("%d,", t->rdma3.reg[i]);
	}
	printf("\n  4th RDMA:");
	for (i = 0; i < t->rdma4.nr_regs; i++) {
		printf("%d,", t->rdma4.reg[i]);
	}

	printf("\n  1st WDMA:");
	for (i = 0; i < t->wdma1.nr_regs; i++) {
		printf("%d,", t->wdma1.reg[i]);
	}
	printf("\n  2nd WDMA:");
	for (i = 0; i < t->wdma2.nr_regs; i++) {
		printf("%d,", t->wdma2.reg[i]);
	}

	printf("\n  WMIX:");
	for (i = 0; i < t->wmix.nr_regs; i++) {
		printf("%d,", t->wmix.reg[i]);
	}
	printf("\n    SC:");
	for (i = 0; i < t->sc.nr_regs; i++) {
		printf("%d,", t->sc.reg[i]);
	}

	printf("\n   LUT:");
	for (i = 0; i < t->lut.nr_regs; i++) {
		printf("%d,", t->lut.reg[i]);
	}

	printf("\n   VIN:");
	for (i = 0; i < t->vin.nr_regs; i++) {
		printf("%d,", t->vin.reg[i]);
	}
	printf("\n   VIN_LUT:");
	for (i = 0; i < t->vin_lut.nr_regs; i++) {
		printf("%d,", t->vin_lut.reg[i]);
	}

	printf("\nOUTCFG:");
	for (i = 0; i < t->outcfg.nr_regs; i++) {
		printf("%d,", t->outcfg.reg[i]);
	}
	printf("\nCONFIG:");
	for (i = 0; i < t->config.nr_regs; i++) {
		printf("%d,", t->config.reg[i]);
	}

	printf("\n=======================================\n\n");
}
