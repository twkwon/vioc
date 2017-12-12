#ifndef __PARSER_H__
#define __PARSER_H__

#include <list.h>

struct getbuf_t {
	int idx;
	int buf_size;
	char *buf;
	struct list_head list;
};

#endif	//__PARSER_H__
