#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <pmap.h>

#define PATH_PROC_PMAP	"/proc/pmap"

int pmap_get_info(const char *name, struct pmap_t *mem);

int get_pmap(char *pname, struct pmap_t *pmap)
{
	int ret = 0;

	ret = pmap_get_info(pname, pmap);
	if (ret < 0)
		printf("error get pmap\n");

	printf("pmap->video.name = %s\n", pname);
	printf("pmap->video.base = 0x%08x\n", pmap->base);
	printf("pmap->video.size = 0x%08x\n", pmap->size);

	return ret;
}

int pmap_get_info(const char *name, struct pmap_t *mem)
{
	int fd;
	int matches;
	char buf[2048];
	const char *p;
	ssize_t nbytes;
	unsigned int base_addr;
	unsigned int size;
	char s[128];

	fd = open(PATH_PROC_PMAP, O_RDONLY);
	if (fd < 0)
		return -1;

	nbytes = read(fd, buf, sizeof(buf));
	close(fd);

	p = buf;
	while (1) {
		matches = sscanf(p, "0x%x 0x%x %s", &base_addr, &size, s);
		if (matches == 3 && !strcmp(name, s)) {
			printf("requested physical memory '%s' (base=0x%x size=0x%x)\n",
					name, base_addr, size);
			mem->base = base_addr;
			mem->size = size;
			return 0;
		}
		p = strchr(p, '\n');
		if (p == NULL)
			break;
		p++;
	}
	printf("can't get physical memory '%s'", name);
	return -1;
}

int test_pmap(void)
{
	int ret = 0;
	struct pmap_t pmap;
	char *pmap_name = "video";

	ret = pmap_get_info(pmap_name, &pmap);
	if (ret < 0)
		printf("error get pmap\n");

	printf("pmap.video.name = %s\n", pmap_name);
	printf("pmap.video.base = 0x%08x\n", pmap.base);
	printf("pmap.video.size = 0x%08x\n", pmap.size);

	return 0;
}
