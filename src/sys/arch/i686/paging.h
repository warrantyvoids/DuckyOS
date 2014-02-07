#ifndef ARCH_i686_PAGING_H
#define ARCH_i686_PAGING_H

void paging_init();
#define PAGING_CACHE_DISABLE   (1 << 4)
#define PAGING_WRITE_THROUGH   (1 << 3)
#define PAGING_USERLAND        (1 << 2)
#define PAGING_READWRITE       (1 << 1)

void * paging_map(void * physical_address, uint16_t flags);

#endif