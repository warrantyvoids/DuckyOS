add_library( arch_i686 STATIC apic.c arch.c entry.s irq.c isr.s paging.c pic.c pit.c tables.c )

target_link_libraries( arch_i686 arch_i686_drv )


add_subdirectory( drv )
