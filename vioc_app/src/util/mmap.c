#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <mmap.h>
#include <vioc.h>

#define MMAP_ADDR	(BASE_ADDR_VIOC)	/* VIOC base address */
#define MMAP_LEN	(0x00100000)		/* 1 MBytea */

addr_t *vioc_mmap(addr_t paddr, unsigned int size)
{
	int fd;
	addr_t *vaddr = NULL;

	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd < 0) {
		perror("/dev/mem open error");
		goto err_fd;
	}

	if (0 == paddr) {
		paddr = MMAP_ADDR;
	}
	if (0 == size) {
		size = MMAP_LEN;
	}

	vaddr = (addr_t *)mmap(0,
				size,
				PROT_READ|PROT_WRITE,
				MAP_SHARED,
				fd,
				paddr);
	if (vaddr == MAP_FAILED) {
		perror("mmap failed");
		vaddr == NULL;
		goto err_mmap;
	}
	printf("mmap: phy_addr:0x%08x -> virt_addr:%p, size:0x%08x\n", paddr, vaddr, size);

err_mmap:
	close(fd);
err_fd:
	return vaddr;
}

void vioc_munmap(addr_t *vaddr)
{
	munmap(vaddr, MMAP_LEN);
}

int test_mmap(void)
{
	unsigned int *vaddr = NULL;
	int dev, i;
	unsigned long tmp1, tmp2;
	unsigned int paddr, length;

	dev = open("/dev/mem", O_RDWR|O_SYNC);
	if (dev < 0) {
		perror("/dev/mem open error");
		exit(1);
	}

	paddr = MMAP_ADDR + 0xd000;
	length = 0x1000;

	vaddr = (unsigned int *)mmap(0,
				length,
				PROT_READ|PROT_WRITE,
				MAP_SHARED,
				dev,
				paddr);
	if (vaddr == MAP_FAILED) {
		perror("mmap failed");
		goto _EXIT;
	}
	printf("mmap: phy_addr:0x%08x -> virt_addr:%p\n", paddr, vaddr);

	/* read test */
	for (i = 0; i < 10; i++) {
		tmp1 = *((unsigned long *)(vaddr + i));
		printf("offset %d 0x%08X\n", i * 4, tmp1);
	}

	/* write test */
	for (i = 0; i < 10; i++) {
		tmp2 = 0x1 + i;
		*((unsigned long *)vaddr + i) = tmp2;
		tmp1 = *((unsigned long *)(vaddr + i));
		printf("offset 0x%02x: 0x%08X == 0x%08X\n", i * 4, tmp2, tmp1);
	}
_EXIT:
	munmap(vaddr, length);
	close(dev);
	return 0;
}

