#include <stdalign.h>
#include <stdint.h>
// Page directory and table
alignas(4096) uint32_t kernelpagedir[1024];
alignas(4096) uint32_t lowpagetable[1024];
alignas(4096) uint32_t miscpagetable[1024];

#define KERNEL_TO_REAL(x) (((uint8_t*)x) + 0x40000000)
#define MISC_PAGEDIR 1023

static uint16_t nMiscIndex;
void paging_init() {
  void * pKernelpagedir = 0;
  void * pLowpagetable = 0;
  void * pMiscpagetable = 0;
  
  nMiscIndex = 0;
  
  //Translate
  pKernelpagedir = KERNEL_TO_REAL(kernelpagedir);
  pLowpagetable = KERNEL_TO_REAL(lowpagetable);
  pMiscpagetable = KERNEL_TO_REAL(miscpagetable);
  
  for (int i = 0; i < 1024; i++) {              // For this code, see http://www.osdev.org/Higher_Half_With_GDT
    lowpagetable[i] = (i * 0x1000) | 0x3;	// map first 4 MB
    kernelpagedir[i] = 0;		        // clear page directory entries
    miscpagetable[i] = 0;
  }
  
  kernelpagedir[0] = (uint32_t) pLowpagetable | 0x3;
  kernelpagedir[768] = (uint32_t) pLowpagetable | 0x3;
  kernelpagedir[MISC_PAGEDIR] = (uint32_t) pMiscpagetable | 0x3;
  
  asm volatile (  "mov %0, %%eax\n"
                  "mov %%eax, %%cr3\n"
                  "mov %%cr0, %%eax\n"
                  "orl $0x80000000, %%eax\n"
                  "mov %%eax, %%cr0\n" : : "m" (pKernelpagedir) );
}

void * paging_map(void * physical, uint16_t flags) {
  
  uint32_t physaddr = (uint32_t)physical & 0xFFFFF000;
  
  miscpagetable[nMiscIndex] = physaddr | flags | 0x01;
  uint32_t virtaddr = ((MISC_PAGEDIR) << 22) | (nMiscIndex << 12) | ((uint32_t)physical & 0x00000FFF);
  
  nMiscIndex++;
  
  asm volatile ( "  invlpg (%0)" : : "r"(virtaddr) : "memory" );
  return (void*)virtaddr;
  
}
