#ifndef __ARCH_i386_UTIL_H
#define __ARCH_i386_UTIL_H

#include <stdint.h>
#include <stdbool.h>

enum cpuid_feature_t {
  CPUID_FEAT_ECX_SSE3 = 0,
  CPUID_FEAT_ECX_PCLMUL,
  CPUID_FEAT_ECX_DTES64,
  CPUID_FEAT_ECX_MONITOR,
  CPUID_FEAT_ECX_DS_CPL,
  CPUID_FEAT_ECX_VMX,
  CPUID_FEAT_ECX_SMX,
  CPUID_FEAT_ECX_EST,
  CPUID_FEAT_ECX_TM2,
  CPUID_FEAT_ECX_SSSE3,
  CPUID_FEAT_ECX_CID,
  CPUID_FEAT_ECX_FMA = 12,
  CPUID_FEAT_ECX_CX16,
  CPUID_FEAT_ECX_ETPRD,
  CPUID_FEAT_ECX_PDCM,
  CPUID_FEAT_ECX_DCA = 18,
  CPUID_FEAT_ECX_SSE4_1,
  CPUID_FEAT_ECX_SSE4_2,
  CPUID_FEAT_ECX_x2APIC,
  CPUID_FEAT_ECX_MOVBE,
  CPUID_FEAT_ECX_POPCNT,
  CPUID_FEAT_ECX_AES = 25,
  CPUID_FEAT_ECX_XSAVE,
  CPUID_FEAT_ECX_OSXAVE,
  CPUID_FEAT_ECX_AVX, 

  CPUID_FEAT_EDX_FPU = 32,
  CPUID_FEAT_EDX_VME,
  CPUID_FEAT_EDX_DE,
  CPUID_FEAT_EDX_PSE,
  CPUID_FEAT_EDX_TSC,
  CPUID_FEAT_EDX_MSR,
  CPUID_FEAT_EDX_PAE,
  CPUID_FEAT_EDX_MCE,
  CPUID_FEAT_EDX_CX8,
  CPUID_FEAT_EDX_APIC,
  CPUID_FEAT_EDX_SEP = 32+11,
  CPUID_FEAT_EDX_MTRR,
  CPUID_FEAT_EDX_PGE,
  CPUID_FEAT_EDX_MCA,
  CPUID_FEAT_EDX_CMOV,
  CPUID_FEAT_EDX_PAT,
  CPUID_FEAT_EDX_PSE36,
  CPUID_FEAT_EDX_PSN,
  CPUID_FEAT_EDX_CLF,
  CPUID_FEAT_EDX_DTES = 32+21,
  CPUID_FEAT_EDX_ACPI,
  CPUID_FEAT_EDX_MMX,
  CPUID_FEAT_EDX_FXSR,
  CPUID_FEAT_EDX_SSE,
  CPUID_FEAT_EDX_SSE2,
  CPUID_FEAT_EDX_SS,
  CPUID_FEAT_EDX_HTT,
  CPUID_FEAT_EDX_TM1,
  CPUID_FEAT_EDX_IA64,
  CPUID_FEAT_EDX_PBE,

};

static inline void wrmsr(uint32_t addr, uint64_t val) {
  uint32_t eax = val & 0xFFFFFFFF;
  uint32_t edx = val >> 32;
  asm volatile("wrmsr" : : "a"(eax), "d"(edx), "c"(addr));
}

static inline uint64_t rdmsr(uint32_t addr) {
  uint32_t eax, edx;
  uint64_t retVal;
  asm volatile("rdmsr" : "=a"(eax), "=d"(edx) : "c"(addr));
  return retVal = ((uint64_t)edx << 32) | (eax);
}

static inline void cpuid_vendor(char vendor[12]) {
  uint32_t * vstr = (uint32_t*)vendor;
  asm volatile ("cpuid" : "=b"(vstr[0]), "=c"(vstr[2]), "=d"(vstr[1]) : "a"( 0 ) : "%eax" );
}

static inline bool cpuid_test_feature( enum cpuid_feature_t feat ) {
  uint32_t res;
  uint32_t bitfield;
  if (feat >= 32) {
    bitfield = 1 << (feat - 32);
    asm volatile ("cpuid" : "=d"(res) : "a"(1) : "eax","ebx","ecx");
  } else {
    bitfield = 1 << feat;
    asm volatile ("cpuid" : "=c"(res) : "a"(1) : "eax","ebx","edx");
  }
  return ((res & bitfield) == bitfield);
}

static inline uint32_t cpuid_get_signature( ) {
  uint32_t sign;
  asm volatile ("cpuid" : "=a"(sign) : "a"(1) : "ebx","ecx","edx");
  return sign;
}

static inline void udelay(unsigned long n) {  
    if (!n)  
        return;  
    asm("1: dec %%eax; jne 1b;"  
        : : "a" (n * 1000));  
}  
  
static inline void mdelay(unsigned long n) {  
    while (--n)  
        udelay(1000);  
}

static inline void outb(uint16_t port, uint8_t value) {
  asm volatile ( "outb %0, %1" : : "a"(value), "Nd" (port));
}
static inline void outw(uint16_t port, uint16_t value) {
  asm volatile ( "outw %0, %w1" :  : "a" (value), "Nd" (port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile ( "inb %1, %0" : "=a" (ret) : "Nd" (port));
  return ret;
}
static inline uint16_t inw(uint16_t port) {
  uint16_t ret;
  asm volatile ( "inw %w1, %0" : "=a" (ret) : "Nd" (port));
  return ret;
}

#endif
