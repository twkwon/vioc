#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <parser.h>
#include <vioc.h>

int read_data(FILE *, struct getbuf_t *);
int parser(struct test_data_t *, struct getbuf_t *, int);
void print_all(struct getbuf_t *);
void delete_all(struct getbuf_t *);


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
		printf("error: file open %s\n", file_name);
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

	print_all(tcase);
	delete_all(tcase);
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
    char *buf = NULL;

    printf("[%s]\n", __func__);

    nr_test = 1;
    while ((read = getline(&line, &len, fp)) != -1) {
        struct getbuf_t *gb;
        gb = (struct getbuf_t *)malloc(sizeof(struct getbuf_t));

        printf("\nRetrieved line of length %zu (excluding terminate NULL character)\n", read);
        printf("line: %s", line);

        gb->idx = nr_test++;
        gb->buf_size = read + 1;
        gb->buf = calloc(gb->buf_size, sizeof(char));
        memcpy(gb->buf, line, (gb->buf_size * sizeof(char)));
        printf("gb->buf (size %d): %s", gb->buf_size, gb->buf);

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
    int i = 0;
	int nr_vioc = 0, nr_vioc_start[16] = {0};
    char c = 0;
    char *buf = NULL;   //temp buf

    printf("\n[%s]\n", __func__);

    list_for_each(pos, &tcase->list) {
        gb = list_entry(pos, struct getbuf_t, list);
        if (gb->idx == test_no) {
            printf("found gb->idx(%d) gb->buf_size(%d)\n", gb->idx, gb->buf_size);
            tdata->test_no = gb->idx;
            buf = calloc(gb->buf_size, sizeof(char));
            memcpy(buf, gb->buf, (gb->buf_size * sizeof(char)));
            continue;
        }
    }

    printf("test_no: %d buf: %s", tdata->test_no, buf);

    i = 0;
    nr_vioc = 0;
    do {
        sscanf(buf+i, "%c", &c);
        //printf("%c", c);
        if (':' == c) {
            printf("':' is %dth\n", i);
            nr_vioc_start[nr_vioc++] = i + 1;
        }
        i++;
    } while ('\n' != c);

    sscanf(buf, "%[^',']", tdata->test_name);
    sscanf(buf + nr_vioc_start[0], "%d,%d,%d,%d,%d,",
            &tdata->rdma.reg[0], &tdata->rdma.reg[1], &tdata->rdma.reg[2], &tdata->rdma.reg[3], &tdata->rdma.reg[4]);
    sscanf(buf + nr_vioc_start[1], "%d,%d,%d,%d,%d",
            &tdata->wdma.reg[0], &tdata->wdma.reg[1], &tdata->wdma.reg[2], &tdata->wdma.reg[3], &tdata->wdma.reg[4]);


    printf("=== %s ===\n", tdata->test_name);
    printf("RDMA:%d:%d:%d:%d:%d\n",
            tdata->rdma.reg[0], tdata->rdma.reg[1], tdata->rdma.reg[2], tdata->rdma.reg[3], tdata->rdma.reg[4]);
    printf("WDMA:%d:%d:%d:%d:%d\n",
            tdata->wdma.reg[0], tdata->wdma.reg[1], tdata->wdma.reg[2], tdata->wdma.reg[3], tdata->wdma.reg[4]);

    free(buf);
	
	return ret;
}

void print_all(struct getbuf_t *t)
{
    struct list_head *pos;
    printf("\n[%s]\n", __func__);

    list_for_each(pos, &t->list) {
        struct getbuf_t *k = list_entry(pos, struct getbuf_t, list);
        printf("idx : %d\n", k->idx);
		printf("size: %d\n", k->buf_size);
        printf("buf : %s\n", k->buf);
    }
}

void delete_all(struct getbuf_t *t)
{
    struct list_head *pos, *q;
    printf("\n[%s]\n", __func__);

    list_for_each_safe(pos, q, &t->list) {
        struct getbuf_t *k = list_entry(pos, struct getbuf_t, list);
        printf("remove tcase->idx : %d\n", k->idx);
        free(k->buf);
        list_del(pos);
        free(k);
    }
}

