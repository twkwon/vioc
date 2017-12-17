#ifndef __PARSER_H__
#define __PARSER_H__

#include <vioc.h>
#include <list.h>

struct getbuf_t {
	int idx;
	int buf_size;
	char *buf;
	struct list_head list;
};

extern int parse_test_case(char *, struct test_data_t *);
extern void print_all_list(struct getbuf_t *, struct test_data_t *);
extern void print_parsed_data(struct test_data_t *);

#endif	//__PARSER_H__
