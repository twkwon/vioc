#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <debug.h>
#include <mmap.h>
#include <vioc.h>


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
		paddr = MMAP_DEFAULT_ADDR;
	}
	if (0 == size) {
		size = MMAP_DEFAULT_LEN;
	}

	vaddr = (addr_t *)mmap(0,
				size,
				PROT_READ|PROT_WRITE,
				MAP_SHARED,
				fd,
				paddr);
	if (vaddr == MAP_FAILED) {
		perror("mmap failed");
		vaddr = NULL;
		goto err_mmap;
	}
	DBG(DL_TEST, "mmap: phy_addr:0x%08x -> virt_addr:%p, size:0x%08x\n", paddr, vaddr, size);

err_mmap:
	close(fd);
err_fd:
	return vaddr;
}

void vioc_munmap(addr_t *vaddr)
{
	munmap(vaddr, MMAP_DEFAULT_LEN);
}
