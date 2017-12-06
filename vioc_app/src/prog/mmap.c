#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <mmap.h>

#define MMAP_ADDR	0x12000000 + 0xd000
#define MMAP_LEN	0x00001000

int test_mmap(void)
{
	unsigned int *vaddr = NULL;
	int dev, i;
	unsigned long tmp1, tmp2;

	dev = open("/dev/mem", O_RDWR|O_SYNC);
	if (dev < 0) {
		perror("/dev/mem open error");
		exit(1);
	}

	vaddr = (unsigned int *)mmap(0,
				MMAP_LEN,
				PROT_READ|PROT_WRITE,
				MAP_SHARED,
				dev,
				MMAP_ADDR);
	if (vaddr == MAP_FAILED) {
		perror("mmap failed");
		goto _EXIT;
	}
	printf("mmap: phy_addr:0x%08x -> virt_addr:%p\n", (unsigned int)MMAP_ADDR, vaddr);

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
	munmap(vaddr, MMAP_LEN);
	close(dev);
	return 0;
}

